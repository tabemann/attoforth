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
#include "af/prim.h"

/* Register primitives */
void af_register_prims(af_global_t* global, af_thread_t* thread) {
  global->builtin_literal_runtime =
    af_register_prim(global, thread, NULL, af_prim_literal_runtime, FALSE);
  global->builtin_exit =
    af_register_prim(global, thread, NULL, af_prim_exit, FALSE);
  af_register_prim(global, thread, "LITERAL", af_prim_literal, TRUE);
  af_register_prim(global, thread, "CREATE", af_prim_create, FALSE);
  af_register_prim(global, thread, ":", af_prim_colon, TRUE);
  af_register_prim(global, thread, ";", af_prim_semi, TRUE);
  af_register_prim(global, thread, "IMMEDIATE", af_prim_immediate, TRUE);
  af_register_prim(global, thread, "DUP", af_prim_dup, FALSE);
  af_register_prim(global, thread, "DROP", af_prim_drop, FALSE);
  af_register_prim(global, thread, "SWAP", af_prim_swap, FALSE);
  af_register_prim(global, thread, "OVER", af_prim_over, FALSE);
  af_register_prim(global, thread, "ROT", af_prim_rot, FALSE);
  af_register_prim(global, thread, "PICK", af_prim_pick, FALSE);
  af_register_prim(global, thread, "ROLL", af_prim_roll, FALSE);
  af_register_prim(global, thread, "2DUP", af_prim_2dup, FALSE);
  af_register_prim(global, thread, "2DROP", af_prim_2drop, FALSE);
  af_register_prim(global, thread, "2SWAP", af_prim_2swap, FALSE);
  af_register_prim(global, thread, "2OVER", af_prim_2drop, FALSE);
  af_register_prim(global, thread, "2ROT", af_prim_2drop, FALSE);
  af_register_prim(global, thread, "+", af_prim_add, FALSE);
  af_register_prim(global, thread, "-", af_prim_sub, FALSE);
  af_register_prim(global, thread, "*", af_prim_mul, FALSE);
  af_register_prim(global, thread, "/", af_prim_div, FALSE);
  af_register_prim(global, thread, "@", af_prim_load, FALSE);
  af_register_prim(global, thread, "!", af_prim_store, FALSE);
  af_register_prim(global, thread, "C@", af_prim_c_load, FALSE);
  af_register_prim(global, thread, "C!", af_prim_c_store, FALSE);
  af_register_prim(global, thread, ">R", af_prim_to_r, FALSE);
  af_register_prim(global, thread, "R>", af_prim_from_r, FALSE);
  af_register_prim(global, thread, "R@", af_prim_r_load, FALSE);
  af_register_prim(global, thread, "HERE", af_prim_here, FALSE);
}

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

/* Literal runtime primitive */
void af_prim_literal_runtime(af_global_t* global, af_thread_t* thread) {
  if(--thread->data_stack_current >= thread->data_stack_top) {
    *thread->data_stack_current =
      (uint64_t)(*(thread->interpreter_pointer + 1));
    AF_ADVANCE_IP(thread, 2);
  } else {
    af_handle_data_stack_overflow(global, thread);
  }
}

/* LITERAL primitive - immediate */
void af_prim_literal(af_global_t* global, af_thread_t* thread) {
  af_compiled_t* slot;
  if(!thread->is_compiling) {
    af_handle_compile_only(global, thread);
    return;
  }
  if(thread->data_stack_current >= thread->data_stack_base) {
    af_handle_data_space_underflow(global, thread);
    return;
  }
  slot = af_allot_compile(global, thread, 2);
  if(slot) {
    (slot - 1)->compiled_call = global->builtin_literal_runtime;
    slot->compiled_uint64 = *thread->data_stack_current++;
    (slot + 1)->compiled_call = global->builtin_exit;
    AF_ADVANCE_IP(thread, 1);
  }
}

/* CREATE primitive */
void af_prim_create(af_global_t* global, af_thread_t* thread) {
  uint8_t* name;
  size_t name_size;
  void* word_space;
  af_word_t* word;
  af_compiled_t* secondary;
  if(!af_word_wait(global, thread, ' ')) {
    return;
  }
  name = af_word(global, thread, ' ');
  name_size = sizeof(uint8_t) * (*name + 1);
  word_space = af_allocate(global, thread, sizeof(af_word_t) + name_size);
  memmove(word_space + sizeof(af_word_t), name, name_size);
  word = word_space;
  secondary = af_allocate_compile(global, thread, 3);
  word->next_word = global->first_word;
  word->is_immediate = FALSE;
  word->name = word_space + sizeof(af_word_t);
  word->code = af_prim_next;
  word->secondary = secondary;
  secondary->compiled_call = global->builtin_literal_runtime;
  (secondary + 1)->compiled_uint64_t = (uint64_t)thread->allot_space_current;
  (secondary + 2)->compiled_call = global->builtin_exit;
  thread->most_recent_word = word;
  global->first_word = word;
  AF_ADVANCE_IP(thread, 1);
}

/* Colon definition primitive - immediate */
void af_prim_colon(af_global_t* global, af_thread_t* thread) {
  uint8_t* name;
  size_t name_size;
  void* word_space;
  af_word_t* word;
  af_compiled_t* secondary;
  if(!af_word_wait(global, thread, ' ')) {
    return;
  }
  name = af_word(global, thread, ' ');
  name_size = sizeof(uint8_t) * (*name + 1);
  word_space = af_allocate(global, thread, sizeof(af_word_t) + name_size);
  memmove(word_space + sizeof(af_word_t), name, name_size);
  word = word_space;
  secondary = af_allocate_compile(global, thread, 1);
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

/* Semicolon definition primitive - immediate */
void af_prim_semi(af_global_t* global, af_thread_t* thread) {
  if(!thread->is_compiling) {
    af_handle_compile_only(global, thread);
    return;
  }
  thread->is_compiling = false;
}

/* Immediate primitive - immediate */
void af_prim_immediate(af_global_t* global, af_thread_t* thread) {
  if(thread->most_recent_word) {
    thread->most_recent_word->is_immediate = true;
  }
}

/* DUP primitive */
void af_prim_dup(af_global_t* global, af_thread_t* thread) {
  uint64_t value;
  if(thread->data_stack_current >= thread->data_stack_base) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  if(thread->data_stack_current <= thread->data_stack_top) {
    af_handle_data_stack_overflow(global, thread);
    return;
  }
  value = *thread->data_stack_current--;
  *thread->data_stack_current = value;
  AF_ADVANCE_IP(thread, 1);
}

/* DROP primitive */
void af_prim_drop(af_global_t* global, af_thread_t* thread) {
  if(thread->data_stack_current >= thread->data_stack_base) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  thread->data_stack_current++;
  AF_ADVANCE_IP(thread, 1);
}

/* SWAP primitive */
void af_prim_swap(af_global_t* global, af_thread_t* thread) {
  uint64_t value;
  if(thread->data_stack_current >= (thread->data_stack_base - 1)) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  value = *thread->data_stack_current;
  *thread->data_stack_current = *(thread->data_stack_current + 1);
  *(thread->data_stack_current + 1) = value;
  AF_ADVANCE_IP(thread, 1);
}

/* OVER primitive */
void af_prim_over(af_global_t* global, af_thread_t* thread) {
  uint64_t value;
  if(thread->data_stack_current >= (thread->data_stack_base - 1)) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  if(thread->data_stack_current <= thread->data_stack_top) {
    af_handle_data_stack_overflow(global, thread);
    return;
  }
  value = *(thread->data_stack_current + 1);
  *(--thread->data_stack_current) = value;
  AF_ADVANCE_IP(thread, 1);
}

/* ROT primitive */
void af_prim_rot(af_global_t* global, af_thread_t* thread) {
  uint64_t value;
  if(thread->data_stack_current >= (thread->data_stack_base - 2)) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  if(thread->data_stack_current <= thread->data_stack_top) {
    af_handle_data_stack_overflow(global, thread);
    return;
  }
  value = *(thread->data_stack_current + 2);
  *(thread->data_stack_current + 2) = *(thread->data_stack_current + 1);
  *(thread->data_stack_current + 1) = *thread->data_stack_current;
  *thread->data_stack_current = value;
  AF_ADVANCE_IP(thread, 1);
}

/* PICK primitive */
void af_prim_pick(af_global_t* global, af_thread_t* thread) {
  uint64_t index;
  if(thread->data_stack_current >= thread->data_stack_base) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  index = *thread->data_stack_current;
  if(thread->data_stack_current >= (thread->data_stack_base - (index + 1))) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  *thread->data_stack_current = *(thread->data_stack_current + index + 1);
  AF_ADVANCE_IP(thread, 1);
}

/* ROLL primitive */
void af_prim_roll(af_global_t* global, af_thread_t* thread) {
  uint64_t index;
  uint64_t value;
  if(thread->data_stack_current >= thread->data_stack_base) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  index = *thread->data_stack_current++;
  if(thread->data_stack_current >= (thread->data_stack_base - index)) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  value = *(thread->data_stack_current + index);
  while(index > 0) {
    *(thread->data_stack_current + index) =
      *(thread->data_stack_current + (index - 1));
    index--;
  }
  *thread->data_stack_current = value;
  AF_ADVANCE_IP(thread, 1);
}

/* 2DUP primitive */
void af_prim_2dup(af_global_t* global, af_thread_t* thread) {
  uint64_t value0;
  uint64_t value1;
  if(thread->data_stack_current >= (thread->data_stack_base - 1)) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  if(thread->data_stack_current <= (thread->data_stack_top - 1)) {
    af_handle_data_stack_overflow(global, thread);
    return;
  }
  value0 = *(thread->data_stack_current + 1);
  value1 = *thread->data_stack_current;
  *(--thread->data_stack_current) = value0;
  *(--thread->data_stack_current) = value1;
  AF_ADVANCE_IP(thread, 1);
}

/* 2DROP primitive */
void af_prim_2drop(af_global_t* global, af_thread_t* thread) {
  if(thread->data_stack_current >= (thread->data_stack_base - 1)) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  thread->data_stack_current += 2;
  AF_ADVANCE_IP(thread, 1);
}

/* 2SWAP primitive */
void af_prim_2swap(af_global_t* global, af_thread_t* thread) {
  uint64_t value0;
  uint64_t value1;
  if(thread->data_stack_current >= (thread->data_stack_base - 3)) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  value0 = *(thread->data_stack_current + 3);
  value1 = *(thread->data_stack_current + 2);
  *(thread->data_stack_current + 3) = *(thread->data_stack_current + 1);
  *(thread->data_stack_current + 2) = *thread->data_stack_current;
  *(thread->data_stack_current + 1) = value0;
  *thread->data_stack_current = value1;
  AF_ADVANCE_IP(thread, 1);
}

/* 2OVER primitive */
void af_prim_2over(af_global_t* global, af_thread_t* thread) {
  uint64_t value0;
  uint64_t value1;
  if(thread->data_stack_current >= (thread->data_stack_base - 3)) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  if(thread->data_stack_current <= (thread->data_stack_top - 1)) {
    af_handle_data_stack_overflow(global, thread);
    return;
  }
  value0 = *(thread->data_stack_current + 3);
  value1 = *(thread->data_stack_current + 2);
  *(--thread->data_stack_current) = value0;
  *(--thread->data_stack_current) = value1;
  AF_ADVANCE_IP(thread, 1);
}

/* 2ROT primitive */
void af_prim_2rot(af_global_t* global, af_thread_t* thread) {
  uint64_t value0;
  uint64_t value1;
  if(thread->data_stack_current >= (thread->data_stack_base - 5)) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  if(thread->data_stack_current <= thread->data_stack_top) {
    af_handle_data_stack_overflow(global, thread);
    return;
  }
  value0 = *(thread->data_stack_current + 5);
  value1 = *(thread->data_stack_current + 4);
  *(thread->data_stack_current + 5) = *(thread->data_stack_current + 3);
  *(thread->data_stack_current + 4) = *(thread->data_stack_current + 2);
  *(thread->data_stack_current + 3) = *(thread->data_stack_current + 1);
  *(thread->data_stack_current + 2) = *thread->data_stack_current;
  *(thread->data_stack_current + 1) = value0;
  *thread->data_stack_current = value1;;
  AF_ADVANCE_IP(thread, 1);
}

/* + primitive */
void af_prim_add(af_global_t* global, af_thread_t* thread) {
  int64_t value0;
  int64_t value1;
  if(thread->data_stack_current >= (thread->data_stack_base - 1)) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  value0 = *((int64_t*)thread->data_stack_current + 1);
  value1 = *(int64_t*)thread->data_stack_current;
  *(++(int64_t*)thread->data_stack_current) = value0 + value1;
  AF_ADVANCE_IP(thread, 1);
}

/* - primitive */
void af_prim_sub(af_global_t* global, af_thread_t* thread) {
  int64_t value0;
  int64_t value1;
  if(thread->data_stack_current >= (thread->data_stack_base - 1)) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  value0 = *((int64_t*)thread->data_stack_current + 1);
  value1 = *(int64_t*)thread->data_stack_current;
  *(++(int64_t*)thread->data_stack_current) = value0 - value1;
  AF_ADVANCE_IP(thread, 1);
}

/* * primitive */
void af_prim_mul(af_global_t* global, af_thread_t* thread) {
  int64_t value0;
  int64_t value1;
  if(thread->data_stack_current >= (thread->data_stack_base - 1)) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  value0 = *((int64_t*)thread->data_stack_current + 1);
  value1 = *(int64_t*)thread->data_stack_current;
  *(++(int64_t*)thread->data_stack_current) = value0 * value1;
  AF_ADVANCE_IP(thread, 1);
}

/* / primitive */
void af_prim_div(af_global_t* global, af_thread_t* thread) {
  int64_t value0;
  int64_t value1;
  if(thread->data_stack_current >= (thread->data_stack_base - 1)) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  value0 = *((int64_t*)thread->data_stack_current + 1);
  value1 = *(int64_t*)thread->data_stack_current;
  if(value1 == 0) {
    af_handle_divide_by_zero(global, thread);
    return;
  }
  *(++(int64_t*)thread->data_stack_current) = value0 * value1;
  AF_ADVANCE_IP(thread, 1);
}

/* @ primitive */
void af_prim_load(af_global_t* global, af_thread_t* thread) {
  if(thread->data_stack_current >= thread->data_stack_base) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  *thread->data_stack_current = *(uint64_t*)(*thread->data_stack_current);
  AF_ADVANCE_IP(thread, 1);
}

/* ! primitive */
void af_prim_store(af_global_t* global, af_thread_t* thread) {
  if(thread->data_stack_current >= (thread->data_stack_base - 1)) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  *(uint64_t*)(*thread->data_stack_current) = *(thread->data_stack_current + 1);
  thread->data_stack_current += 2;
  AF_ADVANCE_IP(thread, 1);
}

/* C@ primitive */
void af_prim_c_load(af_global_t* global, af_thread_t* thread) {
  if(thread->data_stack_current >= thread->data_stack_base) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  *thread->data_stack_current = *(uint8_t*)(*thread->data_stack_current);
  AF_ADVANCE_IP(thread, 1);
}

/* C! primitive */
void af_prim_c_store(af_global_t* global, af_thread_t* thread) {
  if(thread->data_stack_current >= (thread->data_stack_base - 1)) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  *(uint8_t*)(*thread->data_stack_current) =
    *(thread->data_stack_current + 1) & 0xFF;
  thread->data_stack_current += 2;
  AF_ADVANCE_IP(thread, 1);
}

/* >R primitive */
void af_prim_to_r(af_global_t* global, af_thread_t* thread) {
  if(thread->data_stack_current >= thread->data_stack_base) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  if(thread->return_stack_current <= thread->return_stack_top) {
    af_handle_return_stack_overflow(global, thread);
    return;
  }
  *(--thread->return_stack_current) = (uint64_t*)*thread->data_stack_current++;
  AF_ADVANCE_IP(thread, 1);
}

/* R> primitive */
void af_prim_from_r(af_global_t* global, af_thread_t* thread) {
  if(thread->data_stack_current <= thread->data_stack_top) {
    af_handle_data_stack_overflow(global, thread);
    return;
  }
  if(thread->return_stack_current >= thread->return_stack_base) {
    af_handle_return_stack_underflow(global, thread);
    return;
  }
  *(--thread->data_stack_current) = (uint64_t)*thread->return_stack_current++;
  AF_ADVANCE_IP(thread, 1);
}

/* R@ primitive */
void af_prim_r_load(af_global_t* global, af_thread_t* thread) {
  if(thread->data_stack_current <= thread->data_stack_top) {
    af_handle_data_stack_overflow(global, thread);
    return;
  }
  if(thread->return_stack_current >= thread->return_stack_base) {
    af_handle_return_stack_underflow(global, thread);
    return;
  }
  *(--thread->data_stack_current) = (uint64_t)*thread->return_stack_current;
  AF_ADVANCE_IP(thread, 1);
}

/* HERE primitive */
void af_prim_here(af_global_t* global, af_thread_t* thread) {
  if(thread->data_stack_current <= thread->data_stack_top) {
    af_handle_data_stack_overflow(global, thread);
    return;
  }
  *(--thread->data_stack_current) = (uint64_t)thread->data_space_current;
  AF_ADVANCE_IP(thread, 1);
}
