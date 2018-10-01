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
#define AF_ADVANCE_IP(task, increment) \
  { (task)->interpreter_pointer = (task)->interpreter_pointer ?	\
      (task)->interpreter_pointer + (increment) : NULL; }

/* Verify data stack has room to read */
#define AF_VERIFY_DATA_STACK_READ(global, task, cells) \
  { if((task)->data_stack_current >=		    \
       ((task)->data_stack_base - ((cells) - 1))) { \
      af_handle_data_stack_underflow((global), (task));	\
      return; \
    } }

/* Verify data stack has room to expand */
#define AF_VERIFY_DATA_STACK_EXPAND(global, task, cells) \
  { if((task)->data_stack_current <=		      \
       ((task)->data_stack_top + ((cells) - 1))) {  \
      af_handle_data_stack_overflow((global), (task));	\
      return; \
    } }

/* Verify return stack has room to read */
#define AF_VERIFY_RETURN_STACK_READ(global, task, cells) \
  { if((task)->return_stack_current >=		    \
       ((task)->return_stack_base - ((cells) - 1))) { \
      af_handle_return_stack_underflow((global), (task));	\
      return; \
    } }

/* Verify return stack has room to expand */
#define AF_VERIFY_RETURN_STACK_EXPAND(global, task, cells) \
  { if((task)->return_stack_current <=		      \
       ((task)->return_stack_top + ((cells) - 1))) {  \
      af_handle_return_stack_overflow((global), (task));	\
      return; \
    } }

/* Verify that a task is compiling */
#define AF_VERIFY_COMPILING(global, task) \
  { if(!(task)->is_compiling) {	    \
      af_handle_compile_only((global), (task)); \
      return; \
    } }

/* Verify that a task is interpreting */
#define AF_VERIFY_INTERPRETING(global, task) \
  { if(!(task)->is_compiling) {	    \
      af_handle_interpret_only((global), (task)); \
      return; \
    } }

/* Verify that a task is not interactive */
#define AF_VERIFY_NOT_INTERACTIVE(global, task) \
  { if(!(task)->interpreter_pointer) {	    \
      af_handle_compile_only((global), (task)); \
      return; \
    } }

/* Verify that a word has been created in the current task */
#define AF_VERIFY_WORD_CREATED(global, task) \
  { if(!(task)->most_recent_word) {	    \
      af_handle_no_word_created((global), (task)); \
      return; \
    } }

/* Macro to get name length of word */
#define AF_WORD_NAME_LEN(word) \
  (*((af_byte_t*)(word) - 1))

/* Macro to get name data of word */
#define AF_WORD_NAME_DATA(word) \
  (((af_byte_t*)(word) - 1) - AF_WORD_NAME_LEN(word))

/* Macro to get data of word */
#define AF_WORD_DATA(word) \
  ((af_byte_t*)((af_word_t*)(word) + 1))

/* Macro to execute word */
#define AF_WORD_EXECUTE(global, task, word) \
  { af_word_t* _word_ = (word); \
    af_task_t* _task_ = (task);	      \
    af_global_t* _global_ = (global); \
    (_task_)->current_word = (_word_);	      \
    /*af_print_state((_global_), (_task_));*/ \
    (_word_)->code((_global_), (_task_)); }

/* Function declarations */

af_global_t* af_global_init(void);

void af_global_execute(af_global_t* global);

void af_task_loop(af_global_t* global);

void af_lock(af_global_t* global);

void af_unlock(af_global_t* global);

void af_print_state(af_global_t* global, af_task_t* task);

af_task_t* af_spawn(af_global_t* global, af_task_t* parent_task);

af_task_t* af_spawn_no_data(af_global_t* global, af_task_t* parent_task);

void af_set_init_word(af_global_t* global, af_task_t* task,
		      af_word_t* word);

void af_interpret(af_global_t* global, af_task_t* task);

void af_push_data(af_global_t* global, af_task_t* task, af_cell_t data);

void af_push_return(af_global_t* global, af_task_t* task,
		    af_compiled_t* pointer);

void af_start(af_global_t* global, af_task_t* task);

void af_kill(af_global_t* global, af_task_t* task);

void af_yield(af_global_t* global, af_task_t* task);

void af_wait(af_global_t* global, af_task_t* task);

void af_wake(af_global_t* global, af_task_t* task);

void af_reset(af_global_t* global, af_task_t* task);

void af_handle_data_stack_overflow(af_global_t* global, af_task_t* task);

void af_handle_return_stack_overflow(af_global_t* global, af_task_t* task);

void af_handle_data_stack_underflow(af_global_t* global, af_task_t* task);

void af_handle_return_stack_underflow(af_global_t* global, af_task_t* task);

void af_handle_word_expected(af_global_t* global, af_task_t* task);

void af_handle_data_space_overflow(af_global_t* global, af_task_t* task);

void af_handle_parse_error(af_global_t* global, af_task_t* task);

void af_handle_word_not_found(af_global_t* global, af_task_t* task);

void af_handle_out_of_memory(af_global_t* global, af_task_t* task);

void af_handle_divide_by_zero(af_global_t* global, af_task_t* task);

void af_handle_compile_only(af_global_t* global, af_task_t* task);

void af_handle_interpret_only(af_global_t* global, af_task_t* task);

void af_handle_no_word_created(af_global_t* global, af_task_t* task);

void af_handle_not_interactive(af_global_t* global, af_task_t* task);

void af_handle_wordlist_too_large(af_global_t* global, af_task_t* task);

void* af_guarantee(af_global_t* global, af_task_t* task, size_t size);

void* af_allocate(af_global_t* global, af_task_t* task, size_t size);

void* af_allot(af_global_t* global, af_task_t* task, ssize_t size);

af_bool_t af_word_available(af_global_t* global, af_task_t* task,
			    af_byte_t delimiter);

af_byte_t* af_word(af_global_t* global, af_task_t* task, af_byte_t delimiter);

af_bool_t af_parse_name_available(af_global_t* global, af_task_t* task);

af_byte_t* af_parse_name(af_global_t* global, af_task_t* task,
		       af_cell_t* length);

af_bool_t af_parse_number(af_global_t* global, af_byte_t* text,
			  size_t length, af_sign_cell_t* result);

af_input_t* af_new_string_input(af_global_t* global, af_byte_t* buffer,
				af_cell_t count);

af_word_t* af_register_prim(af_global_t* global, af_task_t* task,
			    af_byte_t* name, af_prim_t prim,
			    af_bool_t is_immediate, af_wordlist_t* wordlist);

af_word_t* af_search_wordlist(af_wordlist_t* wordlist, af_byte_t* name,
			      af_cell_t name_length);

af_word_t* af_lookup(af_global_t* global, af_task_t* task, af_byte_t* name,
		     af_cell_t name_length);

#endif /* AF_INNER_H */
