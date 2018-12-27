/* Copyright (c) 2018, Travis Bemann
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <time.h>
#include <termios.h>
#include "af/common.h"
#include "af/inner.h"
#include "af/cond.h"
#include "af/io.h"

/* Function forward declarations */

/* Wake the IO manager */
void af_io_wake(af_io_t* io);

/* Add IO action */
void af_io_add(af_io_t* io, af_io_action_t* action);

/* Check whether a given file descriptor is currently active */
af_bool_t af_io_is_active(af_io_t* io, af_io_fd_t fd);

/* Set or unset non-blocking on a file; return TRUE on success */
af_bool_t af_fd_set_blocking(int fd, af_bool_t blocking);

/* Remove done actions from the active action list */
void af_io_remove_done(af_io_t* io);

/* Implements IO manager thread */
void* af_io_main(void* arg);

/* Prepare stdin */
af_bool_t af_io_prepare_stdin(void);

void timespec_diff(struct timespec *start, struct timespec *stop,
                   struct timespec *result);

/* Function definitions */

/* Initialize IO manager */
af_bool_t af_io_init(af_io_t* io, af_global_t* global) {
  af_io_fd_t pipefd[2];
  if(pthread_mutex_init(&io->mutex, NULL)) {
    return FALSE;
  }
  if(pipe(pipefd)) {
    pthread_mutex_destroy(&io->mutex);
    return FALSE;
  }
  io->global = global;
  io->break_fd_out = pipefd[0];
  io->break_fd_in = pipefd[1];
  if(!af_fd_set_blocking(io->break_fd_in, TRUE)) {
    close(io->break_fd_out);
    close(io->break_fd_in);
    pthread_mutex_destroy(&io->mutex);
    return FALSE;
  }
  io->active_action_count = 0;
  io->first_active_action = NULL;
  io->first_waiting_action = NULL;
  io->last_waiting_action = NULL;
  io->first_done_action = NULL;
  io->to_be_destroyed = FALSE;
  io->handler_task = NULL;
  if(!af_io_prepare_stdin()) {
    close(io->break_fd_out);
    close(io->break_fd_in);
    pthread_mutex_destroy(&io->mutex);
    return FALSE;
  }
  if(pthread_create(&io->pthread, NULL, af_io_main, io)) {
    close(io->break_fd_out);
    close(io->break_fd_in);
    pthread_mutex_destroy(&io->mutex);
    return FALSE;
  }
  return TRUE;
}

/* Prepare stdin */
af_bool_t af_io_prepare_stdin(void) {
  struct termios tp;
  if(isatty(STDIN_FILENO)) {
    if(tcgetattr(STDIN_FILENO, &tp) == -1) {
      return FALSE;      
    }
    if(tp.c_lflag & ECHO || tp.c_lflag & ICANON) {
      tp.c_lflag &= ~ECHO & ~ICANON;
      if(tcsetattr(STDIN_FILENO, TCSANOW, &tp) == -1) {
	return FALSE;
      }
    }
  }
  return TRUE;
}

/* Cleanup stdin */
af_bool_t af_io_cleanup_stdin(void) {
  struct termios tp;
  af_fd_set_blocking(STDIN_FILENO, TRUE);
  if(isatty(STDIN_FILENO)) {
    if(tcgetattr(STDIN_FILENO, &tp) == -1) {
      return FALSE;      
    }
    if(!(tp.c_lflag & ECHO) || !(tp.c_lflag & ICANON)) {
      tp.c_lflag |= ECHO | ICANON;
      if(tcsetattr(STDIN_FILENO, TCSANOW, &tp) == -1) {
	return FALSE;
      }
    }
  }
  return TRUE;
}

/* Destroy IO manager */
void af_io_destroy(af_io_t* io) {
  io->to_be_destroyed = TRUE;
  af_io_wake(io);
}

/* Marky IO action to be destroyed */
void af_io_action_destroy(af_io_action_t* action) {
  af_io_t* io = action->io;
  pthread_mutex_lock(&io->mutex);
  if(action->is_done) {
    if(action->prev_action) {
      action->prev_action->next_action = action->next_action;
    } else {
      io->first_done_action = action->next_action;
    }
    if(action->next_action) {
      action->next_action->prev_action = action->prev_action;
    }
    free(action);
  } else {
    action->to_be_destroyed = TRUE;
  }
  pthread_mutex_unlock(&io->mutex);
}

/* Get pending write count */
af_cell_t af_io_get_pending_write_count(af_io_t* io) {
  af_cell_t pending_write_count;
  pthread_mutex_lock(&io->mutex);
  pending_write_count = io->pending_write_count;
  pthread_mutex_unlock(&io->mutex);
  return pending_write_count;
}

/* Set IO handler task */
void af_io_set_handler_task(af_io_t* io, af_task_t* task) {
  pthread_mutex_lock(&io->mutex);
  io->handler_task = task;
  pthread_mutex_unlock(&io->mutex);
}

/* Get IO handler taks */
af_task_t* af_io_get_handler_task(af_io_t* io) {
  return io->handler_task;
}

/* Get IO action state */
void af_io_action_get_state(af_io_action_t* action, af_io_state_t* state) {
  pthread_mutex_lock(&action->io->mutex);
  state->buffer = action->buffer;
  state->count = action->count;
  state->index = action->index;
  state->offset = action->offset;
  state->is_done = action->is_done;
  state->is_closed = action->is_closed;
  state->has_hangup = action->has_hangup;
  state->has_error = action->has_error;
  pthread_mutex_unlock(&action->io->mutex);
}

/* Get whether IO action is done */
af_bool_t af_io_state_is_done(af_io_state_t* state) {
  return state->is_done;
}

/* Get whether IO action is closed */
af_bool_t af_io_state_is_closed(af_io_state_t* state) {
  return state->is_closed;
}

/* Get whether IO action has a hangup */
af_bool_t af_io_state_has_hangup(af_io_state_t* state) {
  return state->has_hangup;
}

/* Get whether IO action has an error */
af_bool_t af_io_state_has_error(af_io_state_t* state) {
  return state->has_error;
}

/* Get IO action buffer */
af_byte_t* af_io_state_get_buffer(af_io_state_t* state) {
  return state->buffer;
}

/* Get IO action buffer index */
af_cell_t af_io_state_get_index(af_io_state_t* state) {
  return state->index;
}

/* Get IO action buffer count */
af_cell_t af_io_state_get_count(af_io_state_t* state) {
  return state->count;
}

/* Get IO action file offset */
af_sign_cell_t af_io_state_get_offset(af_io_state_t* state) {
  return state->offset;
}

/* Open a file */
af_io_fd_t af_io_open(af_byte_t* path, af_io_size_t count, af_io_flags_t flags,
		      af_io_mode_t mode, af_io_error_t* error) {
  af_byte_t* path_buffer = malloc(sizeof(af_byte_t) * (count + 1));
  af_io_fd_t fd;
  memcpy(path_buffer, path, count);
  path_buffer[count] = 0;
  if((fd = open(path_buffer, flags, mode)) >= 0) {
    *error = 0;
    free(path_buffer);
    return fd;
  } else {
    *error = errno;
    free(path_buffer);
    return -1;
  }
}

/* Open a pipe */
af_bool_t af_io_pipe(af_io_fd_t* in, af_io_fd_t* out, af_io_error_t* error) {
  af_io_fd_t fds[2];
  if(!pipe(fds)) {
    *out = fds[0];
    *in = fds[1];
    *error = 0;
    return TRUE;
  } else {
    *error = errno;
    return FALSE;
  }
}

/* Delete a file */
af_bool_t af_io_delete(af_byte_t* path, af_io_size_t count,
		       af_io_error_t* error) {
  af_byte_t* path_buffer = malloc(sizeof(af_byte_t) * (count + 1));
  memcpy(path_buffer, path, count);
  path_buffer[count] = 0;
  if(!unlink(path_buffer)) {
    *error = 0;
    free(path_buffer);
    return TRUE;
  } else {
    *error = errno;
    free(path_buffer);
    return FALSE;
  }
}

/* Delete a file */
af_bool_t af_io_delete_dir(af_byte_t* path, af_io_size_t count,
			   af_io_error_t* error) {
  af_byte_t* path_buffer = malloc(sizeof(af_byte_t) * (count + 1));
  memcpy(path_buffer, path, count);
  path_buffer[count] = 0;
  if(!unlink(path_buffer)) {
    *error = 0;
    free(path_buffer);
    return TRUE;
  } else {
    *error = errno;
    free(path_buffer);
    return FALSE;
  }
}

/* Rename a file */
af_bool_t af_io_rename(af_byte_t* path1, af_io_size_t count1,
		       af_byte_t* path2, af_io_size_t count2,
		       af_io_error_t* error) {
  af_byte_t* path_buffer1 = malloc(sizeof(af_byte_t) * (count1 + 1));
  af_byte_t* path_buffer2 = malloc(sizeof(af_byte_t) * (count2 + 1));
  memcpy(path_buffer1, path1, count1);
  path_buffer1[count1] = 0;
  memcpy(path_buffer2, path2, count2);
  path_buffer2[count2] = 0;
  if(!rename(path_buffer1, path_buffer2)) {
    *error = 0;
    free(path_buffer1);
    free(path_buffer2);
    return TRUE;
  } else {
    *error = errno;
    free(path_buffer1);
    free(path_buffer2);
    return TRUE;
  }
}

/* Detect whether a file descriptor is a terminal */
af_bool_t af_io_isatty(af_io_fd_t fd, af_io_error_t* error) {
  if(isatty(fd)) {
    *error = 0;
    return TRUE;
  } else if(errno == EINVAL || errno == ENOTTY) {
    *error = 0;
    return FALSE;
  } else {
    *error = errno;
    return FALSE;
  }
}

/* Get monotonic time */
void af_io_get_monotonic_time(af_time_t* monotonic_time) {
  struct timespec time;
  clock_gettime(CLOCK_MONOTONIC, &time);
  monotonic_time->sec = time.tv_sec;
  monotonic_time->nsec = time.tv_nsec;
}

/* Get real time */
void af_io_get_real_time(af_time_t* monotonic_time) {
  struct timespec time;
  clock_gettime(CLOCK_REALTIME, &time);
  monotonic_time->sec = time.tv_sec;
  monotonic_time->nsec = time.tv_nsec;
}

/* Sleep */
af_io_action_t* af_io_sleep(af_io_t* io, af_time_t* sleep_until,
			    af_task_t* task_to_wake) {
  af_io_action_t* action;
  if(!(action = malloc(sizeof(af_io_action_t)))) {
    return NULL;
  }
  action->type = AF_IO_TYPE_SLEEP;
  action->to_be_destroyed = FALSE;
  action->fd = 0;
  action->buffer = NULL;
  action->count = 0;
  action->index = 0;
  action->offset = 0;
  action->whence = AF_IO_SEEK_CUR;
  action->sleep_until.tv_sec = sleep_until->sec;
  action->sleep_until.tv_nsec = sleep_until->nsec;
  action->is_buffer_freeable = FALSE;
  action->task_to_wake = task_to_wake;
  action->is_done = FALSE;
  action->is_closed = FALSE;
  action->has_hangup = FALSE;
  action->has_error = FALSE;
  af_io_add(io, action);
  return action;
}

af_io_action_t* af_io_tell(af_io_t* io, af_io_fd_t fd,
			   af_task_t* task_to_wake) {
  af_io_action_t* action;
  if(!(action = malloc(sizeof(af_io_action_t)))) {
    return NULL;
  }
  action->type = AF_IO_TYPE_TELL;
  action->to_be_destroyed = FALSE;
  action->fd = fd;
  action->buffer = NULL;
  action->count = 0;
  action->index = 0;
  action->offset = 0;
  action->whence = AF_IO_SEEK_CUR;
  action->sleep_until.tv_sec = 0;
  action->sleep_until.tv_nsec = 0;
  action->is_buffer_freeable = FALSE;
  action->task_to_wake = task_to_wake;
  action->is_done = FALSE;
  action->is_closed = FALSE;
  action->has_hangup = FALSE;
  action->has_error = FALSE;
  af_io_add(io, action);
  return action;
}

/* Blocking close of file descriptor */
af_io_action_t* af_io_close_block(af_io_t* io, af_io_fd_t fd,
				  af_task_t* task_to_wake) {
  af_io_action_t* action;
  if(!(action = malloc(sizeof(af_io_action_t)))) {
    return NULL;
  }
  action->type = AF_IO_TYPE_CLOSE;
  action->to_be_destroyed = FALSE;
  action->fd = fd;
  action->buffer = NULL;
  action->count = 0;
  action->index = 0;
  action->offset = 0;
  action->whence = AF_IO_SEEK_CUR;
  action->sleep_until.tv_sec = 0;
  action->sleep_until.tv_nsec = 0;
  action->is_buffer_freeable = FALSE;
  action->task_to_wake = task_to_wake;
  action->is_done = FALSE;
  action->is_closed = FALSE;
  action->has_hangup = FALSE;
  action->has_error = FALSE;
  af_io_add(io, action);
  return action;
}

/* Asynchronous close of file descriptor */
af_io_action_t* af_io_close_async(af_io_t* io, af_io_fd_t fd) {
  af_io_action_t* action;
  if(!(action = malloc(sizeof(af_io_action_t)))) {
    return NULL;
  }
  action->type = AF_IO_TYPE_CLOSE;
  action->to_be_destroyed = FALSE;
  action->fd = fd;
  action->buffer = NULL;
  action->count = 0;
  action->index = 0;
  action->offset = 0;
  action->whence = AF_IO_SEEK_CUR;
  action->sleep_until.tv_sec = 0;
  action->sleep_until.tv_nsec = 0;
  action->is_buffer_freeable = FALSE;
  action->task_to_wake = NULL;
  action->is_done = FALSE;
  action->is_closed = FALSE;
  action->has_hangup = FALSE;
  action->has_error = FALSE;
  af_io_add(io, action);
  return action;
}

/* Blocking seek of file descriptor */
af_io_action_t* af_io_seek_block(af_io_t* io, af_io_fd_t fd, af_io_off_t offset,
				 af_io_whence_t whence,
				 af_task_t* task_to_wake) {
  af_io_action_t* action;
  if(!(action = malloc(sizeof(af_io_action_t)))) {
    return NULL;
  }
  action->type = AF_IO_TYPE_SEEK;
  action->to_be_destroyed = FALSE;
  action->fd = fd;
  action->buffer = NULL;
  action->count = 0;
  action->index = 0;
  action->offset = offset;
  action->whence = whence;
  action->sleep_until.tv_sec = 0;
  action->sleep_until.tv_nsec = 0;
  action->is_buffer_freeable = FALSE;
  action->task_to_wake = task_to_wake;
  action->is_done = FALSE;
  action->is_closed = FALSE;
  action->has_hangup = FALSE;
  action->has_error = FALSE;
  af_io_add(io, action);
  return action;
}

/* Asynchronous seek of file descriptor */
af_io_action_t* af_io_seek_async(af_io_t* io, af_io_fd_t fd, af_io_off_t offset,
				 af_io_whence_t whence) {
  af_io_action_t* action;
  if(!(action = malloc(sizeof(af_io_action_t)))) {
    return NULL;
  }
  action->type = AF_IO_TYPE_SEEK;
  action->to_be_destroyed = FALSE;
  action->fd = fd;
  action->buffer = NULL;
  action->count = 0;
  action->index = 0;
  action->offset = offset;
  action->whence = whence;
  action->sleep_until.tv_sec = 0;
  action->sleep_until.tv_nsec = 0;
  action->is_buffer_freeable = FALSE;
  action->task_to_wake = NULL;
  action->is_done = FALSE;
  action->is_closed = FALSE;
  action->has_hangup = FALSE;
  action->has_error = FALSE;
  af_io_add(io, action);
  return action;
}

/* Start blocking read */
af_io_action_t* af_io_read_block(af_io_t* io, af_io_fd_t fd, af_byte_t* buffer,
				 af_io_size_t count, af_task_t* task_to_wake) {
  af_io_action_t* action;
  if(!(action = malloc(sizeof(af_io_action_t)))) {
    return NULL;
  }
  action->type = AF_IO_TYPE_READ;
  action->to_be_destroyed = FALSE;
  action->fd = fd;
  action->buffer = buffer;
  action->count = count;
  action->index = 0;
  action->offset = 0;
  action->whence = AF_IO_SEEK_CUR;
  action->sleep_until.tv_sec = 0;
  action->sleep_until.tv_nsec = 0;
  action->is_buffer_freeable = FALSE;
  action->task_to_wake = task_to_wake;
  action->is_done = count == 0;
  action->is_closed = FALSE;
  action->has_hangup = FALSE;
  action->has_error = FALSE;
  af_fd_set_blocking(fd, FALSE);
  af_io_add(io, action);
  return action;
}

/* Start blocking write */
af_io_action_t* af_io_write_block(af_io_t* io, af_io_fd_t fd, af_byte_t* buffer,
				  af_io_size_t count, af_task_t* task_to_wake) {
  af_io_action_t* action;
  if(!(action = malloc(sizeof(af_io_action_t)))) {
    return NULL;
  }
  action->type = AF_IO_TYPE_WRITE;
  action->to_be_destroyed = FALSE;
  action->fd = fd;
  action->buffer = buffer;
  action->count = count;
  action->index = 0;
  action->offset = 0;
  action->whence = AF_IO_SEEK_CUR;
  action->sleep_until.tv_sec = 0;
  action->sleep_until.tv_nsec = 0;
  action->is_buffer_freeable = FALSE;
  action->task_to_wake = task_to_wake;
  action->is_done = count == 0;
  action->is_closed = FALSE;
  action->has_hangup = FALSE;
  action->has_error = FALSE;
  af_fd_set_blocking(fd, FALSE);
  af_io_add(io, action);
  return action;
}

/* Start asynchronous read */
af_io_action_t* af_io_read_async(af_io_t* io, af_io_fd_t fd, af_byte_t* buffer,
				 af_io_size_t count) {
  af_io_action_t* action;
  if(!(action = malloc(sizeof(af_io_action_t)))) {
    return NULL;
  }
  action->type = AF_IO_TYPE_READ;
  action->to_be_destroyed = FALSE;
  action->fd = fd;
  action->buffer = buffer;
  action->count = count;
  action->index = 0;
  action->offset = 0;
  action->whence = AF_IO_SEEK_CUR;
  action->sleep_until.tv_sec = 0;
  action->sleep_until.tv_nsec = 0;
  action->is_buffer_freeable = FALSE;
  action->task_to_wake = NULL;
  action->is_done = count == 0;
  action->is_closed = FALSE;
  action->has_hangup = FALSE;
  action->has_error = FALSE;
  af_fd_set_blocking(fd, FALSE);
  af_io_add(io, action);
  return action;
}

/* Start asynchronous write */
af_io_action_t* af_io_write_async(af_io_t* io, af_io_fd_t fd, af_byte_t* buffer,
				  af_io_size_t count) {
  af_io_action_t* action;
  af_byte_t* buffer_copy;
  if(!(action = malloc(sizeof(af_io_action_t)))) {
    return NULL;
  }
  if(!(buffer_copy = malloc(sizeof(af_byte_t) * count))) {
    free(action);
    return NULL;
  }
  memcpy(buffer_copy, buffer, sizeof(af_byte_t) * count);
  action->type = AF_IO_TYPE_WRITE;
  action->to_be_destroyed = FALSE;
  action->fd = fd;
  action->buffer = buffer_copy;
  action->count = count;
  action->index = 0;
  action->offset = 0;
  action->whence = AF_IO_SEEK_CUR;
  action->sleep_until.tv_sec = 0;
  action->sleep_until.tv_nsec = 0;
  action->is_buffer_freeable = TRUE;
  action->task_to_wake = NULL;
  action->is_done = count == 0;
  action->is_closed = FALSE;
  action->has_hangup = FALSE;
  action->has_error = FALSE;
  af_fd_set_blocking(fd, FALSE);
  af_io_add(io, action);
  return action;
}

/* Carry out non-blocking read */
ssize_t af_io_read_nonblock(int fd, af_byte_t* buffer, af_io_size_t count,
			    af_bool_t* again, af_io_error_t* error) {
  ssize_t size;
  af_fd_set_blocking(fd, FALSE);
  size = read(fd, buffer, sizeof(af_byte_t) * count);
  *again = size == -1 && (errno == EAGAIN || errno == EWOULDBLOCK ||
			  errno == EINTR) ? TRUE : FALSE;
  *error = size != -1 || (size == -1 && (errno == EAGAIN ||
					 errno == EWOULDBLOCK ||
					 errno == EINTR)) ? 0 : errno;
  return size;
}

/* Carry out non-blocking read */
ssize_t af_io_write_nonblock(int fd, af_byte_t* buffer, af_io_size_t count,
			     af_bool_t* again, af_io_error_t* error) {
  ssize_t size;
  af_fd_set_blocking(fd, FALSE);
  size = write(fd, buffer, sizeof(af_byte_t) * count);
  *again = size == -1 && (errno == EAGAIN || errno == EWOULDBLOCK ||
			  errno == EINTR) ? TRUE : FALSE;
  *error = size != -1 || (size == -1 && (errno == EAGAIN ||
					 errno == EWOULDBLOCK ||
					 errno == EINTR)) ? 0 : errno;
  return size;
}

/* Set or unset non-blocking on a file; return TRUE on success */
af_bool_t af_fd_set_blocking(int fd, af_bool_t blocking) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1) {
    return TRUE;
  }
  if (blocking) {
    flags &= ~O_NONBLOCK;
  } else {
    flags |= O_NONBLOCK;
  }
  return fcntl(fd, F_SETFL, flags) != -1 ? TRUE : FALSE;
}

/* Wake the IO manager */
void af_io_wake(af_io_t* io) {
  af_byte_t buffer;
  while(write(io->break_fd_in, &buffer, sizeof(af_byte_t)) == -1 &&
	errno == EAGAIN) {
  }
}

/* Check whether a given file descriptor is currently active */
af_bool_t af_io_is_active(af_io_t* io, af_io_fd_t fd) {
  af_io_action_t* current_action = io->first_active_action;
  while(current_action) {
    if(current_action->fd == fd && current_action->type != AF_IO_TYPE_SLEEP) {
      return TRUE;
    }
    current_action = current_action->next_action;
  }
  return FALSE;
}

/* Add IO action */
void af_io_add(af_io_t* io, af_io_action_t* action) {
  af_bool_t wake = FALSE;
  pthread_mutex_lock(&io->mutex);
  action->io = io;
  if(action->type == AF_IO_TYPE_WRITE) {
    io->pending_write_count++;
  }
  if(action->type != AF_IO_TYPE_SLEEP && af_io_is_active(io, action->fd)) {
    action->prev_action = io->last_waiting_action;
    action->next_action = NULL;
    if(action->prev_action) {
      action->prev_action->next_action = action;
    } else {
      io->first_waiting_action = action;
    }
    io->last_waiting_action = action;
  } else {
    action->prev_action = NULL;
    action->next_action = io->first_active_action;
    if(action->next_action) {
      action->next_action->prev_action = action;
    }
    io->first_active_action = action;
    io->active_action_count++;
      wake = TRUE;
  }
  pthread_mutex_unlock(&io->mutex);
  if(wake) {
    af_io_wake(io);
  }
}

/* Implements IO manager thread */
void* af_io_main(void* arg) {
  af_io_t* io = arg;
  while(!io->to_be_destroyed) {
    af_io_action_t* current_action;
    af_cell_t active_fd_count;
    struct pollfd* fds;
    af_io_action_t** actions;
    af_cell_t current_index = 0;
    pthread_mutex_lock(&io->mutex);
    af_bool_t still_looping;
    struct timespec current_time;
    struct timespec wait_time;
    int used_wait_time = -1;
    af_bool_t wait_time_set = FALSE;
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    do {
      still_looping = FALSE;
      current_action = io->first_active_action;
      while(current_action) {
	if(current_action->type == AF_IO_TYPE_CLOSE) {
	  current_action->is_done = TRUE;
	  close(current_action->fd);
	  still_looping = TRUE;
	} else if(current_action->type == AF_IO_TYPE_SEEK) {
	  current_action->is_done = TRUE;
	  if((lseek(current_action->fd, current_action->offset,
		  current_action->whence)) == -1) {
	    current_action->has_error = TRUE;
	  }
	  still_looping = TRUE;
	} else if(current_action->type == AF_IO_TYPE_TELL) {
	  current_action->is_done = TRUE;
	  if((current_action->offset =
	      lseek(current_action->fd, 0, SEEK_CUR)) == -1) {
	    current_action->has_error = TRUE;
	  }
	  still_looping = TRUE;
	} else if(current_action->type == AF_IO_TYPE_SLEEP) {
	  if(current_time.tv_sec > current_action->sleep_until.tv_sec ||
	     (current_time.tv_sec == current_action->sleep_until.tv_sec &&
	      current_time.tv_nsec > current_action->sleep_until.tv_nsec)) {
	    current_action->is_done = TRUE;
	    still_looping = TRUE;
	  } else {
	    struct timespec current_wait_time;
	    timespec_diff(&current_time, &current_action->sleep_until,
			  &current_wait_time);
	    if(wait_time_set) {
	      if(current_wait_time.tv_sec < wait_time.tv_sec ||
		 (current_wait_time.tv_sec == wait_time.tv_sec &&
		  current_wait_time.tv_nsec < wait_time.tv_nsec)) {
		wait_time.tv_sec = current_wait_time.tv_sec;
		wait_time.tv_nsec = current_wait_time.tv_nsec;
	      }
	    } else {
	      wait_time.tv_sec = current_wait_time.tv_sec;
	      wait_time.tv_nsec = current_wait_time.tv_nsec;
	      wait_time_set = TRUE;
	    }
	  }
	}
	current_action = current_action->next_action;
      }
      af_io_remove_done(io);
    } while(still_looping);
    if(!(fds = malloc(sizeof(struct pollfd) * (io->active_action_count + 1)))) {
      close(io->break_fd_out);
      close(io->break_fd_in);
      pthread_mutex_destroy(&io->mutex);
      return NULL;
    }
    if(!(actions = malloc(sizeof(af_io_action_t*) * io->active_action_count))) {
      free(fds);
      close(io->break_fd_out);
      close(io->break_fd_in);
      pthread_mutex_destroy(&io->mutex);
      return NULL;
    }
    current_action = io->first_active_action;
    while(current_action) {
      if(current_action->type != AF_IO_TYPE_CLOSE &&
	 current_action->type != AF_IO_TYPE_SEEK &&
	 current_action->type != AF_IO_TYPE_TELL &&
	 current_action->type != AF_IO_TYPE_SLEEP) {
	fds[current_index].fd = current_action->fd;
	if(current_action->type == AF_IO_TYPE_READ) {
	  fds[current_index].events = POLLIN;
	} else {
	  fds[current_index].events = POLLOUT;
	}
	actions[current_index] = current_action;
	fds[current_index++].revents = 0;
      }
      current_action = current_action->next_action;
    }
    fds[current_index].fd = io->break_fd_out;
    fds[current_index].events = POLLIN;
    fds[current_index].revents = 0;
    pthread_mutex_unlock(&io->mutex);
    if(wait_time_set) {
      used_wait_time =
	(wait_time.tv_sec * 1000) + (wait_time.tv_nsec / 1000000);
    }
    if((active_fd_count = poll(fds, current_index + 1, used_wait_time))
       != -1) {
      af_cell_t current_active_index = 0;
      pthread_mutex_lock(&io->mutex);
      clock_gettime(CLOCK_MONOTONIC, &current_time);
      current_action = io->first_active_action;
      while(current_action) {
	if(current_action->type == AF_IO_TYPE_SLEEP) {
	  if(current_time.tv_sec > current_action->sleep_until.tv_sec ||
	     (current_time.tv_sec == current_action->sleep_until.tv_sec &&
	      current_time.tv_nsec > current_action->sleep_until.tv_nsec)) {
	    current_action->is_done = TRUE;
	  }
	}
	current_action = current_action->next_action;
      }
      while(current_active_index < current_index) {
	if(fds[current_active_index].revents & POLLHUP) {
	  actions[current_active_index]->has_hangup = TRUE;
	}
	if(fds[current_active_index].revents & POLLERR) {
	  actions[current_active_index]->is_done = TRUE;
	  actions[current_active_index]->has_error = TRUE;
	}
	if(fds[current_active_index].revents & POLLIN) {
	  if(actions[current_active_index]->type == AF_IO_TYPE_READ) {
	    if(actions[current_active_index]->fd == STDIN_FILENO) {
	      af_io_prepare_stdin();
	    }
	    ssize_t size = read(actions[current_active_index]->fd,
				actions[current_active_index]->buffer +
				actions[current_active_index]->index,
				actions[current_active_index]->count -
				actions[current_active_index]->index);
	    if(size > 0) {
	      actions[current_active_index]->index += size;
	      if(actions[current_active_index]->index >=
		 actions[current_active_index]->count) {
		actions[current_active_index]->is_done = TRUE;
	      }
	    } else if (!size) {
	      actions[current_active_index]->is_done = TRUE;
	      actions[current_active_index]->is_closed = TRUE;
	    } else if(errno == EAGAIN ||
		      errno == EWOULDBLOCK ||
		      errno == EINTR) {
	      /* Do nothing */
	    } else {
	      actions[current_active_index]->is_done = TRUE;
	      actions[current_active_index]->is_closed = TRUE;
	      actions[current_active_index]->has_error = TRUE;
	    }
	  }
	}
	if(fds[current_active_index].revents & POLLOUT) {
	  if(actions[current_active_index]->type == AF_IO_TYPE_WRITE) {
	    ssize_t size = write(actions[current_active_index]->fd,
				 actions[current_active_index]->buffer +
				 actions[current_active_index]->index,
				 actions[current_active_index]->count -
				 actions[current_active_index]->index);
	    if(size > 0) {
	      actions[current_active_index]->index += size;
	      if(actions[current_active_index]->index >=
		 actions[current_active_index]->count) {
		actions[current_active_index]->is_done = TRUE;
	      }
	    } else if (!size) {
	      actions[current_active_index]->is_done = TRUE;
	      actions[current_active_index]->is_closed = TRUE;
	    } else if(errno == EAGAIN ||
		      errno == EWOULDBLOCK ||
		      errno == EINTR) {
	      /* Do nothing */
	    } else {
	      actions[current_active_index]->is_done = TRUE;
	      actions[current_active_index]->is_closed = TRUE;
	      actions[current_active_index]->has_error = TRUE;
	    }
	  }
	}
	current_active_index++;
      }
      if(fds[current_index].revents & POLLERR ||
	 fds[current_index].revents & POLLHUP) {
	free(actions);
	free(fds);
	close(io->break_fd_out);
	close(io->break_fd_in);
	pthread_mutex_destroy(&io->mutex);
	return NULL;
      }
      if(fds[current_index].revents & POLLIN) {
	af_byte_t buffer;
	ssize_t size = read(io->break_fd_out, &buffer, sizeof(af_byte_t));
	if(size > 0) {
	  /* Do nothing */
	} else if (size == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
	  /* Do nothing */
	} else {
	  free(actions);
	  free(fds);
	  close(io->break_fd_out);
	  close(io->break_fd_in);
	  pthread_mutex_destroy(&io->mutex);
	  return NULL;
	}
      }
      free(actions);
      free(fds);
      af_io_remove_done(io);
      pthread_mutex_unlock(&io->mutex);
    } else {
      free(actions);
      free(fds);
      close(io->break_fd_out);
      close(io->break_fd_in);
      pthread_mutex_destroy(&io->mutex);
      return NULL;
    }
  }
  close(io->break_fd_out);
  close(io->break_fd_in);
  pthread_mutex_destroy(&io->mutex);
  return NULL;
}

/* Remove done actions from the active action list */
void af_io_remove_done(af_io_t* io) {
  af_io_action_t* first_new_action = NULL;
  af_io_action_t* current_action = io->first_active_action;
  while(current_action) {
    af_io_action_t* next_action = current_action->next_action;
    if(current_action->is_done) {
      af_io_action_t* current_waiting_action = io->first_waiting_action;
      af_bool_t found = FALSE;
      if(current_action->type == AF_IO_TYPE_WRITE) {
	io->pending_write_count--;
      }
      if(current_action->prev_action) {
	current_action->prev_action->next_action =
	  current_action->next_action;
      } else {
	io->first_active_action = current_action->next_action;
      }
      if(current_action->next_action) {
	current_action->next_action->prev_action =
	  current_action->prev_action;
      }
      if(!current_action->to_be_destroyed) {
	current_action->prev_action = NULL;
	current_action->next_action = io->first_done_action;
	if(io->first_done_action) {
	  io->first_done_action->prev_action = current_action;
	}
	io->first_done_action = current_action;
      }
      while(current_waiting_action) {
	if(current_waiting_action->fd == current_action->fd) {
	  found = TRUE;
	  break;
	}
	current_waiting_action = current_waiting_action->next_action;
      }
      if(found) {
	if(current_waiting_action->prev_action) {
	  current_waiting_action->prev_action->next_action =
	    current_waiting_action->next_action;
	} else {
	  io->first_waiting_action = current_waiting_action->next_action;
	}
	if(current_waiting_action->next_action) {
	  current_waiting_action->next_action->prev_action =
	    current_waiting_action->prev_action;
	} else {
	  io->last_waiting_action = current_waiting_action->prev_action;
	}
	current_waiting_action->prev_action = NULL;
	current_waiting_action->next_action = io->first_active_action;
	if(current_waiting_action->next_action) {
	  current_waiting_action->next_action->prev_action =
	    current_waiting_action;
	}
	io->first_active_action = current_waiting_action;
      } else {
	io->active_action_count--;
      }
      if(current_action->task_to_wake) {
	pthread_mutex_unlock(&io->mutex);
	af_lock(io->global);
	af_wake(io->global, current_action->task_to_wake);
	if(io->handler_task) {
	  af_begin_atomic(io->global, io->handler_task);
	}
	af_unlock(io->global);
	af_cond_signal(&io->global->cond);
	pthread_mutex_lock(&io->mutex);
      } else if(io->handler_task) {
	pthread_mutex_unlock(&io->mutex);
	af_lock(io->global);
	if(io->handler_task) {
	  af_begin_atomic(io->global, io->handler_task);
	}
	af_unlock(io->global);
	af_cond_signal(&io->global->cond);
	pthread_mutex_lock(&io->mutex);
      }
      if(current_action->to_be_destroyed) {
	if(current_action->is_buffer_freeable) {
	  free(current_action->buffer);
	}
	free(current_action);
      }
    }
    current_action = next_action;
  }
}

void timespec_diff(struct timespec *start, struct timespec *stop,
                   struct timespec *result) {
  if ((stop->tv_nsec - start->tv_nsec) < 0) {
    result->tv_sec = stop->tv_sec - start->tv_sec - 1;
    result->tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
  } else {
    result->tv_sec = stop->tv_sec - start->tv_sec;
    result->tv_nsec = stop->tv_nsec - start->tv_nsec;
  }
}
