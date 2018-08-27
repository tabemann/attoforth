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

/* Next primitive */
void af_prim_next(af_global_t* global, af_thread_t* thread);

/* Exit primitive */
void af_prim_exit(af_global_t* global, af_thread_t* thread);

/* Literal runtime primitive */
void af_prim_literal_runtime(af_global_t* global, af_thread_t* thread);

/* LITERAL primitive - immediate */
void af_prim_literal(af_global_t* global, af_thread_t* thread);

/* CREATE primitive */
void af_prim_create(af_global_t* global, af_thread_t* thread);

/* Colon definition primitive - immediate */
void af_prim_colon(af_global_t* global, af_thread_t* thread);

/* Semicolon definition primitive - immediate */
void af_prim_semi(af_global_t* global, af_thread_t* thread);

/* Immediate primitive - immediate */
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

/* @ primitive */
void af_prim_load(af_global_t* global, af_thread_t* thread);

/* ! primitive */
void af_prim_store(af_global_t* global, af_thread_t* thread);

/* C@ primitive */
void af_prim_c_load(af_global_t* global, af_thread_t* thread);

/* C! primitive */
void af_prim_c_store(af_global_t* global, af_thread_t* thread);

/* >R primitive */
void af_prim_to_r(af_global_t* global, af_thread_t* thread);

/* R> primitive */
void af_prim_from_r(af_global_t* global, af_thread_t* thread);

/* R@ primitive */
void af_prim_r_load(af_global_t* global, af_thread_t* thread);

/* HERE primitive */
void af_prim_here(af_global_t* global, af_thread_t* thread);

#endif /* AF_PRIM_H */
