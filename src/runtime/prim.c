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
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <errno.h>
#include "af/common.h"
#include "af/types.h"
#include "af/inner.h"
#include "af/io.h"
#include "af/prim.h"

/* Function forward declarations */

/* Docol primitive */
void af_prim_docol(af_task_t* task);

/* EXIT primitive */
void af_prim_exit(af_task_t* task);

/* Push the data address of a word primitive */
void af_prim_push_data(af_task_t* task);

/* Runtime of DOES> primitve */
void af_prim_do_does(af_task_t* task);

/* (LITERAL) primitive */
void af_prim_literal_runtime(af_task_t* task);

/* (FLITERAL) primitive */
void af_prim_f_literal_runtime(af_task_t* task);

/* CREATE primitive */
void af_prim_create(af_task_t* task);

/* CREATE-WITH-NAME primitive */
void af_prim_create_with_name(af_task_t* task);

/* : primitive */
void af_prim_colon(af_task_t* task);

/* :NONAME primitive */
void af_prim_colon_noname(af_task_t* task);

/* CREATE-NONAME-AT primitive */
void af_prim_create_noname_at(af_task_t* task);

/* ; primitive - immediate */
void af_prim_semi(af_task_t* task);

/* IMMEDIATE primitive */
void af_prim_immediate(af_task_t* task);

/* COMPILE-ONLY primitive */
void af_prim_compile_only(af_task_t* task);

/* SMART primitive */
void af_prim_smart(af_task_t* task);

/* IS-IMMEDIATE primitive */
void af_prim_is_immediate(af_task_t* task);

/* IS-HIDDEN primitive */
void af_prim_is_hidden(af_task_t* task);

/* IS-COMPILE-ONLY primitive */
void af_prim_is_compile_only(af_task_t* task);

/* IS-SMART primitive */
void af_prim_is_smart(af_task_t* task);

/* DUP primitive */
void af_prim_dup(af_task_t* task);

/* DROP primitive */
void af_prim_drop(af_task_t* task);

/* SWAP primitive */
void af_prim_swap(af_task_t* task);

/* OVER primitive */
void af_prim_over(af_task_t* task);

/* ROT primitive */
void af_prim_rot(af_task_t* task);

/* PICK primitive */
void af_prim_pick(af_task_t* task);

/* ROLL primitive */
void af_prim_roll(af_task_t* task);

/* + primitive */
void af_prim_add(af_task_t* task);

/* - primitive */
void af_prim_sub(af_task_t* task);

/* * primitive */
void af_prim_mul(af_task_t* task);

/* / primitive */
void af_prim_div(af_task_t* task);

/* MOD primitive */
void af_prim_mod(af_task_t* task);

/* U/ primitive */
void af_prim_u_div(af_task_t* task);

/* UMOD primitive */
void af_prim_u_mod(af_task_t* task);

/* INVERT primitive */
void af_prim_invert(af_task_t* task);

/* AND primitive */
void af_prim_and(af_task_t* task);

/* OR primitive */
void af_prim_or(af_task_t* task);

/* XOR primitive */
void af_prim_xor(af_task_t* task);

/* LSHIFT primitive */
void af_prim_lshift(af_task_t* task);

/* RSHIFT primitive */
void af_prim_rshift(af_task_t* task);

/* ARSHIFT primitive */
void af_prim_arshift(af_task_t* task);

/* < primitive */
void af_prim_lt(af_task_t* task);

/* <= primitive */
void af_prim_lte(af_task_t* task);

/* > primitive */
void af_prim_gt(af_task_t* task);

/* >= primitive */
void af_prim_gte(af_task_t* task);

/* = primitive */
void af_prim_eq(af_task_t* task);

/* <> primitive */
void af_prim_ne(af_task_t* task);

/* U< primitive */
void af_prim_u_lt(af_task_t* task);

/* U<= primitive */
void af_prim_u_lte(af_task_t* task);

/* U> primitive */
void af_prim_u_gt(af_task_t* task);

/* U>= primitive */
void af_prim_u_gte(af_task_t* task);

/* FDUP primitive */
void af_prim_f_dup(af_task_t* task);

/* FDROP primitive */
void af_prim_f_drop(af_task_t* task);

/* FSWAP primitive */
void af_prim_f_swap(af_task_t* task);

/* FOVER primitive */
void af_prim_f_over(af_task_t* task);

/* FROT primitive */
void af_prim_f_rot(af_task_t* task);

/* FPICK primitive */
void af_prim_f_pick(af_task_t* task);

/* FROLL primitive */
void af_prim_f_roll(af_task_t* task);

/* S>F primitive */
void af_prim_s_to_f(af_task_t* task);

/* D>F primitive */
void af_prim_d_to_f(af_task_t* task);

/* F>S primitive */
void af_prim_f_to_s(af_task_t* task);

/* F>D primitive */
void af_prim_f_to_d(af_task_t* task);

/* F+ primitive */
void af_prim_f_add(af_task_t* task);

/* F- primitive */
void af_prim_f_sub(af_task_t* task);

/* F* primitive */
void af_prim_f_mul(af_task_t* task);

/* F/ primitive */
void af_prim_f_div(af_task_t* task);

/* FMOD primitive */
void af_prim_f_mod(af_task_t* task);

/* F** primitive */
void af_prim_f_pow(af_task_t* task);

/* F< primitive */
void af_prim_f_lt(af_task_t* task);

/* F<= primitive */
void af_prim_f_lte(af_task_t* task);

/* F> primitive */
void af_prim_f_gt(af_task_t* task);

/* F>= primitive */
void af_prim_f_gte(af_task_t* task);

/* F= primitive */
void af_prim_f_eq(af_task_t* task);

/* F<> primitive */
void af_prim_f_ne(af_task_t* task);

/* D+ primitive */
void af_prim_d_add(af_task_t* task);

/* D- primitive */
void af_prim_d_sub(af_task_t* task);

/* D* primitive */
void af_prim_d_mul(af_task_t* task);

/* D/ primitive */
void af_prim_d_div(af_task_t* task);

/* DMOD primitive */
void af_prim_d_mod(af_task_t* task);

/* DU/ primitive */
void af_prim_d_u_div(af_task_t* task);

/* DUMOD primitive */
void af_prim_d_u_mod(af_task_t* task);

/* DINVERT primitive */
void af_prim_d_invert(af_task_t* task);

/* DAND primitive */
void af_prim_d_and(af_task_t* task);

/* DOR primitive */
void af_prim_d_or(af_task_t* task);

/* DXOR primitive */
void af_prim_d_xor(af_task_t* task);

/* DLSHIFT primitive */
void af_prim_d_lshift(af_task_t* task);

/* DRSHIFT primitive */
void af_prim_d_rshift(af_task_t* task);

/* DARSHIFT primitive */
void af_prim_d_arshift(af_task_t* task);

/* D< primitive */
void af_prim_d_lt(af_task_t* task);

/* D<= primitive */
void af_prim_d_lte(af_task_t* task);

/* D> primitive */
void af_prim_d_gt(af_task_t* task);

/* D>= primitive */
void af_prim_d_gte(af_task_t* task);

/* D= primitive */
void af_prim_d_eq(af_task_t* task);

/* D<> primitive */
void af_prim_d_ne(af_task_t* task);

/* DU< primitive */
void af_prim_d_u_lt(af_task_t* task);

/* DU<= primitive */
void af_prim_d_u_lte(af_task_t* task);

/* DU> primitive */
void af_prim_d_u_gt(af_task_t* task);

/* DU>= primitive */
void af_prim_d_u_gte(af_task_t* task);

/* @ primitive */
void af_prim_fetch(af_task_t* task);

/* ! primitive */
void af_prim_store(af_task_t* task);

/* +! primitive */
void af_prim_add_store(af_task_t* task);

/* W@ primitive */
void af_prim_w_fetch(af_task_t* task);

/* W! primitive */
void af_prim_w_store(af_task_t* task);

/* H@ primitive */
void af_prim_h_fetch(af_task_t* task);

/* H! primitive */
void af_prim_h_store(af_task_t* task);

/* C@ primitive */
void af_prim_c_fetch(af_task_t* task);

/* C! primitive */
void af_prim_c_store(af_task_t* task);

/* C@ primitive */
void af_prim_c_fetch(af_task_t* task);

/* C! primitive */
void af_prim_c_store(af_task_t* task);

/* F@ primitive */
void af_prim_f_fetch(af_task_t* task);

/* F! primitive */
void af_prim_f_store(af_task_t* task);

/* F+! primitive */
void af_prim_f_add_store(af_task_t* task);

/* SF@ primitive */
void af_prim_sf_fetch(af_task_t* task);

/* SF! primitive */
void af_prim_sf_store(af_task_t* task);

/* SF+! primitive */
void af_prim_sf_add_store(af_task_t* task);

/* DF@ primitive */
void af_prim_df_fetch(af_task_t* task);

/* DF! primitive */
void af_prim_df_store(af_task_t* task);

/* DF+! primitive */
void af_prim_df_add_store(af_task_t* task);

/* >R primitive */
void af_prim_to_r(af_task_t* task);

/* R> primitive */
void af_prim_from_r(af_task_t* task);

/* R@ primitive */
void af_prim_r_fetch(af_task_t* task);

/* HERE primitive */
void af_prim_here(af_task_t* task);

/* DOES> primitive */
void af_prim_does(af_task_t* task);

/* DOES-AT> primitive */
void af_prim_does_at(af_task_t* task);

/* NAME>STRING primitive */
void af_prim_name_to_string(af_task_t* task);

/* NAME>CODE primitive */
void af_prim_name_to_code(af_task_t* task);

/* NAME>SECONDARY primitive */
void af_prim_name_to_secondary(af_task_t* task);

/* NAME>NEXT primitive */
void af_prim_name_to_next(af_task_t* task);

/* FIRST-WORD primitive */
void af_prim_first_word(af_task_t* task);

/* ALL-WORDS primitive */
void af_prim_all_words(af_task_t* task);

/* NAME>NEXT-ALL primitive */
void af_prim_name_to_next_all(af_task_t* task);

/* >BODY primitive */
void af_prim_to_body(af_task_t* task);

/* SP@ primitive */
void af_prim_sp_fetch(af_task_t* task);

/* SP! primitive */
void af_prim_sp_store(af_task_t* task);

/* SP0 primitive */
void af_prim_sp0(af_task_t* task);

/* SPF primitive */
void af_prim_spf(af_task_t* task);

/* FP@ primitive */
void af_prim_fp_fetch(af_task_t* task);

/* FP! primitive */
void af_prim_fp_store(af_task_t* task);

/* FP0 primitive */
void af_prim_fp0(af_task_t* task);

/* FPF primitive */
void af_prim_fpf(af_task_t* task);

/* RP@ primitive */
void af_prim_rp_fetch(af_task_t* task);

/* RP! primitive */
void af_prim_rp_store(af_task_t* task);

/* RP0 primitive */
void af_prim_rp0(af_task_t* task);

/* RPF primitive */
void af_prim_rpf(af_task_t* task);

/* T@ primitive */
void af_prim_t_fetch(af_task_t* task);

/* T! primitive */
void af_prim_t_store(af_task_t* task);

/* WT@ primitive */
void af_prim_wt_fetch(af_task_t* task);

/* WT! primitive */
void af_prim_wt_store(af_task_t* task);

/* HT@ primitive */
void af_prim_ht_fetch(af_task_t* task);

/* HT! primitive */
void af_prim_ht_store(af_task_t* task);

/* CT@ primitive */
void af_prim_ct_fetch(af_task_t* task);

/* CT! primitive */
void af_prim_ct_store(af_task_t* task);

/* FT@ primitive */
void af_prim_ft_fetch(af_task_t* task);

/* FT! primitive */
void af_prim_ft_store(af_task_t* task);

/* SFT@ primitive */
void af_prim_sft_fetch(af_task_t* task);

/* SFT! primitive */
void af_prim_sft_store(af_task_t* task);

/* DFT@ primitive */
void af_prim_dft_fetch(af_task_t* task);

/* DFT! primitive */
void af_prim_dft_store(af_task_t* task);

/* TASK-LOCAL primitive */
void af_prim_task_local(af_task_t* task);

/* TASK-TRACE primitive */
void af_prim_task_trace(af_task_t* task);

/* GLOBAL-TRACE primitive */
void af_prim_global_trace(af_task_t* task);

/* LITTLE-ENDIAN primitive */
void af_prim_little_endian(af_task_t* task);

/* ' primitve */
void af_prim_tick(af_task_t* task);

/* ['] primitive - immediate */
void af_prim_bracket_tick(af_task_t* task);

/* EXECUTE primitive */
void af_prim_execute(af_task_t* task);

/* STATE primitive */
void af_prim_state(af_task_t* task);

/* LATESTXT primitive */
void af_prim_latestxt(af_task_t* task);

/* >LATESTXT primitive */
void af_prim_to_latestxt(af_task_t* task);

/* >IN primitive */
void af_prim_to_in(af_task_t* task);

/* SOURCE primitive */
void af_prim_source(af_task_t* task);

/* >INPUT primitive */
void af_prim_to_input(af_task_t* task);

/* INPUT> primitive */
void af_prim_from_input(af_task_t* task);

/* >DATA primitive */
void af_prim_to_data(af_task_t* task);

/* >FLOAT primitive */
void af_prim_to_float(af_task_t* task);

/* >RETURN primitive */
void af_prim_to_return(af_task_t* task);

/* >ABORT primitive */
void af_prim_to_abort(af_task_t* task);

/* ABORT> primitive */
void af_prim_from_abort(af_task_t* task);

/* >TERMINATE primitive */
void af_prim_to_terminate(af_task_t* task);

/* TERMINATE> primitive */
void af_prim_from_terminate(af_task_t* task);

/* >DEFAULT-ABORT primitive */
void af_prim_to_default_abort(af_task_t* task);

/* DEFAULT-ABORT> primitive */
void af_prim_from_default_abort(af_task_t* task);

/* ALLOCATE primitive */
void af_prim_allocate(af_task_t* task);

/* FREE primitive */
void af_prim_free(af_task_t* task);

/* RESIZE primitive */
void af_prim_resize(af_task_t* task);

/* ALLOT primitive */
void af_prim_allot(af_task_t* task);

/* UNUSED primitive */
void af_prim_unused(af_task_t* task);

/* WORD primitive */
void af_prim_word(af_task_t* task);

/* PARSE-NAME primitive */
void af_prim_parse_name(af_task_t* task);

/* PARSE-NUMBER primitive */
void af_prim_parse_number(af_task_t* task);

/* PARSE-2NUMBER primitive */
void af_prim_parse_2number(af_task_t* task);

/* PARSE-FLOAT primitive */
void af_prim_parse_float(af_task_t* task);

/* FORMAT-NUMBER primitive */
void af_prim_format_number(af_task_t* task);

/* FORMAT-UNSIGNED-NUMBER primitive */
void af_prim_format_unsigned_number(af_task_t* task);

/* FORMAT-2NUMBER primitive */
void af_prim_format_2number(af_task_t* task);

/* FORMAT-UNSIGNED-2NUMBER primitive */
void af_prim_format_unsigned_2number(af_task_t* task);

/* FORMAT-FLOAT primitive */
void af_prim_format_float(af_task_t* task);

/* FORMAT-FLOAT-SCI primitive */
void af_prim_format_float_sci(af_task_t* task);

/* BASE primitive */
void af_prim_base(af_task_t* task);

/* FIND-WORD primitive */
void af_prim_find_word(af_task_t* task);

/* CAS primitive */
void af_prim_cas(af_task_t* task);

/* 2CAS primitive */
void af_prim_2cas(af_task_t* task);

/* THIS-TASK primitive */
void af_prim_this_task(af_task_t* task);

/* SPAWN primitive */
void af_prim_spawn(af_task_t* task);

/* SPAWN-NO-DATA primitive */
void af_prim_spawn_no_data(af_task_t* task);

/* >FREE-DATA-ON-EXIT primitive */
void af_prim_free_data_on_exit(af_task_t* task);

/* >CONSOLE-INPUT primitive */
void af_prim_to_console_input(af_task_t* task);

/* CONSOLE-INPUT> primitive */
void af_prim_from_console_input(af_task_t* task);

/* >INIT-WORD primitive */
void af_prim_to_init_word(af_task_t* task);

/* START primitive */
void af_prim_start(af_task_t* task);

/* BYE primitive */
void af_prim_bye(af_task_t* task);

/* TERMINATE primitive */
void af_prim_terminate(af_task_t* task);

/* KILL primitive */
void af_prim_kill(af_task_t* task);

/* YIELD primitive */
void af_prim_yield(af_task_t* task);

/* WAIT primitive */
void af_prim_wait(af_task_t* task);

/* END-ATOMIC-AND-WAIT primitive */
void af_prim_end_atomic_and_wait(af_task_t* task);

/* WAKE primitive */
void af_prim_wake(af_task_t* task);

/* BEGIN-ATOMIC primitive */
void af_prim_begin_atomic(af_task_t* task);

/* END-ATOMIC primitive */
void af_prim_end_atomic(af_task_t* task);

/* YIELDS primitive */
void af_prim_yields(af_task_t* task);

/* TASKS primitive */
void af_prim_tasks(af_task_t* task);

/* >HANDLER-TASK primitive */
void af_prim_to_handler_task(af_task_t* task);

/* HANDLER-TASK> primitive */
void af_prim_from_handler_task(af_task_t* task);

/* [ primitive - immediate */
void af_prim_open_bracket(af_task_t* task);

/* ] primitive */
void af_prim_close_bracket(af_task_t* task);

/* BRANCH primitive */
void af_prim_branch(af_task_t* task);

/* ?BRANCH primitive */
void af_prim_cond_branch(af_task_t* task);

/* MOVE primitive */
void af_prim_move(af_task_t* task);

/* CELL-SIZE primitive */
void af_prim_cell_size(af_task_t* task);

/* FLOAT-SIZE primitive */
void af_prim_float_size(af_task_t* task);

/* SFLOAT-SIZE primitive */
void af_prim_sfloat_size(af_task_t* task);

/* DFLOAT-SIZE primitive */
void af_prim_dfloat_size(af_task_t* task);

/* FORTH-WORDLIST primitive */
void af_prim_forth_wordlist(af_task_t* task);

/* IO-WORDLIST primitive */
void af_prim_io_wordlist(af_task_t* task);

/* TASK-WORDLIST primitive */
void af_prim_task_wordlist(af_task_t* task);

/* GET-CURRENT primitive */
void af_prim_get_current(af_task_t* task);

/* GET-ORDER primitive */
void af_prim_get_order(af_task_t* task);

/* SEARCH-WORDLIST primitive */
void af_prim_search_wordlist(af_task_t* task);

/* SET-CURRENT primitive */
void af_prim_set_current(af_task_t* task);

/* SET-ORDER primitive */
void af_prim_set_order(af_task_t* task);

/* WORDLIST primitive */
void af_prim_wordlist(af_task_t* task);

/* ARGC primitive */
void af_prim_argc(af_task_t* task);

/* ARGV primitive */
void af_prim_argv(af_task_t* task);

/* GETENV primitive */
void af_prim_getenv(af_task_t* task);

/* SETENV primitive */
void af_prim_setenv(af_task_t* task);

/* UNSETENV primitive */
void af_prim_unsetenv(af_task_t* task);

/* >IO-HANDLER-TASK primitive */
void af_prim_to_io_handler_task(af_task_t* task);

/* IO-HANDLER-TASK> primitive */
void af_prim_from_io_handler_task(af_task_t* task);

/* IO-ACTION-DESTROY primitive */
void af_prim_io_action_destroy(af_task_t* task);

/* IO-ACTION-GET-STATE primitive */
void af_prim_io_action_get_state(af_task_t* task);

/* IO-STATE-DESTROY primitive */
void af_prim_io_state_destroy(af_task_t* task);

/* IO-STATE-IS-DONE primitive */
void af_prim_io_state_is_done(af_task_t* task);

/* IO-STATE-IS-CLOSED primitive */
void af_prim_io_state_is_closed(af_task_t* task);

/* IO-STATE-HAS-HANGUP primitive */
void af_prim_io_state_has_hangup(af_task_t* task);

/* IO-STATE-HAS-ERROR primitive */
void af_prim_io_state_has_error(af_task_t* task);

/* IO-STATE-GET-BUFFER primitive */
void af_prim_io_state_get_buffer(af_task_t* task);

/* IO-STATE-GET-INDEX primitive */
void af_prim_io_state_get_index(af_task_t* task);

/* IO-STATE-GET-COUNT primitive */
void af_prim_io_state_get_count(af_task_t* task);

/* IO-STATE-GET-OFFSET primitive */
void af_prim_io_state_get_offset(af_task_t* task);

/* IO-OPEN primitive */
void af_prim_io_open(af_task_t* task);

/* IO-PIPE primitive */
void af_prim_io_pipe(af_task_t* task);

/* IO-DELETE primitive */
void af_prim_io_delete(af_task_t* task);

/* IO-DELETE-DIR primitive */
void af_prim_io_delete_dir(af_task_t* task);

/* IO-RENAME primitive */
void af_prim_io_rename(af_task_t* task);

/* IO-ISATTY primitive */
void af_prim_io_isatty(af_task_t* task);

/* IO-GET-PENDING-WRITE-COUNT primitive */
void af_prim_io_get_pending_write_count(af_task_t* task);

/* IO-GET-MONOTONIC-TIME primitive */
void af_prim_io_get_monotonic_time(af_task_t* task);

/* IO-GET-REAL-TIME primitive */
void af_prim_io_get_real_time(af_task_t* task);

/* IO-SLEEP primitive */
void af_prim_io_sleep(af_task_t* task);

/* IO-TELL primitive */
void af_prim_io_tell(af_task_t* task);

/* IO-CLOSE-BLOCK primitive */
void af_prim_io_close_block(af_task_t* task);

/* IO-CLOSE-ASYNC primitive */
void af_prim_io_close_async(af_task_t* task);

/* IO-SEEK-BLOCK primitive */
void af_prim_io_seek_block(af_task_t* task);

/* IO-SEEK-ASYNC primitive */
void af_prim_io_seek_async(af_task_t* task);

/* IO-READ-BLOCK primitive */
void af_prim_io_read_block(af_task_t* task);

/* IO-WRITE-BLOCK primitive */
void af_prim_io_write_block(af_task_t* task);

/* IO-READ-ASYNC primitive */
void af_prim_io_read_async(af_task_t* task);

/* IO-WRITE-ASYNC primitive */
void af_prim_io_write_async(af_task_t* task);

/* IO-READ-NONBLOCK primitive */
void af_prim_io_read_nonblock(af_task_t* task);

/* IO-WRITE-NONBLOCK primitive */
void af_prim_io_write_nonblock(af_task_t* task);

/* IO-STDIN primitive */
void af_prim_io_stdin(af_task_t* task);

/* IO-STDOUT primitive */
void af_prim_io_stdout(af_task_t* task);

/* IO-STDERR primitive */
void af_prim_io_stderr(af_task_t* task);

/* IO-RDONLY primitive */
void af_prim_io_rdonly(af_task_t* task);

/* IO-WRONLY primitive */
void af_prim_io_wronly(af_task_t* task);

/* IO-RDWR primitive */
void af_prim_io_rdwr(af_task_t* task);

/* IO-APPEND primitive */
void af_prim_io_append(af_task_t* task);

/* IO-TRUNC primitive */
void af_prim_io_trunc(af_task_t* task);

/* IO-CREAT primitive */
void af_prim_io_creat(af_task_t* task);

/* IO-EXCL primitive */
void af_prim_io_excl(af_task_t* task);

/* IO-IRWXU primitive */
void af_prim_io_irwxu(af_task_t* task);

/* IO-IRUSR primitive */
void af_prim_io_irusr(af_task_t* task);

/* IO-IWUSR primitive */
void af_prim_io_iwusr(af_task_t* task);

/* IO-IXUSR primitive */
void af_prim_io_ixusr(af_task_t* task);

/* IO-IRWXG primitive */
void af_prim_io_irwxg(af_task_t* task);

/* IO-IRGRP primitive */
void af_prim_io_irgrp(af_task_t* task);

/* IO-IWGRP primitive */
void af_prim_io_iwgrp(af_task_t* task);

/* IO-IXGRP primitive */
void af_prim_io_ixgrp(af_task_t* task);

/* IO-IRWXO primitive */
void af_prim_io_irwxo(af_task_t* task);

/* IO-IROTH primitive */
void af_prim_io_iroth(af_task_t* task);

/* IO-IWOTH primitive */
void af_prim_io_iwoth(af_task_t* task);

/* IO-IXOTH primitive */
void af_prim_io_ixoth(af_task_t* task);

/* IO-ISUID primitive */
void af_prim_io_isuid(af_task_t* task);

/* IO-ISGID primitive */
void af_prim_io_isgid(af_task_t* task);

/* IO-ISVTX primitive */
void af_prim_io_isvtx(af_task_t* task);

/* IO-SEEK-SET primitive */
void af_prim_io_seek_set(af_task_t* task);

/* IO-SEEK-CUR primitive */
void af_prim_io_seek_cur(af_task_t* task);

/* IO-SEEK-END primitive */
void af_prim_io_seek_end(af_task_t* task);

/* INPUT-SIZE primitive */
void af_prim_input_size(af_task_t* task);

/* INPUT-NEXT-INPUT primitive */
void af_prim_input_next_input(af_task_t* task);

/* INPUT-BUFFER primitive */
void af_prim_input_buffer(af_task_t* task);

/* INPUT-COUNT primitive */
void af_prim_input_count(af_task_t* task);

/* INPUT-INDEX primitive */
void af_prim_input_index(af_task_t* task);

/* INPUT-IS-CLOSED primitive */
void af_prim_input_is_closed(af_task_t* task);

/* INPUT-CLEANUP primitive */
void af_prim_input_cleanup(af_task_t* task);

/* INPUT-REFILL primitive */
void af_prim_input_refill(af_task_t* task);

/* INPUT-SOURCE-ID primitive */
void af_prim_input_source_id(af_task_t* task);

/* INPUT-ARG primitive */
void af_prim_input_arg(af_task_t* task);

/* DEBUGGER primitive */
void af_prim_debugger(af_task_t* task);

/* PRIM-DOCOL primitive */
void af_prim_prim_docol(af_task_t* task);

/* PRIM-PUSH primitive */
void af_prim_prim_push(af_task_t* task);

/* PRIM-DOES primitive */
void af_prim_prim_does(af_task_t* task);

/* BASE-INTERPRETER primitive */
void af_prim_base_interpreter(af_task_t* task);

/* Function definitions */

/* Register primitives */
void af_register_prims(af_global_t* global, af_task_t* task) {
  af_word_t* base_interpreter;
  global->builtin_literal_runtime =
    af_register_prim(global, task, "(LITERAL)", af_prim_literal_runtime, 0,
		     global->forth_wordlist);
  global->builtin_f_literal_runtime =
    af_register_prim(global, task, "(FLITERAL)", af_prim_f_literal_runtime, 0,
		     global->forth_wordlist);
  global->builtin_exit =
    af_register_prim(global, task, "EXIT", af_prim_exit, 0,
		     global->forth_wordlist);
  af_register_prim(global, task, "CREATE", af_prim_create, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "CREATE-WITH-NAME", af_prim_create_with_name,
		   0, global->forth_wordlist);
  af_register_prim(global, task, ":", af_prim_colon, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, ":NONAME", af_prim_colon_noname, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "CREATE-NONAME-AT", af_prim_create_noname_at,
		   0, global->forth_wordlist);
  af_register_prim(global, task, ";", af_prim_semi, AF_WORD_IMMEDIATE,
		   global->forth_wordlist);
  af_register_prim(global, task, "IMMEDIATE", af_prim_immediate, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "COMPILE-ONLY", af_prim_compile_only, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "SMART", af_prim_smart, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "IS-IMMEDIATE", af_prim_is_immediate, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "IS-HIDDEN", af_prim_is_hidden, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "IS-COMPILE-ONLY", af_prim_is_compile_only, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "IS-SMART", af_prim_is_smart, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "DUP", af_prim_dup, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "DROP", af_prim_drop, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "SWAP", af_prim_swap, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "OVER", af_prim_over, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "ROT", af_prim_rot, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "PICK", af_prim_pick, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "ROLL", af_prim_roll, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "+", af_prim_add, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "-", af_prim_sub, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "*", af_prim_mul, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "/", af_prim_div, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "MOD", af_prim_mod, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "U/", af_prim_u_div, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "UMOD", af_prim_u_mod, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "INVERT", af_prim_invert, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "AND", af_prim_and, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "OR", af_prim_or, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "XOR", af_prim_xor, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "LSHIFT", af_prim_lshift, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "RSHIFT", af_prim_rshift, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "ARSHIFT", af_prim_arshift, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "<", af_prim_lt, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "<=", af_prim_lte, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, ">", af_prim_gt, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, ">=", af_prim_gte, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "=", af_prim_eq, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "<>", af_prim_ne, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "U<", af_prim_u_lt, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "U<=", af_prim_u_lte, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "U>", af_prim_u_gt, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "U>=", af_prim_u_gte, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "FDUP", af_prim_f_dup, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "FDROP", af_prim_f_drop, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "FSWAP", af_prim_f_swap, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "FOVER", af_prim_f_over, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "FROT", af_prim_f_rot, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "FPICK", af_prim_f_pick, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "FROLL", af_prim_f_roll, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "S>F", af_prim_s_to_f, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "D>F", af_prim_d_to_f, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "F>S", af_prim_f_to_s, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "F>D", af_prim_f_to_d, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "F+", af_prim_f_add, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "F-", af_prim_f_sub, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "F*", af_prim_f_mul, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "F/", af_prim_f_div, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "FMOD", af_prim_f_mod, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "F**", af_prim_f_pow, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "F<", af_prim_f_lt, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "F<=", af_prim_f_lte, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "F>", af_prim_f_gt, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "F>=", af_prim_f_gte, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "F=", af_prim_f_eq, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "F<>", af_prim_f_ne, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "D+", af_prim_d_add, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "D-", af_prim_d_sub, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "D*", af_prim_d_mul, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "D/", af_prim_d_div, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "DMOD", af_prim_d_mod, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "DU/", af_prim_d_u_div, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "DUMOD", af_prim_d_u_mod, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "DINVERT", af_prim_d_invert, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "DAND", af_prim_d_and, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "DOR", af_prim_d_or, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "DXOR", af_prim_d_xor, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "DLSHIFT", af_prim_d_lshift, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "DRSHIFT", af_prim_d_rshift, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "DARSHIFT", af_prim_d_arshift, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "D<", af_prim_d_lt, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "D<=", af_prim_d_lte, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "D>", af_prim_d_gt, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "D>=", af_prim_d_gte, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "D=", af_prim_d_eq, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "D<>", af_prim_d_ne, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "DU<", af_prim_d_u_lt, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "DU<=", af_prim_d_u_lte, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "DU>", af_prim_d_u_gt, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "DU>=", af_prim_d_u_gte, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "@", af_prim_fetch, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "!", af_prim_store, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "+!", af_prim_add_store, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "W@", af_prim_w_fetch, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "W!", af_prim_w_store, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "H@", af_prim_h_fetch, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "H!", af_prim_h_store, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "C@", af_prim_c_fetch, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "C!", af_prim_c_store, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "F@", af_prim_f_fetch, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "F!", af_prim_f_store, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "F+!", af_prim_f_add_store, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "SF@", af_prim_sf_fetch, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "SF!", af_prim_sf_store, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "SF+!", af_prim_sf_add_store, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "DF@", af_prim_df_fetch, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "DF!", af_prim_df_store, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "DF+!", af_prim_df_add_store, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, ">R", af_prim_to_r, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "R>", af_prim_from_r, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "R@", af_prim_r_fetch, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "HERE", af_prim_here, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "DOES>", af_prim_does, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "DOES-AT>", af_prim_does_at, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "NAME>STRING", af_prim_name_to_string, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "NAME>CODE", af_prim_name_to_code, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "NAME>SECONDARY", af_prim_name_to_secondary,
		   0, global->forth_wordlist);
  af_register_prim(global, task, "NAME>NEXT", af_prim_name_to_next, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "FIRST-WORD", af_prim_first_word, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "ALL-WORDS", af_prim_all_words, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "NAME>NEXT-ALL", af_prim_name_to_next_all, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, ">BODY", af_prim_to_body, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "SP@", af_prim_sp_fetch, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "SP!", af_prim_sp_store, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "SP0", af_prim_sp0, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "SPF", af_prim_spf, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "FP@", af_prim_fp_fetch, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "FP!", af_prim_fp_store, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "FP0", af_prim_fp0, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "FPF", af_prim_fpf, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "RP@", af_prim_rp_fetch, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "RP!", af_prim_rp_store, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "RP0", af_prim_rp0, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "RPF", af_prim_rpf, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "T@", af_prim_t_fetch, 0,
		   global->task_wordlist);
  af_register_prim(global, task, "T!", af_prim_t_store, 0,
		   global->task_wordlist);
  af_register_prim(global, task, "WT@", af_prim_wt_fetch, 0,
		   global->task_wordlist);
  af_register_prim(global, task, "WT!", af_prim_wt_store, 0,
		   global->task_wordlist);
  af_register_prim(global, task, "HT@", af_prim_ht_fetch, 0,
		   global->task_wordlist);
  af_register_prim(global, task, "HT!", af_prim_ht_store, 0,
		   global->task_wordlist);
  af_register_prim(global, task, "CT@", af_prim_ct_fetch, 0,
		   global->task_wordlist);
  af_register_prim(global, task, "CT!", af_prim_ct_store, 0,
		   global->task_wordlist);
  af_register_prim(global, task, "FT@", af_prim_ft_fetch, 0,
		   global->task_wordlist);
  af_register_prim(global, task, "FT!", af_prim_ft_store, 0,
		   global->task_wordlist);
  af_register_prim(global, task, "SFT@", af_prim_sft_fetch, 0,
		   global->task_wordlist);
  af_register_prim(global, task, "SFT!", af_prim_sft_store, 0,
		   global->task_wordlist);
  af_register_prim(global, task, "DFT@", af_prim_dft_fetch, 0,
		   global->task_wordlist);
  af_register_prim(global, task, "DFT!", af_prim_dft_store, 0,
		   global->task_wordlist);
  af_register_prim(global, task, "TASK-LOCAL",
		   af_prim_task_local, 0, global->task_wordlist);
  af_register_prim(global, task, "TASK-TRACE",
		   af_prim_task_trace, 0, global->task_wordlist);
  af_register_prim(global, task, "GLOBAL-TRACE",
		   af_prim_global_trace, 0, global->forth_wordlist);
  af_register_prim(global, task, "LITTLE-ENDIAN", af_prim_little_endian, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "'", af_prim_tick, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "[']", af_prim_bracket_tick,
		   AF_WORD_IMMEDIATE | AF_WORD_COMPILE_ONLY,
		   global->forth_wordlist);
  af_register_prim(global, task, "EXECUTE", af_prim_execute, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "STATE", af_prim_state, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "LATESTXT", af_prim_latestxt, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, ">LATESTXT", af_prim_to_latestxt, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, ">IN", af_prim_to_in, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "SOURCE", af_prim_source, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, ">INPUT", af_prim_to_input, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "INPUT>", af_prim_from_input, 0,
		   global->io_wordlist);
  af_register_prim(global, task, ">DATA", af_prim_to_data, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, ">FLOAT", af_prim_to_float, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, ">RETURN", af_prim_to_return, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, ">ABORT", af_prim_to_abort, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "ABORT>", af_prim_from_abort, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, ">TERMINATE", af_prim_to_terminate, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "TERMINATE>", af_prim_from_terminate, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, ">DEFAULT-ABORT",
		   af_prim_to_default_abort, 0, global->forth_wordlist);
  af_register_prim(global, task, "DEFAULT-ABORT>",
		   af_prim_from_default_abort, 0, global->forth_wordlist);
  af_register_prim(global, task, "ALLOCATE", af_prim_allocate, 0,
		   global->forth_wordlist);
  global->builtin_free =
    af_register_prim(global, task, "FREE", af_prim_free, 0,
		     global->forth_wordlist);
  af_register_prim(global, task, "RESIZE", af_prim_resize, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "ALLOT", af_prim_allot, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "WORD", af_prim_word, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "PARSE-NAME", af_prim_parse_name, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "PARSE-NUMBER", af_prim_parse_number, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "PARSE-2NUMBER", af_prim_parse_2number, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "PARSE-FLOAT", af_prim_parse_float, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "FORMAT-NUMBER", af_prim_format_number, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "FORMAT-UNSIGNED-NUMBER",
		   af_prim_format_unsigned_number, 0, global->forth_wordlist);
  af_register_prim(global, task, "FORMAT-2NUMBER", af_prim_format_2number, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "FORMAT-UNSIGNED-2NUMBER",
		   af_prim_format_unsigned_2number, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "FORMAT-FLOAT", af_prim_format_float, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "FORMAT-FLOAT-SCI", af_prim_format_float_sci,
		   0, global->forth_wordlist);
  af_register_prim(global, task, "BASE", af_prim_base, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "FIND-WORD", af_prim_find_word, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "CAS", af_prim_cas, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "2CAS", af_prim_2cas, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "THIS-TASK", af_prim_this_task, 0,
		   global->task_wordlist);
  af_register_prim(global, task, "SPAWN", af_prim_spawn, 0,
		   global->task_wordlist);
  af_register_prim(global, task, "SPAWN-NO-DATA", af_prim_spawn_no_data, 0,
		   global->task_wordlist);
  af_register_prim(global, task, ">FREE-DATA-ON-EXIT",
		   af_prim_free_data_on_exit, 0,
		   global->task_wordlist);
  af_register_prim(global, task, ">CONSOLE-INPUT", af_prim_to_console_input, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "CONSOLE-INPUT>",
		   af_prim_from_console_input, 0,
		   global->io_wordlist);
  af_register_prim(global, task, ">INIT-WORD", af_prim_to_init_word, 0,
		   global->task_wordlist);
  af_register_prim(global, task, "START", af_prim_start, 0,
		   global->task_wordlist);
  af_register_prim(global, task, "BYE", af_prim_bye, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "TERMINATE", af_prim_terminate, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "KILL", af_prim_kill, 0,
		   global->task_wordlist);
  af_register_prim(global, task, "YIELD", af_prim_yield, 0,
		   global->task_wordlist);
  af_register_prim(global, task, "WAIT", af_prim_wait, 0,
		   global->task_wordlist);
  af_register_prim(global, task, "END-ATOMIC-AND-WAIT",
		   af_prim_end_atomic_and_wait, 0, global->task_wordlist);
  af_register_prim(global, task, "WAKE", af_prim_wake, 0,
		   global->task_wordlist);
  af_register_prim(global, task, "BEGIN-ATOMIC", af_prim_begin_atomic, 0,
		   global->task_wordlist);
  af_register_prim(global, task, "END-ATOMIC", af_prim_end_atomic, 0,
		   global->task_wordlist);
  af_register_prim(global, task, "YIELDS", af_prim_yields, 0,
		   global->task_wordlist);
  af_register_prim(global, task, "TASKS", af_prim_tasks, 0,
		   global->task_wordlist);
  af_register_prim(global, task, ">HANDLER-TASK",
		   af_prim_to_handler_task, 0, global->task_wordlist);
  af_register_prim(global, task, "HANDLER-TASK>",
		   af_prim_from_handler_task, 0, global->task_wordlist);
  af_register_prim(global, task, "[", af_prim_open_bracket, AF_WORD_IMMEDIATE,
		   global->forth_wordlist);
  af_register_prim(global, task, "]", af_prim_close_bracket, AF_WORD_IMMEDIATE,
		   global->forth_wordlist);
  af_register_prim(global, task, "BRANCH", af_prim_branch, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "?BRANCH", af_prim_cond_branch, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "MOVE", af_prim_move, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "CELL-SIZE", af_prim_cell_size, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "FLOAT-SIZE", af_prim_float_size, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "SFLOAT-SIZE", af_prim_sfloat_size, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "DFLOAT-SIZE", af_prim_dfloat_size, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "FORTH-WORDLIST",
		   af_prim_forth_wordlist, 0, global->forth_wordlist);
  af_register_prim(global, task, "IO-WORDLIST",
		   af_prim_io_wordlist, 0, global->forth_wordlist);
  af_register_prim(global, task, "TASK-WORDLIST",
		   af_prim_task_wordlist, 0, global->forth_wordlist);
  af_register_prim(global, task, "GET-CURRENT", af_prim_get_current, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "GET-ORDER", af_prim_get_order, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "SEARCH-WORDLIST",
		   af_prim_search_wordlist, 0, global->forth_wordlist);
  af_register_prim(global, task, "SET-CURRENT", af_prim_set_current, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "SET-ORDER", af_prim_set_order, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "WORDLIST", af_prim_wordlist, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "ARGC", af_prim_argc, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "ARGV", af_prim_argv, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "GETENV", af_prim_getenv, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "SETENV", af_prim_setenv, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "UNSETENV", af_prim_unsetenv, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, ">IO-HANDLER-TASK",
		   af_prim_to_io_handler_task, 0, global->io_wordlist);
  af_register_prim(global, task, "IO-HANDLER-TASK>",
		   af_prim_from_io_handler_task, 0, global->io_wordlist);
  af_register_prim(global, task, "IO-ACTION-DESTROY",
		   af_prim_io_action_destroy, 0, global->io_wordlist);
  af_register_prim(global, task, "IO-ACTION-GET-STATE",
		   af_prim_io_action_get_state, 0, global->io_wordlist);
  af_register_prim(global, task, "IO-STATE-DESTROY", af_prim_io_state_destroy,
		   0, global->io_wordlist);
  af_register_prim(global, task, "IO-STATE-IS-DONE",
		   af_prim_io_state_is_done, 0, global->io_wordlist);
  af_register_prim(global, task, "IO-STATE-IS-CLOSED",
		   af_prim_io_state_is_closed, 0, global->io_wordlist);
  af_register_prim(global, task, "IO-STATE-HAS-HANGUP",
		   af_prim_io_state_has_hangup, 0, global->io_wordlist);
  af_register_prim(global, task, "IO-STATE-HAS-ERROR",
		   af_prim_io_state_has_error, 0, global->io_wordlist);
  af_register_prim(global, task, "IO-STATE-GET-BUFFER",
		   af_prim_io_state_get_buffer, 0, global->io_wordlist);
  af_register_prim(global, task, "IO-STATE-GET-INDEX",
		   af_prim_io_state_get_index, 0, global->io_wordlist);
  af_register_prim(global, task, "IO-STATE-GET-COUNT",
		   af_prim_io_state_get_count, 0, global->io_wordlist);
  af_register_prim(global, task, "IO-STATE-GET-OFFSET",
		   af_prim_io_state_get_offset, 0, global->io_wordlist);
  af_register_prim(global, task, "IO-OPEN", af_prim_io_open, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-PIPE", af_prim_io_pipe, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-DELETE", af_prim_io_delete, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-DELETE-DIR", af_prim_io_delete_dir, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-RENAME", af_prim_io_rename, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-ISATTY", af_prim_io_isatty, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-GET-PENDING-WRITE-COUNT",
		   af_prim_io_get_pending_write_count, 0, global->io_wordlist);
  af_register_prim(global, task, "IO-GET-MONOTONIC-TIME",
		   af_prim_io_get_monotonic_time, 0, global->io_wordlist);
  af_register_prim(global, task, "IO-GET-REAL-TIME",
		   af_prim_io_get_real_time, 0, global->io_wordlist);
  af_register_prim(global, task, "IO-SLEEP", af_prim_io_sleep, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-TELL", af_prim_io_tell, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-CLOSE-BLOCK", af_prim_io_close_block, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-CLOSE-ASYNC", af_prim_io_close_async, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-SEEK-BLOCK", af_prim_io_seek_block, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-SEEK-ASYNC", af_prim_io_seek_async, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-READ-BLOCK", af_prim_io_read_block, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-WRITE-BLOCK", af_prim_io_write_block, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-READ-ASYNC", af_prim_io_read_async, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-WRITE-ASYNC", af_prim_io_write_async, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-READ-NONBLOCK", af_prim_io_read_nonblock,
		   0, global->io_wordlist);
  af_register_prim(global, task, "IO-WRITE-NONBLOCK", af_prim_io_write_nonblock,
		   0, global->io_wordlist);
  af_register_prim(global, task, "IO-STDIN", af_prim_io_stdin, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-STDOUT", af_prim_io_stdout, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-STDERR", af_prim_io_stderr, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-RDONLY", af_prim_io_rdonly, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-WRONLY", af_prim_io_wronly, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-RDWR", af_prim_io_rdwr, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-APPEND", af_prim_io_append, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-TRUNC", af_prim_io_trunc, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-CREAT", af_prim_io_creat, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-EXCL", af_prim_io_excl, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-IRWXU", af_prim_io_irwxu, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-IRUSR", af_prim_io_irusr, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-IWUSR", af_prim_io_iwusr, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-IXUSR", af_prim_io_ixusr, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-IRWXG", af_prim_io_irwxg, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-IRGRP", af_prim_io_irgrp, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-IWGRP", af_prim_io_iwgrp, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-IXGRP", af_prim_io_ixgrp, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-IRWXO", af_prim_io_irwxo, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-IROTH", af_prim_io_iroth, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-IWOTH", af_prim_io_iwoth, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-IXOTH", af_prim_io_ixoth, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-ISUID", af_prim_io_isuid, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-ISGID", af_prim_io_isgid, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-ISVTX", af_prim_io_isvtx, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-SEEK-SET", af_prim_io_seek_set, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-SEEK-CUR", af_prim_io_seek_cur, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "IO-SEEK-END", af_prim_io_seek_end, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "INPUT-SIZE", af_prim_input_size, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "INPUT-NEXT-INPUT", af_prim_input_next_input,
		   0, global->io_wordlist);
  af_register_prim(global, task, "INPUT-BUFFER", af_prim_input_buffer, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "INPUT-COUNT", af_prim_input_count, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "INPUT-INDEX", af_prim_input_index, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "INPUT-IS-CLOSED", af_prim_input_is_closed, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "INPUT-CLEANUP", af_prim_input_cleanup, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "INPUT-REFILL", af_prim_input_refill, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "INPUT-SOURCE-ID", af_prim_input_source_id, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "INPUT-ARG", af_prim_input_arg, 0,
		   global->io_wordlist);
  af_register_prim(global, task, "DEBUGGER", af_prim_debugger, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "PRIM-DOCOL", af_prim_prim_docol, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "PRIM-PUSH", af_prim_prim_push, 0,
		   global->forth_wordlist);
  af_register_prim(global, task, "PRIM-DOES", af_prim_prim_does, 0,
		   global->forth_wordlist);
  base_interpreter =
    af_register_prim(global, task, "BASE-INTERPRETER", af_prim_base_interpreter,
		     0, global->forth_wordlist);
  global->base_interpreter_code[0].compiled_call = base_interpreter;
  global->base_interpreter_code[1].compiled_call = base_interpreter;
}

/* Docol primitive */
void af_prim_docol(af_task_t* task) {
  af_global_t* global = task->global;
  if(task->return_stack_current > task->return_stack_top) {
    *(--task->return_stack_current) = task->interpreter_pointer + 1;
    task->interpreter_pointer = task->current_word->secondary;
  } else {
    af_handle_return_stack_overflow(global, task);
  }
}

/* Exit primitive */
void af_prim_exit(af_task_t* task) {
  af_global_t* global = task->global;
  if(task->return_stack_current < task->return_stack_base) {
    task->interpreter_pointer = *task->return_stack_current++;
  } else {
    af_handle_return_stack_underflow(global, task);
  }
}

/* Push the data address of a word primitive */
void af_prim_push_data(af_task_t* task) {
  af_global_t* global = task->global;
  if(task->data_stack_current > task->data_stack_top) {
    *(--task->data_stack_current) =
      (af_cell_t)AF_WORD_DATA(task->current_word);
    AF_ADVANCE_IP(task, 1);
  } else {
    af_handle_data_stack_overflow(global, task);
  }
}

/* Runtime of DOES> primitve */
void af_prim_do_does(af_task_t* task) {
  af_global_t* global = task->global;
  if(task->data_stack_current > task->data_stack_top) {
    if(task->return_stack_current > task->return_stack_top) {
      *(--task->data_stack_current) =
	(af_cell_t)AF_WORD_DATA(task->current_word);
      *(--task->return_stack_current) =
	task->interpreter_pointer ? task->interpreter_pointer + 1 : NULL;
      task->interpreter_pointer = task->current_word->secondary;
    } else {
      af_handle_return_stack_overflow(global, task);
    }
  } else {
    af_handle_data_stack_overflow(global, task);
  }
}

/* (LITERAL) primitive */
void af_prim_literal_runtime(af_task_t* task) {
  af_global_t* global = task->global;
  if(--task->data_stack_current >= task->data_stack_top) {
    *task->data_stack_current =
      (task->interpreter_pointer + 1)->compiled_cell;
    AF_ADVANCE_IP(task, 2);
  } else {
    af_handle_data_stack_overflow(global, task);
  }
}

/* (FLITERAL) primitive */
void af_prim_f_literal_runtime(af_task_t* task) {
  af_global_t* global = task->global;
  if(--task->float_stack_current >= task->float_stack_top) {
    *task->float_stack_current =
      *(af_float_t*)(&(task->interpreter_pointer + 1)->compiled_cell);
    AF_ADVANCE_IP(task, 1 + (sizeof(af_float_t) / sizeof(af_compiled_t)));
  } else {
    af_handle_float_stack_overflow(global, task);
  }
}

/* LITERAL primitive - immediate */
void af_prim_literal(af_task_t* task) {
  af_global_t* global = task->global;
  af_compiled_t* slot;
  AF_VERIFY_COMPILING(task);
  AF_VERIFY_DATA_STACK_READ(task, 1);
  slot = af_allot(global, task, sizeof(af_compiled_t) * 2);
  if(slot) {
    slot->compiled_call = global->builtin_literal_runtime;
    (slot + 1)->compiled_cell = *task->data_stack_current++;
    AF_ADVANCE_IP(task, 1);
  }
}

/* CREATE primitive */
void af_prim_create(af_task_t* task) {
  af_global_t* global = task->global;
  af_byte_t* name;
  af_cell_t name_length;
  size_t data_space_current = (size_t)task->data_space_current;
  size_t name_size;
  size_t pad_size;
  void* word_space;
  af_word_t* word;
  af_compiled_t* secondary;
  if(!af_parse_name_available(global, task)) {
    af_handle_word_expected(global, task);
    return;
  }
  name = af_parse_name(global, task, &name_length);
  name_length = name_length < 256 ? name_length : 255;
  name_size = (name_length + 1) * sizeof(af_byte_t);
  pad_size = ((data_space_current + name_size) % sizeof(af_cell_t)) == 0 ?
    0 * sizeof(af_byte_t) :
    sizeof(af_cell_t) - ((data_space_current + name_size) % sizeof(af_cell_t));
  if(!(word_space = af_allocate(global, task,
				sizeof(af_word_t) + name_size + pad_size))) {
    return;
  }
  word = word_space + name_size + pad_size;
  AF_WORD_NAME_LEN(word) = (af_byte_t)name_length;
  memmove(AF_WORD_NAME_DATA(word), name, name_length);
  word->flags = 0;
  word->code = af_prim_push_data;
  word->secondary = NULL;
  word->next_of_all_words = global->first_of_all_words;
  global->first_of_all_words = word;
  task->most_recent_word = word;
  word->next_word = task->current_wordlist->first_word;
  task->current_wordlist->first_word = word;
  AF_ADVANCE_IP(task, 1);
}

/* CREATE-WITH-NAME primitive */
void af_prim_create_with_name(af_task_t* task) {
  af_global_t* global = task->global;
  af_byte_t* name;
  af_cell_t name_length;
  size_t data_space_current = (size_t)task->data_space_current;
  size_t name_size;
  size_t pad_size;
  void* word_space;
  af_word_t* word;
  af_compiled_t* secondary;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  name_length = *task->data_stack_current++;
  name = (af_byte_t*)(*task->data_stack_current++);
  name_size = (name_length + 1) * sizeof(af_byte_t);
  pad_size = ((data_space_current + name_size) % sizeof(af_cell_t)) == 0 ?
    0 * sizeof(af_byte_t) :
    sizeof(af_cell_t) - ((data_space_current + name_size) % sizeof(af_cell_t));
  if(!(word_space = af_allocate(global, task,
				sizeof(af_word_t) + name_size + pad_size))) {
    return;
  }
  word = word_space + name_size + pad_size;
  AF_WORD_NAME_LEN(word) = (af_byte_t)name_length;
  memmove(AF_WORD_NAME_DATA(word), name, name_length);
  word->flags = 0;
  word->code = af_prim_push_data;
  word->secondary = NULL;
  word->next_of_all_words = global->first_of_all_words;
  global->first_of_all_words = word;
  task->most_recent_word = word;
  word->next_word = task->current_wordlist->first_word;
  task->current_wordlist->first_word = word;
  AF_ADVANCE_IP(task, 1);
}

/* : primitive */
void af_prim_colon(af_task_t* task) {
  af_global_t* global = task->global;
  af_byte_t* name;
  af_cell_t name_length;
  size_t data_space_current = (size_t)task->data_space_current;
  size_t name_size;
  size_t pad_size;
  void* word_space;
  af_word_t* word;
  if(!af_parse_name_available(global, task)) {
    af_handle_word_expected(global, task);
    return;
  }
  name = af_parse_name(global, task, &name_length);
  name_length = name_length < 256 ? name_length : 255;
  name_size = (name_length + 1) * sizeof(af_byte_t);
  pad_size = ((data_space_current + name_size) % sizeof(af_cell_t)) == 0 ?
    0 * sizeof(af_byte_t) :
    sizeof(af_cell_t) - ((data_space_current + name_size) % sizeof(af_cell_t));
  if(!(word_space = af_allocate(global, task,
				sizeof(af_word_t) + name_size + pad_size))) {
    return;
  }
  word = word_space + name_size + pad_size;
  AF_WORD_NAME_LEN(word) = (af_byte_t)name_length;
  memmove(AF_WORD_NAME_DATA(word), name, name_length);
  word->next_word = task->current_wordlist->first_word;
  word->flags = AF_WORD_HIDDEN;
  word->code = af_prim_docol;
  word->secondary = (af_compiled_t*)(word + 1);
  word->next_of_all_words = global->first_of_all_words;
  global->first_of_all_words = word;
  task->most_recent_word = word;
  task->current_wordlist->first_word = word;
  task->is_compiling = TRUE;
  AF_ADVANCE_IP(task, 1);
}

/* :NONAME primitive - immediate */
void af_prim_colon_noname(af_task_t* task) {
  af_global_t* global = task->global;
  void* word_space;
  size_t data_space_current = (size_t)task->data_space_current;
  size_t name_size = sizeof(af_byte_t);
  size_t pad_size;
  af_word_t* word;
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  pad_size = ((data_space_current + name_size) % sizeof(af_cell_t)) == 0 ?
    0 * sizeof(af_byte_t) :
    sizeof(af_cell_t) - ((data_space_current + name_size) % sizeof(af_cell_t));
  if(!(word_space = af_allocate(global, task,
				sizeof(af_word_t) + name_size + pad_size))) {
    return;
  }
  word = word_space + name_size + pad_size;;
  word->next_word = NULL;
  word->flags = AF_WORD_HIDDEN;
  word->code = af_prim_docol;
  word->secondary = (af_compiled_t*)(word + 1);
  AF_WORD_NAME_LEN(word) = 0;
  task->most_recent_word = word;
  task->is_compiling = TRUE;
  *(--task->data_stack_current) = (af_cell_t)word;
  AF_ADVANCE_IP(task, 1);
}

/* CREATE-NONAME-AT primitive */
void af_prim_create_noname_at(af_task_t* task) {
  void* word_space;
  size_t name_size = sizeof(af_byte_t);
  size_t pad_size;
  af_word_t* word;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  word_space = (void*)(*task->data_stack_current);
  pad_size = (((size_t)word_space + name_size) % sizeof(af_cell_t)) == 0 ?
    0 * sizeof(af_byte_t) :
    sizeof(af_cell_t) - (((size_t)word_space + name_size) % sizeof(af_cell_t));
  word = word_space + name_size + pad_size;
  word->next_word = NULL;
  word->flags = 0;
  word->code = af_prim_push_data;
  word->secondary = (af_compiled_t*)(word + 1);
  AF_WORD_NAME_LEN(word) = 0;
  task->most_recent_word = word;
  task->is_compiling = FALSE;
  *task->data_stack_current = (af_cell_t)word;
  AF_ADVANCE_IP(task, 1);  
}

/* ; primitive - immediate */
void af_prim_semi(af_task_t* task) {
  af_global_t* global = task->global;
  void* space;
  AF_VERIFY_COMPILING(task);
  if(!(space = af_allot(global, task, sizeof(af_compiled_t) * 2))) {
    return;
  }
  ((af_compiled_t*)space)->compiled_call = global->builtin_exit;
  ((af_compiled_t*)space + 1)->compiled_cell = 0;
  task->most_recent_word->flags &= ~AF_WORD_HIDDEN;
  task->is_compiling = FALSE;
  AF_ADVANCE_IP(task, 1);
}

/* IMMEDIATE primitive */
void af_prim_immediate(af_task_t* task) {
  AF_VERIFY_WORD_CREATED(task);
  task->most_recent_word->flags |= AF_WORD_IMMEDIATE;
  AF_ADVANCE_IP(task, 1);
}

/* COMPILE-ONLY primitive */
void af_prim_compile_only(af_task_t* task) {
  AF_VERIFY_WORD_CREATED(task);
  task->most_recent_word->flags |= AF_WORD_COMPILE_ONLY;
  AF_ADVANCE_IP(task, 1);
}

/* SMART primitive */
void af_prim_smart(af_task_t* task) {
  AF_VERIFY_WORD_CREATED(task);
  task->most_recent_word->flags |= AF_WORD_SMART;
  AF_ADVANCE_IP(task, 1);
}

/* IS-IMMEDIATE primitive */
void af_prim_is_immediate(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  *task->data_stack_current =
    (af_cell_t)(((af_word_t*)
		 (*task->data_stack_current))->flags
		& AF_WORD_IMMEDIATE ? TRUE : FALSE);
  AF_ADVANCE_IP(task, 1);
}

/* IS-HIDDEN primitive */
void af_prim_is_hidden(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  *task->data_stack_current =
    (af_cell_t)(((af_word_t*)
		 (*task->data_stack_current))->flags
		& AF_WORD_HIDDEN ? TRUE : FALSE);
  AF_ADVANCE_IP(task, 1);
}

/* IS-COMPILE-ONLY primitive */
void af_prim_is_compile_only(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  *task->data_stack_current =
    (af_cell_t)(((af_word_t*)
		 (*task->data_stack_current))->flags
		& AF_WORD_COMPILE_ONLY ? TRUE : FALSE);
  AF_ADVANCE_IP(task, 1);
}

/* IS-SMART primitive */
void af_prim_is_smart(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  *task->data_stack_current =
    (af_cell_t)(((af_word_t*)
		 (*task->data_stack_current))->flags
		& AF_WORD_SMART ? TRUE : FALSE);
  AF_ADVANCE_IP(task, 1);
}

/* DUP primitive */
void af_prim_dup(af_task_t* task) {
  af_cell_t value;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  value = *task->data_stack_current;
  *(--task->data_stack_current) = value;
  AF_ADVANCE_IP(task, 1);
}

/* DROP primitive */
void af_prim_drop(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  task->data_stack_current++;
  AF_ADVANCE_IP(task, 1);
}

/* SWAP primitive */
void af_prim_swap(af_task_t* task) {
  af_cell_t value;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  value = *task->data_stack_current;
  *task->data_stack_current = *(task->data_stack_current + 1);
  *(task->data_stack_current + 1) = value;
  AF_ADVANCE_IP(task, 1);
}

/* OVER primitive */
void af_prim_over(af_task_t* task) {
  af_cell_t value;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  value = *(task->data_stack_current + 1);
  *(--task->data_stack_current) = value;
  AF_ADVANCE_IP(task, 1);
}

/* ROT primitive */
void af_prim_rot(af_task_t* task) {
  af_cell_t value;
  AF_VERIFY_DATA_STACK_READ(task, 3);
  value = *(task->data_stack_current + 2);
  *(task->data_stack_current + 2) = *(task->data_stack_current + 1);
  *(task->data_stack_current + 1) = *task->data_stack_current;
  *task->data_stack_current = value;
  AF_ADVANCE_IP(task, 1);
}

/* PICK primitive */
void af_prim_pick(af_task_t* task) {
  af_cell_t index;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  index = *task->data_stack_current;
  AF_VERIFY_DATA_STACK_READ(task, index + 2);
  *task->data_stack_current = *(task->data_stack_current + index + 1);
  AF_ADVANCE_IP(task, 1);
}

/* ROLL primitive */
void af_prim_roll(af_task_t* task) {
  af_cell_t index;
  af_cell_t value;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  index = *task->data_stack_current++;
  AF_VERIFY_DATA_STACK_READ(task, index + 1);
  value = *(task->data_stack_current + index);
  while(index > 0) {
    *(task->data_stack_current + index) =
      *(task->data_stack_current + (index - 1));
    index--;
  }
  *task->data_stack_current = value;
  AF_ADVANCE_IP(task, 1);
}

/* + primitive */
void af_prim_add(af_task_t* task) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  value0 = *((af_sign_cell_t*)task->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)task->data_stack_current;
  *(++task->data_stack_current) = (af_cell_t)(value0 + value1);
  AF_ADVANCE_IP(task, 1);
}

/* - primitive */
void af_prim_sub(af_task_t* task) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  value0 = *((af_sign_cell_t*)task->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)task->data_stack_current;
  *(++task->data_stack_current) = (af_cell_t)(value0 - value1);
  AF_ADVANCE_IP(task, 1);
}

/* * primitive */
void af_prim_mul(af_task_t* task) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  value0 = *((af_sign_cell_t*)task->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)task->data_stack_current;
  *(++task->data_stack_current) = (af_cell_t)(value0 * value1);
  AF_ADVANCE_IP(task, 1);
}

/* / primitive */
void af_prim_div(af_task_t* task) {
  af_global_t* global = task->global;
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  value0 = *((af_sign_cell_t*)task->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)task->data_stack_current;
  if(value1 == 0) {
    af_handle_divide_by_zero(global, task);
    return;
  }
  *(++task->data_stack_current) = (af_cell_t)(value0 / value1);
  AF_ADVANCE_IP(task, 1);
}

/* MOD primitive */
void af_prim_mod(af_task_t* task) {
  af_global_t* global = task->global;
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  value0 = *((af_sign_cell_t*)task->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)task->data_stack_current;
  if(value1 == 0) {
    af_handle_divide_by_zero(global, task);
    return;
  }
  *(++task->data_stack_current) = (af_cell_t)(value0 % value1);
  AF_ADVANCE_IP(task, 1);
}

/* U/ primitive */
void af_prim_u_div(af_task_t* task) {
  af_global_t* global = task->global;
  af_cell_t value0;
  af_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  value0 = *(task->data_stack_current + 1);
  value1 = *task->data_stack_current;
  if(value1 == 0) {
    af_handle_divide_by_zero(global, task);
    return;
  }
  *(++task->data_stack_current) = (af_cell_t)(value0 / value1);
  AF_ADVANCE_IP(task, 1);
}

/* UMOD primitive */
void af_prim_u_mod(af_task_t* task) {
  af_global_t* global = task->global;
  af_cell_t value0;
  af_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  value0 = *(task->data_stack_current + 1);
  value1 = *task->data_stack_current;
  if(value1 == 0) {
    af_handle_divide_by_zero(global, task);
    return;
  }
  *(++task->data_stack_current) = (af_cell_t)(value0 % value1);
  AF_ADVANCE_IP(task, 1);
}

/* INVERT primitive */
void af_prim_invert(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  *task->data_stack_current = ~(*task->data_stack_current);
  AF_ADVANCE_IP(task, 1);
}

/* AND primitive */
void af_prim_and(af_task_t* task) {
  af_cell_t value0;
  af_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  value0 = *(task->data_stack_current + 1);
  value1 = *task->data_stack_current;
  *(++task->data_stack_current) = value0 & value1;
  AF_ADVANCE_IP(task, 1);
}

/* OR primitive */
void af_prim_or(af_task_t* task) {
  af_cell_t value0;
  af_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  value0 = *(task->data_stack_current + 1);
  value1 = *task->data_stack_current;
  *(++task->data_stack_current) = value0 | value1;
  AF_ADVANCE_IP(task, 1);
}

/* XOR primitive */
void af_prim_xor(af_task_t* task) {
  af_cell_t value0;
  af_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  value0 = *(task->data_stack_current + 1);
  value1 = *task->data_stack_current;
  *(++task->data_stack_current) = value0 ^ value1;
  AF_ADVANCE_IP(task, 1);
}

/* LSHIFT primitive */
void af_prim_lshift(af_task_t* task) {
  af_cell_t value0;
  af_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  value0 = *(task->data_stack_current + 1);
  value1 = *task->data_stack_current;
  *(++task->data_stack_current) = value0 << value1;
  AF_ADVANCE_IP(task, 1);
}

/* RSHIFT primitive */
void af_prim_rshift(af_task_t* task) {
  af_cell_t value0;
  af_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  value0 = *(task->data_stack_current + 1);
  value1 = *task->data_stack_current;
  *(++task->data_stack_current) = value0 >> value1;
  AF_ADVANCE_IP(task, 1);
}

/* ARSHIFT primitive */
void af_prim_arshift(af_task_t* task) {
  af_sign_cell_t value0;
  af_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  value0 = *(task->data_stack_current + 1);
  value1 = *task->data_stack_current;
  *(++task->data_stack_current) = (af_cell_t)(value0 >> value1);
  AF_ADVANCE_IP(task, 1);
}

/* < primitive */
void af_prim_lt(af_task_t* task) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  value0 = *((af_sign_cell_t*)task->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)task->data_stack_current;
  *(++task->data_stack_current) = (af_cell_t)(value0 < value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* <= primitive */
void af_prim_lte(af_task_t* task) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  value0 = *((af_sign_cell_t*)task->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)task->data_stack_current;
  *(++task->data_stack_current) = (af_cell_t)(value0 <= value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* > primitive */
void af_prim_gt(af_task_t* task) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  value0 = *((af_sign_cell_t*)task->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)task->data_stack_current;
  *(++task->data_stack_current) = (af_cell_t)(value0 > value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* >= primitive */
void af_prim_gte(af_task_t* task) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  value0 = *((af_sign_cell_t*)task->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)task->data_stack_current;
  *(++task->data_stack_current) = (af_cell_t)(value0 >= value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* = primitive */
void af_prim_eq(af_task_t* task) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  value0 = *((af_sign_cell_t*)task->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)task->data_stack_current;
  *(++task->data_stack_current) = (af_cell_t)(value0 == value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* <> primitive */
void af_prim_ne(af_task_t* task) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  value0 = *((af_sign_cell_t*)task->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)task->data_stack_current;
  *(++task->data_stack_current) = (af_cell_t)(value0 != value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* U< primitive */
void af_prim_u_lt(af_task_t* task) {
  af_cell_t value0;
  af_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  value0 = *(task->data_stack_current + 1);
  value1 = *task->data_stack_current;
  *(++task->data_stack_current) = (af_cell_t)(value0 < value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* U<= primitive */
void af_prim_u_lte(af_task_t* task) {
  af_cell_t value0;
  af_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  value0 = *(task->data_stack_current + 1);
  value1 = *task->data_stack_current;
  *(++task->data_stack_current) = (af_cell_t)(value0 <= value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* U> primitive */
void af_prim_u_gt(af_task_t* task) {
  af_cell_t value0;
  af_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  value0 = *(task->data_stack_current + 1);
  value1 = *task->data_stack_current;
  *(++task->data_stack_current) = (af_cell_t)(value0 > value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* U>= primitive */
void af_prim_u_gte(af_task_t* task) {
  af_cell_t value0;
  af_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  value0 = *(task->data_stack_current + 1);
  value1 = *task->data_stack_current;
  *(++task->data_stack_current) = (af_cell_t)(value0 >= value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* FDUP primitive */
void af_prim_f_dup(af_task_t* task) {
  af_float_t value;
  AF_VERIFY_FLOAT_STACK_READ(task, 1);
  AF_VERIFY_FLOAT_STACK_EXPAND(task, 1);
  value = *task->float_stack_current;
  *(--task->float_stack_current) = value;
  AF_ADVANCE_IP(task, 1);
}

/* FDROP primitive */
void af_prim_f_drop(af_task_t* task) {
  AF_VERIFY_FLOAT_STACK_READ(task, 1);
  task->float_stack_current++;
  AF_ADVANCE_IP(task, 1);
}

/* FSWAP primitive */
void af_prim_f_swap(af_task_t* task) {
  af_float_t value;
  AF_VERIFY_FLOAT_STACK_READ(task, 2);
  value = *task->float_stack_current;
  *task->float_stack_current = *(task->float_stack_current + 1);
  *(task->float_stack_current + 1) = value;
  AF_ADVANCE_IP(task, 1);
}

/* FOVER primitive */
void af_prim_f_over(af_task_t* task) {
  af_float_t value;
  AF_VERIFY_FLOAT_STACK_READ(task, 2);
  AF_VERIFY_FLOAT_STACK_EXPAND(task, 1);
  value = *(task->float_stack_current + 1);
  *(--task->float_stack_current) = value;
  AF_ADVANCE_IP(task, 1);
}

/* FROT primitive */
void af_prim_f_rot(af_task_t* task) {
  af_float_t value;
  AF_VERIFY_FLOAT_STACK_READ(task, 3);
  value = *(task->float_stack_current + 2);
  *(task->float_stack_current + 2) = *(task->float_stack_current + 1);
  *(task->float_stack_current + 1) = *task->float_stack_current;
  *task->float_stack_current = value;
  AF_ADVANCE_IP(task, 1);
}

/* FPICK primitive */
void af_prim_f_pick(af_task_t* task) {
  af_cell_t index;
  af_float_t value;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  index = *task->data_stack_current++;
  AF_VERIFY_FLOAT_STACK_READ(task, index + 1);
  value = *(task->float_stack_current + index);
  *(--task->float_stack_current) = value;
  AF_ADVANCE_IP(task, 1);
}

/* FROLL primitive */
void af_prim_f_roll(af_task_t* task) {
  af_cell_t index;
  af_float_t value;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  index = *task->data_stack_current++;
  AF_VERIFY_FLOAT_STACK_READ(task, index + 1);
  value = *(task->float_stack_current + index);
  while(index > 0) {
    *(task->float_stack_current + index) =
      *(task->float_stack_current + (index - 1));
    index--;
  }
  *task->float_stack_current = value;
  AF_ADVANCE_IP(task, 1);
}

/* S>F primitive */
void af_prim_s_to_f(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  AF_VERIFY_FLOAT_STACK_EXPAND(task, 1);
  *(--task->float_stack_current) = (af_float_t)(*task->data_stack_current++);
  AF_ADVANCE_IP(task, 1);
}

/* D>F primitive */
void af_prim_d_to_f(af_task_t* task) {
  af_sign_2cell_t value;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  AF_VERIFY_FLOAT_STACK_EXPAND(task, 1);
  AF_LOAD_SIGN_2CELL(task, 0, value);
  task->data_stack_current += 2;
  *(--task->float_stack_current) = (af_float_t)value;
  AF_ADVANCE_IP(task, 1);
}

/* F>S primitive */
void af_prim_f_to_s(af_task_t* task) {
  af_float_t value;
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  AF_VERIFY_FLOAT_STACK_READ(task, 1);
  value = *task->float_stack_current++;
  if(value > 0.0) {
    value = floor(value);
  } else if(value < 0.0) {
    value = ceil(value);
  }
  *(--task->data_stack_current) = (af_cell_t)value;
  AF_ADVANCE_IP(task, 1);
}

/* F>D primitive */
void af_prim_f_to_d(af_task_t* task) {
  af_float_t value;
  AF_VERIFY_DATA_STACK_EXPAND(task, 2);
  AF_VERIFY_FLOAT_STACK_READ(task, 1);
  value = *task->float_stack_current++;
  if(value > 0.0) {
    value = floor(value);
  } else if(value < 0.0) {
    value = ceil(value);
  }
  task->data_stack_current -= 2;
  AF_STORE_2CELL(task, 0, (af_sign_2cell_t)value);
  AF_ADVANCE_IP(task, 1);
}

/* F+ primitive */
void af_prim_f_add(af_task_t* task) {
  af_float_t value0;
  af_float_t value1;
  AF_VERIFY_FLOAT_STACK_READ(task, 2);
  value0 = *(task->float_stack_current + 1);
  value1 = *task->float_stack_current;
  *(++task->float_stack_current) = value0 + value1;
  AF_ADVANCE_IP(task, 1);
}

/* F- primitive */
void af_prim_f_sub(af_task_t* task) {
  af_float_t value0;
  af_float_t value1;
  AF_VERIFY_FLOAT_STACK_READ(task, 2);
  value0 = *(task->float_stack_current + 1);
  value1 = *task->float_stack_current;
  *(++task->float_stack_current) = value0 - value1;
  AF_ADVANCE_IP(task, 1);
}

/* F* primitive */
void af_prim_f_mul(af_task_t* task) {
  af_float_t value0;
  af_float_t value1;
  AF_VERIFY_FLOAT_STACK_READ(task, 2);
  value0 = *(task->float_stack_current + 1);
  value1 = *task->float_stack_current;
  *(++task->float_stack_current) = value0 * value1;
  AF_ADVANCE_IP(task, 1);
}

/* F/ primitive */
void af_prim_f_div(af_task_t* task) {
  af_float_t value0;
  af_float_t value1;
  AF_VERIFY_FLOAT_STACK_READ(task, 2);
  value0 = *(task->float_stack_current + 1);
  value1 = *task->float_stack_current;
  *(++task->float_stack_current) = value0 / value1;
  AF_ADVANCE_IP(task, 1);
}

/* FMOD primitive */
void af_prim_f_mod(af_task_t* task) {
  af_float_t value0;
  af_float_t value1;
  AF_VERIFY_FLOAT_STACK_READ(task, 2);
  value0 = *(task->float_stack_current + 1);
  value1 = *task->float_stack_current;
  *(++task->float_stack_current) = fmod(value0, value1);
  AF_ADVANCE_IP(task, 1);
}

/* F** primitive */
void af_prim_f_pow(af_task_t* task) {
  af_float_t value0;
  af_float_t value1;
  AF_VERIFY_FLOAT_STACK_READ(task, 2);
  value0 = *(task->float_stack_current + 1);
  value1 = *task->float_stack_current;
  *(++task->float_stack_current) = pow(value0, value1);
  AF_ADVANCE_IP(task, 1);
}

/* F< primitive */
void af_prim_f_lt(af_task_t* task) {
  af_float_t value0;
  af_float_t value1;
  AF_VERIFY_FLOAT_STACK_READ(task, 2);
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  value1 = *task->float_stack_current++;
  value0 = *task->float_stack_current++;
  *(--task->data_stack_current) = (af_cell_t)(value0 < value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* F<= primitive */
void af_prim_f_lte(af_task_t* task) {
  af_float_t value0;
  af_float_t value1;
  AF_VERIFY_FLOAT_STACK_READ(task, 2);
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  value1 = *task->float_stack_current++;
  value0 = *task->float_stack_current++;
  *(--task->data_stack_current) = (af_cell_t)(value0 <= value1 ? -1 : 0);
}

/* F> primitive */
void af_prim_f_gt(af_task_t* task) {
  af_float_t value0;
  af_float_t value1;
  AF_VERIFY_FLOAT_STACK_READ(task, 2);
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  value1 = *task->float_stack_current++;
  value0 = *task->float_stack_current++;
  *(--task->data_stack_current) = (af_cell_t)(value0 > value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* F>= primitive */
void af_prim_f_gte(af_task_t* task) {
  af_float_t value0;
  af_float_t value1;
  AF_VERIFY_FLOAT_STACK_READ(task, 2);
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  value1 = *task->float_stack_current++;
  value0 = *task->float_stack_current++;
  *(--task->data_stack_current) = (af_cell_t)(value0 >= value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* F= primitive */
void af_prim_f_eq(af_task_t* task) {
  af_float_t value0;
  af_float_t value1;
  AF_VERIFY_FLOAT_STACK_READ(task, 2);
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  value1 = *task->float_stack_current++;
  value0 = *task->float_stack_current++;
  *(--task->data_stack_current) = (af_cell_t)(value0 == value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* F<> primitive */
void af_prim_f_ne(af_task_t* task) {
  af_float_t value0;
  af_float_t value1;
  AF_VERIFY_FLOAT_STACK_READ(task, 2);
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  value1 = *task->float_stack_current++;
  value0 = *task->float_stack_current++;
  *(--task->data_stack_current) = (af_cell_t)(value0 != value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* D+ primitive */
void af_prim_d_add(af_task_t* task) {
  af_sign_2cell_t value0;
  af_sign_2cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 4);
  AF_LOAD_SIGN_2CELL(task, 2, value0);
  AF_LOAD_SIGN_2CELL(task, 0, value1);
  task->data_stack_current += 2;
  AF_STORE_2CELL(task, 0, value0 + value1);
  AF_ADVANCE_IP(task, 1);
}

/* D- primitive */
void af_prim_d_sub(af_task_t* task) {
  af_sign_2cell_t value0;
  af_sign_2cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 4);
  AF_LOAD_SIGN_2CELL(task, 2, value0);
  AF_LOAD_SIGN_2CELL(task, 0, value1);
  task->data_stack_current += 2;
  AF_STORE_2CELL(task, 0, value0 - value1);
  AF_ADVANCE_IP(task, 1);
}

/* D* primitive */
void af_prim_d_mul(af_task_t* task) {
  af_sign_2cell_t value0;
  af_sign_2cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 4);
  AF_LOAD_SIGN_2CELL(task, 2, value0);
  AF_LOAD_SIGN_2CELL(task, 0, value1);
  task->data_stack_current += 2;
  AF_STORE_2CELL(task, 0, value0 * value1);
  AF_ADVANCE_IP(task, 1);
}

/* D/ primitive */
void af_prim_d_div(af_task_t* task) {
  af_global_t* global = task->global;
  af_sign_2cell_t value0;
  af_sign_2cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 4);
  AF_LOAD_SIGN_2CELL(task, 2, value0);
  AF_LOAD_SIGN_2CELL(task, 0, value1);
  if(value1 == 0) {
    af_handle_divide_by_zero(global, task);
    return;
  }
  task->data_stack_current += 2;
  AF_STORE_2CELL(task, 0, value0 / value1);
  AF_ADVANCE_IP(task, 1);
}

/* DMOD primitive */
void af_prim_d_mod(af_task_t* task) {
  af_global_t* global = task->global;
  af_sign_2cell_t value0;
  af_sign_2cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 4);
  AF_LOAD_SIGN_2CELL(task, 2, value0);
  AF_LOAD_SIGN_2CELL(task, 0, value1);
  if(value1 == 0) {
    af_handle_divide_by_zero(global, task);
    return;
  }
  task->data_stack_current += 2;
  AF_STORE_2CELL(task, 0, value0 % value1);
  AF_ADVANCE_IP(task, 1);
}

/* DU/ primitive */
void af_prim_d_u_div(af_task_t* task) {
  af_global_t* global = task->global;
  af_2cell_t value0;
  af_2cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 4);
  AF_LOAD_2CELL(task, 2, value0);
  AF_LOAD_2CELL(task, 0, value1);
  if(value1 == 0) {
    af_handle_divide_by_zero(global, task);
    return;
  }
  task->data_stack_current += 2;
  AF_STORE_2CELL(task, 0, value0 / value1);
  AF_ADVANCE_IP(task, 1);
}

/* DUMOD primitive */
void af_prim_d_u_mod(af_task_t* task) {
  af_global_t* global = task->global;
  af_2cell_t value0;
  af_2cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 4);
  AF_LOAD_2CELL(task, 2, value0);
  AF_LOAD_2CELL(task, 0, value1);
  if(value1 == 0) {
    af_handle_divide_by_zero(global, task);
    return;
  }
  task->data_stack_current += 2;
  AF_STORE_2CELL(task, 0, value0 % value1);
  AF_ADVANCE_IP(task, 1);
}

/* DINVERT primitive */
void af_prim_d_invert(af_task_t* task) {
  af_2cell_t value;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  AF_LOAD_2CELL(task, 0, value);
  AF_STORE_2CELL(task, 0, ~value);
  AF_ADVANCE_IP(task, 1);
}

/* DAND primitive */
void af_prim_d_and(af_task_t* task) {
  af_2cell_t value0;
  af_2cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 4);
  AF_LOAD_2CELL(task, 2, value0);
  AF_LOAD_2CELL(task, 0, value1);
  task->data_stack_current += 2;
  AF_STORE_2CELL(task, 0, value0 & value1);
  AF_ADVANCE_IP(task, 1);
}

/* DOR primitive */
void af_prim_d_or(af_task_t* task) {
  af_2cell_t value0;
  af_2cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 4);
  AF_LOAD_2CELL(task, 2, value0);
  AF_LOAD_2CELL(task, 0, value1);
  task->data_stack_current += 2;
  AF_STORE_2CELL(task, 0, value0 | value1);
  AF_ADVANCE_IP(task, 1);
}

/* DXOR primitive */
void af_prim_d_xor(af_task_t* task) {
  af_2cell_t value0;
  af_2cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 4);
  AF_LOAD_2CELL(task, 2, value0);
  AF_LOAD_2CELL(task, 0, value1);
  task->data_stack_current += 2;
  AF_STORE_2CELL(task, 0, value0 ^ value1);
  AF_ADVANCE_IP(task, 1);
}

/* DLSHIFT primitive */
void af_prim_d_lshift(af_task_t* task) {
  af_2cell_t value0;
  af_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 3);
  AF_LOAD_2CELL(task, 1, value0);
  value1 = *task->data_stack_current++;
  AF_STORE_2CELL(task, 0, value0 << value1);
  AF_ADVANCE_IP(task, 1);
}

/* DRSHIFT primitive */
void af_prim_d_rshift(af_task_t* task) {
  af_2cell_t value0;
  af_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 3);
  AF_LOAD_2CELL(task, 1, value0);
  value1 = *task->data_stack_current++;
  AF_STORE_2CELL(task, 0, value0 >> value1);
  AF_ADVANCE_IP(task, 1);
}

/* DARSHIFT primitive */
void af_prim_d_arshift(af_task_t* task) {
  af_sign_2cell_t value0;
  af_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 3);
  AF_LOAD_SIGN_2CELL(task, 1, value0);
  value1 = *task->data_stack_current++;
  AF_STORE_2CELL(task, 0, value0 >> value1);
  AF_ADVANCE_IP(task, 1);
}

/* D< primitive */
void af_prim_d_lt(af_task_t* task) {
  af_sign_2cell_t value0;
  af_sign_2cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 4);
  AF_LOAD_SIGN_2CELL(task, 2, value0);
  AF_LOAD_SIGN_2CELL(task, 0, value1);
  task->data_stack_current += 3;
  *task->data_stack_current = (af_cell_t)(value0 < value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* D<= primitive */
void af_prim_d_lte(af_task_t* task) {
  af_sign_2cell_t value0;
  af_sign_2cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 4);
  AF_LOAD_SIGN_2CELL(task, 2, value0);
  AF_LOAD_SIGN_2CELL(task, 0, value1);
  task->data_stack_current += 3;
  *task->data_stack_current = (af_cell_t)(value0 <= value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* D> primitive */
void af_prim_d_gt(af_task_t* task) {
  af_sign_2cell_t value0;
  af_sign_2cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 4);
  AF_LOAD_SIGN_2CELL(task, 2, value0);
  AF_LOAD_SIGN_2CELL(task, 0, value1);
  task->data_stack_current += 3;
  *task->data_stack_current = (af_cell_t)(value0 > value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* D>= primitive */
void af_prim_d_gte(af_task_t* task) {
  af_sign_2cell_t value0;
  af_sign_2cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 4);
  AF_LOAD_SIGN_2CELL(task, 2, value0);
  AF_LOAD_SIGN_2CELL(task, 0, value1);
  task->data_stack_current += 3;
  *task->data_stack_current = (af_cell_t)(value0 >= value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* D= primitive */
void af_prim_d_eq(af_task_t* task) {
  af_sign_2cell_t value0;
  af_sign_2cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 4);
  AF_LOAD_SIGN_2CELL(task, 2, value0);
  AF_LOAD_SIGN_2CELL(task, 0, value1);
  task->data_stack_current += 3;
  *task->data_stack_current = (af_cell_t)(value0 == value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* D<> primitive */
void af_prim_d_ne(af_task_t* task) {
  af_sign_2cell_t value0;
  af_sign_2cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 4);
  AF_LOAD_SIGN_2CELL(task, 2, value0);
  AF_LOAD_SIGN_2CELL(task, 0, value1);
  task->data_stack_current += 3;
  *task->data_stack_current = (af_cell_t)(value0 != value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* DU< primitive */
void af_prim_d_u_lt(af_task_t* task) {
  af_2cell_t value0;
  af_2cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 4);
  AF_LOAD_2CELL(task, 2, value0);
  AF_LOAD_2CELL(task, 0, value1);
  task->data_stack_current += 3;
  *task->data_stack_current = (af_cell_t)(value0 < value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* DU<= primitive */
void af_prim_d_u_lte(af_task_t* task) {
  af_2cell_t value0;
  af_2cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 4);
  AF_LOAD_2CELL(task, 2, value0);
  AF_LOAD_2CELL(task, 0, value1);
  task->data_stack_current += 3;
  *task->data_stack_current = (af_cell_t)(value0 <= value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* DU> primitive */
void af_prim_d_u_gt(af_task_t* task) {
  af_2cell_t value0;
  af_2cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 4);
  AF_LOAD_2CELL(task, 2, value0);
  AF_LOAD_2CELL(task, 0, value1);
  task->data_stack_current += 3;
  *task->data_stack_current = (af_cell_t)(value0 > value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* DU>= primitive */
void af_prim_d_u_gte(af_task_t* task) {
  af_2cell_t value0;
  af_2cell_t value1;
  AF_VERIFY_DATA_STACK_READ(task, 4);
  AF_LOAD_2CELL(task, 2, value0);
  AF_LOAD_2CELL(task, 0, value1);
  task->data_stack_current += 3;
  *task->data_stack_current = (af_cell_t)(value0 >= value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* @ primitive */
void af_prim_fetch(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  *task->data_stack_current = *(af_cell_t*)(*task->data_stack_current);
  AF_ADVANCE_IP(task, 1);
}

/* ! primitive */
void af_prim_store(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 2);
  *(af_cell_t*)(*task->data_stack_current) = *(task->data_stack_current + 1);
  task->data_stack_current += 2;
  AF_ADVANCE_IP(task, 1);
}

/* +! primitive */
void af_prim_add_store(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 2);
  *(af_cell_t*)(*task->data_stack_current) +=
    *(task->data_stack_current + 1);
  task->data_stack_current += 2;
  AF_ADVANCE_IP(task, 1);
}

/* W@ primitive */
void af_prim_w_fetch(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  *task->data_stack_current = *(uint32_t*)(*task->data_stack_current);
  AF_ADVANCE_IP(task, 1);
}

/* W! primitive */
void af_prim_w_store(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 2);
  *(uint32_t*)(*task->data_stack_current) =
    *(task->data_stack_current + 1) & 0xFFFFFFFF;
  task->data_stack_current += 2;
  AF_ADVANCE_IP(task, 1);
}

/* H@ primitive */
void af_prim_h_fetch(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  *task->data_stack_current = *(uint16_t*)(*task->data_stack_current);
  AF_ADVANCE_IP(task, 1);
}

/* H! primitive */
void af_prim_h_store(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 2);
  *(uint16_t*)(*task->data_stack_current) =
    *(task->data_stack_current + 1) & 0xFFFF;
  task->data_stack_current += 2;
  AF_ADVANCE_IP(task, 1);
}

/* C@ primitive */
void af_prim_c_fetch(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  *task->data_stack_current = *(af_byte_t*)(*task->data_stack_current);
  AF_ADVANCE_IP(task, 1);
}

/* C! primitive */
void af_prim_c_store(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 2);
  *(af_byte_t*)(*task->data_stack_current) =
    *(task->data_stack_current + 1) & 0xFF;
  task->data_stack_current += 2;
  AF_ADVANCE_IP(task, 1);
}

/* F@ primitive */
void af_prim_f_fetch(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  AF_VERIFY_FLOAT_STACK_EXPAND(task, 1);
  *(--task->float_stack_current) = *(af_float_t*)(*task->data_stack_current++);
  AF_ADVANCE_IP(task, 1);
}

/* F! primitive */
void af_prim_f_store(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  AF_VERIFY_FLOAT_STACK_READ(task, 1);
  *(af_float_t*)(*task->data_stack_current++) = *task->float_stack_current++;
  AF_ADVANCE_IP(task, 1);
}

/* F+! primitive */
void af_prim_f_add_store(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  AF_VERIFY_FLOAT_STACK_READ(task, 1);
  *(af_float_t*)(*task->data_stack_current++) += *task->float_stack_current++;
  AF_ADVANCE_IP(task, 1);
}

/* SF@ primitive */
void af_prim_sf_fetch(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  AF_VERIFY_FLOAT_STACK_EXPAND(task, 1);
  *(--task->float_stack_current) = *(af_sfloat_t*)(*task->data_stack_current++);
  AF_ADVANCE_IP(task, 1);
}

/* SF! primitive */
void af_prim_sf_store(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  AF_VERIFY_FLOAT_STACK_READ(task, 1);
  *(af_sfloat_t*)(*task->data_stack_current++) = *task->float_stack_current++;
  AF_ADVANCE_IP(task, 1);
}

/* SF+! primitive */
void af_prim_sf_add_store(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  AF_VERIFY_FLOAT_STACK_READ(task, 1);
  *(af_sfloat_t*)(*task->data_stack_current++) += *task->float_stack_current++;
  AF_ADVANCE_IP(task, 1);
}

/* DF@ primitive */
void af_prim_df_fetch(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  AF_VERIFY_FLOAT_STACK_EXPAND(task, 1);
  *(--task->float_stack_current) = *(af_dfloat_t*)(*task->data_stack_current++);
  AF_ADVANCE_IP(task, 1);
}

/* DF! primitive */
void af_prim_df_store(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  AF_VERIFY_FLOAT_STACK_READ(task, 1);
  *(af_dfloat_t*)(*task->data_stack_current++) = *task->float_stack_current++;
  AF_ADVANCE_IP(task, 1);
}

/* DF+! primitive */
void af_prim_df_add_store(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  AF_VERIFY_FLOAT_STACK_READ(task, 1);
  *(af_dfloat_t*)(*task->data_stack_current++) += *task->float_stack_current++;
  AF_ADVANCE_IP(task, 1);
}

/* >R primitive */
void af_prim_to_r(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  AF_VERIFY_RETURN_STACK_EXPAND(task, 1);
  *(af_cell_t*)(--task->return_stack_current) = *task->data_stack_current++;
  AF_ADVANCE_IP(task, 1);
}

/* R> primitive */
void af_prim_from_r(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  AF_VERIFY_RETURN_STACK_READ(task, 1);
  *(--task->data_stack_current) =
    *(af_cell_t*)(task->return_stack_current++);
  AF_ADVANCE_IP(task, 1);
}

/* R@ primitive */
void af_prim_r_fetch(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  AF_VERIFY_RETURN_STACK_READ(task, 1);
  *(--task->data_stack_current) =
    *(af_cell_t*)(task->return_stack_current);
  AF_ADVANCE_IP(task, 1);
}

/* HERE primitive */
void af_prim_here(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)task->data_space_current;
  AF_ADVANCE_IP(task, 1);
}

/* DOES> primitive */
void af_prim_does(af_task_t* task) {
  AF_VERIFY_WORD_CREATED(task);
  AF_VERIFY_NOT_INTERACTIVE(task);
  AF_VERIFY_RETURN_STACK_READ(task, 1);
  task->most_recent_word->code = af_prim_do_does;
  task->most_recent_word->secondary = task->interpreter_pointer + 1;
  task->interpreter_pointer = *task->return_stack_current++;
}

/* DOES-AT> primitive */
void af_prim_does_at(af_task_t* task) {
  AF_VERIFY_WORD_CREATED(task);
  AF_VERIFY_NOT_INTERACTIVE(task);
  task->most_recent_word->code = af_prim_do_does;
  task->most_recent_word->secondary =
    (af_compiled_t*)((task->interpreter_pointer + 1)->compiled_cell);
  AF_ADVANCE_IP(task, 2);
}

/* NAME>STRING primitive */
void af_prim_name_to_string(af_task_t* task) {
  af_word_t* word;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  word = (af_word_t*)(*task->data_stack_current);
  *task->data_stack_current = (af_cell_t)AF_WORD_NAME_DATA(word);
  *(--task->data_stack_current) = (af_cell_t)AF_WORD_NAME_LEN(word);
  AF_ADVANCE_IP(task, 1);
}

/* NAME>CODE primitive */
void af_prim_name_to_code(af_task_t* task) {
  af_word_t* word;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  word = (af_word_t*)(*task->data_stack_current);
  *task->data_stack_current = (af_cell_t)word->code;
  AF_ADVANCE_IP(task, 1);
}

/* NAME>SECONDARY primitive */
void af_prim_name_to_secondary(af_task_t* task) {
  af_word_t* word;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  word = (af_word_t*)(*task->data_stack_current);
  *task->data_stack_current = (af_cell_t)word->secondary;
  AF_ADVANCE_IP(task, 1);
}

/* NAME>NEXT primitive */
void af_prim_name_to_next(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  *task->data_stack_current =
    (af_cell_t)(((af_word_t*)(*task->data_stack_current))->next_word);
  AF_ADVANCE_IP(task, 1);
}

/* FIRST-WORD primitive */
void af_prim_first_word(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  *task->data_stack_current =
    (af_cell_t)(((af_wordlist_t*)(*task->data_stack_current))->first_word);
  AF_ADVANCE_IP(task, 1);
}

/* ALL-WORDS primitive */
void af_prim_all_words(af_task_t* task) {
  af_global_t* global = task->global;
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)global->first_of_all_words;
  AF_ADVANCE_IP(task, 1);
}

/* NAME>NEXT-ALL primitive */
void af_prim_name_to_next_all(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  *task->data_stack_current =
    (af_cell_t)(((af_word_t*)(*task->data_stack_current))->next_of_all_words);
  AF_ADVANCE_IP(task, 1);
}

/* >BODY primitive */
void af_prim_to_body(af_task_t* task) {
  af_word_t* word;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  word = (af_word_t*)(*task->data_stack_current);
  *task->data_stack_current = (af_cell_t)AF_WORD_DATA(word);
  AF_ADVANCE_IP(task, 1);
}

/* SP@ primitive */
void af_prim_sp_fetch(af_task_t* task) {
  af_cell_t* addr;
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  addr = task->data_stack_current;
  *(--task->data_stack_current) = (af_cell_t)addr;
  AF_ADVANCE_IP(task, 1);
}

/* SP! primitive */
void af_prim_sp_store(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  task->data_stack_current = (af_cell_t*)(*task->data_stack_current);
  AF_ADVANCE_IP(task, 1);
}

/* SP0 primitive */
void af_prim_sp0(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)task->data_stack_base;
  AF_ADVANCE_IP(task, 1);
}

/* SPF primitive */
void af_prim_spf(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)task->data_stack_top;
  AF_ADVANCE_IP(task, 1);
}

/* FP@ primitive */
void af_prim_fp_fetch(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)task->float_stack_current;
  AF_ADVANCE_IP(task, 1);
}

/* FP! primitive */
void af_prim_fp_store(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  task->float_stack_current = (af_float_t*)(*task->data_stack_current++);
  AF_ADVANCE_IP(task, 1);
}

/* FP0 primitive */
void af_prim_fp0(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)task->float_stack_base;
  AF_ADVANCE_IP(task, 1);
}

/* FPF primitive */
void af_prim_fpf(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)task->float_stack_top;
  AF_ADVANCE_IP(task, 1);
}

/* RP@ primitive */
void af_prim_rp_fetch(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)task->return_stack_current;
  AF_ADVANCE_IP(task, 1);
}

/* RP! primitive */
void af_prim_rp_store(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  task->return_stack_current = (af_compiled_t**)(*task->data_stack_current++);
  AF_ADVANCE_IP(task, 1);
}

/* RP0 primitive */
void af_prim_rp0(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)task->return_stack_base;
  AF_ADVANCE_IP(task, 1);
}

/* RPF primitive */
void af_prim_rpf(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)task->return_stack_top;
  AF_ADVANCE_IP(task, 1);
}

/* T@ primitive */
void af_prim_t_fetch(af_task_t* task) {
  af_cell_t offset;
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  offset = *(task->data_stack_current + 1);
  target_task = (af_task_t*)(*task->data_stack_current);
  *(++task->data_stack_current) =
    *(af_cell_t*)(target_task->task_local_space_base + offset);
  AF_ADVANCE_IP(task, 1);
}

/* T! primitive */
void af_prim_t_store(af_task_t* task) {
  af_cell_t value;
  af_cell_t offset;
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(task, 3);
  target_task = (af_task_t*)(*task->data_stack_current++);
  offset = *task->data_stack_current++;
  value = *task->data_stack_current++;
  *(af_cell_t*)(target_task->task_local_space_base + offset) = value;
  AF_ADVANCE_IP(task, 1);
}

/* WT@ primitive */
void af_prim_wt_fetch(af_task_t* task) {
  af_cell_t offset;
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  offset = *(task->data_stack_current + 1);
  target_task = (af_task_t*)(*task->data_stack_current);
  *(++task->data_stack_current) =
    *(uint32_t*)(target_task->task_local_space_base + offset);
  AF_ADVANCE_IP(task, 1);
}

/* WT! primitive */
void af_prim_wt_store(af_task_t* task) {
  af_cell_t value;
  af_cell_t offset;
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(task, 3);
  target_task = (af_task_t*)(*task->data_stack_current++);
  offset = *task->data_stack_current++;
  value = *task->data_stack_current++;
  *(uint32_t*)(target_task->task_local_space_base + offset) =
    value & 0xFFFFFFFF;
  AF_ADVANCE_IP(task, 1);
}

/* HT@ primitive */
void af_prim_ht_fetch(af_task_t* task) {
  af_cell_t offset;
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  offset = *(task->data_stack_current + 1);
  target_task = (af_task_t*)(*task->data_stack_current);
  *(++task->data_stack_current) =
    *(uint16_t*)(target_task->task_local_space_base + offset);
  AF_ADVANCE_IP(task, 1);
}

/* HT! primitive */
void af_prim_ht_store(af_task_t* task) {
  af_cell_t value;
  af_cell_t offset;
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(task, 3);
  target_task = (af_task_t*)(*task->data_stack_current++);
  offset = *task->data_stack_current++;
  value = *task->data_stack_current++;
  *(uint16_t*)(target_task->task_local_space_base + offset) =
    value & 0xFFFF;
  AF_ADVANCE_IP(task, 1);
}

/* CT@ primitive */
void af_prim_ct_fetch(af_task_t* task) {
  af_cell_t offset;
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  offset = *(task->data_stack_current + 1);
  target_task = (af_task_t*)(*task->data_stack_current);
  *(++task->data_stack_current) =
    *(af_byte_t*)(target_task->task_local_space_base + offset);
  AF_ADVANCE_IP(task, 1);
}

/* CT! primitive */
void af_prim_ct_store(af_task_t* task) {
  af_cell_t value;
  af_cell_t offset;
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(task, 3);
  target_task = (af_task_t*)(*task->data_stack_current++);
  offset = *task->data_stack_current++;
  value = *task->data_stack_current++;
  *(af_byte_t*)(target_task->task_local_space_base + offset) = value & 0xFF;
  AF_ADVANCE_IP(task, 1);
}

/* FT@ primitive */
void af_prim_ft_fetch(af_task_t* task) {
  af_cell_t offset;
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  AF_VERIFY_FLOAT_STACK_EXPAND(task, 1);
  target_task = (af_task_t*)(*task->data_stack_current++);
  offset = *(task->data_stack_current++);
  *(--task->float_stack_current) =
    *(af_float_t*)(target_task->task_local_space_base + offset);
  AF_ADVANCE_IP(task, 1);
}

/* FT! primitive */
void af_prim_ft_store(af_task_t* task) {
  af_float_t value;
  af_cell_t offset;
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  AF_VERIFY_FLOAT_STACK_READ(task, 1);
  target_task = (af_task_t*)(*task->data_stack_current++);
  offset = *task->data_stack_current++;
  value = *task->float_stack_current++;
  *(af_float_t*)(target_task->task_local_space_base + offset) = value;
  AF_ADVANCE_IP(task, 1);
}

/* SFT@ primitive */
void af_prim_sft_fetch(af_task_t* task) {
  af_cell_t offset;
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  AF_VERIFY_FLOAT_STACK_EXPAND(task, 1);
  target_task = (af_task_t*)(*task->data_stack_current++);
  offset = *(task->data_stack_current++);
  *(--task->float_stack_current) =
    *(af_sfloat_t*)(target_task->task_local_space_base + offset);
  AF_ADVANCE_IP(task, 1);
}

/* SFT! primitive */
void af_prim_sft_store(af_task_t* task) {
  af_float_t value;
  af_cell_t offset;
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  AF_VERIFY_FLOAT_STACK_READ(task, 1);
  target_task = (af_task_t*)(*task->data_stack_current++);
  offset = *task->data_stack_current++;
  value = *task->float_stack_current++;
  *(af_sfloat_t*)(target_task->task_local_space_base + offset) = value;
  AF_ADVANCE_IP(task, 1);
}

/* DFT@ primitive */
void af_prim_dft_fetch(af_task_t* task) {
  af_cell_t offset;
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  AF_VERIFY_FLOAT_STACK_EXPAND(task, 1);
  target_task = (af_task_t*)(*task->data_stack_current++);
  offset = *(task->data_stack_current++);
  *(--task->float_stack_current) =
    *(af_dfloat_t*)(target_task->task_local_space_base + offset);
  AF_ADVANCE_IP(task, 1);
}

/* DFT! primitive */
void af_prim_dft_store(af_task_t* task) {
  af_float_t value;
  af_cell_t offset;
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  AF_VERIFY_FLOAT_STACK_READ(task, 1);
  target_task = (af_task_t*)(*task->data_stack_current++);
  offset = *task->data_stack_current++;
  value = *task->float_stack_current++;
  *(af_dfloat_t*)(target_task->task_local_space_base + offset) = value;
  AF_ADVANCE_IP(task, 1);
}

/* TASK-LOCAL primitive */
void af_prim_task_local(af_task_t* task) {
  af_global_t* global = task->global;
  void* default_data;
  af_cell_t size;
  af_cell_t allocated_size;
  af_task_t* current_task;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  default_data = (void*)(*(task->data_stack_current + 1));
  size = *task->data_stack_current;
  allocated_size = global->task_local_space_size_allocated;
  if(allocated_size + size > global->task_local_space_size) {
    af_cell_t new_space_size = global->task_local_space_size * 2;
    current_task = global->first_task;
    if(new_space_size < allocated_size + size) {
      new_space_size = allocated_size + size;
    }
    global->default_task_local_space_base =
      realloc(global->default_task_local_space_base, new_space_size);
    while(current_task) {
      current_task->task_local_space_base =
	realloc(current_task->task_local_space_base, new_space_size);
      current_task = current_task->next_task;
    }
    global->task_local_space_size = new_space_size;
  }
  memcpy(global->default_task_local_space_base + allocated_size,
	 default_data, size);
  current_task = global->first_task;
  while(current_task) {
    memcpy(current_task->task_local_space_base + allocated_size,
	   default_data, size);
    current_task = current_task->next_task;
  }
  *(++task->data_stack_current) = allocated_size;
  global->task_local_space_size_allocated += size;
  AF_ADVANCE_IP(task, 1);
}

/* TASK-TRACE primitive */
void af_prim_task_trace(af_task_t* task) {
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  target_task = (af_task_t*)(*(task->data_stack_current++));
  target_task->do_trace = (af_bool_t)(*(task->data_stack_current++));
  AF_ADVANCE_IP(task, 1);
}

/* global-TRACE primitive */
void af_prim_global_trace(af_task_t* task) {
  af_global_t* global = task->global;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  global->do_trace = (af_bool_t)(*(task->data_stack_current++));
  AF_ADVANCE_IP(task, 1);
}

/* LITTLE-ENDIAN primitive */
void af_prim_little_endian(af_task_t* task) {
  af_cell_t value = 1;
  AF_VERIFY_DATA_STACK_EXPAND(task,  1);
  *(--task->data_stack_current) = *(af_byte_t*)(&value) ? TRUE : FALSE;
  AF_ADVANCE_IP(task, 1);
}

/* ' primitve */
void af_prim_tick(af_task_t* task) {
  af_global_t* global = task->global;
  af_byte_t* name;
  af_cell_t name_length;
  af_word_t* word;
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  if(!af_parse_name_available(global, task)) {
    af_handle_word_expected(global, task);
    return;
  }
  name = af_parse_name(global, task, &name_length);

  if(!(word = af_lookup(global, task, name, name_length))) {
#ifdef DEBUG
    af_byte_t* buffer = malloc(name_length + 1);
    memcpy(buffer, name, name_length);
    buffer[name_length] = 0;
    fprintf(stderr, "Got name: \"%s\"\n", buffer);
    free(buffer);
#endif
    
    af_handle_word_not_found(global, task);
    return;
  }
  *(--task->data_stack_current) = (af_cell_t)word;
  AF_ADVANCE_IP(task, 1);
}

/* ['] primitive - immediate */
void af_prim_bracket_tick(af_task_t* task) {
  af_global_t* global = task->global;
  af_byte_t* name;
  af_cell_t name_length;
  af_word_t* word;
  af_compiled_t* code;
  AF_VERIFY_COMPILING(task);
  if(!af_parse_name_available(global, task)) {
    af_handle_word_expected(global, task);
    return;
  }
  if(!(code = af_allot(global, task, sizeof(af_compiled_t) * 2))) {
    return;
  }
  name = af_parse_name(global, task, &name_length);
  if(!(word = af_lookup(global, task, name, name_length))) {
#ifdef DEBUG
    af_byte_t* buffer = malloc(name_length + 1);
    memcpy(buffer, name, name_length);
    buffer[name_length] = 0;
    fprintf(stderr, "Got name: \"%s\"\n", buffer);
    free(buffer);
#endif

    af_handle_word_not_found(global, task);
    return;
  }
  code->compiled_call = global->builtin_literal_runtime;
  (code + 1)->compiled_cell = (af_cell_t)word;
  AF_ADVANCE_IP(task, 1);
}

/* EXECUTE primitive */
void af_prim_execute(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  AF_WORD_EXECUTE(task, (af_word_t*)(*task->data_stack_current++));
}

/* STATE primitive */
void af_prim_state(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)(&task->is_compiling);
  AF_ADVANCE_IP(task, 1);
}

/* LATESTXT primitive */
void af_prim_latestxt(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)task->most_recent_word;
  AF_ADVANCE_IP(task, 1);
}

/* >LATESTXT primitive */
void af_prim_to_latestxt(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  task->most_recent_word = (af_word_t*)(*(task->data_stack_current++));
  AF_ADVANCE_IP(task, 1);
}

/* >IN primitive */
void af_prim_to_in(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)(&task->current_input->index);
  AF_ADVANCE_IP(task, 1);
}

/* SOURCE primitive */
void af_prim_source(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 2);
  *(--task->data_stack_current) = (af_cell_t)task->current_input->buffer;
  *(--task->data_stack_current) = task->current_input->count;
  AF_ADVANCE_IP(task, 1);
}

/* >INPUT primitive */
void af_prim_to_input(af_task_t* task) {
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  target_task = (af_task_t*)(*task->data_stack_current++);
  target_task->current_input = (af_input_t*)(*task->data_stack_current++);
  AF_ADVANCE_IP(task, 1);
}

/* INPUT> primitive */
void af_prim_from_input(af_task_t* task) {
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  target_task = (af_task_t*)(*task->data_stack_current);
  *task->data_stack_current = (af_cell_t)target_task->current_input;
  AF_ADVANCE_IP(task, 1);
}

/* >DATA primitive */
void af_prim_to_data(af_task_t* task) {
  af_global_t* global = task->global;
  af_task_t* target_task;
  af_cell_t value;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  target_task = (af_task_t*)(*task->data_stack_current++);
  value = *task->data_stack_current++;
  af_push_data(global, target_task, value);
  AF_ADVANCE_IP(task, 1);
}

/* >FLOAT primitive */
void af_prim_to_float(af_task_t* task) {
  af_global_t* global = task->global;
  af_task_t* target_task;
  af_float_t value;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  AF_VERIFY_FLOAT_STACK_READ(task, 1);
  target_task = (af_task_t*)(*task->data_stack_current++);
  value = *task->float_stack_current++;
  af_push_float(global, target_task, value);
  AF_ADVANCE_IP(task, 1);
}

/* >RETURN primitive */
void af_prim_to_return(af_task_t* task) {
  af_global_t* global = task->global;
  af_task_t* target_task;
  af_compiled_t* pointer;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  target_task = (af_task_t*)(*task->data_stack_current++);
  pointer = (af_compiled_t*)(*task->data_stack_current++);
  af_push_return(global, target_task, pointer);
  AF_ADVANCE_IP(task, 1);
}

/* >ABORT primitive */
void af_prim_to_abort(af_task_t* task) {
  af_task_t* target_task;
  af_word_t* word;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  target_task = (af_task_t*)(*task->data_stack_current++);
  target_task->abort = (af_word_t*)(*task->data_stack_current++);
  AF_ADVANCE_IP(task, 1);
}

/* ABORT> primitive */
void af_prim_from_abort(af_task_t* task) {
  af_task_t* target_task;
  af_word_t* word;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  target_task = (af_task_t*)(*task->data_stack_current);
  *task->data_stack_current = (af_cell_t)target_task->abort;
  AF_ADVANCE_IP(task, 1);
}

/* >TERMINATE primitive */
void af_prim_to_terminate(af_task_t* task) {
  af_task_t* target_task;
  af_word_t* word;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  target_task = (af_task_t*)(*task->data_stack_current++);
  target_task->terminate = (af_word_t*)(*task->data_stack_current++);
  AF_ADVANCE_IP(task, 1);
}

/* TERMINATE> primitive */
void af_prim_from_terminate(af_task_t* task) {
  af_task_t* target_task;
  af_word_t* word;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  target_task = (af_task_t*)(*task->data_stack_current);
  *task->data_stack_current = (af_cell_t)target_task->terminate;
  AF_ADVANCE_IP(task, 1);
}

/* >DEFAULT-ABORT primitive */
void af_prim_to_default_abort(af_task_t* task) {
  af_global_t* global = task->global;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  global->default_abort = (af_word_t*)(*task->data_stack_current++);
  AF_ADVANCE_IP(task, 1);
}

/* DEFAULT-ABORT> primitive */
void af_prim_from_default_abort(af_task_t* task) {
  af_global_t* global = task->global;
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)global->default_abort;
  AF_ADVANCE_IP(task, 1);
}

/* ALLOCATE primitive */
void af_prim_allocate(af_task_t* task) {
  void* data;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  if(!(data = malloc((size_t)(*task->data_stack_current)))) {
    *task->data_stack_current = (af_cell_t)NULL;
    *(--task->data_stack_current) = 1;
    return;
  }
  *task->data_stack_current = (af_cell_t)data;
  *(--task->data_stack_current) = 0;
  AF_ADVANCE_IP(task, 1);
}

/* FREE primitive */
void af_prim_free(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  free((void*)(*task->data_stack_current));
  *task->data_stack_current = 0;
  AF_ADVANCE_IP(task, 1);
}

/* RESIZE primitive */
void af_prim_resize(af_task_t* task) {
  void* data;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  if(!(data = realloc((void*)(*(task->data_stack_current + 1)),
		      (size_t)(*task->data_stack_current)))) {
    *(task->data_stack_current + 1) = (af_cell_t)NULL;
    *task->data_stack_current = 1;
    return;
  }
  *(task->data_stack_current + 1) = (af_cell_t)data;
  *task->data_stack_current = 0;
  AF_ADVANCE_IP(task, 1);
}

/* ALLOT primitive */
void af_prim_allot(af_task_t* task) {
  af_global_t* global = task->global;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  if(!af_allot(global, task, (ssize_t)(*task->data_stack_current++))) {
    return;
  }
  AF_ADVANCE_IP(task, 1);
}

/* UNUSED primitive */
void af_prim_unused(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) =
    (af_cell_t)(task->data_stack_top - task->data_stack_current);
  AF_ADVANCE_IP(task, 1);
}

/* WORD primitive */
void af_prim_word(af_task_t* task) {
  af_global_t* global = task->global;
  af_byte_t delimiter;
  af_byte_t* text;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  delimiter = (af_byte_t)(*task->data_stack_current & 0xFF);
  text = af_word(global, task, delimiter);
  *task->data_stack_current = (af_cell_t)text;
  AF_ADVANCE_IP(task, 1);
}

/* PARSE-NAME primitive */
void af_prim_parse_name(af_task_t* task) {
  af_global_t* global = task->global;
  af_byte_t* text;
  af_cell_t length;
  AF_VERIFY_DATA_STACK_EXPAND(task, 2);
  if(!af_parse_name_available(global, task)) {
    *(--task->data_stack_current) = (af_cell_t)NULL;
    *(--task->data_stack_current) = 0;
  } else {
    text = af_parse_name(global, task, &length);
    *(--task->data_stack_current) = (af_cell_t)text;
    *(--task->data_stack_current) = length;
  }
  AF_ADVANCE_IP(task, 1);
}

/* PARSE-NUMBER primitive */
void af_prim_parse_number(af_task_t* task) {
  af_global_t* global = task->global;
  af_byte_t* text;
  af_cell_t length;
  af_sign_cell_t result;
  af_bool_t success;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  text = (af_byte_t*)(*(task->data_stack_current + 1));
  length = *task->data_stack_current;
  success = af_parse_number(global, task->base, text, length, &result);
  *(task->data_stack_current + 1) = (af_cell_t)result;
  *task->data_stack_current = (af_cell_t)success;
  AF_ADVANCE_IP(task, 1);
}

/* PARSE-2NUMBER primitive */
void af_prim_parse_2number(af_task_t* task) {
  af_global_t* global = task->global;
  af_byte_t* text;
  af_cell_t length;
  af_sign_2cell_t result;
  af_bool_t success;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  text = (af_byte_t*)(*(task->data_stack_current + 1));
  length = *task->data_stack_current;
  success = af_parse_2number(global, task->base, text, length, &result);
  AF_STORE_2CELL(task, 0, result);
  *(--task->data_stack_current) = (af_cell_t)success;
  AF_ADVANCE_IP(task, 1);
}

/* PARSE-FLOAT primitive */
void af_prim_parse_float(af_task_t* task) {
  af_global_t* global = task->global;
  af_byte_t* text;
  af_cell_t length;
  af_float_t result;
  af_bool_t success;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  AF_VERIFY_FLOAT_STACK_EXPAND(task, 1);
  text = (af_byte_t*)(*(task->data_stack_current + 1));
  length = *task->data_stack_current;
  success = af_parse_float(global, text, length, &result);
  *(--task->float_stack_current) = result;
  *(++task->data_stack_current) = (af_cell_t)success;
  AF_ADVANCE_IP(task, 1);
}

/* FORMAT-NUMBER primitive */
void af_prim_format_number(af_task_t* task) {
  af_byte_t buffer[64];
  af_byte_t* current = buffer + sizeof(buffer);
  af_cell_t length = 0;
  af_cell_t value;
  af_bool_t negative = FALSE;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  value = *task->data_stack_current;
  if(value == 0) {
    *(--current) = '0';
    length++;
  } else if(task->base == 10) {
    af_sign_cell_t signed_value = (af_sign_cell_t)value;
    if(signed_value < 0) {
      negative = TRUE;
      value = (af_cell_t)(-signed_value);
    }
    while(value > 0) {
      af_byte_t part = value % 10;
      part += '0';
      value /= 10;
      *(--current) = part;
      length++;
    }
    if(negative) {
      *(--current) = '-';
      length++;
    }
  } else if(task->base >= 2 && task->base <= 36) {
    while(value > 0) {
      af_byte_t part = value % task->base;
      if(part < 10) {
	part += '0';
      } else {
	part += 'A' - 10;
      }
      value /= task->base;
      *(--current) = part;
      length++;
    }
  }
  memcpy(task->data_space_current, current, length);
  *task->data_stack_current = (af_cell_t)task->data_space_current;
  *(--task->data_stack_current) = length;
  AF_ADVANCE_IP(task, 1);
}

/* FORMAT-UNSIGNED-NUMBER primitive */
void af_prim_format_unsigned_number(af_task_t* task) {
  af_byte_t buffer[64];
  af_byte_t* current = buffer + sizeof(buffer);
  af_cell_t length = 0;
  af_cell_t value;
  af_bool_t negative = FALSE;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  value = *task->data_stack_current;
  if(value == 0) {
    *(--current) = '0';
    length++;
  } else if(task->base >= 2 && task->base <= 36) {
    while(value > 0) {
      af_byte_t part = value % task->base;
      if(part < 10) {
	part += '0';
      } else {
	part += 'A' - 10;
      }
      value /= task->base;
      *(--current) = part;
      length++;
    }
  }
  memcpy(task->data_space_current, current, length);
  *task->data_stack_current = (af_cell_t)task->data_space_current;
  *(--task->data_stack_current) = length;
  AF_ADVANCE_IP(task, 1);
}

/* FORMAT-2NUMBER primitive */
void af_prim_format_2number(af_task_t* task) {
  af_byte_t buffer[128];
  af_byte_t* current = buffer + sizeof(buffer);
  af_cell_t length = 0;
  af_2cell_t value;
  af_bool_t negative = FALSE;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  AF_LOAD_2CELL(task, 0, value);
  if(value == 0) {
    *(--current) = '0';
    length++;
  } else if(task->base == 10) {
    af_sign_2cell_t signed_value = (af_sign_2cell_t)value;
    if(signed_value < 0) {
      negative = TRUE;
      value = (af_2cell_t)(-signed_value);
    }
    while(value > 0) {
      af_byte_t part = value % 10;
      part += '0';
      value /= 10;
      *(--current) = part;
      length++;
    }
    if(negative) {
      *(--current) = '-';
      length++;
    }
  } else if(task->base >= 2 && task->base <= 36) {
    while(value > 0) {
      af_byte_t part = value % task->base;
      if(part < 10) {
	part += '0';
      } else {
	part += 'A' - 10;
      }
      value /= task->base;
      *(--current) = part;
      length++;
    }
  }
  memcpy(task->data_space_current, current, length);
  *(task->data_stack_current + 1) = (af_cell_t)task->data_space_current;
  *task->data_stack_current = length;
  AF_ADVANCE_IP(task, 1);
}

/* FORMAT-UNSIGNED-2NUMBER primitive */
void af_prim_format_unsigned_2number(af_task_t* task) {
  af_byte_t buffer[128];
  af_byte_t* current = buffer + sizeof(buffer);
  af_cell_t length = 0;
  af_2cell_t value;
  af_bool_t negative = FALSE;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  AF_LOAD_2CELL(task, 0, value);
  if(value == 0) {
    *(--current) = '0';
    length++;
  } else if(task->base >= 2 && task->base <= 36) {
    while(value > 0) {
      af_byte_t part = value % task->base;
      if(part < 10) {
	part += '0';
      } else {
	part += 'A' - 10;
      }
      value /= task->base;
      *(--current) = part;
      length++;
    }
  }
  memcpy(task->data_space_current, current, length);
  *(task->data_stack_current + 1) = (af_cell_t)task->data_space_current;
  *task->data_stack_current = length;
  AF_ADVANCE_IP(task, 1);
}

/* FORMAT-FLOAT primitive */
void af_prim_format_float(af_task_t* task) {
  af_byte_t buffer[64];
  af_byte_t format[64];
  af_cell_t length;
  af_float_t value;
  int precision;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  AF_VERIFY_FLOAT_STACK_READ(task, 1);
  AF_VERIFY_DATA_STACK_EXPAND(task, 2);
  precision = *task->data_stack_current;
  if(precision < 0) {
    precision = 0;
  }
  value = *task->float_stack_current++;
  snprintf(format, sizeof(format), "%%.%df", precision);
  snprintf(buffer, sizeof(buffer), format, value);
  length = strlen(buffer);
  memcpy(task->data_space_current, buffer, length);
  *task->data_stack_current = (af_cell_t)task->data_space_current;
  *(--task->data_stack_current) = length;
  AF_ADVANCE_IP(task, 1);
}

/* FORMAT-FLOAT-SCI primitive */
void af_prim_format_float_sci(af_task_t* task) {
  af_byte_t buffer[64];
  af_byte_t format[64];
  af_cell_t length;
  af_float_t value;
  int precision;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  AF_VERIFY_FLOAT_STACK_READ(task, 1);
  AF_VERIFY_DATA_STACK_EXPAND(task, 2);
  precision = *task->data_stack_current;
  if(precision < 0) {
    precision = 0;
  }
  value = *task->float_stack_current++;
  snprintf(format, sizeof(format), "%%.%de", precision);
  snprintf(buffer, sizeof(buffer), format, value);
  length = strlen(buffer);
  memcpy(task->data_space_current, buffer, length);
  *task->data_stack_current = (af_cell_t)task->data_space_current;
  *(--task->data_stack_current) = length;
  AF_ADVANCE_IP(task, 1);
}

/* BASE primitive */
void af_prim_base(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)(&task->base);
  AF_ADVANCE_IP(task, 1);
}

/* FIND-WORD primitive */
void af_prim_find_word(af_task_t* task) {
  af_global_t* global = task->global;
  af_byte_t* text;
  af_cell_t length;
  af_word_t* word;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  text = (af_byte_t*)(*(task->data_stack_current + 1));
  length = *task->data_stack_current;
  word = af_lookup(global, task, text, length);
  if(!word) {
    AF_VERIFY_DATA_STACK_EXPAND(task, 1);
    *(--task->data_stack_current) = 0;
  } else {
    *(task->data_stack_current + 1) = (af_cell_t)word;
    *task->data_stack_current =
      (word->flags & AF_WORD_IMMEDIATE) ? 1 : (af_cell_t)(-1);
  }
  AF_ADVANCE_IP(task, 1);
}

/* CAS primitive */
void af_prim_cas(af_task_t* task) {
  af_cell_t* addr;
  AF_VERIFY_DATA_STACK_READ(task, 3);
  addr = (af_cell_t*)(*task->data_stack_current);
  if(*addr == *(task->data_stack_current + 2)) {
    *addr = *(task->data_stack_current + 1);
    task->data_stack_current += 2;
    *task->data_stack_current = TRUE;
  } else {
    task->data_stack_current += 2;
    *task->data_stack_current = FALSE;
  }
  AF_ADVANCE_IP(task, 1);
}

/* 2CAS primitive */
void af_prim_2cas(af_task_t* task) {
  af_cell_t* addr;
  AF_VERIFY_DATA_STACK_READ(task, 5);
  addr = (af_cell_t*)(*task->data_stack_current);
  if(*addr == *(task->data_stack_current + 3) &&
     *(addr + 1) == *(task->data_stack_current + 4)) {
    *addr = *(task->data_stack_current + 1);
    *(addr + 1) = *(task->data_stack_current + 2);
    task->data_stack_current += 4;
    *task->data_stack_current = TRUE;
  } else {
    task->data_stack_current += 4;
    *task->data_stack_current = TRUE;
  }
  AF_ADVANCE_IP(task, 1);
}

/* THIS-TASK primitive */
void af_prim_this_task(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)task;
  AF_ADVANCE_IP(task, 1);
}

/* SPAWN primitive */
void af_prim_spawn(af_task_t* task) {
  af_global_t* global = task->global;
  af_task_t* new_task;
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  if(!(new_task = af_spawn(global, task))) {
    return;
  }
  *(--task->data_stack_current) = (af_cell_t)new_task;
  AF_ADVANCE_IP(task, 1);
}

/* SPAWN-NO-DATA primitive */
void af_prim_spawn_no_data(af_task_t* task) {
  af_global_t* global = task->global;
  af_task_t* new_task;
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  if(!(new_task = af_spawn_no_data(global, task))) {
    return;
  }
  *(--task->data_stack_current) = (af_cell_t)new_task;
  AF_ADVANCE_IP(task, 1);
}

/* >FREE-DATA-ON-EXIT primitive */
void af_prim_free_data_on_exit(af_task_t* task) {
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  target_task = (af_task_t*)(*task->data_stack_current++);
  target_task->free_data_on_exit = (af_bool_t)(*task->data_stack_current++);
  AF_ADVANCE_IP(task, 1);
}

/* >CONSOLE-INPUT primitive */
void af_prim_to_console_input(af_task_t* task) {
  af_task_t* target_task;
  af_input_t* current_input;
  af_input_t* console_input;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  target_task = (af_task_t*)(*task->data_stack_current++);
  console_input = (af_input_t*)(*task->data_stack_current++);
  current_input = target_task->current_input;
  while(current_input && current_input->next_input) {
    current_input = current_input->next_input;
  }
  if(current_input) {
    current_input->next_input = console_input;
  } else {
    target_task->current_input = console_input;
  }
  AF_ADVANCE_IP(task, 1);
}

/* CONSOLE-INPUT> primitive */
void af_prim_from_console_input(af_task_t* task) {
  af_task_t* target_task;
  af_input_t* current_input;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  target_task = (af_task_t*)(*task->data_stack_current);
  current_input = target_task->current_input;
  while(current_input && current_input->next_input) {
    current_input = current_input->next_input;
  }
  *task->data_stack_current = (af_cell_t)current_input;
  AF_ADVANCE_IP(task, 1);
}

/* >INIT-WORD primitive */
void af_prim_to_init_word(af_task_t* task) {
  af_global_t* global = task->global;
  af_task_t* target_task;
  af_word_t* word;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  target_task = (af_task_t*)(*task->data_stack_current++);
  word = (af_word_t*)(*task->data_stack_current++);
  af_set_init_word(global, target_task, word);
  AF_ADVANCE_IP(task, 1);
}

/* START primitive */
void af_prim_start(af_task_t* task) {
  af_global_t* global = task->global;
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  target_task = (af_task_t*)(*task->data_stack_current++);
  af_start(global, target_task);
  AF_ADVANCE_IP(task, 1);
}

/* BYE primitive */
void af_prim_bye(af_task_t* task) {
  af_global_t* global = task->global;
  af_bye(global);
  AF_ADVANCE_IP(task, 1);
}

/* TERMINATE primitive */
void af_prim_terminate(af_task_t* task) {
  af_global_t* global = task->global;
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  target_task = (af_task_t*)(*task->data_stack_current++);
  af_terminate(global, target_task);
  AF_ADVANCE_IP(task, 1);
}

/* KILL primitive */
void af_prim_kill(af_task_t* task) {
  af_global_t* global = task->global;
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  target_task = (af_task_t*)(*task->data_stack_current++);
  af_kill(global, target_task);
  AF_ADVANCE_IP(task, 1);
}

/* YIELD primitive */
void af_prim_yield(af_task_t* task) {
  af_global_t* global = task->global;
  af_yield(global, task);
  AF_ADVANCE_IP(task, 1);
}

/* WAIT primitive */
void af_prim_wait(af_task_t* task) {
  af_global_t* global = task->global;
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  target_task = (af_task_t*)(*task->data_stack_current++);
  af_wait(global, target_task);
  AF_ADVANCE_IP(task, 1);
}

/* END-ATOMIC-AND-WAIT primitive */
void af_prim_end_atomic_and_wait(af_task_t* task) {
  af_global_t* global = task->global;
  af_end_atomic_and_wait(global, task);
  AF_ADVANCE_IP(task, 1);
}

/* WAKE primitive */
void af_prim_wake(af_task_t* task) {
  af_global_t* global = task->global;
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  target_task = (af_task_t*)(*task->data_stack_current++);
  af_wake(global, target_task);
  AF_ADVANCE_IP(task, 1);
}

/* BEGIN-ATOMIC primitive */
void af_prim_begin_atomic(af_task_t* task) {
  af_global_t* global = task->global;
  af_begin_atomic(global, task);
  AF_ADVANCE_IP(task, 1);
}

/* END-ATOMIC primitive */
void af_prim_end_atomic(af_task_t* task) {
  af_global_t* global = task->global;
  af_end_atomic(global, task);
  AF_ADVANCE_IP(task, 1);
}

/* YIELDS primitive */
void af_prim_yields(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 2);
  task->data_stack_current -= 2;
  AF_STORE_2CELL(task, 0, task->yields);
  AF_ADVANCE_IP(task, 1);
}

/* TASKS primitive */
void af_prim_tasks(af_task_t* task) {
  af_global_t* global = task->global;
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = global->task_count;
  AF_ADVANCE_IP(task, 1);
}

/* >HANDLER-TASK primitive */
void af_prim_to_handler_task(af_task_t* task) {
  af_global_t* global = task->global;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  global->handler_task = (af_task_t*)(*task->data_stack_current++);
  AF_ADVANCE_IP(task, 1);
}

/* HANDLER-TASK> primitive */
void af_prim_from_handler_task(af_task_t* task) {
  af_global_t* global = task->global;
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)global->handler_task;
  AF_ADVANCE_IP(task, 1);
}

/* [ primitive - immediate */
void af_prim_open_bracket(af_task_t* task) {
  task->is_compiling = FALSE;
  AF_ADVANCE_IP(task, 1);
}

/* ] primitive */
void af_prim_close_bracket(af_task_t* task) {
  AF_VERIFY_WORD_CREATED(task);
  task->is_compiling = TRUE;
  AF_ADVANCE_IP(task, 1);
}

/* BRANCH primitive */
void af_prim_branch(af_task_t* task) {
  af_global_t* global = task->global;
  if(task->interpreter_pointer) {
    task->interpreter_pointer =
      (af_compiled_t*)((task->interpreter_pointer + 1)->compiled_cell);
  } else {
    af_handle_not_interactive(global, task);
  }
}

/* ?BRANCH primitive */
void af_prim_cond_branch(af_task_t* task) {
  af_global_t* global = task->global;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  if(task->interpreter_pointer) {
    af_cell_t value = *task->data_stack_current++;
    if(!value) {
      task->interpreter_pointer =
	(af_compiled_t*)((task->interpreter_pointer + 1)->compiled_cell);
    } else {
      AF_ADVANCE_IP(task, 2);
    }
  } else {
    af_handle_not_interactive(global, task);
  }
}

/* MOVE primitive */
void af_prim_move(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 3);
  memmove((void*)(*(task->data_stack_current + 1)),
	  (void*)(*(task->data_stack_current + 2)),
	  (size_t)(*task->data_stack_current));
  task->data_stack_current += 3;
  AF_ADVANCE_IP(task, 1);
}

/* CELL-SIZE primitive */
void af_prim_cell_size(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)sizeof(af_cell_t);
  AF_ADVANCE_IP(task, 1);
}

/* FLOAT-SIZE primitive */
void af_prim_float_size(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)sizeof(af_float_t);
  AF_ADVANCE_IP(task, 1);
}

/* SFLOAT-SIZE primitive */
void af_prim_sfloat_size(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)sizeof(af_sfloat_t);
  AF_ADVANCE_IP(task, 1);
}

/* DFLOAT-SIZE primitive */
void af_prim_dfloat_size(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)sizeof(af_dfloat_t);
  AF_ADVANCE_IP(task, 1);
}

/* FORTH-WORDLIST primitive */
void af_prim_forth_wordlist(af_task_t* task) {
  af_global_t* global = task->global;
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)global->forth_wordlist;
  AF_ADVANCE_IP(task, 1);
}

/* IO-WORDLIST primitive */
void af_prim_io_wordlist(af_task_t* task) {
  af_global_t* global = task->global;
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)global->io_wordlist;
  AF_ADVANCE_IP(task, 1);
}

/* TASK-WORDLIST primitive */
void af_prim_task_wordlist(af_task_t* task) {
  af_global_t* global = task->global;
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)global->task_wordlist;
  AF_ADVANCE_IP(task, 1);
}

/* GET-CURRENT primitive */
void af_prim_get_current(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)task->current_wordlist;
  AF_ADVANCE_IP(task, 1);
}

/* GET-ORDER primitive */
void af_prim_get_order(af_task_t* task) {
  af_wordlist_t** current = task->wordlist_order + task->wordlist_order_count;
  af_cell_t count = task->wordlist_order_count;
  AF_VERIFY_DATA_STACK_EXPAND(task, task->wordlist_order_count + 1);
  while(count--) {
    *(--task->data_stack_current) = (af_cell_t)(*(--current));
  }
  *(--task->data_stack_current) = task->wordlist_order_count;
  AF_ADVANCE_IP(task, 1);
}

/* SEARCH-WORDLIST primitive */
void af_prim_search_wordlist(af_task_t* task) {
  af_word_t* word;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  if(!(word = af_search_wordlist((af_wordlist_t*)(*task->data_stack_current),
				 (af_byte_t*)(*(task->data_stack_current + 2)),
				 *(task->data_stack_current + 1)))) {
    task->data_stack_current += 2;
    *task->data_stack_current = 0;
  } else {
    task->data_stack_current++;
    *(task->data_stack_current + 1) = (af_cell_t)word;
    *task->data_stack_current =
      (word->flags & AF_WORD_IMMEDIATE) ? 1 : (af_cell_t)(-1);
  }
  AF_ADVANCE_IP(task, 1);
}

/* SET-CURRENT primitive */
void af_prim_set_current(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  task->current_wordlist = (af_wordlist_t*)(*task->data_stack_current++);
  AF_ADVANCE_IP(task, 1);
}

/* SET-ORDER primitive */
void af_prim_set_order(af_task_t* task) {
  af_global_t* global = task->global;
  af_cell_t count;
  af_wordlist_t** current = task->wordlist_order;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  count = *task->data_stack_current;
  if(count != (af_cell_t)-1) {
    AF_VERIFY_DATA_STACK_READ(task, count + 1);
    if(count > task->wordlist_order_max_count) {
      af_handle_wordlist_too_large(global, task);
      return;
    }
    task->wordlist_order_count = count;
    task->data_stack_current++;
    while(count--) {
      *current++ = (af_wordlist_t*)(*task->data_stack_current++);
    }
  } else {
    task->wordlist_order_count = 1;
    task->data_stack_current++;
    *current = global->forth_wordlist;
  }
  AF_ADVANCE_IP(task, 1);
}

/* WORDLIST primitive */
void af_prim_wordlist(af_task_t* task) {
  af_global_t* global = task->global;
  af_wordlist_t* wordlist;
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  if(!(wordlist = malloc(sizeof(af_wordlist_t)))) {
    af_handle_out_of_memory(global, task);
    return;
  }
  wordlist->first_word = NULL;
  *(--task->data_stack_current) = (af_cell_t)wordlist;
  AF_ADVANCE_IP(task, 1);
}

/* ARGC primitive */
void af_prim_argc(af_task_t* task) {
  af_global_t* global = task->global;
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = global->argc;
  AF_ADVANCE_IP(task, 1);
}

/* ARGV primitive */
void af_prim_argv(af_task_t* task) {
  af_global_t* global = task->global;
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)global->argv;
  AF_ADVANCE_IP(task, 1);
}

/* GETENV primitive */
void af_prim_getenv(af_task_t* task) {
  af_global_t* global = task->global;
  char* name;
  char* value;
  af_byte_t* name_addr;
  af_cell_t name_len;
  int error = 0;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  name_len = *task->data_stack_current++;
  name_addr = (af_byte_t*)(*task->data_stack_current);
  if(!(name = malloc(name_len + 1))) {
    af_handle_out_of_memory(global, task);
    return;
  }
  memcpy(name, name_addr, name_len);
  name[name_len] = 0;
  value = getenv(name);
  free(name);
  *task->data_stack_current = (af_cell_t)value;
  AF_ADVANCE_IP(task, 1);  
}

/* SETENV primitive */
void af_prim_setenv(af_task_t* task) {
  af_global_t* global = task->global;
  char* value;
  char* name;
  af_byte_t* value_addr;
  af_cell_t value_len;
  af_byte_t* name_addr;
  af_cell_t name_len;
  int overwrite;
  int error = 0;
  AF_VERIFY_DATA_STACK_READ(task, 5);
  overwrite = *task->data_stack_current++ ? 1 : 0;
  name_len = *task->data_stack_current++;
  name_addr = (af_byte_t*)(*task->data_stack_current++);
  value_len = *task->data_stack_current++;
  value_addr = (af_byte_t*)(*task->data_stack_current);
  if(!(name = malloc(name_len + 1))) {
    af_handle_out_of_memory(global, task);
    return;
  }
  memcpy(name, name_addr, name_len);
  name[name_len] = 0;
  if(!(value = malloc(value_len + 1))) {
    free(name);
    af_handle_out_of_memory(global, task);
    return;
  }
  memcpy(value, value_addr, value_len);
  value[value_len] = 0;
  if(setenv(name, value, overwrite)) {
    error = errno;
  }
  free(value);
  free(name);
  *task->data_stack_current = error;
  AF_ADVANCE_IP(task, 1);
}

/* UNSETENV primitive */
void af_prim_unsetenv(af_task_t* task) {
  af_global_t* global = task->global;
  char* name;
  af_byte_t* name_addr;
  af_cell_t name_len;
  int error = 0;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  name_len = *task->data_stack_current++;
  name_addr = (af_byte_t*)(*task->data_stack_current);
  if(!(name = malloc(name_len + 1))) {
    af_handle_out_of_memory(global, task);
    return;
  }
  memcpy(name, name_addr, name_len);
  name[name_len] = 0;
  if(unsetenv(name)) {
    error = errno;
  }
  free(name);
  *task->data_stack_current = error;
  AF_ADVANCE_IP(task, 1);
}

/* >IO-HANDLER-TASK primitive */
void af_prim_to_io_handler_task(af_task_t* task) {
  af_global_t* global = task->global;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  af_io_set_handler_task(&global->io,
			 (af_task_t*)(*task->data_stack_current++));
  AF_ADVANCE_IP(task, 1);
}

/* IO-HANDLER-TASK> primitive */
void af_prim_from_io_handler_task(af_task_t* task) {
  af_global_t* global = task->global;
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) =
    (af_cell_t)af_io_get_handler_task(&global->io);
  AF_ADVANCE_IP(task, 1);
}
/* IO-ACTION-DESTROY primitive */
void af_prim_io_action_destroy(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  af_io_action_destroy((af_io_action_t*)(*task->data_stack_current++));
  AF_ADVANCE_IP(task, 1);
}

/* IO-ACTION-GET-STATE primitive */
void af_prim_io_action_get_state(af_task_t* task) {
  af_global_t* global = task->global;
  af_io_state_t* state;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  if(!(state = malloc(sizeof(af_io_state_t)))) {
    af_handle_out_of_memory(global, task);
    return;
  }
  af_io_action_get_state((af_io_action_t*)(*task->data_stack_current), state);
  *task->data_stack_current = (af_cell_t)state;
  AF_ADVANCE_IP(task, 1);
}

/* IO-STATE-DESTROY primitive */
void af_prim_io_state_destroy(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  free((void*)(*task->data_stack_current++));
  AF_ADVANCE_IP(task, 1);
}

/* IO-STATE-IS-DONE primitive */
void af_prim_io_state_is_done(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  *(af_bool_t*)task->data_stack_current =
    af_io_state_is_done((af_io_state_t*)(*task->data_stack_current));
  AF_ADVANCE_IP(task, 1);
}

/* IO-STATE-IS-CLOSED primitive */
void af_prim_io_state_is_closed(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  *(af_bool_t*)task->data_stack_current =
    af_io_state_is_closed((af_io_state_t*)(*task->data_stack_current));
  AF_ADVANCE_IP(task, 1);
}

/* IO-STATE-HAS-HANGUP primitive */
void af_prim_io_state_has_hangup(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  *(af_bool_t*)task->data_stack_current =
    af_io_state_has_hangup((af_io_state_t*)(*task->data_stack_current));
  AF_ADVANCE_IP(task, 1);
}

/* IO-STATE-HAS-ERROR primitive */
void af_prim_io_state_has_error(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  *(af_bool_t*)task->data_stack_current =
    af_io_state_has_error((af_io_state_t*)(*task->data_stack_current));
  AF_ADVANCE_IP(task, 1);
}

/* IO-STATE-GET-BUFFER primitive */
void af_prim_io_state_get_buffer(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  *(af_byte_t**)task->data_stack_current =
    af_io_state_get_buffer((af_io_state_t*)(*task->data_stack_current));
  AF_ADVANCE_IP(task, 1);
}

/* IO-STATE-GET-INDEX primitive */
void af_prim_io_state_get_index(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  *task->data_stack_current =
    af_io_state_get_index((af_io_state_t*)(*task->data_stack_current));
  AF_ADVANCE_IP(task, 1);
}

/* IO-STATE-GET-COUNT primitive */
void af_prim_io_state_get_count(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  *task->data_stack_current =
    af_io_state_get_count((af_io_state_t*)(*task->data_stack_current));
  AF_ADVANCE_IP(task, 1);
}

/* IO-STATE-GET-OFFSET primitive */
void af_prim_io_state_get_offset(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  *task->data_stack_current =
    (af_cell_t) af_io_state_get_offset((af_io_state_t*)
				       (*task->data_stack_current));
  AF_ADVANCE_IP(task, 1);
}

/* IO-OPEN primitive */
void af_prim_io_open(af_task_t* task) {
  af_io_fd_t fd;
  af_io_error_t error;
  AF_VERIFY_DATA_STACK_READ(task, 4);
  fd = af_io_open((af_byte_t*)(*(task->data_stack_current + 3)),
		  *(task->data_stack_current + 2),
		  (af_io_flags_t)(*(task->data_stack_current + 1)),
		  (af_io_mode_t)(*task->data_stack_current),
		  &error);
  task->data_stack_current += 2;
  *(task->data_stack_current + 1) = (af_cell_t)fd;
  *task->data_stack_current = (af_cell_t)error;
  AF_ADVANCE_IP(task, 1);
}

/* IO-PIPE primitive */
void af_prim_io_pipe(af_task_t* task) {
  af_io_fd_t in;
  af_io_fd_t out;
  af_io_error_t error;
  AF_VERIFY_DATA_STACK_EXPAND(task, 3);
  af_io_pipe(&in, &out, &error);
  *(--task->data_stack_current) = (af_cell_t)in;
  *(--task->data_stack_current) = (af_cell_t)out;
  *(--task->data_stack_current) = (af_cell_t)error;
  AF_ADVANCE_IP(task, 1);
}

/* IO-DELETE primitive */
void af_prim_io_delete(af_task_t* task) {
  af_io_error_t error;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  af_io_delete((af_byte_t*)(*(task->data_stack_current + 1)),
	       *task->data_stack_current, &error);
  task->data_stack_current++;
  *task->data_stack_current = (af_cell_t)error;
  AF_ADVANCE_IP(task, 1);
}

/* IO-DELETE-DIR primitive */
void af_prim_io_delete_dir(af_task_t* task) {
  af_io_error_t error;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  af_io_delete_dir((af_byte_t*)(*(task->data_stack_current + 1)),
		   *task->data_stack_current, &error);
  task->data_stack_current++;
  *task->data_stack_current = (af_cell_t)error;
  AF_ADVANCE_IP(task, 1);
}

/* IO-RENAME primitive */
void af_prim_io_rename(af_task_t* task) {
  af_io_error_t error;
  AF_VERIFY_DATA_STACK_READ(task, 4);
  af_io_rename((af_byte_t*)(*(task->data_stack_current + 3)),
	       *(task->data_stack_current + 2),
	       (af_byte_t*)(*(task->data_stack_current + 1)),
	       *task->data_stack_current, &error);
  task->data_stack_current += 3;
  *task->data_stack_current = (af_cell_t)error;
  AF_ADVANCE_IP(task, 1);
}

/* IO-ISATTY primitive */
void af_prim_io_isatty(af_task_t* task) {
  af_io_error_t error;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *task->data_stack_current = af_io_isatty(*task->data_stack_current, &error);
  *(--task->data_stack_current) = error;
  AF_ADVANCE_IP(task, 1);
}

/* IO-GET-PENDING-WRITE-COUNT primitive */
void af_prim_io_get_pending_write_count(af_task_t* task) {
  af_global_t* global = task->global;
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = af_io_get_pending_write_count(&global->io);
  AF_ADVANCE_IP(task, 1);
}

/* IO-GET-MONOTONIC-TIME primitive */
void af_prim_io_get_monotonic_time(af_task_t* task) {
  af_time_t monotonic_time;
  AF_VERIFY_DATA_STACK_EXPAND(task, 2);
  af_io_get_monotonic_time(&monotonic_time);
  *(--task->data_stack_current) = monotonic_time.sec;
  *(--task->data_stack_current) = monotonic_time.nsec;
  AF_ADVANCE_IP(task, 1);
}

/* IO-GET-REAL-TIME primitive */
void af_prim_io_get_real_time(af_task_t* task) {
  af_time_t real_time;
  AF_VERIFY_DATA_STACK_EXPAND(task, 2);
  af_io_get_real_time(&real_time);
  *(--task->data_stack_current) = real_time.sec;
  *(--task->data_stack_current) = real_time.nsec;
  AF_ADVANCE_IP(task, 1);
}

/* IO-SLEEP primitive */
void af_prim_io_sleep(af_task_t* task) {
  af_global_t* global = task->global;
  af_time_t sleep_until;
  af_io_action_t* action;
  AF_VERIFY_DATA_STACK_READ(task, 2);
  sleep_until.nsec = *task->data_stack_current++;
  sleep_until.sec = *task->data_stack_current++;
  action = af_io_sleep(&global->io, &sleep_until, task);
  af_io_action_destroy(action);
  AF_ADVANCE_IP(task, 1);
  af_wait(global, task);
}

/* IO-TELL primitive */
void af_prim_io_tell(af_task_t* task) {
  af_global_t* global = task->global;
  af_io_action_t* action;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  action = af_io_tell(&global->io, (af_io_fd_t)(*task->data_stack_current),
		      task);
  *task->data_stack_current = (af_cell_t)action;
  AF_ADVANCE_IP(task, 1);
  af_wait(global, task);
}

/* IO-CLOSE-BLOCK primitive */
void af_prim_io_close_block(af_task_t* task) {
  af_global_t* global = task->global;
  af_io_action_t* action;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  action = af_io_close_block(&global->io,
			     (af_io_fd_t)(*task->data_stack_current),
			     task);
  *task->data_stack_current = (af_cell_t)action;
  AF_ADVANCE_IP(task, 1);
  af_wait(global, task);
}

/* IO-CLOSE-ASYNC primitive */
void af_prim_io_close_async(af_task_t* task) {
  af_global_t* global = task->global;
  af_io_action_t* action;
  AF_VERIFY_DATA_STACK_READ(task, 1);
  action = af_io_close_async(&global->io,
			     (af_io_fd_t)(*task->data_stack_current));
  *task->data_stack_current = (af_cell_t)action;
  AF_ADVANCE_IP(task, 1);
}

/* IO-SEEK-BLOCK primitive */
void af_prim_io_seek_block(af_task_t* task) {
  af_global_t* global = task->global;
  af_io_action_t* action;
  af_io_fd_t fd;
  af_io_off_t offset;
  af_io_whence_t whence;
  AF_VERIFY_DATA_STACK_READ(task, 3);
  fd = *task->data_stack_current;
  whence = *(task->data_stack_current + 1);
  offset = *(af_sign_cell_t*)(task->data_stack_current + 2);
  action = af_io_seek_block(&global->io, fd, offset, whence, task);
  task->data_stack_current += 2;
  *task->data_stack_current = (af_cell_t)action;
  AF_ADVANCE_IP(task, 1);
  af_wait(global, task);
}

/* IO-SEEK-ASYNC primitive */
void af_prim_io_seek_async(af_task_t* task) {
  af_global_t* global = task->global;
  af_io_action_t* action;
  af_io_fd_t fd;
  af_io_off_t offset;
  af_io_whence_t whence;
  AF_VERIFY_DATA_STACK_READ(task, 3);
  fd = *task->data_stack_current;
  whence = *(task->data_stack_current + 1);
  offset = *(af_sign_cell_t*)(task->data_stack_current + 2);
  action = af_io_seek_async(&global->io, fd, offset, whence);
  task->data_stack_current += 2;
  *task->data_stack_current = (af_cell_t)action;
  AF_ADVANCE_IP(task, 1);
}

/* IO-READ-BLOCK primitive */
void af_prim_io_read_block(af_task_t* task) {
  af_global_t* global = task->global;
  af_io_action_t* action;
  AF_VERIFY_DATA_STACK_READ(task, 3);
  action = af_io_read_block(&global->io,
			    (af_io_fd_t)(*task->data_stack_current),
			    (af_byte_t*)(*(task->data_stack_current + 2)),
			    *(task->data_stack_current + 1),
			    task);
  task->data_stack_current += 2;
  *task->data_stack_current = (af_cell_t)action;
  AF_ADVANCE_IP(task, 1);
  af_wait(global, task);
}

/* IO-WRITE-BLOCK primitive */
void af_prim_io_write_block(af_task_t* task) {
  af_global_t* global = task->global;
  af_io_action_t* action;
  AF_VERIFY_DATA_STACK_READ(task, 3);
  action = af_io_write_block(&global->io,
			     (af_io_fd_t)(*task->data_stack_current),
			     (af_byte_t*)(*(task->data_stack_current + 2)),
			     *(task->data_stack_current + 1),
			     task);
  task->data_stack_current += 2;
  *task->data_stack_current = (af_cell_t)action;
  AF_ADVANCE_IP(task, 1);
  af_wait(global, task);
}

/* IO-READ-ASYNC primitive */
void af_prim_io_read_async(af_task_t* task) {
  af_global_t* global = task->global;
  af_io_action_t* action;
  AF_VERIFY_DATA_STACK_READ(task, 3);
  action = af_io_read_async(&global->io,
			    (af_io_fd_t)(*task->data_stack_current),
			    (af_byte_t*)(*(task->data_stack_current + 2)),
			    *(task->data_stack_current + 1));
  task->data_stack_current += 2;
  *task->data_stack_current = (af_cell_t)action;
  AF_ADVANCE_IP(task, 1);
}

/* IO-WRITE-ASYNC primitive */
void af_prim_io_write_async(af_task_t* task) {
  af_global_t* global = task->global;
  af_io_action_t* action;
  AF_VERIFY_DATA_STACK_READ(task, 3);
  action = af_io_write_async(&global->io,
			     (af_io_fd_t)(*task->data_stack_current),
			     (af_byte_t*)(*(task->data_stack_current + 2)),
			     *(task->data_stack_current + 1));
  task->data_stack_current += 2;
  *task->data_stack_current = (af_cell_t)action;
  AF_ADVANCE_IP(task, 1);
}

/* IO-READ-NONBLOCK primitive */
void af_prim_io_read_nonblock(af_task_t* task) {
  af_bool_t again;
  af_io_error_t error;
  ssize_t size;
  AF_VERIFY_DATA_STACK_READ(task, 3);
  size = af_io_read_nonblock((af_io_fd_t)(*task->data_stack_current),
			     (af_byte_t*)(*(task->data_stack_current + 2)),
			     *(task->data_stack_current + 1),
			     &again, &error);
  *(af_sign_cell_t*)(task->data_stack_current + 2) = (af_sign_cell_t)size;
  *(af_bool_t*)(task->data_stack_current + 1) = again;
  *task->data_stack_current = error;
  AF_ADVANCE_IP(task, 1);
}

/* IO-WRITE-NONBLOCK primitive */
void af_prim_io_write_nonblock(af_task_t* task) {
  af_bool_t again;
  af_io_error_t error;
  ssize_t size;
  AF_VERIFY_DATA_STACK_READ(task, 3);
  size = af_io_write_nonblock((af_io_fd_t)(*task->data_stack_current),
			      (af_byte_t*)(*(task->data_stack_current + 2)),
			      *(task->data_stack_current + 1),
			      &again, &error);
  *(af_sign_cell_t*)(task->data_stack_current + 2) = (af_sign_cell_t)size;
  *(af_bool_t*)(task->data_stack_current + 1) = again;
  *task->data_stack_current = error;
  AF_ADVANCE_IP(task, 1);
}

/* IO-STDIN primitive */
void af_prim_io_stdin(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_STDIN;
  AF_ADVANCE_IP(task, 1);
}

/* IO-STDOUT primitive */
void af_prim_io_stdout(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_STDOUT;
  AF_ADVANCE_IP(task, 1);
}

/* IO-STDERR primitive */
void af_prim_io_stderr(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_STDERR;
  AF_ADVANCE_IP(task, 1);
}

/* IO-RDONLY primitive */
void af_prim_io_rdonly(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_RDONLY;
  AF_ADVANCE_IP(task, 1);
}

/* IO-WRONLY primitive */
void af_prim_io_wronly(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_WRONLY;
  AF_ADVANCE_IP(task, 1);
}

/* IO-RDWR primitive */
void af_prim_io_rdwr(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_RDWR;
  AF_ADVANCE_IP(task, 1);
}

/* IO-APPEND primitive */
void af_prim_io_append(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_APPEND;
  AF_ADVANCE_IP(task, 1);
}

/* IO-TRUNC primitive */
void af_prim_io_trunc(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_TRUNC;
  AF_ADVANCE_IP(task, 1);
}

/* IO-CREAT primitive */
void af_prim_io_creat(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_CREAT;
  AF_ADVANCE_IP(task, 1);
}

/* IO-EXCL primitive */
void af_prim_io_excl(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_EXCL;
  AF_ADVANCE_IP(task, 1);
}

/* IO-IRWXU primitive */
void af_prim_io_irwxu(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_IRWXU;
  AF_ADVANCE_IP(task, 1);
}

/* IO-IRUSR primitive */
void af_prim_io_irusr(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_IRUSR;
  AF_ADVANCE_IP(task, 1);
}

/* IO-IWUSR primitive */
void af_prim_io_iwusr(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_IWUSR;
  AF_ADVANCE_IP(task, 1);
}

/* IO-IXUSR primitive */
void af_prim_io_ixusr(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_IXUSR;
  AF_ADVANCE_IP(task, 1);
}

/* IO-IRWXG primitive */
void af_prim_io_irwxg(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_IRWXG;
  AF_ADVANCE_IP(task, 1);
}

/* IO-IRGRP primitive */
void af_prim_io_irgrp(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_IRGRP;
  AF_ADVANCE_IP(task, 1);
}

/* IO-IWGRP primitive */
void af_prim_io_iwgrp(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_IWGRP;
  AF_ADVANCE_IP(task, 1);
}

/* IO-IXGRP primitive */
void af_prim_io_ixgrp(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_IXGRP;
  AF_ADVANCE_IP(task, 1);
}

/* IO-IRWXO primitive */
void af_prim_io_irwxo(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_IRWXO;
  AF_ADVANCE_IP(task, 1);
}

/* IO-IROTH primitive */
void af_prim_io_iroth(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_IROTH;
  AF_ADVANCE_IP(task, 1);
}

/* IO-IWOTH primitive */
void af_prim_io_iwoth(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_IWOTH;
  AF_ADVANCE_IP(task, 1);
}

/* IO-IXOTH primitive */
void af_prim_io_ixoth(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_IXOTH;
  AF_ADVANCE_IP(task, 1);
}

/* IO-ISUID primitive */
void af_prim_io_isuid(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_ISUID;
  AF_ADVANCE_IP(task, 1);
}

/* IO-ISGID primitive */
void af_prim_io_isgid(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_ISGID;
  AF_ADVANCE_IP(task, 1);
}

/* IO-ISVTX primitive */
void af_prim_io_isvtx(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_ISVTX;
  AF_ADVANCE_IP(task, 1);
}

/* IO-SEEK-SET primitive */
void af_prim_io_seek_set(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_SEEK_SET;
  AF_ADVANCE_IP(task, 1);
}

/* IO-SEEK-CUR primitive */
void af_prim_io_seek_cur(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_SEEK_CUR;
  AF_ADVANCE_IP(task, 1);
}

/* IO-SEEK-END primitive */
void af_prim_io_seek_end(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_SEEK_END;
  AF_ADVANCE_IP(task, 1);
}


/* INPUT-SIZE primitive */
void af_prim_input_size(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)sizeof(af_input_t);
  AF_ADVANCE_IP(task, 1);
}

/* INPUT-NEXT-INPUT primitive */
void af_prim_input_next_input(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  *task->data_stack_current =
    (af_cell_t)(&((af_input_t*)(*task->data_stack_current))->next_input);
  AF_ADVANCE_IP(task, 1);
}

/* INPUT-BUFFER primitive */
void af_prim_input_buffer(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  *task->data_stack_current =
    (af_cell_t)(&((af_input_t*)(*task->data_stack_current))->buffer);
  AF_ADVANCE_IP(task, 1);
}

/* INPUT-COUNT primitive */
void af_prim_input_count(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  *task->data_stack_current =
    (af_cell_t)(&((af_input_t*)(*task->data_stack_current))->count);
  AF_ADVANCE_IP(task, 1);
}

/* INPUT-INDEX primitive */
void af_prim_input_index(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  *task->data_stack_current =
    (af_cell_t)(&((af_input_t*)(*task->data_stack_current))->index);
  AF_ADVANCE_IP(task, 1);
}

/* INPUT-IS-CLOSED primitive */
void af_prim_input_is_closed(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  *task->data_stack_current =
    (af_cell_t)(&((af_input_t*)(*task->data_stack_current))->is_closed);
  AF_ADVANCE_IP(task, 1);
}

/* INPUT-CLEANUP primitive */
void af_prim_input_cleanup(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  *task->data_stack_current =
    (af_cell_t)(&((af_input_t*)(*task->data_stack_current))->cleanup);
  AF_ADVANCE_IP(task, 1);
}

/* INPUT-REFILL primitive */
void af_prim_input_refill(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  *task->data_stack_current =
    (af_cell_t)(&((af_input_t*)(*task->data_stack_current))->refill);
  AF_ADVANCE_IP(task, 1);
}

/* INPUT-SOURCE-ID primitive */
void af_prim_input_source_id(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  *task->data_stack_current =
    (af_cell_t)(&((af_input_t*)(*task->data_stack_current))->source_id);
  AF_ADVANCE_IP(task, 1);
}

/* INPUT-ARG primitive */
void af_prim_input_arg(af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(task, 1);
  *task->data_stack_current =
    (af_cell_t)(&((af_input_t*)(*task->data_stack_current))->arg);
  AF_ADVANCE_IP(task, 1);
}

/* DEBUGGER primitive */
void af_prim_debugger(af_task_t* task) {
  AF_ADVANCE_IP(task, 1);
}

/* PRIM-DOCOL primitive */
void af_prim_prim_docol(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)af_prim_docol;
  AF_ADVANCE_IP(task, 1);
}

/* PRIM-PUSH primitive */
void af_prim_prim_push(af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)af_prim_push_data;
  AF_ADVANCE_IP(task, 1);
}

/* PRIM-DOES primitive */
void af_prim_prim_does(af_task_t* task)  {
  AF_VERIFY_DATA_STACK_EXPAND(task, 1);
  *(--task->data_stack_current) = (af_cell_t)af_prim_do_does;
  AF_ADVANCE_IP(task, 1);
}

/* BASE-INTERPRETER primitive */
void af_prim_base_interpreter(af_task_t* task) {
  af_global_t* global = task->global;
  task->interpreter_pointer = &global->base_interpreter_code[0];
  if(af_parse_name_available(global, task)) {
    af_cell_t length;
    af_byte_t* text = af_parse_name(global, task, &length);
    /* char* print_text = malloc(length + 1); */
    /* memcpy(print_text, text, length); */
    /* print_text[length] = '\0'; */
    /* if(task->is_compiling) { */
    /*   printf("Compiling name: %s\n", print_text) */
    /* } else { */
    /*   printf("Evaluating name: %s\n", print_text); */
    /* } */
    af_word_t* word = af_lookup(global, task, text, length);
    if(word) {
      if(task->is_compiling && !(word->flags & AF_WORD_IMMEDIATE)) {
	af_compiled_t* slot = af_allot(global, task,
				       sizeof(af_compiled_t));
	if(slot) {
	  slot->compiled_call = word;
	}
      } else if(!task->is_compiling && (word->flags & AF_WORD_COMPILE_ONLY)) {
	char* buffer = malloc(length + 1);
	memcpy(buffer, text, length);
	buffer[length] = 0;
	printf("%s: ", buffer);
	free(buffer);
	af_handle_compile_only(global, task);
      } else {
	AF_WORD_EXECUTE(task, word);
      }
    } else {
      af_sign_cell_t result;
      if(af_parse_number(global, task->base, text, (size_t)length, &result)) {
	if(task->is_compiling) {
	  af_compiled_t* slot = af_allot(global, task,
					 sizeof(af_compiled_t) * 2);
	  if(slot) {
	    slot->compiled_call = global->builtin_literal_runtime;
	    (slot + 1)->compiled_sign_cell = result;
	  }
	} else {
	  if(--task->data_stack_current >= task->data_stack_top) {
	    *task->data_stack_current = (af_cell_t)result;
	  } else {
	    af_handle_data_stack_overflow(global, task);
	  }
	}
      } else {
	af_float_t float_result;
	if(af_parse_float(global, text, (size_t)length, &float_result)) {
	  if(task->is_compiling) {
	    af_compiled_t* slot = af_allot(global, task,
					   sizeof(af_compiled_t) +
					   sizeof(af_float_t));
	    if(slot) {
	      slot->compiled_call = global->builtin_f_literal_runtime;
	      *(af_float_t*)(&(slot + 1)->compiled_cell) = float_result;
	    }
	  } else {
	    if(--task->float_stack_current >= task->float_stack_top) {
	      *task->float_stack_current = float_result;
	    } else {
	      af_handle_float_stack_overflow(global, task);
	    }
	  }
	} else {
	  af_2cell_t result2;
	  if(af_parse_2number(global, task->base, text, (size_t)length,
			      &result2)) {
	    if(task->is_compiling) {
	      af_compiled_t* slot = af_allot(global, task,
					     sizeof(af_compiled_t) * 4);
	      if(slot) {
		slot->compiled_call = global->builtin_literal_runtime;
		(slot + 1)->compiled_cell =
		  (af_cell_t)(result2 &
			      ((~(af_2cell_t)0) >> (sizeof(af_cell_t) << 3)));
		(slot + 2)->compiled_call = global->builtin_literal_runtime;
		(slot + 3)->compiled_cell =
		  (af_cell_t)(result2 >> (sizeof(af_cell_t) << 3));
	      }
	    } else {
	      if((task->data_stack_current - 2) >= task->data_stack_top) {
		*(--task->data_stack_current) =
		  (af_cell_t)(result2 &
			      ((~(af_2cell_t)0) >> (sizeof(af_cell_t) << 3)));
		*(--task->data_stack_current) =
		  (af_cell_t)(result2 >> (sizeof(af_cell_t) << 3));
	      } else {
		af_handle_data_stack_overflow(global, task);
	      }
	    }
	  } else {
	    char* buffer = malloc(length + 1);
	    memcpy(buffer, text, length);
	    buffer[length] = 0;
	    printf("%s: ", buffer);
	    free(buffer);
	    af_handle_parse_error(global, task);
	  }
	}
      }
    }
  } else {
    af_kill(global, task);
  }
}
