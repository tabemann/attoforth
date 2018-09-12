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

#ifndef AF_IO_H
#define AF_IO_H

#include <stdint.h>
#include <stddef.h>
#include <fcntl.h>
#include <unistd.h>
#include "af/types.h"

/* Constants */

#define AF_IO_TYPE_READ ((af_io_type_t)0)
#define AF_IO_TYPE_WRITE ((af_io_type_t)1)
#define AF_IO_TYPE_CLOSE ((af_io_type_t)2)
#define AF_IO_STDIN ((af_io_fd_t)STDIN_FILENO)
#define AF_IO_STDOUT ((af_io_fd_t)STDOUT_FILENO)
#define AF_IO_STDERR ((af_io_fd_t)STDERR_FILENO)
#define AF_IO_RDONLY ((af_io_flags_t)O_RDONLY)
#define AF_IO_WRONLY ((af_io_flags_t)O_WRONLY)
#define AF_IO_RDWR ((af_io_flags_t)O_RDWR)
#define AF_IO_APPEND ((af_io_flags_t)O_APPEND)
#define AF_IO_TRUNC ((af_io_flags_t)O_TRUNC)
#define AF_IO_CREAT ((af_io_flags_t)O_CREAT)
#define AF_IO_EXCL ((af_io_flags_t)O_EXCL)
#define AF_IO_IRWXU ((af_io_mode_t)00700)
#define AF_IO_IRUSR ((af_io_mode_t)00400)
#define AF_IO_IWUSR ((af_io_mode_t)00200)
#define AF_IO_IXUSR ((af_io_mode_t)00100)
#define AF_IO_IRWXG ((af_io_mode_t)00070)
#define AF_IO_IRGRP ((af_io_mode_t)00040)
#define AF_IO_IWGRP ((af_io_mode_t)00020)
#define AF_IO_IXGRP ((af_io_mode_t)00010)
#define AF_IO_IRWXO ((af_io_mode_t)00007)
#define AF_IO_IROTH ((af_io_mode_t)00004)
#define AF_IO_IWOTH ((af_io_mode_t)00002)
#define AF_IO_IXOTH ((af_io_mode_t)00001)
#define AF_IO_ISUID ((af_io_mode_t)0004000)
#define AF_IO_ISGID ((af_io_mode_t)0002000)
#define AF_IO_ISVTX ((af_io_mode_t)0001000)

/* Function declarations */

/* Initialize IO manager */
void af_io_init(af_io_t* io, af_global_t* global);

/* Destroy IO manager */
void af_io_destroy(af_io_t* io);

/* Marky IO action to be destroyed */
void af_io_action_destroy(af_io_action_t* action);

/* Get IO action state */
void af_io_action_get_state(af_io_action_t* action, af_io_state_t* state);

/* Get whether IO action is done */
af_bool_t af_io_state_is_done(af_io_state_t* state);

/* Get whether IO action is closed */
af_bool_t af_io_state_is_closed(af_io_state_t* state);

/* Get whether IO action has a hangup */
af_bool_t af_io_state_has_hangup(af_io_state_t* state);

/* Get whether IO action has an error */
af_bool_t af_io_state_has_error(af_io_state_t* state);

/* Get IO action buffer */
uint8_t* af_io_state_get_buffer(af_io_state_t* state);

/* Get IO action buffer index */
uint64_t af_io_state_get_index(af_io_state_t* state);

/* Get IO action buffer count */
uint64_t af_io_state_get_count(af_io_state_t* state);

/* Open a file */
af_io_fd_t af_io_open(uint8_t* path, uint64_t count, af_io_flags_t flags,
		      af_io_mode_t mode, af_io_error_t* error);

/* Open a pipe */
af_bool_t af_io_pipe(af_io_fd_t* in, af_io_fd_t* out, af_io_error_t* error);

/* Blocking close of file descriptor */
af_io_action_t* af_io_close_block(af_io_t* io, af_io_fd_t fd,
				  af_thread_t* thread_to_wake);

/* Asynchronous close of file descriptor */
af_io_action_t* af_io_close_async(af_io_t* io, af_io_fd_t fd);

/* Start blocking read */
af_io_action_t* af_io_read_block(af_io_t* io, af_io_fd_t fd, uint8_t* buffer,
				 uint64_t count, af_thread_t* thread_to_wake);

/* Start blocking write */
af_io_action_t* af_io_write_block(af_io_t* io, af_io_fd_t fd, uint8_t* buffer,
				  uint64_t count, af_thread_t* thread_to_wake);

/* Start asynchronous read */
af_io_action_t* af_io_read_async(af_io_t* io, af_io_fd_t fd, uint8_t* buffer,
				 uint64_t count);

/* Start asynchronous write */
af_io_action_t* af_io_write_async(af_io_t* io, af_io_fd_t fd, uint8_t* buffer,
				  uint64_t count);

/* Carry out non-blocking read */
ssize_t af_io_read_nonblock(af_io_fd_t fd, uint8_t* buffer, uint64_t count,
			    af_bool_t* again);

/* Carry out non-blocking read */
ssize_t af_io_write_nonblock(af_io_fd_t fd, uint8_t* buffer, uint64_t count,
			     af_bool_t* again);

#endif /* AF_IO_H */
