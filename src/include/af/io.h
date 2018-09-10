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
#include "af/types.h"

/* Constants */

#define AF_IO_READ ((af_io_type_t)0);
#define AF_IO_WRITE ((af_io_type_t)1);

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

/* Start blocking read */
af_io_action_t* af_io_read_block(af_io_t* io, int fd, uint8_t* buffer,
				 uint64_t count, af_thread_t* thread_to_wake);

/* Start blocking write */
af_io_action_t* af_io_write_block(af_io_t* io, int fd, uint8_t* buffer,
				  uint64_t count, af_thread_t* thread_to_wake);

/* Start asynchronous read */
af_io_action_t* af_io_read_async(af_io_t* io, int fd, uint8_t* buffer,
				 uint64_t count);

/* Start asynchronous write */
af_io_action_t* af_io_write_async(af_io_t* io, int fd, uint8_t* buffer,
				  uint64_t count);

/* Carry out non-blocking read */
ssize_t af_io_read_nonblock(int fd, uint8_t* buffer, uint64_t count,
			    af_bool_t* again);

/* Carry out non-blocking read */
ssize_t af_io_write_nonblock(int fd, uint8_t* buffer, uint64_t count,
			     af_bool_t* again);

#endif /* AF_IO_H */
