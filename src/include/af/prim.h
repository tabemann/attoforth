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

#ifndef AF_PRIM_H
#define AF_PRIM_H

#include <stdint.h>
#include <stddef.h>
#include "af/common.h"
#include "af/inner.h"

/* Register primitves */
void af_register_prims(af_global_t* global, af_thread_t* thread);

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

#endif /* AF_PRIM_H */
