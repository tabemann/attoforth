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
typedef struct af_word_t af_word_t;
typedef struct af_global_t af_global_t;
typedef struct af_thread_t af_thread_t;
typedef struct af_input_t af_input_t;

/* Type declarations */

typedef void (*af_prim_t)(af_global_t* global, af_thread_t* thread);

typedef union af_compiled_t {
  af_word_t* compiled_call;
  uint64_t compiled_uint64;
  int64_t compiled_int64;
  double compiled_double;
} af_compiled_t;

typedef struct af_word_t {
  af_word_t* next_word;
  af_bool_t is_immediate;
  af_prim_t code;
  uint64_t* data;
  af_compiled_t* secondary;
} af_word_t;

typedef struct af_global_t {
  af_thread_t* first_thread;
  uint64_t threads_active_count;
  af_cond_t cond;
  af_word_t* first_word;
  uint64_t default_data_stack_count;
  uint64_t default_return_stack_count;
  size_t min_guaranteed_data_space_size;
  size_t default_data_space_size;
  size_t min_guaranteed_compile_space_count;
  size_t default_compile_space_count;
  uint64_t default_cycles_before_yield;
  af_word_t* builtin_literal_runtime;
  af_word_t* builtin_exit;
  af_word_t* builtin_postpone_runtime;
} af_global_t;

typedef struct af_thread_t {
  af_thread_t* next_thread;
  uint64_t base_cycles_before_yield;
  uint64_t current_cycles_before_yield;
  uint64_t current_cycles_left;
  af_bool_t is_compiling;
  af_bool_t is_to_be_freed;
  af_word_t* init_word;
  af_compiled_t* interpreter_pointer;
  uint64_t* data_stack_current;
  af_compiled_t** return_stack_current;
  uint64_t* data_stack_top;
  af_compiled_t** return_stack_top;
  uint64_t* data_stack_base;
  af_compiled_t** return_stack_base;
  void* data_space_current;
  void* data_space_top;
  void* data_space_base;
  af_word_t* most_recent_word;
  af_input_t* console_input;
  af_input_t* current_input;
  uint64_t base;
  af_word_t* current_interactive_word;
  af_bool_t repeat_interactive;
} af_thread_t;

typedef struct af_input_t {
  af_input_t* next_input;
  uint8_t* buffer;
  uint64_t count;
  uint64_t index;
  af_bool_t is_closed;
  af_bool_t is_freeable;
} af_input_t;

/* Macro to advance interpreter pointer */
#define AF_ADVANCE_IP(thread, increment) \
  { (thread)->interpreter_pointer = (thread)->interpreter_pointer ?	\
      (thread)->interpreter_pointer + (increment) : NULL; }

/* Verify data stack has room to read */
#define AF_VERIFY_DATA_STACK_READ(global, thread, cells) \
  { if((thread)->data_stack_current >=		    \
       ((thread)->data_stack_base - ((cells) - 1))) { \
      af_handle_data_stack_underflow((global), (thread));	\
      return; \
    } }

/* Verify data stack has room to expand */
#define AF_VERIFY_DATA_STACK_EXPAND(global, thread, cells) \
  { if((thread)->data_stack_current <=		      \
       ((thread)->data_stack_top + ((cells) - 1))) {  \
      af_handle_data_stack_overflow((global), (thread));	\
      return; \
    } }

/* Verify return stack has room to read */
#define AF_VERIFY_RETURN_STACK_READ(global, thread, cells) \
  { if((thread)->return_stack_current >=		    \
       ((thread)->return_stack_base - ((cells) - 1))) { \
      af_handle_return_stack_underflow((global), (thread));	\
      return; \
    } }

/* Verify return stack has room to expand */
#define AF_VERIFY_RETURN_STACK_EXPAND(global, thread, cells) \
  { if((thread)->return_stack_current <=		      \
       ((thread)->return_stack_top + ((cells) - 1))) {  \
      af_handle_return_stack_overflow((global), (thread));	\
      return; \
    } }

/* Verify that a thread is compiling */
#define AF_VERIFY_COMPILING(global, thread) \
  { if(!(thread)->is_compiling) {	    \
      af_handle_compile_only((global), (thread)); \
      return; \
    } }

/* Verify that a thread is interpreting */
#define AF_VERIFY_INTERPRETING(global, thread) \
  { if(!(thread)->is_compiling) {	    \
      af_handle_interpret_only((global), (thread)); \
      return; \
    } }

/* Verify that a thread is not interactive */
#define AF_VERIFY_NOT_INTERACTIVE(global, thread) \
  { if(!(thread)->interpreter_pointer) {	    \
      af_handle_compile_only((global), (thread)); \
      return; \
    } }

/* Verify that a word has been created in the current thread */
#define AF_VERIFY_WORD_CREATED(global, thread) \
  { if(!(thread)->most_recent_word) {	    \
      af_handle_no_word_created((global), (thread)); \
      return; \
    } }

/* Macro to get name length of word */
#define AF_WORD_NAME_LEN(word) \
  (*(uint8_t*)((af_word_t*)word + sizeof(af_word_t)))

/* Macro to get name data of word */
#define AF_WORD_NAME_DATA(word) \
  ((uint8_t*)((af_word_t*)word + sizeof(af_word_t)) + sizeof(uint8_t))

/* Function declarations */

void af_thread_loop(af_global_t* global);

af_thread_t* af_spawn(af_global_t* global);

void af_set_console(af_global_t* global, af_thread_t* thread,
		    af_input_t* input);

void af_set_init_word(af_global_t* global, af_thread_t* thread,
		      af_word_t* word);

void af_interpret(af_global_t* global, af_thread_t* thread, af_input_t* input);

void af_start(af_global_t* global, af_thread_t* thread);

void af_kill(af_global_t* global, af_thread_t* thread);

void af_yield(af_global_t* global, af_thread_t* thread);

void af_sleep(af_global_t* global, af_thread_t* thread);

void af_wake(af_global_t* global, af_thread_t* thread);

void af_reset(af_global_t* global, af_thread_t* thread);

void af_quit(af_global_t* global, af_thread_t* thread);

void af_repeat_interactive(af_global_t* global, af_thread_t* thread);

void af_pop_input(af_global_t* global, af_thread_t* thread);

void af_handle_data_stack_overflow(af_global_t* global, af_thread_t* thread);

void af_handle_return_stack_overflow(af_global_t* global, af_thread_t* thread);

void af_handle_data_stack_underflow(af_global_t* global, af_thread_t* thread);

void af_handle_return_stack_underflow(af_global_t* global, af_thread_t* thread);

void af_handle_unexpected_input_closure(af_global_t* global,
					af_thread_t* thread);

void af_handle_data_space_overflow(af_global_t* global, af_thread_t* thread);

void af_handle_parse_error(af_global_t* global, af_thread_t* thread);

void af_handle_word_not_found(af_global_t* global, af_thread_t* thread);

void af_handle_out_of_memory(af_global_t* global, af_thread_t* thread);

void af_handle_divide_by_zero(af_global_t* global, af_thread_t* thread);

void af_handle_compile_only(af_global_t* global, af_thread_t* thread);

void af_handle_interpret_only(af_global_t* global, af_thread_t* thread);

void af_handle_no_word_created(af_global_t* global, af_thread_t* thread);

void af_handle_not_interactive(af_global_t* global, af_thread_t* thread);

void* af_guarantee(af_global_t* global, af_thread_t* thread, size_t size);

void* af_allocate(af_global_t* global, af_thread_t* thread, size_t size);

void* af_allot(af_global_t* global, af_thread_t* thread, size_t size);

af_bool_t af_word_available(af_global_t* global, af_thread_t* thread,
			    uint8_t delimiter);

af_bool_t af_word_wait(af_global_t* global, af_thread_t* thread,
		       uint8_t delimiter);

uint8_t* af_word(af_global_t* global, af_thread_t* thread, uint8_t delimiter);

af_bool_t af_parse_name_available(af_global_t* global, af_thread_t* thread);

af_bool_t af_parse_name_wait(af_global_t* global, af_thread_t* thread);

uint8_t* af_parse_name(af_global_t* global, af_thread_t* thread,
		       uint64_t* length);

af_input_t* af_new_string_input(uint8_t* text, uint64_t count);

void af_evaluate(af_global_t* global, af_thread_t* thread, uint8_t* text,
		 uint64_t count);

af_word_t* af_register_prim(af_global_t* global, af_thread_t* thread,
			    uint8_t* name, af_prim_t prim,
			    af_bool_t is_immediate);

#endif /* AF_INNER_H */
