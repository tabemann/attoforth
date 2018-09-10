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
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
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
af_bool_t af_io_is_active(af_io_t* io, int fd, af_io_type_t type)

/* Set or unset non-blocking on a file; return TRUE on success */
af_bool_t af_fd_set_blocking(int fd, af_bool_t blocking);

/* Implements IO manager thread */
void* af_io_main(void* arg);

/* Function definitions */

/* Initialize IO manager */
void af_io_init(af_io_t* io, af_global_t* global) {
  int pipefd[2];
  if(pthread_mutex_init(&io->mutex, NULL)) {
    return NULL;
  }
  if(pipe(pipefd)) {
    pthread_mutex_destroy(&io->mutex);
    return NULL;
  }
  io->global = global;
  io->break_fd_out = pipefd[0];
  io->break_fd_in = pipefd[1];
  if(!af_set_nonblocking(io->break_fd_in, TRUE)) {
    close(io->break_fd_out);
    close(io->break_fd_in);
    pthread_mutex_destroy(&io->mutex);
    return NULL;
  }
  io->active_action_count = 0;
  io->first_active_action = NULL;
  io->first_waiting_action = NULL;
  io->last_waiting_action = NULL;
  io->first_done_action = NULL;
  io->to_be_destroyed = FALSE;
  if(pthread_create(&io->pthread, NULL, af_io_main, io)) {
    close(io->break_fd_out);
    close(io->break_fd_in);
    pthread_mutex_destroy(&io->mutex);
    return NULL;
  }
  return io;
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
    if(action->next_acton) {
      action->next_action->prev_action = action->prev_action;
    }
    free(action);
  } else {
    action->to_be_destroyed = TRUE;
  }
  pthread_mutex_unlock(&io->mutex);
}

/* Get IO action state */
void af_io_action_get_state(af_io_action_t* action, af_io_state_t* state) {
  pthread_mutex_lock(&action->io->mutex);
  state->buffer = action->buffer;
  state->count = action->count;
  state->index = action->index;
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
uint8_t* af_io_state_get_buffer(af_io_state_t* state) {
  return state->buffer;
}

/* Get IO action buffer index */
uint64_t af_io_state_get_index(af_io_state_t* state) {
  return state->index;
}

/* Get IO action buffer count */
uint64_t af_io_state_get_count(af_io_state_t* state) {
  return state->count;
}

/* Start blocking read */
af_io_action_t* af_io_read_block(af_io_t* io, int fd, uint8_t* buffer,
				 uint64_t count, af_thread_t* thread_to_wake) {
  af_io_action_t* action;
  if(!(action = malloc(sizeof(af_io_action_t)))) {
    return NULL;
  }
  action->type = AF_IO_READ;
  action->to_be_destroyed = FALSE;
  action->fd = fd;
  action->buffer = buffer;
  action->count = count;
  action->index = 0;
  action->is_buffer_freeable = FALSE;
  action->thread_to_wake = thread_to_wake;
  action->is_done = count == 0;
  action->is_closed = FALSE;
  action->has_hangup = FALSE;
  action->has_error = FALSE;
  af_fd_set_nonblocking(fd, TRUE);
  af_io_add(io, action);
  return action;
}

/* Start blocking write */
af_io_action_t* af_io_write_block(af_io_t* io, int fd, uint8_t* buffer,
				  uint64_t count, af_thread_t* thread_to_wake) {
  af_io_action_t* action;
  if(!(action = malloc(sizeof(af_io_action_t)))) {
    return NULL;
  }
  action->type = AF_IO_WRITE;
  action->to_be_destroyed = FALSE;
  action->fd = fd;
  action->buffer = buffer;
  action->count = count;
  action->index = 0;
  action->is_buffer_freeable = FALSE;
  action->thread_to_wake = thread_to_wake;
  action->is_done = count == 0;
  action->is_closed = FALSE;
  action->has_hangup = FALSE;
  action->has_error = FALSE;
  af_fd_set_nonblocking(fd, TRUE);
  af_io_add(io, action);
  return action;
}

/* Start asynchronous read */
af_io_action_t* af_io_read_async(af_io_t* io, int fd, uint8_t* buffer,
				 uint64_t count) {
  af_io_action_t* action;
  if(!(action = malloc(sizeof(af_io_action_t)))) {
    return NULL;
  }
  action->type = AF_IO_READ;
  action->to_be_destroyed = FALSE;
  action->fd = fd;
  action->buffer = buffer;
  action->count = count;
  action->index = 0;
  action->is_buffer_freeable = FALSE;
  action->thread_to_wake = NULL;
  action->is_done = count == 0;
  action->is_closed = FALSE;
  action->has_hangup = FALSE;
  action->has_error = FALSE;
  af_fd_set_nonblocking(fd, TRUE);
  af_io_add(io, action);
  return action;
}

/* Start asynchronous write */
af_io_action_t* af_io_write_async(af_io_t* io, int fd, uint8_t* buffer,
				  uint64_t count) {
  af_io_action_t* action;
  uint8_t* buffer_copy;
  if(!(action = malloc(sizeof(af_io_action_t)))) {
    return NULL;
  }
  if(!(buffer_copy = malloc(sizeof(uint8_t) * count))) {
    free(action);
    return NULL;
  }
  memcpy(buffer_copy, buffer, sizeof(uint8_t) * count);
  action->type = AF_IO_WRITE;
  action->to_be_destroyed = FALSE;
  action->fd = fd;
  action->buffer = buffer_copy;
  action->count = count;
  action->index = 0;
  action->is_buffer_freeable = TRUE;
  action->thread_to_wake = NULL;
  action->is_done = count == 0;
  action->is_closed = FALSE;
  action->has_hangup = FALSE;
  action->has_error = FALSE;
  af_fd_set_nonblocking(fd, TRUE);
  af_io_add(io, action);
  return action;
}

/* Carry out non-blocking read */
ssize_t af_io_read_nonblock(int fd, uint8_t* buffer, uint64_t count,
			    af_bool_t* again) {
  ssize_t size;
  af_fd_set_nonblocking(fd, TRUE);
  size = read(fd, buffer, sizeof(uint8_t) * count);
  *again = size == -1 && (errno == EAGAIN || errno == EWOULDBLOCK ||
			  errno = EINTR) ? TRUE : FALSE;
  return size;
}

/* Carry out non-blocking read */
ssize_t af_io_write_nonblock(int fd, uint8_t* buffer, uint64_t count,
			     af_bool_t* again) {
  ssize_t size;
  af_fd_set_nonblocking(fd, TRUE);
  size = write(fd, buffer, sizeof(uint8_t) * count);
  *again = size == -1 && (errno == EAGAIN || errno == EWOULDBLOCK ||
			  errno = EINTR) ? TRUE : FALSE;
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
  uint8_t buffer;
  while(write(io->break_fd_in, &buffer, sizeof(uint8_t)) == -1 &&
	errno == EAGAIN) {
  }
}

/* Check whether a given file descriptor is currently active */
af_bool_t af_io_is_active(af_io_t* io, int fd, af_io_type_t type) {
  af_io_action_t* current_action = io->first_active_action;
  while(current_action) {
    if(current_action->type == type && current_action->fd == fd) {
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
  if(count > 0) {
    if(af_io_is_active(io, fd, action->type)) {
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
  } else {
    action->prev_action = NULL;
    action->next_action = io->first_done_action;
    if(action->next_action) {
      action->next_action->prev_action = action;
    }
    io->first_done_action = action;
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
    int* fds;
    af_io_action_t** read_actions;
    af_io_action_t** write_actions;
    af_io_action_t* current_action;
    uint64_t current_index = 0;
    uint64_t active_fd_count;
    struct pollfd* pollfds
    pthread_mutex_lock(&io->mutex);
    if(!(fds = malloc(sizeof(int) * io->active_action_count))) {
      close(io->break_fd_out);
      close(io->break_fd_in);
      pthread_mutex_destroy(&io->mutex);
      return;
    }
    if(!(read_actions = malloc(sizeof(af_io_action_t*) *
			       io->active_action_count))) {
      free(fds);
      close(io->break_fd_out);
      close(io->break_fd_in);
      pthread_mutex_destroy(&io->mutex);
      return;
    }
    memset(read_actions, 0, sizeof(af_io_action_t*) * io->active_action_count);
    if(!(write_actions = malloc(sizeof(af_io_action_t*) *
				io->active_action_count))) {
      free(read_actions);
      close(io->break_fd_out);
      close(io->break_fd_in);
      pthread_mutex_destroy(&io->mutex);
      return;
    }
    memset(write_actions, 0, sizeof(af_io_action_t*) * io->active_action_count);
    current_action = io->first_active_action;
    while(current_action) {
      uint64_t current_search_index = 0;
      af_bool_t found = FALSE;
      while(current_search_index < current_index) {
	if(current_action->fd == fds[current_search_index]) {
	  if(current_action->type == AF_IO_READ) {
	    read_actions[current_search_index] = current_action;
	  } else {
	    write_actions[current_search_index] = current_action;
	  }
	  found = TRUE;
	  break;
	} else {
	  current_search_index++;
	}
      }
      if(!found) {
	fds[current_index] = current_action->fd;
	if(current_action->type == AF_IO_READ) {
	  read_actions[current_index] = current_action;
	} else {
	  write_actions[current_index] = current_action;
	}
	current_index++;
      }
      current_action = current_action->next_action;
    }
    if(!(pollfds = malloc(sizeof(struct pollfd) * (current_index + 1)))) {
      free(write_actions);
      free(read_actions);
      close(io->break_fd_out);
      close(io->break_fd_in);
      pthread_mutex_destroy(&io->mutex);
      return;
    }
    for(int i = 0; i < current_index; i++) {
      pollfds[i].fd = fds[i];
      if(read_actions[i] && write_actions[i]) {
	pollfds[i].events = POLLIN | POLLOUT;
      } else if(read.actions[i]) {
	pollfds[i].events = POLLIN;
      } else {
	pollfds[i].events = POLLOUT;
      }
      pollfds[i].revents = 0;
    }
    pollfds[current_index].fd = io->break_fd_out;
    pollfds[current_index].events = POLLIN;
    pollfds[current_index].revents = 0;
    pthread_mutex_unlock(&io->mutex);
    if((active_fd_count = poll(pollfds, current_index + 1, -1)) != -1) {
      pthread_mutex_lock(&io->mutex);
      uint64_t current_active_index = 0;
      while(active_fd_count && current_active_index < current_index) {
	af_bool_t has_event = FALSE;
	if(pollfds[current_active_index].revents & POLLHUP) {
	  if(read_actions[current_active_index]) {
	    read_actions[current_active_index]->has_hangup = TRUE;
	  }
	  if(write_actions[current_active_index]) {
	    write_actions[current_active_index]->has_hangup = TRUE;
	  }
	  has_event = TRUE;
	}
	if(pollfds[current_active_index].revents & POLLERR) {
	  if(read_actions[current_active_index]) {
	    read_actions[current_active_index]->is_done = TRUE;
	    read_actions[current_active_index]->has_error = TRUE;
	  }
	  if(write_actions[current_active_index]) {
	    write_actions[current_active_index]->is_done = TRUE;
	    write_actions[current_active_index]->has_error = TRUE;
	  }
	  has_event = TRUE;
	}
	if(pollfds[current_active_index].revents & POLLIN) {
	  if(read_action[current_active_index]) {
	    ssize_t size = read(read_action[current_active_index]->fd,
				read_action[current_active_index]->buffer +
				read_action[current_active_index]->index,
				read_action[current_active_index]->count -
				read_action[current_active_index]->index);
	    if(size > 0) {
	      read_action[current_active_index]->index += size;
	      if(read_action[current_active_index]->index >=
		 read_action[current_active_index]->count) {
		read_action[current_active_index]->is_done = TRUE;
	      }
	    } else if (!size) {
	      read_action[current_active_index]->is_done = TRUE;
	      read_action[current_active_index]->is_closed = TRUE;
	    } else if(errno == EAGAIN ||
		      errno == EWOULDBLOCK ||
		      errno == EINTR) {
	      /* Do nothing */
	    } else {
	      read_action[current_active_index]->is_done = TRUE;
	      read_action[current_active_index]->is_closed = TRUE
	      read_action[current_active_index]->has_error = TRUE;
	    }
	  }
	  has_event = TRUE;
	}
	if(pollfds[current_active_index].revents & POLLOUT) {
	  if(write_action[current_active_index]) {
	    ssize_t size = write(write_action[current_active_index]->fd,
				 write_action[current_active_index]->buffer +
				 write_action[current_active_index]->index,
				 write_action[current_active_index]->count -
				 write_action[current_active_index]->index);
	    if(size > 0) {
	      write_action[current_active_index]->index += size;
	      if(write_action[current_active_index]->index >=
		 write_action[current_active_index]->count) {
		write_action[current_active_index]->is_done = TRUE;
	      }
	    } else if (!size) {
	      write_action[current_active_index]->is_done = TRUE;
	      write_action[current_active_index]->is_closed = TRUE;
	    } else if(errno == EAGAIN ||
		      errno == EWOULDBLOCK ||
		      errno == EINTR) {
	      /* Do nothing */
	    } else {
	      write_action[current_active_index]->is_done = TRUE;
	      write_action[current_active_index]->is_closed = TRUE
	      write_action[current_active_index]->has_error = TRUE;
	    }
	  }
	  has_event = TRUE;
	}
	if(has_event) {
	  active_fd_count--;
	}
	current_active_index++;
      }
      if(active_fd_count) {
	if(pollfds[current_index].revents & POLLERR ||
	   pollfds[current_index].revents & POLLHUP) {
	    free(pollfds);
	    free(write_actions);
	    free(read_actions);
	    close(io->break_fd_out);
	    close(io->break_fd_in);
	    pthread_mutex_destroy(&io->mutex);
	    return;
	}
	if(pollfds[current_index].revents & POLLIN) {
	  uint8_t buffer;
	  ssize_t size = read(io->break_fd_out, &buffer, sizeof(uint8_t));
	  if(size > 0) {
	    /* Do nothing */
	  } else {
	    free(pollfds);
	    free(write_actions);
	    free(read_actions);
	    close(io->break_fd_out);
	    close(io->break_fd_in);
	    pthread_mutex_destroy(&io->mutex);
	    return;
	  }
	}
      }
      free(pollfds);
      free(write_actions);
      free(read_actions);
      af_io_remove_done(io);
      pthread_mutex_unlock(&io->mutex);
    } else {
      free(pollfds);
      free(write_actions);
      free(read_actions);
      close(io->break_fd_out);
      close(io->break_fd_in);
      pthread_mutex_destroy(&io->mutex);
      return;
    }
  }
  close(io->break_fd_out);
  close(io->break_fd_in);
  pthread_mutex_destroy(&io->mutex);
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
      }
      io->first_done_action = current_action;
      while(current_waiting_action) {
	if(current_waiting_action->fd == current_action->fd &&
	   current_waiting_action->type == current_action->type) {
	  found = TRUE;
	  break;
	}
	current_waiting_action = current_waiting_action->next_acction;
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
      }
      if(current_action->thread_to_wake) {
	af_lock(io->global);
	af_wake(current_action->thread_to_wake);
	af_unlock(io->global);
	af_cond_signal(&io->global->cond);
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
