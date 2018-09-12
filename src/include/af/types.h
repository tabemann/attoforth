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

#ifndef AF_TYPES_H
#define AF_TYPES_H

#include <stdint.h>
#include <stddef.h>
#include <pthread.h>

/* Forward type declarations */

typedef union af_compiled_t af_compiled_t;
typedef struct af_word_t af_word_t;
typedef struct af_global_t af_global_t;
typedef struct af_thread_t af_thread_t;
typedef struct af_input_t af_input_t;
typedef struct af_io_t af_io_t;
typedef struct af_io_action_t af_io_action_t;
typedef struct af_io_state_t af_io_state_t;
typedef struct af_cond_t af_cond_t;

/* Type definitions */

typedef uint64_t af_cell_t;

typedef int64_t af_sign_cell_t;

typedef uint8_t af_byte_t;

typedef af_sign_cell_t af_bool_t;

typedef af_cell_t af_io_type_t;

typedef int af_io_fd_t;

typedef int af_io_flags_t;

typedef mode_t af_io_mode_t;

typedef int af_io_error_t;

typedef void (*af_prim_t)(af_global_t* global, af_thread_t* thread);

typedef union af_compiled_t {
  af_word_t* compiled_call;
  af_cell_t compiled_cell;
  af_sign_cell_t compiled_sign_cell;
  double compiled_double;
} af_compiled_t;

typedef struct af_word_t {
  af_word_t* next_word;
  af_bool_t is_immediate;
  af_prim_t code;
  af_cell_t* data;
  af_compiled_t* secondary;
} af_word_t;

typedef struct af_global_t {
  af_thread_t* first_thread;
  af_cell_t threads_active_count;
  af_cond_t cond;
  pthread_mutex_t mutex;
  af_io_t io;
  af_word_t* first_word;
  af_cell_t default_data_stack_count;
  af_cell_t default_return_stack_count;
  size_t min_guaranteed_data_space_size;
  size_t default_data_space_size;
  size_t min_guaranteed_compile_space_count;
  size_t default_compile_space_count;
  af_cell_t default_cycles_before_yield;
  af_word_t* builtin_literal_runtime;
  af_word_t* builtin_exit;
  af_word_t* builtin_postpone_runtime;
} af_global_t;

typedef struct af_thread_t {
  af_thread_t* next_thread;
  af_cell_t base_cycles_before_yield;
  af_cell_t current_cycles_before_yield;
  af_cell_t current_cycles_left;
  af_bool_t is_compiling;
  af_bool_t is_to_be_freed;
  af_word_t* init_word;
  af_compiled_t* interpreter_pointer;
  af_cell_t* data_stack_current;
  af_compiled_t** return_stack_current;
  af_cell_t* data_stack_top;
  af_compiled_t** return_stack_top;
  af_cell_t* data_stack_base;
  af_compiled_t** return_stack_base;
  void* data_space_current;
  void* data_space_top;
  void* data_space_base;
  af_word_t* most_recent_word;
  af_input_t* console_input;
  af_input_t* current_input;
  af_cell_t base;
  af_word_t* current_interactive_word;
  af_bool_t repeat_interactive;
} af_thread_t;

typedef struct af_input_t {
  af_input_t* next_input;
  af_byte_t* buffer;
  af_cell_t count;
  af_cell_t index;
  af_bool_t is_closed;
  af_bool_t is_freeable;
  af_bool_t is_buffer_freeable;
  af_word_t* refill;
  af_cell_t arg;
} af_input_t;

typedef struct af_io_t {
  pthread_t pthread;
  pthread_mutex_t mutex;
  af_global_t* global;
  int break_fd_in;
  int break_fd_out;
  af_cell_t active_action_count;
  af_io_action_t* first_active_action;
  af_io_action_t* first_waiting_action;
  af_io_action_t* last_waiting_action;
  af_io_action_t* first_done_action;
  af_bool_t to_be_destroyed;
  af_bool_t ready_for_destruction;
} af_io_t;

typedef struct af_io_action_t {
  af_io_action_t* prev_action;
  af_io_action_t* next_action;
  af_io_t* io;
  af_io_type_t type;
  af_bool_t to_be_destroyed;
  int fd;
  af_byte_t* buffer;
  af_cell_t count;
  af_cell_t index;
  af_bool_t is_buffer_freeable;
  af_thread_t* thread_to_wake;
  af_bool_t is_done;
  af_bool_t is_closed;
  af_bool_t has_hangup;
  af_bool_t has_error;
} af_io_action_t;

typedef struct af_io_state_t {
  af_byte_t* buffer;
  af_cell_t count;
  af_cell_t index;
  af_bool_t is_done;
  af_bool_t is_closed;
  af_bool_t has_hangup;
  af_bool_t has_error;
} af_io_state_t;

typedef struct af_cond_t {
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  af_cell_t count;
} af_cond_t;

#endif /* AF_TYPES_H */
