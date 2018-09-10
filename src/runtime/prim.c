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
#include "af/types.h"
#include "af/inner.h"
#include "af/io.h"
#include "af/prim.h"

/* Function forward declarations */

/* Docol primitive */
void af_prim_docol(af_global_t* global, af_thread_t* thread);

/* EXIT primitive */
void af_prim_exit(af_global_t* global, af_thread_t* thread);

/* Push the data address of a word primitive */
void af_prim_push_data(af_global_t* global, af_thread_t* thread);

/* Runtime of DOES> primitve */
void af_prim_do_does(af_global_t* global, af_thread_t* thread);

/* (LITERAL) primitive */
void af_prim_literal_runtime(af_global_t* global, af_thread_t* thread);

/* CREATE primitive */
void af_prim_create(af_global_t* global, af_thread_t* thread);

/* : primitive */
void af_prim_colon(af_global_t* global, af_thread_t* thread);

/* :NONAME primitive */
void af_prim_colon_noname(af_global_t* global, af_thread_t* thread);

/* ; primitive - immediate */
void af_prim_semi(af_global_t* global, af_thread_t* thread);

/* IMMEDIATE primitive */
void af_prim_immediate(af_global_t* global, af_thread_t* thread);

/* DUP primitive */
void af_prim_dup(af_global_t* global, af_thread_t* thread);

/* DROP primitive */
void af_prim_drop(af_global_t* global, af_thread_t* thread);

/* SWAP primitive */
void af_prim_swap(af_global_t* global, af_thread_t* thread);

/* OVER primitive */
void af_prim_over(af_global_t* global, af_thread_t* thread);

/* ROT primitive */
void af_prim_rot(af_global_t* global, af_thread_t* thread);

/* PICK primitive */
void af_prim_pick(af_global_t* global, af_thread_t* thread);

/* ROLL primitive */
void af_prim_roll(af_global_t* global, af_thread_t* thread);

/* 2DUP primitive */
void af_prim_2dup(af_global_t* global, af_thread_t* thread);

/* 2DROP primitive */
void af_prim_2drop(af_global_t* global, af_thread_t* thread);

/* 2SWAP primitive */
void af_prim_2swap(af_global_t* global, af_thread_t* thread);

/* 2OVER primitive */
void af_prim_2over(af_global_t* global, af_thread_t* thread);

/* 2ROT primitive */
void af_prim_2rot(af_global_t* global, af_thread_t* thread);

/* + primitive */
void af_prim_add(af_global_t* global, af_thread_t* thread);

/* - primitive */
void af_prim_sub(af_global_t* global, af_thread_t* thread);

/* * primitive */
void af_prim_mul(af_global_t* global, af_thread_t* thread);

/* / primitive */
void af_prim_div(af_global_t* global, af_thread_t* thread);

/* INVERT primitive */
void af_prim_invert(af_global_t* global, af_thread_t* thread);

/* AND primitive */
void af_prim_and(af_global_t* global, af_thread_t* thread);

/* OR primitive */
void af_prim_or(af_global_t* global, af_thread_t* thread);

/* XOR primitive */
void af_prim_xor(af_global_t* global, af_thread_t* thread);

/* < primitive */
void af_prim_lt(af_global_t* global, af_thread_t* thread);

/* <= primitive */
void af_prim_lte(af_global_t* global, af_thread_t* thread);

/* > primitive */
void af_prim_gt(af_global_t* global, af_thread_t* thread);

/* >= primitive */
void af_prim_gte(af_global_t* global, af_thread_t* thread);

/* = primitive */
void af_prim_eq(af_global_t* global, af_thread_t* thread);

/* <> primitive */
void af_prim_ne(af_global_t* global, af_thread_t* thread);

/* @ primitive */
void af_prim_fetch(af_global_t* global, af_thread_t* thread);

/* ! primitive */
void af_prim_store(af_global_t* global, af_thread_t* thread);

/* +! primitive */
void af_prim_add_store(af_global_t* global, af_thread_t* thread);

/* C@ primitive */
void af_prim_c_fetch(af_global_t* global, af_thread_t* thread);

/* C! primitive */
void af_prim_c_store(af_global_t* global, af_thread_t* thread);

/* 2@ primitive */
void af_prim_2_fetch(af_global_t* global, af_thread_t* thread);

/* 2! primitive */
void af_prim_2_store(af_global_t* global, af_thread_t* thread);

/* , primitive */
void af_prim_comma(af_global_t* global, af_thread_t* thread);

/* C, primitive */
void af_prim_c_comma(af_global_t* global, af_thread_t* thread);

/* 2, primitive */
void af_prim_2_comma(af_global_t* global, af_thread_t* thread);

/* >R primitive */
void af_prim_to_r(af_global_t* global, af_thread_t* thread);

/* R> primitive */
void af_prim_from_r(af_global_t* global, af_thread_t* thread);

/* R@ primitive */
void af_prim_r_fetch(af_global_t* global, af_thread_t* thread);

/* 2>R primitive */
void af_prim_two_to_r(af_global_t* global, af_thread_t* thread);

/* 2R> primitve */
void af_prim_two_from_r(af_global_t* global, af_thread_t* thread);

/* 2R@ primitive */
void af_prim_two_r_fetch(af_global_t* global, af_thread_t* thread);

/* HERE primitive */
void af_prim_here(af_global_t* global, af_thread_t* thread);

/* DOES> primitive */
void af_prim_does(af_global_t* global, af_thread_t* thread);

/* >BODY primitive */
void af_prim_to_body(af_global_t* global, af_thread_t* thread);

/* ' primitve */
void af_prim_tick(af_global_t* global, af_thread_t* thread);

/* ['] primitive - immediate */
void af_prim_bracket_tick(af_global_t* global, af_thread_t* thread);

/* EXECUTE primitive */
void af_prim_execute(af_global_t* global, af_thread_t* thread);

/* LATESTXT primitive */
void af_prim_latestxt(af_global_t* global, af_thread_t* thread);

/* >IN primitive */
void af_prim_to_in(af_global_t* global, af_thread_t* thread);

/* SOURCE primitive */
void af_prim_source(af_global_t* global, af_thread_t* thread);

/* EVALUATE primitive */
void af_prim_evaluate(af_global_t* global, af_thread_t* thread);

/* ALLOCATE primitive */
void af_prim_allocate(af_global_t* global, af_thread_t* thread);

/* FREE primitive */
void af_prim_free(af_global_t* global, af_thread_t* thread);

/* RESIZE primitive */
void af_prim_resize(af_global_t* global, af_thread_t* thread);

/* ALIGN primitive */
void af_prim_align(af_global_t* global, af_thread_t* thread);

/* ALLOT primitive */
void af_prim_allot(af_global_t* global, af_thread_t* thread);

/* UNUSED primitive */
void af_prim_unused(af_global_t* global, af_thread_t* thread);

/* WORD primitive */
void af_prim_word(af_global_t* global, af_thread_t* thread);

/* PARSE-NAME primitive */
void af_prim_parse_name(af_global_t* global, af_thread_t* thread);

/* THIS-THREAD primitive */
void af_prim_this_thread(af_global_t* global, af_thread_t* thread);

/* SPAWN-EXECUTE primitive */
void af_prim_spawn_execute(af_global_t* global, af_thread_t* thread);

/* SPAWN-EVALUATE primitive */
void af_prim_spawn_evaluate(af_global_t* global, af_thread_t* thread);

/* KILL primitive */
void af_prim_kill(af_global_t* global, af_thread_t* thread);

/* YIELD primitive */
void af_prim_yield(af_global_t* global, af_thread_t* thread);

/* SLEEP primitive */
void af_prim_sleep(af_global_t* global, af_thread_t* thread);

/* WAKE primitive */
void af_prim_wake(af_global_t* global, af_thread_t* thread);

/* RESET primitive */
void af_prim_reset(af_global_t* global, af_thread_t* thread);

/* QUIT primitive */
void af_prim_quit(af_global_t* global, af_thread_t* thread);

/* ABORT primitive */
void af_prim_abort(af_global_t* global, af_thread_t* thread);

/* [ primitive - immediate */
void af_prim_open_bracket(af_global_t* global, af_thread_t* thread);

/* ] primitive */
void af_prim_close_bracket(af_global_t* global, af_thread_t* thread);

/* POSTPONE primitive - immediate */
void af_prim_postpone(af_global_t* global, af_thread_t* thread);

/* POSTPONE runtime primitive */
void af_prim_postpone_runtime(af_global_t* global, af_thread_t* thread);

/* BRANCH primitive */
void af_prim_branch(af_global_t* global, af_thread_t* thread);

/* 0BRANCH primitive */
void af_prim_0branch(af_global_t* global, af_thread_t* thread);

/* STATE primitive */
void af_prim_state(af_global_t* global, af_thread_t* thread);

/* MOVE primitive */
void af_prim_move(af_global_t* global, af_thread_t* thread);

/* DEPTH primitive */
void af_prim_depth(af_global_t* global, af_thread_t* thread);

/* IO-ACTION-DESTROY primitive */
void af_prim_io_action_destroy(af_global_t* global, af_thread_t* thread);

/* IO-ACTION-GET-STATE primitive */
void af_prim_io_action_get_state(af_global_t* global, af_thread_t* thread);

/* IO-STATE-DESTROY primitive */
void af_prim_io_state_destroy(af_global_t* global, af_thread_t* thread);

/* IO-STATE-IS-DONE primitive */
void af_prim_io_state_is_done(af_global_t* global, af_thread_t* thread);

/* IO-STATE-IS-CLOSED primitive */
void af_prim_io_state_is_closed(af_global_t* global, af_thread_t* thread);

/* IO-STATE-HAS-HANGUP primitive */
void af_prim_io_state_has_hangup(af_global_t* global, af_thread_t* thread);

/* IO-STATE-HAS-ERROR primitive */
void af_prim_io_state_has_error(af_global_t* global, af_thread_t* thread);

/* IO-STATE-GET-BUFFER primitive */
void af_prim_io_state_get_buffer(af_global_t* global, af_thread_t* thread);

/* IO-STATE-GET-INDEX primitive */
void af_prim_io_state_get_index(af_global_t* global, af_thread_t* thread);

/* IO-STATE-GET-COUNT primitive */
void af_prim_io_state_get_count(af_global_t* global, af_thread_t* thread);

/* IO-READ-BLOCK primitive */
void af_prim_io_read_block(af_global_t* global, af_thread_t* thread);

/* IO-WRITE-BLOCK primitive */
void af_prim_io_write_block(af_global_t* global, af_thread_t* thread);

/* IO-READ-ASYNC primitive */
void af_prim_io_read_async(af_global_t* global, af_thread_t* thread);

/* IO-WRITE-ASYNC primitive */
void af_prim_io_write_async(af_global_t* global, af_thread_t* thread);

/* IO-READ-NONBLOCK primitive */
void af_prim_io_read_nonblock(af_global_t* global, af_thread_t* thread);

/* IO-WRITE-NONBLOCK primitive */
void af_prim_io_write_nonblock(af_global_t* global, af_thread_t* thread);

/* Function definitions */

/* Register primitives */
void af_register_prims(af_global_t* global, af_thread_t* thread) {
  global->builtin_literal_runtime =
    af_register_prim(global, thread, "(LITERAL)", af_prim_literal_runtime,
		     FALSE);
  global->builtin_exit =
    af_register_prim(global, thread, "EXIT", af_prim_exit, FALSE);
  global->builtin_postpone_runtime =
    af_register_prim(global, thread, NULL, af_prim_postpone_runtime, FALSE);
  af_register_prim(global, thread, "CREATE", af_prim_create, FALSE);
  af_register_prim(global, thread, ":", af_prim_colon, FALSE);
  af_register_prim(global, thread, ":NONAME", af_prim_colon_noname, FALSE);
  af_register_prim(global, thread, ";", af_prim_semi, TRUE);
  af_register_prim(global, thread, "IMMEDIATE", af_prim_immediate, FALSE);
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
  af_register_prim(global, thread, "INVERT", af_prim_invert, FALSE);
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
  af_register_prim(global, thread, "2>R", af_prim_two_to_r, FALSE);
  af_register_prim(global, thread, "2R>", af_prim_two_from_r, FALSE);
  af_register_prim(global, thread, "2R@", af_prim_two_r_fetch, FALSE);
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
  af_register_prim(global, thread, "THIS-THREAD", af_prim_this_thread, FALSE);
  af_register_prim(global, thread, "SPAWN-EXECUTE", af_prim_spawn_execute,
		   FALSE);
  af_register_prim(global, thread, "SPAWN-EVALUATE", af_prim_spawn_evaluate,
		   FALSE);
  af_register_prim(global, thread, "KILL", af_prim_kill, FALSE);
  af_register_prim(global, thread, "YIELD", af_prim_yield, FALSE);
  af_register_prim(global, thread, "SLEEP", af_prim_sleep, FALSE);
  af_register_prim(global, thread, "WAKE", af_prim_wake, FALSE);
  af_register_prim(global, thread, "RESET", af_prim_reset, FALSE);
  af_register_prim(global, thread, "QUIT", af_prim_quit, FALSE);
  af_register_prim(global, thread, "ABORT", af_prim_abort, FALSE);
  af_register_prim(global, thread, "[", af_prim_open_bracket, TRUE);
  af_register_prim(global, thread, "]", af_prim_close_bracket, FALSE);
  af_register_prim(global, thread, "POSTPONE", af_prim_postpone, TRUE);
  af_register_prim(global, thread, "BRANCH", af_prim_branch, FALSE);
  af_register_prim(global, thread, "0BRANCH", af_prim_0branch, FALSE);
  af_register_prim(global, thread, "STATE", af_prim_state, FALSE);
  af_register_prim(global, thread, "MOVE", af_prim_move, FALSE);
  af_register_prim(global, thread, "DEPTH", af_prim_depth, FALSE);
  af_register_prim(global, thread, "IO-ACTION-DESTROY",
		   af_prim_io_action_destroy, FALSE);
  af_register_prim(global, thread, "IO-ACTION-GET-STATE",
		   af_prim_io_action_get_state, FALSE);
  af_register_prim(global, thread, "IO-STATE-DESTROY", af_prim_io_state_destroy,
		   FALSE);
  af_register_prim(global, thread, "IO-STATE-IS-DONE",
		   af_prim_io_state_is_done, FALSE);
  af_register_prim(global, thread, "IO-STATE-IS-CLOSED",
		   af_prim_io_state_is_closed, FALSE);
  af_register_prim(global, thread, "IO-STATE-HAS-HANGUP",
		   af_prim_io_state_has_hangup, FALSE);
  af_register_prim(global, thread, "IO-STATE-HAS-ERROR",
		   af_prim_io_state_has_error, FALSE);
  af_register_prim(global, thread, "IO-STATE-GET-BUFFER",
		   af_prim_io_state_get_buffer, FALSE);
  af_register_prim(global, thread, "IO-STATE-GET-INDEX",
		   af_prim_io_state_get_index, FALSE);
  af_register_prim(global, thread, "IO-STATE-GET-COUNT",
		   af_prim_io_state_get_count, FALSE);
  af_register_prim(global, thread, "IO-READ-BLOCK", af_prim_io_read_block,
		   FALSE);
  af_register_prim(global, thread, "IO-WRITE-BLOCK", af_prim_io_write_block,
		   FALSE);
  af_register_prim(global, thread, "IO-READ-ASYNC", af_prim_io_read_async,
		   FALSE);
  af_register_prim(global, thread, "IO-WRITE-ASYNC", af_prim_io_write_async,
		   FALSE);
  af_register_prim(global, thread, "IO-READ-NONBLOCK", af_prim_io_read_nonblock,
		   FALSE);
  af_register_prim(global, thread, "IO-WRITE-NONBLOCK",
		   af_prim_io_write_nonblock, FALSE);
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

/* (LITERAL) primitive */
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
  AF_VERIFY_COMPILING(global, thread);
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
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
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
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
  AF_VERIFY_COMPILING(global, thread);
  thread->is_compiling = false;
  AF_ADVANCE_IP(thread, 1);
}

/* IMMEDIATE primitive */
void af_prim_immediate(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_WORD_CREATED(global, thread);
  thread->most_recent_word->is_immediate = true;
  AF_ADVANCE_IP(thread, 1);
}

/* DUP primitive */
void af_prim_dup(af_global_t* global, af_thread_t* thread) {
  uint64_t value;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  value = *thread->data_stack_current;
  *(--thread->data_stack_current) = value;
  AF_ADVANCE_IP(thread, 1);
}

/* DROP primitive */
void af_prim_drop(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  thread->data_stack_current++;
  AF_ADVANCE_IP(thread, 1);
}

/* SWAP primitive */
void af_prim_swap(af_global_t* global, af_thread_t* thread) {
  uint64_t value;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  value = *thread->data_stack_current;
  *thread->data_stack_current = *(thread->data_stack_current + 1);
  *(thread->data_stack_current + 1) = value;
  AF_ADVANCE_IP(thread, 1);
}

/* OVER primitive */
void af_prim_over(af_global_t* global, af_thread_t* thread) {
  uint64_t value;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  value = *(thread->data_stack_current + 1);
  *(--thread->data_stack_current) = value;
  AF_ADVANCE_IP(thread, 1);
}

/* ROT primitive */
void af_prim_rot(af_global_t* global, af_thread_t* thread) {
  uint64_t value;
  AF_VERIFY_DATA_STACK_READ(global, thread, 3);
  value = *(thread->data_stack_current + 2);
  *(thread->data_stack_current + 2) = *(thread->data_stack_current + 1);
  *(thread->data_stack_current + 1) = *thread->data_stack_current;
  *thread->data_stack_current = value;
  AF_ADVANCE_IP(thread, 1);
}

/* PICK primitive */
void af_prim_pick(af_global_t* global, af_thread_t* thread) {
  uint64_t index;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  index = *thread->data_stack_current;
  AF_VERIFY_DATA_STACK_READ(global, thread, index + 2);
  *thread->data_stack_current = *(thread->data_stack_current + index + 1);
  AF_ADVANCE_IP(thread, 1);
}

/* ROLL primitive */
void af_prim_roll(af_global_t* global, af_thread_t* thread) {
  uint64_t index;
  uint64_t value;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  index = *thread->data_stack_current++;
  AF_VERIFY_DATA_STACK_READ(global, thread, index);
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
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 2);
  value0 = *(thread->data_stack_current + 1);
  value1 = *thread->data_stack_current;
  *(--thread->data_stack_current) = value0;
  *(--thread->data_stack_current) = value1;
  AF_ADVANCE_IP(thread, 1);
}

/* 2DROP primitive */
void af_prim_2drop(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  thread->data_stack_current += 2;
  AF_ADVANCE_IP(thread, 1);
}

/* 2SWAP primitive */
void af_prim_2swap(af_global_t* global, af_thread_t* thread) {
  uint64_t value0;
  uint64_t value1;
  AF_VERIFY_DATA_STACK_READ(global, thread, 4);
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
  AF_VERIFY_DATA_STACK_READ(global, thread, 4);
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 2);
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
  AF_VERIFY_DATA_STACK_READ(global, thread, 6);
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
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  value0 = *((int64_t*)thread->data_stack_current + 1);
  value1 = *(int64_t*)thread->data_stack_current;
  *(++(int64_t*)thread->data_stack_current) = value0 + value1;
  AF_ADVANCE_IP(thread, 1);
}

/* - primitive */
void af_prim_sub(af_global_t* global, af_thread_t* thread) {
  int64_t value0;
  int64_t value1;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  value0 = *((int64_t*)thread->data_stack_current + 1);
  value1 = *(int64_t*)thread->data_stack_current;
  *(++(int64_t*)thread->data_stack_current) = value0 - value1;
  AF_ADVANCE_IP(thread, 1);
}

/* * primitive */
void af_prim_mul(af_global_t* global, af_thread_t* thread) {
  int64_t value0;
  int64_t value1;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  value0 = *((int64_t*)thread->data_stack_current + 1);
  value1 = *(int64_t*)thread->data_stack_current;
  *(++(int64_t*)thread->data_stack_current) = value0 * value1;
  AF_ADVANCE_IP(thread, 1);
}

/* / primitive */
void af_prim_div(af_global_t* global, af_thread_t* thread) {
  int64_t value0;
  int64_t value1;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  value0 = *((int64_t*)thread->data_stack_current + 1);
  value1 = *(int64_t*)thread->data_stack_current;
  if(value1 == 0) {
    af_handle_divide_by_zero(global, thread);
    return;
  }
  *(++(int64_t*)thread->data_stack_current) = value0 * value1;
  AF_ADVANCE_IP(thread, 1);
}

/* INVERT primitive */
void af_prim_invert(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  *thread->data_stack_current = ~(*thread->data_stack_current);
  AF_ADVANCE_IP(thread, 1);
}

/* AND primitive */
void af_prim_and(af_global_t* global, af_thread_t* thread) {
  uint64_t value0;
  uint64_t value1;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  value0 = *(thread->data_stack_current + 1);
  value1 = *thread->data_stack_current;
  *(++thread->data_stack_current) = value0 & value1;
  AF_ADVANCE_IP(thread, 1);
}

/* OR primitive */
void af_prim_or(af_global_t* global, af_thread_t* thread) {
  uint64_t value0;
  uint64_t value1;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  value0 = *(thread->data_stack_current + 1);
  value1 = *thread->data_stack_current;
  *(++thread->data_stack_current) = value0 | value1;
  AF_ADVANCE_IP(thread, 1);
}

/* XOR primitive */
void af_prim_xor(af_global_t* global, af_thread_t* thread) {
  uint64_t value0;
  uint64_t value1;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  value0 = *(thread->data_stack_current + 1);
  value1 = *thread->data_stack_current;
  *(++thread->data_stack_current) = value0 ^ value1;
  AF_ADVANCE_IP(thread, 1);
}

/* < primitive */
void af_prim_lt(af_global_t* global, af_thread_t* thread) {
  int64_t value0;
  int64_t value1;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  value0 = *((int64_t*)thread->data_stack_current + 1);
  value1 = *(int64_t*)thread->data_stack_current;
  *(++(int64_t*)thread->data_stack_current) = value0 < value1 ? -1 : 0;
  AF_ADVANCE_IP(thread, 1);
}

/* <= primitive */
void af_prim_lte(af_global_t* global, af_thread_t* thread) {
  int64_t value0;
  int64_t value1;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  value0 = *((int64_t*)thread->data_stack_current + 1);
  value1 = *(int64_t*)thread->data_stack_current;
  *(++(int64_t*)thread->data_stack_current) = value0 <= value1 ? -1 : 0;
  AF_ADVANCE_IP(thread, 1);
}

/* > primitive */
void af_prim_gt(af_global_t* global, af_thread_t* thread) {
  int64_t value0;
  int64_t value1;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  value0 = *((int64_t*)thread->data_stack_current + 1);
  value1 = *(int64_t*)thread->data_stack_current;
  *(++(int64_t*)thread->data_stack_current) = value0 > value1 ? -1 : 0;
  AF_ADVANCE_IP(thread, 1);
}

/* >= primitive */
void af_prim_gte(af_global_t* global, af_thread_t* thread) {
  int64_t value0;
  int64_t value1;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  value0 = *((int64_t*)thread->data_stack_current + 1);
  value1 = *(int64_t*)thread->data_stack_current;
  *(++(int64_t*)thread->data_stack_current) = value0 >= value1 ? -1 : 0;
  AF_ADVANCE_IP(thread, 1);
}

/* = primitive */
void af_prim_eq(af_global_t* global, af_thread_t* thread) {
  int64_t value0;
  int64_t value1;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  value0 = *((int64_t*)thread->data_stack_current + 1);
  value1 = *(int64_t*)thread->data_stack_current;
  *(++(int64_t*)thread->data_stack_current) = value0 == value1 ? -1 : 0;
  AF_ADVANCE_IP(thread, 1);
}

/* <> primitive */
void af_prim_ne(af_global_t* global, af_thread_t* thread) {
  int64_t value0;
  int64_t value1;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  value0 = *((int64_t*)thread->data_stack_current + 1);
  value1 = *(int64_t*)thread->data_stack_current;
  *(++(int64_t*)thread->data_stack_current) = value0 != value1 ? -1 : 0;
  AF_ADVANCE_IP(thread, 1);
}

/* @ primitive */
void af_prim_fetch(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  *thread->data_stack_current = *(uint64_t*)(*thread->data_stack_current);
  AF_ADVANCE_IP(thread, 1);
}

/* ! primitive */
void af_prim_store(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  *(uint64_t*)(*thread->data_stack_current) = *(thread->data_stack_current + 1);
  thread->data_stack_current += 2;
  AF_ADVANCE_IP(thread, 1);
}

/* +! primitive */
void af_prim_add_store(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  *(uint64_t*)(*thread->data_stack_current) +=
    *(thread->data_stack_current + 1);
  thread->data_stack_current += 2;
  AF_ADVANCE_IP(thread, 1);
}

/* C@ primitive */
void af_prim_c_fetch(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  *thread->data_stack_current = *(uint8_t*)(*thread->data_stack_current);
  AF_ADVANCE_IP(thread, 1);
}

/* C! primitive */
void af_prim_c_store(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  *(uint8_t*)(*thread->data_stack_current) =
    *(thread->data_stack_current + 1) & 0xFF;
  thread->data_stack_current += 2;
  AF_ADVANCE_IP(thread, 1);
}

/* 2@ primitive */
void af_prim_2_fetch(af_global_t* global, af_thread_t* thread) {
  uint64_t* addr;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  addr = (uint64_t*)(*thread->data_stack_current);
  *thread->data_stack_current = *(addr + 1);
  *(--thread->data_stack_current) = *addr;
  AF_ADVANCE_IP(thread, 1);
}

/* 2! primitive */
void af_prim_2_store(af_global_t* global, af_thread_t* thread) {
  uint64_t* addr;
  AF_VERIFY_DATA_STACK_READ(global, thread, 3);
  addr = (uint64_t*)(*thread->data_stack_current);
  *addr = *(thread->data_stack_current + 1);
  *(addr + 1) = *(thread->data_stack_current + 2);
  thread->data_stack_current += 3;
  AF_ADVANCE_IP(thread, 1);
}

/* , primitive */
void af_prim_comma(af_global_t* global, af_thread_t* thread) {
  uint64_t* data;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  if((data = (uint64_t*)af_allot(global, thread, sizeof(uint64_t)))) {
    *data = *thread->data_stack_current++;
  }
  AF_ADVANCE_IP(thread, 1);
}

/* C, primitive */
void af_prim_c_comma(af_global_t* global, af_thread_t* thread) {
  uint8_t* data;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  if((data = (uint8_t*)af_allot(global, thread, sizeof(uint8_t)))) {
    *data = (uint8_t)(*thread->data_stack_current++ & 0xFF);
  }
  AF_ADVANCE_IP(thread, 1);
}

/* 2, primitive */
void af_prim_2_comma(af_global_t* global, af_thread_t* thread) {
  uint64_t* data;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  if((data = (uint64_t*)af_allot(global, thread, sizeof(uint64_t) * 2))) {
    *data = *thread->data_stack_current++;
    *(data + 1) = *thread->data_stack_current++;
  }
  AF_ADVANCE_IP(thread, 1);
}

/* >R primitive */
void af_prim_to_r(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  AF_VERIFY_RETURN_STACK_EXPAND(global, thread, 1);
  *(--thread->return_stack_current) = (uint64_t*)*thread->data_stack_current++;
  AF_ADVANCE_IP(thread, 1);
}

/* R> primitive */
void af_prim_from_r(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  AF_VERIFY_RETURN_STACK_READ(global, thread, 1);
  *(--thread->data_stack_current) = (uint64_t)*thread->return_stack_current++;
  AF_ADVANCE_IP(thread, 1);
}

/* R@ primitive */
void af_prim_r_fetch(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  AF_VERIFY_RETURN_STACK_READ(global, thread, 1);
  *(--thread->data_stack_current) = (uint64_t)*thread->return_stack_current;
  AF_ADVANCE_IP(thread, 1);
}

/* 2>R primitive */
void af_prim_two_to_r(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  AF_VERIFY_RETURN_STACK_EXPAND(global, thread, 2);
  *(--thread->return_stack_current) = *(thread->data_stack_current + 1);
  *(--thread->return_stack_current) = *thread->data_stack_current;
  thread->data_stack_current += 2;
  AF_ADVANCE_IP(thread, 1);
}

/* 2R> primitve */
void af_prim_two_from_r(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 2);
  AF_VERIFY_RETURN_STACK_READ(global, thread, 2);
  *(--thread->data_stack_current) = *(thread->return_stack_current + 1);
  *(--thread->data_stack_current) = *thread->return_stack_current;
  thread->return_stack_current += 2;
  AF_ADVANCE_IP(thread, 1);
}

/* 2R@ primitive */
void af_prim_two_r_fetch(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 2);
  AF_VERIFY_RETURN_STACK_READ(global, thread, 2);
  *(--thread->data_stack_current) = *(thread->return_stack_current + 1);
  *(--thread->data_stack_current) = *thread->return_stack_current;
  AF_ADVANCE_IP(thread, 1);
}

/* HERE primitive */
void af_prim_here(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (uint64_t)thread->data_space_current;
  AF_ADVANCE_IP(thread, 1);
}

/* DOES> primitive */
void af_prim_does(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_WORD_CREATED(global, thread);
  AF_VERIFY_NOT_INTERACTIVE(global, thread);
  AF_VERIFY_RETURN_STACK_READ(global, thread, 1);
  thread->most_recent_word->code = af_prim_do_does;
  thread->most_recent_word->secondary = thread->interpreter_pointer + 1;
  thread->interpreter_pointer = *thread->return_stack_current++;
}

/* >BODY primitive */
void af_prim_to_body(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  *thread->data_stack_current =
    (uint64_t)(((af_word_t*)(*thread->data_stack_current))->data);
  AF_ADVANCE_IP(thread, 1);
}

/* ' primitve */
void af_prim_tick(af_global_t* global, af_thread_t* thread) {
  uint8_t* name;
  uint64_t name_length;
  af_word_t* word;
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
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
  AF_VERIFY_COMPILING(global, thread);
  AF_VERIFY_WORD_CREATED(global, thread);
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
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  ((af_word_t*)(*thread->data_stack_current++))->code(global, thread);
}

/* LATESTXT primitive */
void af_prim_latestxt(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (uint64_t)thread->most_recent_word;
  AF_ADVANCE_IP(thread, 1);
}

/* >IN primitive */
void af_prim_to_in(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (uint64_t)(&thread->current_input->index);
  AF_ADVANCE_IP(thread, 1);
}

/* SOURCE primitive */
void af_prim_source(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 2);
  *(--thread->data_stack_current) = (uint64_t)thread->current_input->buffer;
  *(--thread->data_stack_current) = thread->current_input->count;
  AF_ADVANCE_IP(thread, 1);
}

/* EVALUATE primitive */
void af_prim_evaluate(af_global_t* global, af_thread_t* thread) {
  uint8_t* text;
  uint64_t count;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  count = *thread->data_stack_current++;
  text = (uint8_t*)(*thread->data_stack_current++);
  AF_ADVANCE_IP(thread, 1);
  af_evaluate(global, thread, text, count);
}

/* ALLOCATE primitive */
void af_prim_allocate(af_global_t* global, af_thread_t* thread) {
  void* data;
  AF_VEFIFY_DATA_STACK_READ(global, thread, 1);
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
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
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  free((void*)(*thread->data_stack_current));
  *thread->data_stack_current = 0;
  AF_ADVANCE_IP(thread, 1);
}

/* RESIZE primitive */
void af_prim_resize(af_global_t* global, af_thread_t* thread) {
  void* data;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
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
  AF_VERIFY_DATA_STACK_READ(global, thread);
  if(!af_allot(global, thread, (ssize_t)(*thread->data_stack_current++))) {
    return;
  }
  AF_ADVANCE_IP(thread, 1);
}

/* UNUSED primitive */
void af_prim_unused(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread);
  *(--thread->data_stack_current) =
    (uint64_t)(thread->data_stack_top - thread->data_stack_current);
  AF_ADVANCE_IP(thread, 1);
}

/* WORD primitive */
void af_prim_word(af_global_t* global, af_thread_t* thread) {
  uint8_t delimiter;
  uint8_t* text;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  delimiter = (uint8_t)(*thread->data_stack_current & 0xFF);
  text = af_word(global, thread, delimiter);
  *thread->data_stack_current = (uint64_t)text;
  AF_ADVANCE_IP(thread, 1);
}

/* PARSE-NAME primitive */
void af_prim_parse_name(af_global_t* global, af_thread_t* thread) {
  uint8_t* text;
  uint64_t length;
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 2);
  if(!af_parse_name_wait(global, thread)) {
    return;
  }
  text = af_parse_name(global, thread, &length);
  *(--thread->data_stack_current) = (uint64_t)text;
  *(--thread->data_stack_current) = length;
  AF_ADVANCE_IP(thread, 1);
}

/* THIS-THREAD primitive */
void af_prim_this_thread(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (uint64_t)thread;
  AF_ADVANCE_IP(thread, 1);
}

/* SPAWN-EXECUTE primitive */
void af_prim_spawn_execute(af_global_t* global, af_thread_t* thread) {
  af_word_t* word;
  af_thread_t* new_thread;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  word = (af_word_t*)(*thread->data_stack_current);
  if(!(new_thread = af_spawn(global))) {
    return;
  }
  af_set_init_word(global, new_thread, word);
  af_start(global, new_thread);
  *thread->data_stack_current = (uint64_t)new_thread;
  AF_ADVANCE_IP(thread, 1);
}

/* SPAWN-EVALUATE primitive */
void af_prim_spawn_evaluate(af_global_t* global, af_thread_t* thread) {
  af_uint8_t* text;
  af_uint64_t count;
  af_thread_t* new_thread;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  count = *thread->data_stack_current;
  text = (af_uint8_t*)(*(thread->data_stack_current + 1));
  if(!(new_thread = af_spawn(global))) {
    return;
  }
  af_evaluate(global, new_thread, text, count);
  af_start(global, new_thread);
  *(++thread->data_stack_current) = (uint64_t)new_thread;
  AF_ADVANCE_IP(thread, 1);
}

/* KILL primitive */
void af_prim_kill(af_global_t* global, af_thread_t* thread) {
  af_thread_t* target_thread;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  target_thread = (af_thread_t*)(thread->data_stack_current++);
  af_kill(global, target_thread);
  AF_ADVANCE_IP(thread, 1);
}

/* YIELD primitive */
void af_prim_yield(af_global_t* global, af_thread_t* thread) {
  af_yield(global, thread);
  AF_ADVANCE_IP(thread, 1);
}

/* SLEEP primitive */
void af_prim_sleep(af_global_t* global, af_thread_t* thread) {
  af_thread_t* target_thread;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  target_thread = (af_thread_t*)(thread->data_stack_current++);
  af_sleep(global, target_thread);
  AF_ADVANCE_IP(thread, 1);
}

/* WAKE primitive */
void af_prim_wake(af_global_t* global, af_thread_t* thread) {
  af_thread_t* target_thread;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  target_thread = (af_thread_t*)(thread->data_stack_current++);
  af_wake(global, target_thread);
  AF_ADVANCE_IP(thread, 1);
}

/* RESET primitive */
void af_prim_reset(af_global_t* global, af_thread_t* thread) {
  af_thread_t* target_thread;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  target_thread = (af_thread_t*)(thread->data_stack_current++);
  af_reset(global, target_thread);
  AF_ADVANCE_IP(thread, 1);
}

/* QUIT primitive */
void af_prim_quit(af_global_t* global, af_thread_t* thread) {
  af_quit(global, thread);
}

/* ABORT primitive */
void af_prim_abort(af_global_t* global, af_thread_t* thread) {
  af_reset(global, thread);
}

/* [ primitive - immediate */
void af_prim_open_bracket(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_COMPILING(global, thread);
  thread->is_compiling = FALSE;
  AF_ADVANCE_IP(thread, 1);
}

/* ] primitive */
void af_prim_close_bracket(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_WORD_CREATED(global, thread);
  thread->is_compiling = TRUE;
  AF_ADVANCE_IP(thread, 1);
}

/* POSTPONE primitive - immediate */
void af_prim_postpone(af_global_t* global, af_thread_t* thread) {
  uint8_t* name;
  uint64_t name_length;
  af_word_t* word;
  af_compiled_t* slot;
  if(!af_parse_name_wait(global, thread)) {
    return;
  }
  name = af_parse_name(global, thread, &name_length);
  if(!(word = af_lookup(global, thread, name, name_length))) {
    af_handle_word_not_found(global, thread);
    return;
  }
  if(!word->is_immediate) {
    if(!(slot = af_allot(global, thread, sizeof(af_compiled_t) * 2))) {
      return;
    }
    slot->compiled_call = global->builtin_postpone_runtime;
    (slot + 1)->compiled_call = (uint64_t)word;
  } else {
    if(!(slot = af_allot(global, thread, sizeof(af_compiled_t)))) {
      return;
    }
    slot->compiled_call = (uint64_t)word;
  }
  AF_ADVANCE_IP(thread, 1);
}

/* POSTPONE runtime primitive */
void af_prim_postpone_runtime(af_global_t* global, af_thread_t* thread) {
  af_compiled_t* slot = af_allot(global, thread, sizeof(af_compiled_t));
  if(!slot) {
    return;
  }
  slot->compiled_call = (af_compiled_t)(*(thread->interpreter_pointer + 1));
  AF_ADVANCE_IP(thread, 2);
}

/* BRANCH primitive */
void af_prim_branch(af_global_t* global, af_thread_t* thread) {
  if(thread->interpreter_pointer) {
    thread->interpreter_pointer =
      (af_thread_t*)(*(thread->interpreter_pointer + 1));
  } else {
    af_handle_not_interactive(global, thread);
  }
}

/* 0BRANCH primitive */
void af_prim_0branch(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  if(thread->interpreter_pointer) {
    uint64_t* value = *thread->data_stack_current++;
    if(!value) {
      thread->interpreter_pointer =
	(af_thread_t*)(*(thread->interpreter_pointer + 1));
    } else {
      AF_ADVANCE_IP(thread, 2);
    }
  } else {
    af_handle_not_interactive(global, thread);
  }
}

/* STATE primitive */
void af_prim_state(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (uint64_t)(&thread->is_compiling);
  AF_ADVANCE_IP(thread, 1);
}

/* MOVE primitive */
void af_prim_move(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 3);
  memmove((void*)(*(thread->data_stack_current + 1)),
	  (void*)(*(thread->data_stack_current + 2)),
	  (size_t)(*thread->data_stack_current));
  thread->data_stack_current += 3;
  AF_ADVANCE_IP(thread, 1);
}

/* DEPTH primitive */
void af_prim_depth(af_global_t* global, af_thread_t* thread) {
  uint64_t count;
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  count = thread->data_stack_base - thread->data_stack_current;
  *(--thread->data_stack_current) = count;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-ACTION-DESTROY primitive */
void af_prim_io_action_destroy(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  af_io_action_destroy((af_io_t*)(*thread->data_stack_current++));
  AF_ADVANCE_IP(thread, 1);
}

/* IO-ACTION-GET-STATE primitive */
void af_prim_io_action_get_state(af_global_t* global, af_thread_t* thread) {
  af_io_state_t* state;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  if(!(state = malloc(sizeof(af_io_state_t)))) {
    af_handle_out_of_memory(global, thread);
    return;
  }
  af_io_action_get_state((af_io_t*)(*thread->data_stack_current), state);
  *thread->data_stack_current = (uint64_t)state;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-STATE-DESTROY primitive */
void af_prim_io_state_destroy(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  free((void*)(*thread->data_stack_current++));
  AF_ADVANCE_IP(thread, 1);
}

/* IO-STATE-IS-DONE primitive */
void af_prim_io_state_is_done(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  *(af_bool_t*)thread->data_stack_current =
    af_io_state_is_done((af_io_state_t*)(*thread->data_stack_current));
  AF_ADVANCE_IP(thread, 1);
}

/* IO-STATE-IS-CLOSED primitive */
void af_prim_io_state_is_closed(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  *(af_bool_t*)thread->data_stack_current =
    af_io_state_is_closed((af_io_state_t*)(*thread->data_stack_current));
  AF_ADVANCE_IP(thread, 1);
}

/* IO-STATE-HAS-HANGUP primitive */
void af_prim_io_state_has_hangup(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  *(af_bool_t*)thread->data_stack_current =
    af_io_state_has_hangup((af_io_state_t*)(*thread->data_stack_current));
  AF_ADVANCE_IP(thread, 1);
}

/* IO-STATE-HAS-ERROR primitive */
void af_prim_io_state_has_error(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  *(af_bool_t*)thread->data_stack_current =
    af_io_state_has_error((af_io_state_t*)(*thread->data_stack_current));
  AF_ADVANCE_IP(thread, 1);
}

/* IO-STATE-GET-BUFFER primitive */
void af_prim_io_state_get_buffer(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  *(uint8_t**)thread->data_stack_current =
    af_io_state_get_buffer((af_io_state_t*)(*thread->data_stack_current));
  AF_ADVANCE_IP(thread, 1);
}

/* IO-STATE-GET-INDEX primitive */
void af_prim_io_state_get_index(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  *thread->data_stack_current =
    af_io_state_get_index((af_io_state_t*)(*thread->data_stack_current));
  AF_ADVANCE_IP(thread, 1);
}

/* IO-STATE-GET-COUNT primitive */
void af_prim_io_state_get_count(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  *thread->data_stack_current =
    af_io_state_get_count((af_io_state_t*)(*thread->data_stack_current));
  AF_ADVANCE_IP(thread, 1);
}

/* IO-READ-BLOCK primitive */
void af_prim_io_read_block(af_global_t* global, af_thread_t* thread) {
  af_io_action_t* action;
  AF_VERIFY_DATA_STACK_READ(global, thread, 3);
  action = af_io_read_block(&global->io,
			    (int)(*thread->data_stack_current),
			    (uint8_t*)(*(thread->data_stack_current + 2)),
			    *(thread->data_stack_current + 1),
			    thread);
  thread->data_stack_current += 2;
  *thread->data_stack_current = (uint64_t)action;
  AF_ADVANCE_IP(thread, 1);
  af_sleep(global, thread);
}

/* IO-WRITE-BLOCK primitive */
void af_prim_io_write_block(af_global_t* global, af_thread_t* thread) {
  af_io_action_t* action;
  AF_VERIFY_DATA_STACK_READ(global, thread, 3);
  action = af_io_write_block(&global->io,
			     (int)(*thread->data_stack_current),
			     (uint8_t*)(*(thread->data_stack_current + 2)),
			     *(thread->data_stack_current + 1),
			     thread);
  thread->data_stack_current += 2;
  *thread->data_stack_current = (uint64_t)action;
  AF_ADVANCE_IP(thread, 1);
  af_sleep(global, thread);
}

/* IO-READ-ASYNC primitive */
void af_prim_io_read_async(af_global_t* global, af_thread_t* thread) {
  af_io_action_t* action;
  AF_VERIFY_DATA_STACK_READ(global, thread, 3);
  action = af_io_read_async(&global->io,
			    (int)(*thread->data_stack_current),
			    (uint8_t*)(*(thread->data_stack_current + 2)),
			    *(thread->data_stack_current + 1));
  thread->data_stack_current += 2;
  *thread->data_stack_current = (uint64_t)action;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-WRITE-ASYNC primitive */
void af_prim_io_write_async(af_global_t* global, af_thread_t* thread) {
  af_io_action_t* action;
  AF_VERIFY_DATA_STACK_READ(global, thread, 3);
  action = af_io_write_async(&global->io,
			     (int)(*thread->data_stack_current),
			     (uint8_t*)(*(thread->data_stack_current + 2)),
			     *(thread->data_stack_current + 1));
  thread->data_stack_current += 2;
  *thread->data_stack_current = (uint64_t)action;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-READ-NONBLOCK primitive */
void af_prim_io_read_nonblock(af_global_t* global, af_thread_t* thread) {
  af_bool_t again;
  ssize_t size;
  AF_VERIFY_DATA_STACK_READ(global, thread, 3);
  size = af_io_read_nonblock((int)(*thread->data_stack_current),
			     (uint8_t*)(*(thread->data_stack_current + 2)),
			     *(thread->data_stack_current + 1),
			     &again);
  thread->data_stack_current++;
  *(int64_t*)(thread->data_stack_current + 1) = (int64_t)size;
  *(af_bool_t*)thread->data_stack_current = again;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-WRITE-NONBLOCK primitive */
void af_prim_io_write_nonblock(af_global_t* global, af_thread_t* thread) {
  af_bool_t again;
  ssize_t size;
  AF_VERIFY_DATA_STACK_READ(global, thread, 3);
  size = af_io_write_nonblock((int)(*thread->data_stack_current),
			      (uint8_t*)(*(thread->data_stack_current + 2)),
			      *(thread->data_stack_current + 1),
			      &again);
  thread->data_stack_current++;
  *(int64_t*)(thread->data_stack_current + 1) = (int64_t)size;
  *(af_bool_t*)thread->data_stack_current = again;
  AF_ADVANCE_IP(thread, 1);
}
