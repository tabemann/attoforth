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

/* Constants */

#define AF_WORD_IMMEDIATE (1)
#define AF_WORD_HIDDEN (2)

/* Macros */

/* Macro to advance interpreter pointer */
#define AF_ADVANCE_IP(task, increment) \
  {  af_task_t* _task_ = (task); \
     _task_->interpreter_pointer = _task_->interpreter_pointer ? \
       _task_->interpreter_pointer + (increment) : NULL; }

#ifdef DEBUG

/* Verify data stack has room to read */
#define AF_VERIFY_DATA_STACK_READ(global, task, cells) \
  { af_task_t* _task_ = (task); \
    if(_task_->data_stack_current >=		    \
       (_task_->data_stack_base - ((cells) - 1))) { \
      af_handle_data_stack_underflow((global), _task_);	\
      return; \
    } }

/* Verify data stack has room to expand */
#define AF_VERIFY_DATA_STACK_EXPAND(global, task, cells) \
  { af_task_t* _task_ = (task); \
    if(_task_->data_stack_current <=		    \
       (_task_->data_stack_top + ((cells) - 1))) {  \
      af_handle_data_stack_overflow((global), _task_);	\
      return; \
    } }

/* Verify float stack has room to read */
#define AF_VERIFY_FLOAT_STACK_READ(global, task, cells) \
  { af_task_t* _task_ = (task); \
    if(_task_->float_stack_current >=		     \
       (_task_->float_stack_base - ((cells) - 1))) { \
      af_handle_float_stack_underflow((global), _task_);	\
      return; \
    } }

/* Verify float stack has room to expand */
#define AF_VERIFY_FLOAT_STACK_EXPAND(global, task, cells) \
  { af_task_t* _task_ = (task); \
    if(_task_->float_stack_current <=		     \
       (_task_->float_stack_top + ((cells) - 1))) {  \
      af_handle_float_stack_overflow((global), _task_);	\
      return; \
    } }

/* Verify return stack has room to read */
#define AF_VERIFY_RETURN_STACK_READ(global, task, cells) \
  { af_task_t* _task_ = (task); \
    if(_task_->return_stack_current >=		      \
       (_task_->return_stack_base - ((cells) - 1))) { \
      af_handle_return_stack_underflow((global), _task_);	\
      return; \
    } }

/* Verify return stack has room to expand */
#define AF_VERIFY_RETURN_STACK_EXPAND(global, task, cells) \
  { af_task_t* _task_ = (task); \
    if(_task_->return_stack_current <=		      \
       (_task_->return_stack_top + ((cells) - 1))) {  \
      af_handle_return_stack_overflow((global), _task_);	\
      return; \
    } }

/* Verify that a task is compiling */
#define AF_VERIFY_COMPILING(global, task) \
  { af_task_t* _task_ = (task); \
    if(!_task_->is_compiling) {			\
      af_handle_compile_only((global), _task_); \
      return; \
    } }

/* Verify that a task is interpreting */
#define AF_VERIFY_INTERPRETING(global, task) \
  { af_task_t* _task_ = (task); \
    if(!_task_->is_compiling) {			  \
      af_handle_interpret_only((global), _task_); \
      return; \
    } }

/* Verify that a task is not interactive */
#define AF_VERIFY_NOT_INTERACTIVE(global, task) \
  { af_task_t* _task_ = (task); \
    if(!_task_->interpreter_pointer) {		\
      af_handle_compile_only((global), _task_); \
      return; \
    } }

/* Verify that a word has been created in the current task */
#define AF_VERIFY_WORD_CREATED(global, task) \
  { af_task_t* _task_ = (task); \
    if(!_task_->most_recent_word) {		   \
      af_handle_no_word_created((global), _task_); \
      return; \
    } }

#else /* DEBUG */

/* Verify data stack has room to read */
#define AF_VERIFY_DATA_STACK_READ(global, task, cells) \
  {  }

/* Verify data stack has room to expand */
#define AF_VERIFY_DATA_STACK_EXPAND(global, task, cells) \
  {  }

/* Verify float stack has room to read */
#define AF_VERIFY_FLOAT_STACK_READ(global, task, cells) \
  {  }

/* Verify float stack has room to expand */
#define AF_VERIFY_FLOAT_STACK_EXPAND(global, task, cells) \
  {  }

/* Verify return stack has room to read */
#define AF_VERIFY_RETURN_STACK_READ(global, task, cells) \
  {  }

/* Verify return stack has room to expand */
#define AF_VERIFY_RETURN_STACK_EXPAND(global, task, cells) \
  {  }

/* Verify that a task is compiling */
#define AF_VERIFY_COMPILING(global, task) \
  {  }

/* Verify that a task is interpreting */
#define AF_VERIFY_INTERPRETING(global, task) \
  {  }

/* Verify that a task is not interactive */
#define AF_VERIFY_NOT_INTERACTIVE(global, task) \
  {  }

/* Verify that a word has been created in the current task */
#define AF_VERIFY_WORD_CREATED(global, task) \
  {  }

#endif /* DEBUG */



/* Load an unsigned double-cell integer from data stack at cell offset */
#define AF_LOAD_2CELL(task, offset, value) \
  { af_task_t* _task_  = (task); \
    af_cell_t _offset_ = (offset); \
    af_cell_t high = *(_task_->data_stack_current + offset); \
    af_cell_t low = *(_task_->data_stack_current + offset + 1); \
    value = (af_2cell_t)low | ((af_2cell_t)high << (sizeof(af_cell_t) << 3)); }

/* Load a signed double-cell integer from data stack at cell offset */
#define AF_LOAD_SIGN_2CELL(task, offset, value) \
  { af_task_t* _task_  = (task); \
    af_cell_t _offset_ = (offset); \
    af_cell_t high = *(_task_->data_stack_current + offset); \
    af_cell_t low = *(_task_->data_stack_current + offset + 1); \
    value = (af_sign_2cell_t)((af_2cell_t)low | \
			      ((af_2cell_t)high << (sizeof(af_cell_t) << 3))); }

/* Store a double-cell integer to data stack at cell offset */
#define AF_STORE_2CELL(task, offset, value) \
  { af_task_t* _task_ = (task); \
    af_cell_t _offset_ = (offset); \
    af_2cell_t _value_ = (af_2cell_t)(value);	\
    *(_task_->data_stack_current + offset) = \
      (af_cell_t)(value >> (sizeof(af_cell_t) << 3));	\
    *(_task_->data_stack_current + offset + 1) = \
      (af_cell_t)(value & ((~(af_2cell_t)0) >> (sizeof(af_cell_t) << 3))); }

/* Macro to get name length of word */
#define AF_WORD_NAME_LEN(word) \
  (*((af_byte_t*)(word) - 1))

/* Macro to get name data of word */
#define AF_WORD_NAME_DATA(word) \
  (((af_byte_t*)(word) - 1) - AF_WORD_NAME_LEN(word))

/* Macro to get data of word */
#define AF_WORD_DATA(word) \
  ((af_byte_t*)((af_word_t*)(word) + 1))

#ifdef DEBUG

/* Macro to execute word */
#define AF_WORD_EXECUTE(global, task, word) \
  { af_word_t* _word_ = (word); \
    af_task_t* _task_ = (task);	      \
    af_global_t* _global_ = (global); \
    (_task_)->current_word = (_word_);	      \
    if(_task_->do_trace || _global_->do_trace) { \
      af_print_state((_global_), (_task_));	 \
    }					 \
    (_word_)->code((_global_), (_task_)); }

#else /* DEBUG */

/* Macro to execute word */
#define AF_WORD_EXECUTE(global, task, word) \
  { af_word_t* _word_ = (word); \
    af_task_t* _task_ = (task);	      \
    af_global_t* _global_ = (global); \
    (_task_)->current_word = (_word_);	      \
    (_word_)->code((_global_), (_task_)); }

#endif /* DEBUG */

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

void af_handle_float_stack_overflow(af_global_t* global, af_task_t* task);

void af_handle_return_stack_overflow(af_global_t* global, af_task_t* task);

void af_handle_data_stack_underflow(af_global_t* global, af_task_t* task);

void af_handle_float_stack_underflow(af_global_t* global, af_task_t* task);

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

af_bool_t af_parse_number(af_global_t* global, af_cell_t base, af_byte_t* text,
			  size_t length, af_sign_cell_t* result);

af_bool_t af_parse_2number(af_global_t* global, af_cell_t base, af_byte_t* text,
			   size_t length, af_sign_2cell_t* result);

af_bool_t af_parse_float(af_global_t* global, af_byte_t* text,
			 size_t length, af_float_t* result);

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
