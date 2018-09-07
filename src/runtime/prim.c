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
    af_register_prim(global, thread, "EXIT", af_prim_exit, FALSE);
  af_register_prim(global, thread, "LITERAL", af_prim_literal, TRUE);
  af_register_prim(global, thread, "CREATE", af_prim_create, FALSE);
  af_register_prim(global, thread, ":", af_prim_colon, FALSE);
  af_register_prim(global, thread, ":NONAME", af_prim_colon_noname, FALSE);
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
  af_register_prim(global, thread, "NOT", af_prim_not, FALSE);
  af_register_prim(global, thread, "AND", af_prim_and, FALSE);
  af_register_prim(global, thread, "OR", af_prim_or, FALSE);
  af_register_prim(global, thread, "XOR", af_prim_xor, FALSE);
  af_register_prim(global, thread, "<", af_prim_lt, FALSE);
  af_register_prim(global, thread, "<=", af_prim_lte, FALSE);
  af_register_prim(global, thread, ">", af_prim_gt, FALSE);
  af_register_prim(global, thread, ">=", af_prim_gte, FALSE);
  af_register_prim(global, thread, "=", af_prim_eq, FALSE);
  af_register_prim(global, thread, "<>", af_prim_ne, FALSE);
  af_register_prim(global, thread, "@", af_prim_fetch, FALSE);
  af_register_prim(global, thread, "!", af_prim_store, FALSE);
  af_register_prim(global, thread, "+!", af_prim_add_store, FALSE);
  af_register_prim(global, thread, "C@", af_prim_c_fetch, FALSE);
  af_register_prim(global, thread, "C!", af_prim_c_store, FALSE);
  af_register_prim(global, thread, "2@", af_prim_2_fetch, FALSE);
  af_register_prim(global, thread, "2!", af_prim_2_store, FALSE);
  af_register_prim(global, thread, ",", af_prim_comma, FALSE);
  af_register_prim(global, thread, "C,", af_prim_c_comma, FALSE);
  af_register_prim(global, thread, "2,", af_prim_2_comma, FALSE);
  af_register_prim(global, thread, ">R", af_prim_to_r, FALSE);
  af_register_prim(global, thread, "R>", af_prim_from_r, FALSE);
  af_register_prim(global, thread, "R@", af_prim_r_fetch, FALSE);
  af_register_prim(global, thread, "HERE", af_prim_here, FALSE);
  af_register_prim(global, thread, "DOES>", af_prim_does, FALSE);
  af_register_prim(global, thread, ">BODY", af_prim_to_body, FALSE);
  af_register_prim(global, thread, "'", af_prim_tick, FALSE);
  af_register_prim(global, thread, "[']", af_prim_bracket_tick, TRUE);
  af_register_prim(global, thread, "EXECUTE", af_prim_execute, FALSE);
  af_register_prim(global, thread, "LATESTXT", af_prim_latestxt, FALSE);
  af_register_prim(global, thread, ">IN", af_prim_to_in, FALSE);
  af_register_prim(global, thread, "SOURCE", af_prim_source, FALSE);
  af_register_prim(global, thread, "EVALUATE", af_prim_evaluate, FALSE);
  af_register_prim(global, thread, "ALLOCATE", af_prim_allocate, FALSE);
  af_register_prim(global, thread, "FREE", af_prim_free, FALSE);
  af_register_prim(global, thread, "RESIZE", af_prim_resize, FALSE);
  af_register_prim(global, thread, "ALIGN", af_prim_align, FALSE);
  af_register_prim(global, thread, "ALLOT", af_prim_allot, FALSE);
  af_register_prim(global, thread, "WORD", af_prim_word, FALSE);
  af_register_prim(global, thread, "PARSE-NAME", af_prim_parse_name, FALSE);
}

/* Docol primitive */
void af_prim_docol(af_global_t* global, af_thread_t* thread) {
  if(thread->return_stack_current > thread->return_stack_top) {
    *(--thread->return_stack_current) =
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

/* Push the data address of a word primitive */
void af_prim_push_data(af_global_t* global, af_thread_t* thread) {
  if(thread->data_stack_current > thread->data_stack_top) {
    *(--thread->data_stack_current) =
      (uint64_t)thread->interpreter_pointer->compiled_call->data;
    AF_ADVANCE_IP(thread, 1);
  } else {
    af_handle_data_stack_overflow(global, thread);
  }
}

/* Runtime of DOES> primitve */
void af_prim_do_does(af_global_t* global, af_thread_t* thread) {
  if(thread->data_stack_current > thread->data_stack_top) {
    if(thread->return_stack_current > thread->return_stack_top) {
      *(--thread->data_stack_current) =
	(uint64_t)thread->interpreter_pointer->compiled_call->data;
      *(--thread->return_stack_current) =
	interpreter_pointer ? interpreter_pointer + 1 : NULL;
      thread->interpreter_pointer =
	interpreter_pointer->compiled_call->secondary;
    } else {
      af_handle_return_stack_overflow(global, thread);
    }
  } else {
    af_handle_data_stack_overflow(global, thread);
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
  slot = af_allot(global, thread, sizeof(af_compile_t) * 2);
  if(slot) {
    slot->compiled_call = global->builtin_literal_runtime;
    (slot + 1)->compiled_uint64 = *thread->data_stack_current++;
    AF_ADVANCE_IP(thread, 1);
  }
}

/* CREATE primitive */
void af_prim_create(af_global_t* global, af_thread_t* thread) {
  uint8_t* name;
  uint64_t name_length;
  size_t name_size;
  void* word_space;
  af_word_t* word;
  af_compiled_t* secondary;
  if(!af_parse_name_wait(global, thread)) {
    return;
  }
  name = af_parse_name(global, thread, &name_length);
  name_length = name_length < 256 ? name_length : 255;
  name_size = (name_length + 1) * sizeof(uint8_t);
  word_space = af_allocate(global, thread, sizeof(af_word_t) + name_size);
  *(uint8_t)(word_space + sizeof(af_word_t)) = (uint8_t)name_length;
  memmove(word_space + sizeof(af_word_t) + sizeof(uint8_t), name,
	  (size_t)name_length);
  word = word_space;
  word->is_immediate = FALSE;
  word->code = af_prim_push_data;
  word->data = word_space + name_size;
  word->secondary = NULL;
  thread->most_recent_word = word;
  global->first_word = word;
  AF_ADVANCE_IP(thread, 1);
}

/* : primitive */
void af_prim_colon(af_global_t* global, af_thread_t* thread) {
  uint8_t* name;
  uint64_t name_length;
  size_t name_size;
  void* word_space;
  af_word_t* word;
  if(!af_parse_name_wait(global, thread)) {
    return;
  }
  name = af_parse_name(global, thread, &name_length);
  name_length = name_length < 256 ? name_length : 255;
  name_size = (name_length + 1) * sizeof(uint8_t);
  word_space = af_allocate(global, thread, sizeof(af_word_t) + name_size);
  *(uint8_t)(word_space + sizeof(af_word_t)) = (uint8_t)name_length;
  memmove(word_space + sizeof(af_word_t) + sizeof(uint8_t), name,
	  (size_t)name_length);
  word = word_space;
  word->next_word = global->first_word;
  word->is_immediate = FALSE;
  word->code = af_prim_docol;
  word->data = NULL;
  word->secondary = word_space + sizeof(af_word_t) + name_size;
  thread->most_recent_word = word;
  global->first_word = word;
  thread->is_compiling = true;
  AF_ADVANCE_IP(thread, 1);
}

/* :NONAME primitive - immediate */
void af_prim_colon_noname(af_global_t* global, af_thread_t* thread) {
  void* word_space;
  af_word_t* word;
  if(thread->data_stack_current <= thread->data_stack_top) {
    af_handle_data_stack_overflow(global, thread);
    return;
  }
  word_space = af_allocate(global, thread, sizeof(af_word_t));
  word = word_space;
  word->next_word = NULL;
  word->is_immediate = FALSE;
  word->code = af_prim_docol;
  word->data = NULL;
  word->secondary = word_space + sizeof(af_word_t) + name_size;
  thread->most_recent_word = word;
  thread->is_compiling = true;
  *(--thread->data_stack_current) = (uint64_t)word;
  AF_ADVANCE_IP(thread, 1);
}

/* ; primitive - immediate */
void af_prim_semi(af_global_t* global, af_thread_t* thread) {
  if(!thread->is_compiling) {
    af_handle_compile_only(global, thread);
    return;
  }
  thread->is_compiling = false;
  AF_ADVANCE_IP(thread, 1);
}

/* IMMEDIATE primitive - immediate */
void af_prim_immediate(af_global_t* global, af_thread_t* thread) {
  if(thread->most_recent_word) {
    thread->most_recent_word->is_immediate = true;
  }
  AF_ADVANCE_IP(thread, 1);
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

/* NOT primitive */
void af_prim_not(af_global_t* global, af_thread_t* thread) {
  if(thread->data_stack_current >= thread->data_stack_base) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  *thread->data_stack_current = ~(*thread->data_stack_current);
  AF_ADVANCE_IP(thread, 1);
}

/* AND primitive */
void af_prim_and(af_global_t* global, af_thread_t* thread) {
  uint64_t value0;
  uint64_t value1;
  if(thread->data_stack_current >= (thread->data_stack_base - 1)) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  value0 = *(thread->data_stack_current + 1);
  value1 = *thread->data_stack_current;
  *(++thread->data_stack_current) = value0 & value1;
  AF_ADVANCE_IP(thread, 1);
}

/* OR primitive */
void af_prim_or(af_global_t* global, af_thread_t* thread) {
  uint64_t value0;
  uint64_t value1;
  if(thread->data_stack_current >= (thread->data_stack_base - 1)) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  value0 = *(thread->data_stack_current + 1);
  value1 = *thread->data_stack_current;
  *(++thread->data_stack_current) = value0 | value1;
  AF_ADVANCE_IP(thread, 1);
}

/* XOR primitive */
void af_prim_xor(af_global_t* global, af_thread_t* thread) {
  uint64_t value0;
  uint64_t value1;
  if(thread->data_stack_current >= (thread->data_stack_base - 1)) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  value0 = *(thread->data_stack_current + 1);
  value1 = *thread->data_stack_current;
  *(++thread->data_stack_current) = value0 ^ value1;
  AF_ADVANCE_IP(thread, 1);
}

/* < primitive */
void af_prim_lt(af_global_t* global, af_thread_t* thread) {
  int64_t value0;
  int64_t value1;
  if(thread->data_stack_current >= (thread->data_stack_base - 1)) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  value0 = *((int64_t*)thread->data_stack_current + 1);
  value1 = *(int64_t*)thread->data_stack_current;
  *(++(int64_t*)thread->data_stack_current) = value0 < value1 ? -1 : 0;
  AF_ADVANCE_IP(thread, 1);
}

/* <= primitive */
void af_prim_lte(af_global_t* global, af_thread_t* thread) {
  int64_t value0;
  int64_t value1;
  if(thread->data_stack_current >= (thread->data_stack_base - 1)) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  value0 = *((int64_t*)thread->data_stack_current + 1);
  value1 = *(int64_t*)thread->data_stack_current;
  *(++(int64_t*)thread->data_stack_current) = value0 <= value1 ? -1 : 0;
  AF_ADVANCE_IP(thread, 1);
}

/* > primitive */
void af_prim_gt(af_global_t* global, af_thread_t* thread) {
  int64_t value0;
  int64_t value1;
  if(thread->data_stack_current >= (thread->data_stack_base - 1)) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  value0 = *((int64_t*)thread->data_stack_current + 1);
  value1 = *(int64_t*)thread->data_stack_current;
  *(++(int64_t*)thread->data_stack_current) = value0 > value1 ? -1 : 0;
  AF_ADVANCE_IP(thread, 1);
}

/* >= primitive */
void af_prim_gte(af_global_t* global, af_thread_t* thread) {
  int64_t value0;
  int64_t value1;
  if(thread->data_stack_current >= (thread->data_stack_base - 1)) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  value0 = *((int64_t*)thread->data_stack_current + 1);
  value1 = *(int64_t*)thread->data_stack_current;
  *(++(int64_t*)thread->data_stack_current) = value0 >= value1 ? -1 : 0;
  AF_ADVANCE_IP(thread, 1);
}

/* = primitive */
void af_prim_eq(af_global_t* global, af_thread_t* thread) {
  int64_t value0;
  int64_t value1;
  if(thread->data_stack_current >= (thread->data_stack_base - 1)) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  value0 = *((int64_t*)thread->data_stack_current + 1);
  value1 = *(int64_t*)thread->data_stack_current;
  *(++(int64_t*)thread->data_stack_current) = value0 == value1 ? -1 : 0;
  AF_ADVANCE_IP(thread, 1);
}

/* <> primitive */
void af_prim_ne(af_global_t* global, af_thread_t* thread) {
  int64_t value0;
  int64_t value1;
  if(thread->data_stack_current >= (thread->data_stack_base - 1)) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  value0 = *((int64_t*)thread->data_stack_current + 1);
  value1 = *(int64_t*)thread->data_stack_current;
  *(++(int64_t*)thread->data_stack_current) = value0 != value1 ? -1 : 0;
  AF_ADVANCE_IP(thread, 1);
}

/* @ primitive */
void af_prim_fetch(af_global_t* global, af_thread_t* thread) {
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

/* +! primitive */
void af_prim_add_store(af_global_t* global, af_thread_t* thread) {
  if(thread->data_stack_current >= (thread->data_stack_base - 1)) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  *(uint64_t*)(*thread->data_stack_current) +=
    *(thread->data_stack_current + 1);
  thread->data_stack_current += 2;
  AF_ADVANCE_IP(thread, 1);
}

/* C@ primitive */
void af_prim_c_fetch(af_global_t* global, af_thread_t* thread) {
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

/* 2@ primitive */
void af_prim_2_fetch(af_global_t* global, af_thread_t* thread) {
  uint64_t* addr;
  if(thread->data_stack_current >= thread->data_stack_base) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  if(thread->data_stack_current <= thread->data_stack_top) {
    af_handle_data_stack_overflow(global, thread);
    return;
  }
  addr = (uint64_t*)(*thread->data_stack_current);
  *thread->data_stack_current = *(addr + 1);
  *(--thread->data_stack_current) = *addr;
  AF_ADVANCE_IP(thread, 1);
}

/* 2! primitive */
void af_prim_2_store(af_global_t* global, af_thread_t* thread) {
  uint64_t* addr;
  if(thread->data_stack_current >= (thread->data_stack_base - 2)) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  addr = (uint64_t*)(*thread->data_stack_current);
  *addr = *(thread->data_stack_current + 1);
  *(addr + 1) = *(thread->data_stack_current + 2);
  thread->data_stack_current += 3;
  AF_ADVANCE_IP(thread, 1);
}

/* , primitive */
void af_prim_comma(af_global_t* global, af_thread_t* thread) {
  uint64_t* data;
  if(thread->data_stack_current >= thread->data_stack_base) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  if((data = (uint64_t*)af_allot(global, thread, sizeof(uint64_t)))) {
    *data = *thread->data_stack_current++;
  }
  AF_ADVANCE_IP(thread, 1);
}

/* C, primitive */
void af_prim_c_comma(af_global_t* global, af_thread_t* thread) {
  uint8_t* data;
  if(thread->data_stack_current >= thread->data_stack_base) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  if((data = (uint8_t*)af_allot(global, thread, sizeof(uint8_t)))) {
    *data = (uint8_t)(*thread->data_stack_current++ & 0xFF);
  }
  AF_ADVANCE_IP(thread, 1);
}

/* 2, primitive */
void af_prim_2_comma(af_global_t* global, af_thread_t* thread) {
  uint64_t* data;
  if(thread->data_stack_current >= (thread->data_stack_base - 1)) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  if((data = (uint64_t*)af_allot(global, thread, sizeof(uint64_t) * 2))) {
    *data = *thread->data_stack_current++;
    *(data + 1) = *thread->data_stack_current++;
  }
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
void af_prim_r_fetch(af_global_t* global, af_thread_t* thread) {
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

/* DOES> primitive */
void af_prim_does(af_global_t* global, af_thread_t* thread) {
  if(!thread->most_recent_word) {
    af_handle_no_word_created(global, thread);
    return;
  }
  if(!thread->interpreter_pointer) {
    af_handle_not_interactive(global, thread);
    return;
  }
  if(thread->return_stack_current >= thread->return_stack_base) {
    af_handle_return_stack_underflow(global, thread);
    return;
  }
  thread->most_recent_word->code = af_prim_do_does;
  thread->most_recent_word->secondary = thread->interpreter_pointer + 1;
  thread->interpreter_pointer = *thread->return_stack_current++;
}

/* >BODY primitive */
void af_prim_to_body(af_global_t* global, af_thread_t* thread) {
  if(thread->data_stack_current >= thread->data_stack_base) {
    af_handle_return_stack_underflow(global, thread);
    return;
  }
  *thread->data_stack_current =
    (uint64_t)(((af_word_t*)(*thread->data_stack_current))->data);
  AF_ADVANCE_IP(thread, 1);
}

/* ' primitve */
void af_prim_tick(af_global_t* global, af_thread_t* thread) {
  uint8_t* name;
  uint64_t name_length;
  af_word_t* word;
  if(thread->data_stack_current <= thread->data_stack_top) {
    af_handle_data_stack_overflow(global, thread);
    return;
  }
  if(!af_parse_name_wait(global, thread)) {
    return;
  }
  name = af_parse_name(global, thread, &name_length);
  if(!(word = af_lookup(global, name))) {
    af_handle_word_not_found(global, thread);
    return;
  }
  *(--thread->data_stack_current) = (uint64_t)word;
  AF_ADVANCE_IP(thread, 1);
}

/* ['] primitive - immediate */
void af_prim_bracket_tick(af_global_t* global, af_thread_t* thread) {
  uint8_t* name;
  uint64_t name_length;
  af_word_t* word;
  af_compiled_t* code;
  if(!thread->is_compiling) {
    af_handle_compile_only(global, thread);
    return;
  }
  if(!thread->most_recent_word) {
    af_handle_no_word_created(global, thread);
    return;
  }
  if(!af_parse_name_wait(global, thread)) {
    return;
  }
  if(!(code = af_allot(global, thread, sizeof(af_compiled_t) * 2))) {
    return;
  }
  name = af_parse_name(global, thread, &name_length);
  if(!(word = af_lookup(global, name, name_length))) {
    af_handle_word_not_found(global, thread);
    return;
  }
  code->compiled_call = global->builtin_literal_runtime;
  (code + 1)->compiled_uint64 = (uint64_t)word;
  AF_ADVANCE_IP(thread, 1);
}

/* EXECUTE primitive */
void af_prim_execute(af_global_t* global, af_thread_t* thread) {
  af_word_t* word;
  if(thread->data_stack_current >= thread->data_stack_base) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  ((af_word_t*)(*thread->data_stack_current++))->code(global, thread);
}

/* LATESTXT primitive */
void af_prim_latestxt(af_global_t* global, af_thread_t* thread) {
  if(thread->data_stack_current <= thread->data_stack_top) {
    af_handle_data_stack_overflow(global, thread);
    return;
  }
  *(--thread->data_stack_current) = (uint64_t)thread->most_recent_word;
  AF_ADVANCE_IP(thread, 1);
}

/* >IN primitive */
void af_prim_to_in(af_global_t* global, af_thread_t* thread) {
  if(thread->data_stack_current <= thread->data_stack_top) {
    af_handle_data_stack_overflow(global, thread);
    return;
  }
  *(--thread->data_stack_current) = (uint64_t)(&thread->current_input->index);
  AF_ADVANCE_IP(thread, 1);
}

/* SOURCE primitive */
void af_prim_source(af_global_t* global, af_thread_t* thread) {
  if(thread->data_stack_current <= (thread->data_stack_top + 1)) {
    af_handle_data_stack_overflow(global, thread);
    return;
  }
  *(--thread->data_stack_current) = (uint64_t)thread->current_input->buffer;
  *(--thread->data_stack_current) = thread->current_input->count;
  AF_ADVANCE_IP(thread, 1);
}

/* EVALUATE primitive */
void af_prim_evaluate(af_global_t* global, af_thread_t* thread) {
  uint8_t* text;
  uint64_t count;
  if(thread->data_stack_current >= (thread->data_stack_base - 1)) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  count = *thread->data_stack_current++;
  text = (uint8_t*)(*thread->data_stack_current++);
  AF_ADVANCE_IP(thread, 1);
  af_evaluate(global, thread, text, count);
}

/* ALLOCATE primitive */
void af_prim_allocate(af_global_t* global, af_thread_t* thread) {
  void* data;
  if(thread->data_stack_current >= thread->data_stack_base) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  if(thread->data_stack_current <= thread->data_stack_top) {
    af_handle_data_stack_overflow(global, thread);
    return;
  }
  if(!(data = malloc((size_t)(*thread->data_stack_current)))) {
    *thread->data_stack_current = NULL;
    *(--thread->data_stack_current) = 1;
    return;
  }
  *thread->data_stack_current = (uint64_t)data;
  *(--thread->data_stack_current) = 0;
  AF_ADVANCE_IP(thread, 1);
}

/* FREE primitive */
void af_prim_free(af_global_t* global, af_thread_t* thread) {
  if(thread->data_stack_current >= thread->data_stack_base) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  free((void*)(*thread->data_stack_current));
  *thread->data_stack_current = 0;
  AF_ADVANCE_IP(thread, 1);
}

/* RESIZE primitive */
void af_prim_resize(af_global_t* global, af_thread_t* thread) {
  void* data;
  if(thread->data_stack_current >= (thread->data_stack_base - 1)) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  if(!(data = realloc((void*)(*(thread->data_stack_current + 1)),
		      (size_t)(*thread->data_stack_current)))) {
    *(thread->data_stack_current + 1) = NULL;
    *thread->data_stack_current = 1;
    return;
  }
  *(thread->data_stack_current + 1) = (uint64_t)data;
  *thread->data_stack_current = 0;
  AF_ADVANCE_IP(thread, 1);
}

/* ALIGN primitive */
void af_prim_align(af_global_t* global, af_thread_t* thread) {
  uint64_t extra = (uint64_t)thread->data_space_current % sizeof(uint64_t);
  if(extra) {
    if(!af_allot(global, thread, sizeof(uint64_t) - extra)) {
      return;
    }
  }
  AF_ADVANCE_IP(thread, 1);
}

/* ALLOT primitive */
void af_prim_allot(af_global_t* global, af_thread_t* thread) {
  if(thread->data_stack_current >= thread->data_stack_base) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  if(!af_allot(global, thread, (ssize_t)(*thread->data_stack_current++))) {
    return;
  }
  AF_ADVANCE_IP(thread, 1);
}

/* UNUSED primitive */
void af_prim_unused(af_global_t* global, af_thread_t* thread) {
  if(thread->data_stack_current <= thread->data_stack_top) {
    af_handle_data_stack_overflow(global, thread);
    return;
  }
  *(--thread->data_stack_current) =
    (uint64_t)(thread->data_stack_top - thread->data_stack_current);
  AF_ADVANCE_IP(thread, 1);
}

/* WORD primitive */
void af_prim_word(af_global_t* global, af_thread_t* thread) {
  uint8_t delimiter;
  uint8_t* text;
  if(thread->data_stack_current >= thread->data_stack_base) {
    af_handle_data_stack_underflow(global, thread);
    return;
  }
  delimiter = (uint8_t)(*thread->data_stack_current & 0xFF);
  text = af_word(global, thread, delimiter);
  *thread->data_stack_current = (uint64_t)text;
  AF_ADVANCE_IP(thread, 1);
}

/* PARSE-NAME primitive */
void af_prim_parse_name(af_global_t* global, af_thread_t* thread) {
  uint8_t* text;
  uint64_t length;
  if(thread->data_stack_current <= (thread->data_stack_top + 1)) {
    af_handle_data_stack_overflow(global, thread);
    return;
  }
  if(!af_parse_name_wait(global, thread)) {
    return;
  }
  text = af_parse_name(global, thread, &length);
  *(--thread->data_stack_current) = (uint64_t)text;
  *(--thread->data_stack_current) = length;
  AF_ADVANCE_IP(thread, 1);
}
