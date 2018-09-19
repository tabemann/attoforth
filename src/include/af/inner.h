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

#ifndef AF_INNER_H
#define AF_INNER_H

#include <stdint.h>
#include <stddef.h>
#include "af/common.h"
#include "af/types.h"
#include "af/cond.h"

/* Macros */

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
  (*(af_byte_t*)((af_word_t*)(word) + 1))

/* Macro to get name data of word */
#define AF_WORD_NAME_DATA(word) \
  (((af_byte_t*)((af_word_t*)(word) + 1) + sizeof(af_byte_t)))

/* Macro to execute word */
#define AF_WORD_EXECUTE(global, thread, word) \
  { af_word_t* _word_ = (word); \
    af_thread_t* _thread_ = (thread);	      \
    af_global_t* _global_ = (global); \
    (_thread_)->current_word = (_word_);	      \
    af_print_state((_global_), (_thread_));	      \
    (_word_)->code((_global_), (_thread_)); }

/* Function declarations */

af_global_t* af_global_init(void);

void af_global_execute(af_global_t* global);

void af_thread_loop(af_global_t* global);

void af_lock(af_global_t* global);

void af_unlock(af_global_t* global);

void af_print_state(af_global_t* global, af_thread_t* thread);

af_thread_t* af_spawn(af_global_t* global);

void af_set_init_word(af_global_t* global, af_thread_t* thread,
		      af_word_t* word);

void af_interpret(af_global_t* global, af_thread_t* thread);

void af_push_data(af_global_t* global, af_thread_t* thread, af_cell_t data);

void af_push_return(af_global_t* global, af_thread_t* thread,
		    af_compiled_t* pointer);

void af_drop_input(af_global_t* global, af_thread_t* thread);

void af_interactive_endline(af_global_t* global, af_thread_t* thread);

void af_start(af_global_t* global, af_thread_t* thread);

void af_kill(af_global_t* global, af_thread_t* thread);

void af_yield(af_global_t* global, af_thread_t* thread);

void af_sleep(af_global_t* global, af_thread_t* thread);

void af_wake(af_global_t* global, af_thread_t* thread);

void af_reset(af_global_t* global, af_thread_t* thread);

void af_quit(af_global_t* global, af_thread_t* thread);

void af_pop_input(af_global_t* global, af_thread_t* thread);

void af_handle_data_stack_overflow(af_global_t* global, af_thread_t* thread);

void af_handle_return_stack_overflow(af_global_t* global, af_thread_t* thread);

void af_handle_data_stack_underflow(af_global_t* global, af_thread_t* thread);

void af_handle_return_stack_underflow(af_global_t* global, af_thread_t* thread);

void af_handle_word_expected(af_global_t* global, af_thread_t* thread);

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

void* af_allot(af_global_t* global, af_thread_t* thread, ssize_t size);

af_bool_t af_word_available(af_global_t* global, af_thread_t* thread,
			    af_byte_t delimiter);

af_byte_t* af_word(af_global_t* global, af_thread_t* thread, af_byte_t delimiter);

af_bool_t af_parse_name_available(af_global_t* global, af_thread_t* thread);

af_byte_t* af_parse_name(af_global_t* global, af_thread_t* thread,
		       af_cell_t* length);

af_bool_t af_parse_number(af_global_t* global, af_byte_t* text,
			  size_t length, af_sign_cell_t* result);

void af_refill(af_global_t* global, af_thread_t* thread);

af_input_t* af_new_string_input(af_global_t* global, af_byte_t* buffer,
				af_cell_t count);

af_word_t* af_register_prim(af_global_t* global, af_thread_t* thread,
			    af_byte_t* name, af_prim_t prim,
			    af_bool_t is_immediate);

af_word_t* af_lookup(af_global_t* global, af_byte_t* name,
		     af_cell_t name_length);

#endif /* AF_INNER_H */
