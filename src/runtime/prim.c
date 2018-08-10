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
#include "af/common.h"
#include "af/inner.h"

/* Next primitive */
void af_prim_next(af_global_t* global, af_thread_t* thread) {
  uint64_t* return_stack_current = --thread->return_stack_current;
  if(return_stack_current >= thread->return_stack_top) {
    *return_stack_current =
      interpreter_pointer ? interpreter_pointer + 1 : NULL;
    thread->interpreter_pointer =
      interpreter_pointer->compiled_call->secondary;
  } else {
    af_handle_return_stack_overflow(global, thread);
  }
}

/* Exit primitive */
void af_prim_exit(af_global_t* global, af_thread_t* thread) {
  if(thread->return_stack_current < thread->return_stack_base) {
    thread->interpreter_pointer = *thread->return_stack_current++;
  } else {
    af_handle_return_stack_underflow(global, thread);
  }
}

/* Literal primitive */
void af_prim_literal(af_global_t* global, af_thread_t* thread) {
  if(--thread->data_stack_current >= thread->data_stack_top) {
    *thread->data_stack_current =
      (uint64_t)(*(thread->interpreter_pointer + 1));
    thread->interpreter_pointer += 2;
  } else {
    af_handle_data_stack_overflow(global, thread);
  }
}

/* Create primitive */
void af_prim_create(af_global_t* global, af_thread_t* thread) {
  if(!af_word_wait(global, thread, ' ')) {
    return;
  }
  uint8_t* name = af_word(global, thread, ' ');
  size_t name_size = sizeof(uint8_t) * (*name + 1);
  void* word_space = af_allocate(global, thread, sizeof(af_word_t) + name_size);
  memmove(word_space + sizeof(af_word_t), name, name_size);
  af_word_t* word = word_space;
  af_compiled_t* secondary = af_allocate_compile(global, thread, 3);
  word->next_word = global->first_word;
  word->is_immediate = FALSE;
  word->name = word_space + sizeof(af_word_t);
  word->code = af_prim_next;
  word->secondary = secondary;
  secondary->compiled_call = global->builtin_literal;
  (secondary + 1)->compiled_uint64_t = (uint64_t)thread->allot_space_current;
  (secondary + 2)->compiled_call = global->builtin_exit;
  thread->most_recent_word = word;
  global->first_word = word;
  AF_ADVANCE_IP(thread, 1);
}

/* Colon definition primitive */
void af_prim_colon(af_global_t* global, af_thread_t* thread) {
  if(!af_word_wait(global, thread, ' ')) {
    return;
  }
  uint8_t* name = af_word(global, thread, ' ');
  size_t name_size = sizeof(uint8_t) * (*name + 1);
  void* word_space = af_allocate(global, thread, sizeof(af_word_t) + name_size);
  memmove(word_space + sizeof(af_word_t), name, name_size);
  af_word_t* word = word_space;
  af_compiled_t* secondary = af_allocate_compile(global, thread, 1);
  word->next_word = global->first_word;
  word->is_immediate = FALSE;
  word->name = word_space + sizeof(af_word_t);
  word->code = af_prim_next;
  word->secondary = secondary;
  secondary->compiled_call = global->builtin_exit;
  thread->most_recent_word = word;
  global->first_word = word;
  thread->is_compiling = true;
}

/* Semicolon definition primitive */
void af_prim_semi(af_global_t* global, af_thread_t* thread) {
  thread->is_compiling = true;
}

/* Immediate primitive */
void af_prim_immediate(af_global_t* global, af_thread_t* thread) {
  if(thread->most_recent_word) {
    thread->most_recent_word->is_immediate = true;
  }
}
