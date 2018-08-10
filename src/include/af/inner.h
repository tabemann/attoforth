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
 *    this sopppppftware without specific prior written permission.
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

#ifndef AF_INNER_H
#define AF_INNER_H

#include <stdint.h>
#include <stddef.h>
#include "af/common.h"
#include "af/cond.h"

/* Forward declarations of types */

typedef union af_compiled_t af_compiled_t;
typedef struct af_dict_t af_dict_t;
typedef struct af_global_t af_global_t;
typedef struct af_thread_t af_thread_t;

/* Type declarations */

typedef void (*af_prim_t)(af_global_t* global, af_thread_t* thread);

typedef union af_compiled_t {
  af_word_t* compiled_call;
  uint64_t compiled_uint64_t;
  int64_t compiled_int64_t;
  double compiled_double;
} af_compiled_t;

typedef struct af_word_t {
  af_word_t* next_word;
  af_bool_t is_immediate;
  uint8_t* name;
  af_prim_t code;
  af_compiled_t* secondary;
} af_word_t;

typedef struct af_global_t {
  af_thread_t* first_thread;
  uint64_t threads_active_count;
  af_cond_t cond;
  af_word_t* first_word;
  size_t min_guaranteed_data_space_size;
  size_t default_data_space_size;
  size_t min_guaranteed_compile_space_count;
  size_t default_compile_space_count;
  uint64_t default_cycles_before_yield;
  af_word_t* builtin_literal;
  af_word_t* builtin_exit;
} af_global_t;

typedef struct af_thread_t {
  af_thread_t* next_thread;
  uint64_t base_cycles_before_yield;
  uint64_t current_cycles_before_yield;
  uint64_t current_cycles_left;
  af_bool_t is_compiling;
  af_bool_t is_to_be_freed;
  af_compiled_t* interpreter_pointer;
  uint64_t* data_stack_current;
  af_compiled_t** return_stack_current;
  uint64_t* data_stack_top;
  af_compiled_t** return_stack_top;
  uint64_t* data_stack_base;
  af_compiled_t** return_stack_base;
  af_compiled_t* compile_space_current;
  af_compiled_t* compile_space_top;
  af_compiled_t* compile_space_base;
  void* data_space_current;
  void* data_space_top;
  void* data_space_base;
  af_word_t* most_recent_word;
  uint8_t* text_input_buffer;
  uint64_t text_input_count;
  uint64_t text_input_index;
  af_bool_t text_input_closed;
  uint8_t** input_source_buffer;
  uint64_t* input_source_count;
  uint64_t* input_source_index;
  af_bool_t* input_source_closed;
  uint64_t base;
} af_thread_t;

/* Macro to advance interpreter pointer */
#define AF_ADVANCE_IP(thread, increment) \
  { thread->interpreter_pointer = thread->interpreter_pointer ? \
      thread->interpreter_pointer + increment : NULL; }

/* Function declarations */

void af_thread_loop(af_global_t* global);

void af_kill(af_thread_t* thread);

void af_yield(af_thread_t* thread);

void af_sleep(af_thread_t* thread);

void af_reset(af_global_t* global, af_thread_t* thread);

void af_handle_data_stack_overflow(af_global_t* global, af_thread_t* thread);

void af_handle_return_stack_overflow(af_global_t* global, af_thread_t* thread);

void af_handle_data_stack_underflow(af_global_t* global, af_thread_t* thread);

void af_handle_return_stack_underflow(af_global_t* global, af_thread_t* thread);

void af_handle_unexpected_input_closure(af_global_t* global,
					af_thread_t* thread);

void af_handle_data_space_overflow(af_global_t* global, af_thread_t* thread);

void af_handle_compile_space_overflow(af_global_t* global, af_thread_t* thread);

void af_handle_parse_error(af_global_t* global, af_thread_t* thread);

void* af_guarantee(af_global_t* global, af_thread_t* thread, size_t size);

void* af_allocate(af_global_t* global, af_thread_t* thread, size_t size);

void* af_allot(af_global_t* global, af_thread_t* thread, size_t size);

af_compiled_t* af_guarantee_compile(af_global_t* global, af_thread_t* thread,
				    uint64_t count);

af_compiled_t* af_allocate_compile(af_global_t* global, af_thread_t* thread,
				   uint64_t count);

af_compiled_t* af_allot_compile(af_global_t* global, af_thread_t* thread,
				uint64_t count);

af_bool_t af_word_available(af_global_t* global, af_thread_t* thread,
			    uint8_t delimiter);

af_bool_t af_word_wait(af_global_t* global, af_thread_t*, uint8_t delimiter);

uint8_t* af_word(af_global_t* global, af_thread_t* thread, uint8_t delimiter);

#endif /* AF_INNER_H */
