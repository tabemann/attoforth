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

/* MOD primitive */
void af_prim_mod(af_global_t* global, af_thread_t* thread);

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

/* STATE primitive */
void af_prim_state(af_global_t* global, af_thread_t* thread);

/* LATESTXT primitive */
void af_prim_latestxt(af_global_t* global, af_thread_t* thread);

/* >IN primitive */
void af_prim_to_in(af_global_t* global, af_thread_t* thread);

/* SOURCE primitive */
void af_prim_source(af_global_t* global, af_thread_t* thread);

/* >INPUT primitive */
void af_prim_to_input(af_global_t* global, af_thread_t* thread);

/* INPUT> primitive */
void af_prim_from_input(af_global_t* global, af_thread_t* thread);

/* >OUTPUT primitive */
void af_prim_to_output(af_global_t* global, af_thread_t* thread);

/* OUTPUT> primitive */
void af_prim_from_output(af_global_t* global, af_thread_t* thread);

/* >ERROR primitive */
void af_prim_to_error(af_global_t* global, af_thread_t* thread);

/* ERROR> primitive */
void af_prim_from_error(af_global_t* global, af_thread_t* thread);

/* >DATA primitive */
void af_prim_to_data(af_global_t* global, af_thread_t* thread);

/* >RETURN primitive */
void af_prim_to_return(af_global_t* global, af_thread_t* thread);

/* >CLEANUP primitive */
void af_prim_to_cleanup(af_global_t* global, af_thread_t* thread);

/* CLEANUP> primitive */
void af_prim_from_cleanup(af_global_t* global, af_thread_t* thread);

/* >DEFAULT-CLEANUP primitive */
void af_prim_to_default_cleanup(af_global_t* global, af_thread_t* thread);

/* DEFAULT-CLEANUP> primitive */
void af_prim_from_default_cleanup(af_global_t* global, af_thread_t* thread);

/* >DROP-INPUT primitive */
void af_prim_to_drop_input(af_global_t* global, af_thread_t* thread);

/* DROP-INPUT> primitive */
void af_prim_from_drop_input(af_global_t* global, af_thread_t* thread);

/* >DEFAULT-DROP-INPUT primitive */
void af_prim_to_default_drop_input(af_global_t* global, af_thread_t* thread);

/* DEFAULT-DROP-INPUT> primitive */
void af_prim_from_default_drop_input(af_global_t* global, af_thread_t* thread);

/* >INTERACTIVE-ENDLINE primitive */
void af_prim_to_interactive_endline(af_global_t* global, af_thread_t* thread);

/* INTERACTIVE-ENDLINE> primitive */
void af_prim_from_interactive_endline(af_global_t* global, af_thread_t* thread);

/* >DEFAULT-INTERACTIVE-ENDLINE primitive */
void af_prim_to_default_interactive_endline(af_global_t* global, af_thread_t* thread);

/* DEFAULT-INTERACTIVE-ENDLINE> primitive */
void af_prim_from_default_interactive_endline(af_global_t* global, af_thread_t* thread);

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

/* SPAWN primitive */
void af_prim_spawn(af_global_t* global, af_thread_t* thread);

/* >CONSOLE-IN primitive */
void af_prim_to_console_input(af_global_t* global, af_thread_t* thread);

/* >CONSOLE-OUT primitive */
void af_prim_to_console_output(af_global_t* global, af_thread_t* thread);

/* >CONSOLE-ERR primitive */
void af_prim_to_console_error(af_global_t* global, af_thread_t* thread);

/* CONSOLE-INPUT> primitive */
void af_prim_from_console_input(af_global_t* global, af_thread_t* thread);

/* CONSOLE-OUT> primitive */
void af_prim_from_console_output(af_global_t* global, af_thread_t* thread);

/* CONSOLE-ERR> primitive */
void af_prim_from_console_error(af_global_t* global, af_thread_t* thread);

/* >INIT-WORD primitive */
void af_prim_to_init_word(af_global_t* global, af_thread_t* thread);

/* >INTERPRET primitive */
void af_prim_to_interpret(af_global_t* global, af_thread_t* thread);

/* START primitive */
void af_prim_start(af_global_t* global, af_thread_t* thread);

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

/* ?BRANCH primitive */
void af_prim_cond_branch(af_global_t* global, af_thread_t* thread);

/* MOVE primitive */
void af_prim_move(af_global_t* global, af_thread_t* thread);

/* DEPTH primitive */
void af_prim_depth(af_global_t* global, af_thread_t* thread);

/* CELL-SIZE primitive */
void af_prim_cell_size(af_global_t* global, af_thread_t* thread);

/* REFILL primitive */
void af_prim_refill(af_global_t* global, af_thread_t* thread);

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

/* IO-OPEN primitive */
void af_prim_io_open(af_global_t* global, af_thread_t* thread);

/* IO-PIPE primitive */
void af_prim_io_pipe(af_global_t* global, af_thread_t* thread);

/* IO-CLOSE-BLOCK primitive */
void af_prim_io_close_block(af_global_t* global, af_thread_t* thread);

/* IO-CLOSE-ASYNC primitive */
void af_prim_io_close_async(af_global_t* global, af_thread_t* thread);

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

/* IO-STDIN primitive */
void af_prim_io_stdin(af_global_t* global, af_thread_t* thread);

/* IO-STDOUT primitive */
void af_prim_io_stdout(af_global_t* global, af_thread_t* thread);

/* IO-STDERR primitive */
void af_prim_io_stderr(af_global_t* global, af_thread_t* thread);

/* IO-RDONLY primitive */
void af_prim_io_rdonly(af_global_t* global, af_thread_t* thread);

/* IO-WRONLY primitive */
void af_prim_io_wronly(af_global_t* global, af_thread_t* thread);

/* IO-RDWR primitive */
void af_prim_io_rdwr(af_global_t* global, af_thread_t* thread);

/* IO-APPEND primitive */
void af_prim_io_append(af_global_t* global, af_thread_t* thread);

/* IO-TRUNC primitive */
void af_prim_io_trunc(af_global_t* global, af_thread_t* thread);

/* IO-CREAT primitive */
void af_prim_io_creat(af_global_t* global, af_thread_t* thread);

/* IO-EXCL primitive */
void af_prim_io_excl(af_global_t* global, af_thread_t* thread);

/* IO-IRWXU primitive */
void af_prim_io_irwxu(af_global_t* global, af_thread_t* thread);

/* IO-IRUSR primitive */
void af_prim_io_irusr(af_global_t* global, af_thread_t* thread);

/* IO-IWUSR primitive */
void af_prim_io_iwusr(af_global_t* global, af_thread_t* thread);

/* IO-IXUSR primitive */
void af_prim_io_ixusr(af_global_t* global, af_thread_t* thread);

/* IO-IRWXG primitive */
void af_prim_io_irwxg(af_global_t* global, af_thread_t* thread);

/* IO-IRGRP primitive */
void af_prim_io_irgrp(af_global_t* global, af_thread_t* thread);

/* IO-IWGRP primitive */
void af_prim_io_iwgrp(af_global_t* global, af_thread_t* thread);

/* IO-IXGRP primitive */
void af_prim_io_ixgrp(af_global_t* global, af_thread_t* thread);

/* IO-IRWXO primitive */
void af_prim_io_irwxo(af_global_t* global, af_thread_t* thread);

/* IO-IROTH primitive */
void af_prim_io_iroth(af_global_t* global, af_thread_t* thread);

/* IO-IWOTH primitive */
void af_prim_io_iwoth(af_global_t* global, af_thread_t* thread);

/* IO-IXOTH primitive */
void af_prim_io_ixoth(af_global_t* global, af_thread_t* thread);

/* IO-ISUID primitive */
void af_prim_io_isuid(af_global_t* global, af_thread_t* thread);

/* IO-ISGID primitive */
void af_prim_io_isgid(af_global_t* global, af_thread_t* thread);

/* IO-ISVTX primitive */
void af_prim_io_isvtx(af_global_t* global, af_thread_t* thread);

/* INPUT-SIZE primitive */
void af_prim_input_size(af_global_t* global, af_thread_t* thread);

/* INPUT-NEXT-INPUT primitive */
void af_prim_input_next_input(af_global_t* global, af_thread_t* thread);

/* INPUT-BUFFER primitive */
void af_prim_input_buffer(af_global_t* global, af_thread_t* thread);

/* INPUT-COUNT primitive */
void af_prim_input_count(af_global_t* global, af_thread_t* thread);

/* INPUT-INDEX primitive */
void af_prim_input_index(af_global_t* global, af_thread_t* thread);

/* INPUT-IS-CLOSED primitive */
void af_prim_input_is_closed(af_global_t* global, af_thread_t* thread);

/* INPUT-CLEANUP primitive */
void af_prim_input_cleanup(af_global_t* global, af_thread_t* thread);

/* INPUT-REFILL primitive */
void af_prim_input_refill(af_global_t* global, af_thread_t* thread);

/* INPUT-ARG primitive */
void af_prim_input_arg(af_global_t* global, af_thread_t* thread);

/* OUTPUT-SIZE primitive */
void af_prim_output_size(af_global_t* global, af_thread_t* thread);

/* OUTPUT-NEXT-OUTPUT primitive */
void af_prim_output_next_output(af_global_t* global, af_thread_t* thread);

/* OUTPUT-WRITE primitive */
void af_prim_output_write(af_global_t* global, af_thread_t* thread);

/* OUTPUT-CLEANUP primitive */
void af_prim_output_cleanup(af_global_t* global, af_thread_t* thread);

/* OUTPUT-ARG primitive */
void af_prim_output_arg(af_global_t* global, af_thread_t* thread);

/* Function definitions */

/* Register primitives */
void af_register_prims(af_global_t* global, af_thread_t* thread) {
  global->builtin_literal_runtime =
    af_register_prim(global, thread, "(LITERAL)", af_prim_literal_runtime,
		     FALSE);
  global->builtin_exit =
    af_register_prim(global, thread, "EXIT", af_prim_exit, FALSE);
  global->builtin_postpone_runtime =
    af_register_prim(global, thread, "(POSTPONE)", af_prim_postpone_runtime,
		     FALSE);
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
  af_register_prim(global, thread, "MOD", af_prim_mod, FALSE);
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
  af_register_prim(global, thread, "STATE", af_prim_state, FALSE);
  af_register_prim(global, thread, "LATESTXT", af_prim_latestxt, FALSE);
  af_register_prim(global, thread, ">IN", af_prim_to_in, FALSE);
  af_register_prim(global, thread, "SOURCE", af_prim_source, FALSE);
  af_register_prim(global, thread, ">INPUT", af_prim_to_input, FALSE);
  af_register_prim(global, thread, "INPUT>", af_prim_from_input, FALSE);
  af_register_prim(global, thread, ">OUTPUT", af_prim_to_output, FALSE);
  af_register_prim(global, thread, "OUTPUT>", af_prim_from_output, FALSE);
  af_register_prim(global, thread, ">ERROR", af_prim_to_error, FALSE);
  af_register_prim(global, thread, "ERROR>", af_prim_from_error, FALSE);
  af_register_prim(global, thread, ">DATA", af_prim_to_data, FALSE);
  af_register_prim(global, thread, ">RETURN", af_prim_to_return, FALSE);
  af_register_prim(global, thread, ">CLEANUP", af_prim_to_cleanup, FALSE);
  af_register_prim(global, thread, "CLEANUP>", af_prim_from_cleanup, FALSE);
  af_register_prim(global, thread, ">DEFAULT-CLEANUP",
		   af_prim_to_default_cleanup, FALSE);
  af_register_prim(global, thread, "DEFAULT-CLEANUP>",
		   af_prim_from_default_cleanup, FALSE);
  af_register_prim(global, thread, ">DROP-INPUT", af_prim_to_drop_input, FALSE);
  af_register_prim(global, thread, "DROP-INPUT>", af_prim_from_drop_input,
		   FALSE);
  af_register_prim(global, thread, ">DEFAULT-DROP-INPUT",
		   af_prim_to_default_drop_input, FALSE);
  af_register_prim(global, thread, "DEFAULT-DROP-INPUT>",
		   af_prim_from_default_drop_input, FALSE);
  af_register_prim(global, thread, ">INTERACTIVE-ENDLINE", af_prim_to_interactive_endline, FALSE);
  af_register_prim(global, thread, "INTERACTIVE-ENDLINE>", af_prim_from_interactive_endline,
		   FALSE);
  af_register_prim(global, thread, ">DEFAULT-INTERACTIVE-ENDLINE",
		   af_prim_to_default_interactive_endline, FALSE);
  af_register_prim(global, thread, "DEFAULT-INTERACTIVE-ENDLINE>",
		   af_prim_from_default_interactive_endline, FALSE);
  af_register_prim(global, thread, "ALLOCATE", af_prim_allocate, FALSE);
  global->builtin_free =
    af_register_prim(global, thread, "FREE", af_prim_free, FALSE);
  af_register_prim(global, thread, "RESIZE", af_prim_resize, FALSE);
  af_register_prim(global, thread, "ALIGN", af_prim_align, FALSE);
  af_register_prim(global, thread, "ALLOT", af_prim_allot, FALSE);
  af_register_prim(global, thread, "WORD", af_prim_word, FALSE);
  af_register_prim(global, thread, "PARSE-NAME", af_prim_parse_name, FALSE);
  af_register_prim(global, thread, "THIS-THREAD", af_prim_this_thread, FALSE);
  af_register_prim(global, thread, "SPAWN", af_prim_spawn, FALSE);
  af_register_prim(global, thread, ">CONSOLE-INPUT", af_prim_to_console_input,
		   FALSE);
  af_register_prim(global, thread, ">CONSOLE-OUTPUT", af_prim_to_console_output,
		   FALSE);
  af_register_prim(global, thread, ">CONSOLE-ERROR", af_prim_to_console_error,
		   FALSE);
  af_register_prim(global, thread, "CONSOLE-INPUT>",
		   af_prim_from_console_input, FALSE);
  af_register_prim(global, thread, "CONSOLE-OUTPUT>",
		   af_prim_from_console_output, FALSE);
  af_register_prim(global, thread, "CONSOLE-ERROR>",
		   af_prim_from_console_error, FALSE);
  af_register_prim(global, thread, ">INIT-WORD", af_prim_to_init_word, FALSE);
  af_register_prim(global, thread, ">INTERPRET", af_prim_to_interpret, FALSE);
  af_register_prim(global, thread, "START", af_prim_start, FALSE);
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
  af_register_prim(global, thread, "?BRANCH", af_prim_cond_branch, FALSE);
  af_register_prim(global, thread, "MOVE", af_prim_move, FALSE);
  af_register_prim(global, thread, "DEPTH", af_prim_depth, FALSE);
  af_register_prim(global, thread, "CELL-SIZE", af_prim_cell_size, FALSE);
  af_register_prim(global, thread, "REFILL", af_prim_refill, FALSE);
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
  af_register_prim(global, thread, "IO-OPEN", af_prim_io_open, FALSE);
  af_register_prim(global, thread, "IO-PIPE", af_prim_io_pipe, FALSE);
  af_register_prim(global, thread, "IO-CLOSE-BLOCK", af_prim_io_close_block,
		   FALSE);
  af_register_prim(global, thread, "IO-CLOSE-ASYNC", af_prim_io_close_async,
		   FALSE);
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
  af_register_prim(global, thread, "IO-STDIN", af_prim_io_stdin, FALSE);
  af_register_prim(global, thread, "IO-STDOUT", af_prim_io_stdout, FALSE);
  af_register_prim(global, thread, "IO-STDERR", af_prim_io_stderr, FALSE);
  af_register_prim(global, thread, "IO-RDONLY", af_prim_io_rdonly, FALSE);
  af_register_prim(global, thread, "IO-WRONLY", af_prim_io_wronly, FALSE);
  af_register_prim(global, thread, "IO-RDWR", af_prim_io_rdwr, FALSE);
  af_register_prim(global, thread, "IO-APPEND", af_prim_io_append, FALSE);
  af_register_prim(global, thread, "IO-TRUNC", af_prim_io_trunc, FALSE);
  af_register_prim(global, thread, "IO-CREAT", af_prim_io_creat, FALSE);
  af_register_prim(global, thread, "IO-EXCL", af_prim_io_excl, FALSE);
  af_register_prim(global, thread, "IO-IRWXU", af_prim_io_irwxu, FALSE);
  af_register_prim(global, thread, "IO-IRUSR", af_prim_io_irusr, FALSE);
  af_register_prim(global, thread, "IO-IWUSR", af_prim_io_iwusr, FALSE);
  af_register_prim(global, thread, "IO-IXUSR", af_prim_io_ixusr, FALSE);
  af_register_prim(global, thread, "IO-IRWXG", af_prim_io_irwxg, FALSE);
  af_register_prim(global, thread, "IO-IRGRP", af_prim_io_irgrp, FALSE);
  af_register_prim(global, thread, "IO-IWGRP", af_prim_io_iwgrp, FALSE);
  af_register_prim(global, thread, "IO-IXGRP", af_prim_io_ixgrp, FALSE);
  af_register_prim(global, thread, "IO-IRWXO", af_prim_io_irwxo, FALSE);
  af_register_prim(global, thread, "IO-IROTH", af_prim_io_iroth, FALSE);
  af_register_prim(global, thread, "IO-IWOTH", af_prim_io_iwoth, FALSE);
  af_register_prim(global, thread, "IO-IXOTH", af_prim_io_ixoth, FALSE);
  af_register_prim(global, thread, "IO-ISUID", af_prim_io_isuid, FALSE);
  af_register_prim(global, thread, "IO-ISGID", af_prim_io_isgid, FALSE);
  af_register_prim(global, thread, "IO-ISVTX", af_prim_io_isvtx, FALSE);
  af_register_prim(global, thread, "INPUT-SIZE", af_prim_input_size, FALSE);
  af_register_prim(global, thread, "INPUT-NEXT-INPUT", af_prim_input_next_input,
		   FALSE);
  af_register_prim(global, thread, "INPUT-BUFFER", af_prim_input_buffer, FALSE);
  af_register_prim(global, thread, "INPUT-COUNT", af_prim_input_count, FALSE);
  af_register_prim(global, thread, "INPUT-INDEX", af_prim_input_index, FALSE);
  af_register_prim(global, thread, "INPUT-IS-CLOSED", af_prim_input_is_closed,
		   FALSE);
  af_register_prim(global, thread, "INPUT-CLEANUP", af_prim_input_cleanup,
		   FALSE);
  af_register_prim(global, thread, "INPUT-REFILL", af_prim_input_refill, FALSE);
  af_register_prim(global, thread, "INPUT-ARG", af_prim_input_arg, FALSE);
  af_register_prim(global, thread, "OUTPUT-SIZE", af_prim_output_size, FALSE);
  af_register_prim(global, thread, "OUTPUT-NEXT-OUTPUT",
		   af_prim_output_next_output, FALSE);
  af_register_prim(global, thread, "OUTPUT-WRITE", af_prim_output_write, FALSE);
  af_register_prim(global, thread, "OUTPUT-CLEANUP", af_prim_output_cleanup,
		   FALSE);
  af_register_prim(global, thread, "OUTPUT-ARG", af_prim_output_arg, FALSE);
}

/* Docol primitive */
void af_prim_docol(af_global_t* global, af_thread_t* thread) {
  if(thread->return_stack_current > thread->return_stack_top) {
    *(--thread->return_stack_current) =
      thread->interpreter_pointer ? thread->interpreter_pointer + 1 : NULL;
    thread->interpreter_pointer = thread->current_word->secondary;
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
      (af_cell_t)thread->interpreter_pointer->compiled_call->data;
    AF_ADVANCE_IP(thread, 1);
  } else {
    af_handle_data_stack_overflow(global, thread);
  }
}

/* Runtime of DOES> primitve */
void af_prim_do_does(af_global_t* global, af_thread_t* thread) {
  if(thread->data_stack_current > thread->data_stack_top) {
    if(thread->return_stack_current > thread->return_stack_top) {
      *(--thread->data_stack_current) = (af_cell_t)thread->current_word->data;
      *(--thread->return_stack_current) =
	thread->interpreter_pointer ? thread->interpreter_pointer + 1 : NULL;
      thread->interpreter_pointer = thread->current_word->secondary;
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
      (thread->interpreter_pointer + 1)->compiled_cell;
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
  slot = af_allot(global, thread, sizeof(af_compiled_t) * 2);
  if(slot) {
    slot->compiled_call = global->builtin_literal_runtime;
    (slot + 1)->compiled_cell = *thread->data_stack_current++;
    AF_ADVANCE_IP(thread, 1);
  }
}

/* CREATE primitive */
void af_prim_create(af_global_t* global, af_thread_t* thread) {
  af_byte_t* name;
  af_cell_t name_length;
  size_t name_size;
  void* word_space;
  af_word_t* word;
  af_compiled_t* secondary;
  if(!af_parse_name_available(global, thread)) {
    af_handle_word_expected(global, thread);
    return;
  }
  name = af_parse_name(global, thread, &name_length);
  name_length = name_length < 256 ? name_length : 255;
  name_size = (name_length + 1) * sizeof(af_byte_t);
  word_space = af_allocate(global, thread, sizeof(af_word_t) + name_size);
  word = word_space;
  AF_WORD_NAME_LEN(word) = (af_byte_t)name_length;
  memmove(AF_WORD_NAME_DATA(word), name, name_length);
  word->is_immediate = FALSE;
  word->code = af_prim_push_data;
  word->data = word_space + name_size;
  word->secondary = NULL;
  thread->most_recent_word = word;
  word->next_word = global->first_word;
  global->first_word = word;
  AF_ADVANCE_IP(thread, 1);
}

/* : primitive */
void af_prim_colon(af_global_t* global, af_thread_t* thread) {
  af_byte_t* name;
  af_cell_t name_length;
  size_t name_size;
  void* word_space;
  af_word_t* word;
  if(!af_parse_name_available(global, thread)) {
    af_handle_word_expected(global, thread);
    return;
  }
  name = af_parse_name(global, thread, &name_length);
  name_length = name_length < 256 ? name_length : 255;
  name_size = (name_length + 1) * sizeof(af_byte_t);
  word_space = af_allocate(global, thread, sizeof(af_word_t) + name_size);
  word = word_space;
  AF_WORD_NAME_LEN(word) = (af_byte_t)name_length;
  memmove(AF_WORD_NAME_DATA(word), name, name_length);

  /* TESTING */
  char* buffer = malloc(name_length + 1);
  memcpy(buffer, name, name_length);
  buffer[name_length] = '\0';
  printf("STARTING COMPILING: %s\n", buffer);
  free(buffer);


  word->next_word = global->first_word;
  word->is_immediate = FALSE;
  word->code = af_prim_docol;
  word->data = NULL;
  word->secondary = word_space + sizeof(af_word_t) + name_size;
  thread->most_recent_word = word;
  global->first_word = word;
  thread->is_compiling = TRUE;
  AF_ADVANCE_IP(thread, 1);
}

/* :NONAME primitive - immediate */
void af_prim_colon_noname(af_global_t* global, af_thread_t* thread) {
  void* word_space;
  af_word_t* word;
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  if(!(word_space = af_allocate(global, thread,
				sizeof(af_word_t) + sizeof(af_byte_t)))) {
    return;
  }
  word = word_space;
  word->next_word = NULL;
  word->is_immediate = FALSE;
  word->code = af_prim_docol;
  word->data = NULL;
  word->secondary = word_space + sizeof(af_word_t) + sizeof(af_byte_t);
  AF_WORD_NAME_LEN(word) = 0;
  thread->most_recent_word = word;
  thread->is_compiling = TRUE;
  *(--thread->data_stack_current) = (af_cell_t)word;
  AF_ADVANCE_IP(thread, 1);
}

/* ; primitive - immediate */
void af_prim_semi(af_global_t* global, af_thread_t* thread) {
  void* space;
  AF_VERIFY_COMPILING(global, thread);
  if(!(space = af_allot(global, thread, sizeof(af_compiled_t)))) {
    return;
  }
  ((af_compiled_t*)space)->compiled_call = global->builtin_exit;
  thread->is_compiling = FALSE;
  AF_ADVANCE_IP(thread, 1);
}

/* IMMEDIATE primitive */
void af_prim_immediate(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_WORD_CREATED(global, thread);
  thread->most_recent_word->is_immediate = TRUE;
  AF_ADVANCE_IP(thread, 1);
}

/* DUP primitive */
void af_prim_dup(af_global_t* global, af_thread_t* thread) {
  af_cell_t value;
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
  af_cell_t value;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  value = *thread->data_stack_current;
  *thread->data_stack_current = *(thread->data_stack_current + 1);
  *(thread->data_stack_current + 1) = value;
  AF_ADVANCE_IP(thread, 1);
}

/* OVER primitive */
void af_prim_over(af_global_t* global, af_thread_t* thread) {
  af_cell_t value;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  value = *(thread->data_stack_current + 1);
  *(--thread->data_stack_current) = value;
  AF_ADVANCE_IP(thread, 1);
}

/* ROT primitive */
void af_prim_rot(af_global_t* global, af_thread_t* thread) {
  af_cell_t value;
  AF_VERIFY_DATA_STACK_READ(global, thread, 3);
  value = *(thread->data_stack_current + 2);
  *(thread->data_stack_current + 2) = *(thread->data_stack_current + 1);
  *(thread->data_stack_current + 1) = *thread->data_stack_current;
  *thread->data_stack_current = value;
  AF_ADVANCE_IP(thread, 1);
}

/* PICK primitive */
void af_prim_pick(af_global_t* global, af_thread_t* thread) {
  af_cell_t index;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  index = *thread->data_stack_current;
  AF_VERIFY_DATA_STACK_READ(global, thread, index + 2);
  *thread->data_stack_current = *(thread->data_stack_current + index + 1);
  AF_ADVANCE_IP(thread, 1);
}

/* ROLL primitive */
void af_prim_roll(af_global_t* global, af_thread_t* thread) {
  af_cell_t index;
  af_cell_t value;
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
  af_cell_t value0;
  af_cell_t value1;
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
  af_cell_t value0;
  af_cell_t value1;
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
  af_cell_t value0;
  af_cell_t value1;
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
  af_cell_t value0;
  af_cell_t value1;
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
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  value0 = *((af_sign_cell_t*)thread->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)thread->data_stack_current;
  *(++thread->data_stack_current) = (af_cell_t)(value0 + value1);
  AF_ADVANCE_IP(thread, 1);
}

/* - primitive */
void af_prim_sub(af_global_t* global, af_thread_t* thread) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  value0 = *((af_sign_cell_t*)thread->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)thread->data_stack_current;
  *(++thread->data_stack_current) = (af_cell_t)(value0 - value1);
  AF_ADVANCE_IP(thread, 1);
}

/* * primitive */
void af_prim_mul(af_global_t* global, af_thread_t* thread) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  value0 = *((af_sign_cell_t*)thread->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)thread->data_stack_current;
  *(++thread->data_stack_current) = (af_cell_t)(value0 * value1);
  AF_ADVANCE_IP(thread, 1);
}

/* / primitive */
void af_prim_div(af_global_t* global, af_thread_t* thread) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  value0 = *((af_sign_cell_t*)thread->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)thread->data_stack_current;
  if(value1 == 0) {
    af_handle_divide_by_zero(global, thread);
    return;
  }
  *(++thread->data_stack_current) = (af_cell_t)(value0 / value1);
  AF_ADVANCE_IP(thread, 1);
}

/* MOD primitive */
void af_prim_mod(af_global_t* global, af_thread_t* thread) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  value0 = *((af_sign_cell_t*)thread->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)thread->data_stack_current;
  if(value1 == 0) {
    af_handle_divide_by_zero(global, thread);
    return;
  }
  *(++thread->data_stack_current) = (af_cell_t)(value0 % value1);
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
  af_cell_t value0;
  af_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  value0 = *(thread->data_stack_current + 1);
  value1 = *thread->data_stack_current;
  *(++thread->data_stack_current) = value0 & value1;
  AF_ADVANCE_IP(thread, 1);
}

/* OR primitive */
void af_prim_or(af_global_t* global, af_thread_t* thread) {
  af_cell_t value0;
  af_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  value0 = *(thread->data_stack_current + 1);
  value1 = *thread->data_stack_current;
  *(++thread->data_stack_current) = value0 | value1;
  AF_ADVANCE_IP(thread, 1);
}

/* XOR primitive */
void af_prim_xor(af_global_t* global, af_thread_t* thread) {
  af_cell_t value0;
  af_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  value0 = *(thread->data_stack_current + 1);
  value1 = *thread->data_stack_current;
  *(++thread->data_stack_current) = value0 ^ value1;
  AF_ADVANCE_IP(thread, 1);
}

/* < primitive */
void af_prim_lt(af_global_t* global, af_thread_t* thread) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  value0 = *((af_sign_cell_t*)thread->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)thread->data_stack_current;
  *(++thread->data_stack_current) = (af_cell_t)(value0 < value1 ? -1 : 0);
  AF_ADVANCE_IP(thread, 1);
}

/* <= primitive */
void af_prim_lte(af_global_t* global, af_thread_t* thread) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  value0 = *((af_sign_cell_t*)thread->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)thread->data_stack_current;
  *(++thread->data_stack_current) = (af_cell_t)(value0 <= value1 ? -1 : 0);
  AF_ADVANCE_IP(thread, 1);
}

/* > primitive */
void af_prim_gt(af_global_t* global, af_thread_t* thread) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  value0 = *((af_sign_cell_t*)thread->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)thread->data_stack_current;
  *(++thread->data_stack_current) = (af_cell_t)(value0 > value1 ? -1 : 0);
  AF_ADVANCE_IP(thread, 1);
}

/* >= primitive */
void af_prim_gte(af_global_t* global, af_thread_t* thread) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  value0 = *((af_sign_cell_t*)thread->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)thread->data_stack_current;
  *(++thread->data_stack_current) = (af_cell_t)(value0 >= value1 ? -1 : 0);
  AF_ADVANCE_IP(thread, 1);
}

/* = primitive */
void af_prim_eq(af_global_t* global, af_thread_t* thread) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  value0 = *((af_sign_cell_t*)thread->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)thread->data_stack_current;
  *(++thread->data_stack_current) = (af_cell_t)(value0 == value1 ? -1 : 0);
  AF_ADVANCE_IP(thread, 1);
}

/* <> primitive */
void af_prim_ne(af_global_t* global, af_thread_t* thread) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  value0 = *((af_sign_cell_t*)thread->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)thread->data_stack_current;
  *(++thread->data_stack_current) = (af_cell_t)(value0 != value1 ? -1 : 0);
  AF_ADVANCE_IP(thread, 1);
}

/* @ primitive */
void af_prim_fetch(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  *thread->data_stack_current = *(af_cell_t*)(*thread->data_stack_current);
  AF_ADVANCE_IP(thread, 1);
}

/* ! primitive */
void af_prim_store(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  *(af_cell_t*)(*thread->data_stack_current) = *(thread->data_stack_current + 1);
  thread->data_stack_current += 2;
  AF_ADVANCE_IP(thread, 1);
}

/* +! primitive */
void af_prim_add_store(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  *(af_cell_t*)(*thread->data_stack_current) +=
    *(thread->data_stack_current + 1);
  thread->data_stack_current += 2;
  AF_ADVANCE_IP(thread, 1);
}

/* C@ primitive */
void af_prim_c_fetch(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  *thread->data_stack_current = *(af_byte_t*)(*thread->data_stack_current);
  AF_ADVANCE_IP(thread, 1);
}

/* C! primitive */
void af_prim_c_store(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  *(af_byte_t*)(*thread->data_stack_current) =
    *(thread->data_stack_current + 1) & 0xFF;
  thread->data_stack_current += 2;
  AF_ADVANCE_IP(thread, 1);
}

/* 2@ primitive */
void af_prim_2_fetch(af_global_t* global, af_thread_t* thread) {
  af_cell_t* addr;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  addr = (af_cell_t*)(*thread->data_stack_current);
  *thread->data_stack_current = *(addr + 1);
  *(--thread->data_stack_current) = *addr;
  AF_ADVANCE_IP(thread, 1);
}

/* 2! primitive */
void af_prim_2_store(af_global_t* global, af_thread_t* thread) {
  af_cell_t* addr;
  AF_VERIFY_DATA_STACK_READ(global, thread, 3);
  addr = (af_cell_t*)(*thread->data_stack_current);
  *addr = *(thread->data_stack_current + 1);
  *(addr + 1) = *(thread->data_stack_current + 2);
  thread->data_stack_current += 3;
  AF_ADVANCE_IP(thread, 1);
}

/* , primitive */
void af_prim_comma(af_global_t* global, af_thread_t* thread) {
  af_cell_t* data;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  if((data = (af_cell_t*)af_allot(global, thread, sizeof(af_cell_t)))) {
    *data = *thread->data_stack_current++;
  }
  AF_ADVANCE_IP(thread, 1);
}

/* C, primitive */
void af_prim_c_comma(af_global_t* global, af_thread_t* thread) {
  af_byte_t* data;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  if((data = (af_byte_t*)af_allot(global, thread, sizeof(af_byte_t)))) {
    *data = (af_byte_t)(*thread->data_stack_current++ & 0xFF);
  }
  AF_ADVANCE_IP(thread, 1);
}

/* 2, primitive */
void af_prim_2_comma(af_global_t* global, af_thread_t* thread) {
  af_cell_t* data;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  if((data = (af_cell_t*)af_allot(global, thread, sizeof(af_cell_t) * 2))) {
    *data = *thread->data_stack_current++;
    *(data + 1) = *thread->data_stack_current++;
  }
  AF_ADVANCE_IP(thread, 1);
}

/* >R primitive */
void af_prim_to_r(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  AF_VERIFY_RETURN_STACK_EXPAND(global, thread, 1);
  *(af_cell_t*)(--thread->return_stack_current) = *thread->data_stack_current++;
  AF_ADVANCE_IP(thread, 1);
}

/* R> primitive */
void af_prim_from_r(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  AF_VERIFY_RETURN_STACK_READ(global, thread, 1);
  *(--thread->data_stack_current) =
    *(af_cell_t*)(thread->return_stack_current++);
  AF_ADVANCE_IP(thread, 1);
}

/* R@ primitive */
void af_prim_r_fetch(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  AF_VERIFY_RETURN_STACK_READ(global, thread, 1);
  *(--thread->data_stack_current) =
    *(af_cell_t*)(thread->return_stack_current);
  AF_ADVANCE_IP(thread, 1);
}

/* 2>R primitive */
void af_prim_two_to_r(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  AF_VERIFY_RETURN_STACK_EXPAND(global, thread, 2);
  *(af_cell_t*)(--thread->return_stack_current) =
    *(thread->data_stack_current + 1);
  *(af_cell_t*)(--thread->return_stack_current) = *thread->data_stack_current;
  thread->data_stack_current += 2;
  AF_ADVANCE_IP(thread, 1);
}

/* 2R> primitve */
void af_prim_two_from_r(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 2);
  AF_VERIFY_RETURN_STACK_READ(global, thread, 2);
  *(--thread->data_stack_current) =
    *(af_cell_t*)(thread->return_stack_current + 1);
  *(--thread->data_stack_current) =
    *(af_cell_t*)(thread->return_stack_current);
  thread->return_stack_current += 2;
  AF_ADVANCE_IP(thread, 1);
}

/* 2R@ primitive */
void af_prim_two_r_fetch(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 2);
  AF_VERIFY_RETURN_STACK_READ(global, thread, 2);
  *(--thread->data_stack_current) =
    *(af_cell_t*)(thread->return_stack_current + 1);
  *(--thread->data_stack_current) =
    *(af_cell_t*)(thread->return_stack_current);
  AF_ADVANCE_IP(thread, 1);
}

/* HERE primitive */
void af_prim_here(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)thread->data_space_current;
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
    (af_cell_t)(((af_word_t*)(*thread->data_stack_current))->data);
  AF_ADVANCE_IP(thread, 1);
}

/* ' primitve */
void af_prim_tick(af_global_t* global, af_thread_t* thread) {
  af_byte_t* name;
  af_cell_t name_length;
  af_word_t* word;
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  if(!af_parse_name_available(global, thread)) {
    af_handle_word_expected(global, thread);
    return;
  }
  name = af_parse_name(global, thread, &name_length);
  if(!(word = af_lookup(global, name, name_length))) {
    af_handle_word_not_found(global, thread);
    return;
  }
  *(--thread->data_stack_current) = (af_cell_t)word;
  AF_ADVANCE_IP(thread, 1);
}

/* ['] primitive - immediate */
void af_prim_bracket_tick(af_global_t* global, af_thread_t* thread) {
  af_byte_t* name;
  af_cell_t name_length;
  af_word_t* word;
  af_compiled_t* code;
  AF_VERIFY_COMPILING(global, thread);
  AF_VERIFY_WORD_CREATED(global, thread);
  if(!af_parse_name_available(global, thread)) {
    af_handle_word_expected(global, thread);
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
  (code + 1)->compiled_cell = (af_cell_t)word;
  AF_ADVANCE_IP(thread, 1);
}

/* EXECUTE primitive */
void af_prim_execute(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  AF_WORD_EXECUTE(global, thread, (af_word_t*)(*thread->data_stack_current++));
}

/* STATE primitive */
void af_prim_state(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)(&thread->is_compiling);
  AF_ADVANCE_IP(thread, 1);
}

/* LATESTXT primitive */
void af_prim_latestxt(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)thread->most_recent_word;
  AF_ADVANCE_IP(thread, 1);
}

/* >IN primitive */
void af_prim_to_in(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)(&thread->current_input->index);
  AF_ADVANCE_IP(thread, 1);
}

/* SOURCE primitive */
void af_prim_source(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 2);
  *(--thread->data_stack_current) = (af_cell_t)thread->current_input->buffer;
  *(--thread->data_stack_current) = thread->current_input->count;
  AF_ADVANCE_IP(thread, 1);
}

/* >INPUT primitive */
void af_prim_to_input(af_global_t* global, af_thread_t* thread) {
  af_thread_t* target_thread;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  target_thread = (af_thread_t*)(*thread->data_stack_current++);
  target_thread->current_input = (af_input_t*)(*thread->data_stack_current++);
  AF_ADVANCE_IP(thread, 1);
}

/* INPUT> primitive */
void af_prim_from_input(af_global_t* global, af_thread_t* thread) {
  af_thread_t* target_thread;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  target_thread = (af_thread_t*)(*thread->data_stack_current);
  *thread->data_stack_current = (af_cell_t)target_thread->current_input;
  AF_ADVANCE_IP(thread, 1);
}

/* >OUTPUT primitive */
void af_prim_to_output(af_global_t* global, af_thread_t* thread) {
  af_thread_t* target_thread;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  target_thread = (af_thread_t*)(*thread->data_stack_current++);
  target_thread->current_output = (af_output_t*)(*thread->data_stack_current++);
  AF_ADVANCE_IP(thread, 1);
}

/* OUTPUT> primitive */
void af_prim_from_output(af_global_t* global, af_thread_t* thread) {
  af_thread_t* target_thread;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  target_thread = (af_thread_t*)(*thread->data_stack_current);
  *thread->data_stack_current = (af_cell_t)target_thread->current_output;
  AF_ADVANCE_IP(thread, 1);
}

/* >ERROR primitive */
void af_prim_to_error(af_global_t* global, af_thread_t* thread) {
  af_thread_t* target_thread;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  target_thread = (af_thread_t*)(*thread->data_stack_current++);
  target_thread->current_error = (af_output_t*)(*thread->data_stack_current++);
  AF_ADVANCE_IP(thread, 1);
}

/* ERROR> primitive */
void af_prim_from_error(af_global_t* global, af_thread_t* thread) {
  af_thread_t* target_thread;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  target_thread = (af_thread_t*)(*thread->data_stack_current);
  *thread->data_stack_current = (af_cell_t)target_thread->current_error;
  AF_ADVANCE_IP(thread, 1);
}

/* >DATA primitive */
void af_prim_to_data(af_global_t* global, af_thread_t* thread) {
  af_thread_t* target_thread;
  af_cell_t data;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  target_thread = (af_thread_t*)(*thread->data_stack_current++);
  data = *thread->data_stack_current++;
  af_push_data(global, target_thread, data);
  AF_ADVANCE_IP(thread, 1);
}

/* >RETURN primitive */
void af_prim_to_return(af_global_t* global, af_thread_t* thread) {
  af_thread_t* target_thread;
  af_compiled_t* pointer;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  target_thread = (af_thread_t*)(*thread->data_stack_current++);
  pointer = (af_compiled_t*)(*thread->data_stack_current++);
  af_push_return(global, target_thread, pointer);
  AF_ADVANCE_IP(thread, 1);
}

/* >CLEANUP primitive */
void af_prim_to_cleanup(af_global_t* global, af_thread_t* thread) {
  af_thread_t* target_thread;
  af_word_t* word;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  target_thread = (af_thread_t*)(*thread->data_stack_current++);
  target_thread->cleanup = (af_word_t*)(*thread->data_stack_current++);
  AF_ADVANCE_IP(thread, 1);
}

/* CLEANUP> primitive */
void af_prim_from_cleanup(af_global_t* global, af_thread_t* thread) {
  af_thread_t* target_thread;
  af_word_t* word;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  target_thread = (af_thread_t*)(*thread->data_stack_current);
  *thread->data_stack_current = (af_cell_t)target_thread->cleanup;
  AF_ADVANCE_IP(thread, 1);
}

/* >DEFAULT-CLEANUP primitive */
void af_prim_to_default_cleanup(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  global->default_cleanup = (af_word_t*)(*thread->data_stack_current++);
  AF_ADVANCE_IP(thread, 1);
}

/* DEFAULT-CLEANUP> primitive */
void af_prim_from_default_cleanup(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)global->default_cleanup;
  AF_ADVANCE_IP(thread, 1);
}

/* >DROP-INPUT primitive */
void af_prim_to_drop_input(af_global_t* global, af_thread_t* thread) {
  af_thread_t* target_thread;
  af_word_t* word;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  target_thread = (af_thread_t*)(*thread->data_stack_current++);
  target_thread->drop_input = (af_word_t*)(*thread->data_stack_current++);
  AF_ADVANCE_IP(thread, 1);
}

/* DROP-INPUT> primitive */
void af_prim_from_drop_input(af_global_t* global, af_thread_t* thread) {
  af_thread_t* target_thread;
  af_word_t* word;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  target_thread = (af_thread_t*)(*thread->data_stack_current);
  *thread->data_stack_current = (af_cell_t)target_thread->drop_input;
  AF_ADVANCE_IP(thread, 1);
}

/* >DEFAULT-DROP-INPUT primitive */
void af_prim_to_default_drop_input(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  global->default_drop_input = (af_word_t*)(*thread->data_stack_current++);
  AF_ADVANCE_IP(thread, 1);
}

/* DEFAULT-DROP-INPUT> primitive */
void af_prim_from_default_drop_input(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)global->default_drop_input;
  AF_ADVANCE_IP(thread, 1);
}

/* >INTERACTIVE-ENDLINE primitive */
void af_prim_to_interactive_endline(af_global_t* global, af_thread_t* thread) {
  af_thread_t* target_thread;
  af_word_t* word;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  target_thread = (af_thread_t*)(*thread->data_stack_current++);
  target_thread->interactive_endline =
    (af_word_t*)(*thread->data_stack_current++);
  AF_ADVANCE_IP(thread, 1);
}

/* INTERACTIVE-ENDLINE> primitive */
void af_prim_from_interactive_endline(af_global_t* global,
				      af_thread_t* thread) {
  af_thread_t* target_thread;
  af_word_t* word;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  target_thread = (af_thread_t*)(*thread->data_stack_current);
  *thread->data_stack_current = (af_cell_t)target_thread->interactive_endline;
  AF_ADVANCE_IP(thread, 1);
}

/* >DEFAULT-INTERACTIVE-ENDLINE primitive */
void af_prim_to_default_interactive_endline(af_global_t* global,
					    af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  global->default_interactive_endline =
    (af_word_t*)(*thread->data_stack_current++);
  AF_ADVANCE_IP(thread, 1);
}

/* DEFAULT-INTERACTIVE-ENDLINE> primitive */
void af_prim_from_default_interactive_endline(af_global_t* global,
					      af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) =
    (af_cell_t)global->default_interactive_endline;
  AF_ADVANCE_IP(thread, 1);
}

/* ALLOCATE primitive */
void af_prim_allocate(af_global_t* global, af_thread_t* thread) {
  void* data;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  if(!(data = malloc((size_t)(*thread->data_stack_current)))) {
    *thread->data_stack_current = (af_cell_t)NULL;
    *(--thread->data_stack_current) = 1;
    return;
  }
  *thread->data_stack_current = (af_cell_t)data;
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
    *(thread->data_stack_current + 1) = (af_cell_t)NULL;
    *thread->data_stack_current = 1;
    return;
  }
  *(thread->data_stack_current + 1) = (af_cell_t)data;
  *thread->data_stack_current = 0;
  AF_ADVANCE_IP(thread, 1);
}

/* ALIGN primitive */
void af_prim_align(af_global_t* global, af_thread_t* thread) {
  af_cell_t extra = (af_cell_t)thread->data_space_current % sizeof(af_cell_t);
  if(extra) {
    if(!af_allot(global, thread, sizeof(af_cell_t) - extra)) {
      return;
    }
  }
  AF_ADVANCE_IP(thread, 1);
}

/* ALLOT primitive */
void af_prim_allot(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  if(!af_allot(global, thread, (ssize_t)(*thread->data_stack_current++))) {
    return;
  }
  AF_ADVANCE_IP(thread, 1);
}

/* UNUSED primitive */
void af_prim_unused(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) =
    (af_cell_t)(thread->data_stack_top - thread->data_stack_current);
  AF_ADVANCE_IP(thread, 1);
}

/* WORD primitive */
void af_prim_word(af_global_t* global, af_thread_t* thread) {
  af_byte_t delimiter;
  af_byte_t* text;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  delimiter = (af_byte_t)(*thread->data_stack_current & 0xFF);
  text = af_word(global, thread, delimiter);
  *thread->data_stack_current = (af_cell_t)text;
  AF_ADVANCE_IP(thread, 1);
}

/* PARSE-NAME primitive */
void af_prim_parse_name(af_global_t* global, af_thread_t* thread) {
  af_byte_t* text;
  af_cell_t length;
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 2);
  if(!af_parse_name_available(global, thread)) {
    *(--thread->data_stack_current) = (af_cell_t)NULL;
    *(--thread->data_stack_current) = 0;    
  }
  text = af_parse_name(global, thread, &length);
  *(--thread->data_stack_current) = (af_cell_t)text;
  *(--thread->data_stack_current) = length;
  AF_ADVANCE_IP(thread, 1);
}

/* THIS-THREAD primitive */
void af_prim_this_thread(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)thread;
  AF_ADVANCE_IP(thread, 1);
}

/* SPAWN primitive */
void af_prim_spawn(af_global_t* global, af_thread_t* thread) {
  af_thread_t* new_thread;
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  if(!(new_thread = af_spawn(global))) {
    return;
  }
  *thread->data_stack_current = (af_cell_t)new_thread;
  AF_ADVANCE_IP(thread, 1);
}

/* >CONSOLE-INPUT primitive */
void af_prim_to_console_input(af_global_t* global, af_thread_t* thread) {
  af_thread_t* target_thread;
  af_input_t* current_input;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  target_thread = (af_thread_t*)(*thread->data_stack_current++);
  target_thread->console_input = (af_input_t*)(*thread->data_stack_current++);
  current_input = target_thread->current_input;
  while(current_input && current_input->next_input) {
    current_input = current_input->next_input;
  }
  if(current_input) {
    current_input->next_input = target_thread->console_input;
  } else {
    target_thread->current_input = target_thread->console_input;
  }
  AF_ADVANCE_IP(thread, 1);
}

/* >CONSOLE-OUTPUT primitive */
void af_prim_to_console_output(af_global_t* global, af_thread_t* thread) {
  af_thread_t* target_thread;
  af_output_t* current_output;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  target_thread = (af_thread_t*)(*thread->data_stack_current++);
  target_thread->console_output = (af_output_t*)(*thread->data_stack_current++);
  current_output = target_thread->current_output;
  while(current_output && current_output->next_output) {
    current_output = current_output->next_output;
  }
  if(current_output) {
    current_output->next_output = target_thread->console_output;
  } else {
    target_thread->current_output = target_thread->console_output;
  }
  AF_ADVANCE_IP(thread, 1);
}

/* >CONSOLE-ERROR primitive */
void af_prim_to_console_error(af_global_t* global, af_thread_t* thread) {
  af_thread_t* target_thread;
  af_output_t* current_error;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  target_thread = (af_thread_t*)(*thread->data_stack_current++);
  target_thread->console_error = (af_output_t*)(*thread->data_stack_current++);
  current_error = target_thread->current_error;
  while(current_error && current_error->next_output) {
    current_error = current_error->next_output;
  }
  if(current_error) {
    current_error->next_output = target_thread->console_error;
  } else {
    target_thread->current_error = target_thread->console_error;
  }
  AF_ADVANCE_IP(thread, 1);
}

/* CONSOLE-INPUT> primitive */
void af_prim_from_console_input(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  *thread->data_stack_current =
    (af_cell_t)((af_thread_t*)(*thread->data_stack_current))->console_input;
  AF_ADVANCE_IP(thread, 1);
}

/* CONSOLE-OUTPUT> primitive */
void af_prim_from_console_output(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  *thread->data_stack_current =
    (af_cell_t)((af_thread_t*)(*thread->data_stack_current))->console_output;
  AF_ADVANCE_IP(thread, 1);
}

/* CONSOLE-ERROR> primitive */
void af_prim_from_console_error(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  *thread->data_stack_current =
    (af_cell_t)((af_thread_t*)(*thread->data_stack_current))->console_error;
  AF_ADVANCE_IP(thread, 1);
}

/* >INIT-WORD primitive */
void af_prim_to_init_word(af_global_t* global, af_thread_t* thread) {
  af_thread_t* target_thread;
  af_word_t* word;
  AF_VERIFY_DATA_STACK_READ(global, thread, 2);
  target_thread = (af_thread_t*)(*thread->data_stack_current++);
  word = (af_word_t*)(*thread->data_stack_current++);
  af_set_init_word(global, target_thread, word);
  AF_ADVANCE_IP(thread, 1);
}

/* >INTERPRET primitive */
void af_prim_to_interpret(af_global_t* global, af_thread_t* thread) {
  af_thread_t* target_thread;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  target_thread = (af_thread_t*)(*thread->data_stack_current++);
  af_interpret(global, target_thread);
  AF_ADVANCE_IP(thread, 1);
}

/* START primitive */
void af_prim_start(af_global_t* global, af_thread_t* thread) {
  af_thread_t* target_thread;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  target_thread = (af_thread_t*)(*thread->data_stack_current++);
  af_start(global, target_thread);
  AF_ADVANCE_IP(thread, 1);
}

/* KILL primitive */
void af_prim_kill(af_global_t* global, af_thread_t* thread) {
  af_thread_t* target_thread;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  target_thread = (af_thread_t*)(*thread->data_stack_current++);
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
  target_thread = (af_thread_t*)(*thread->data_stack_current++);
  af_sleep(global, target_thread);
  AF_ADVANCE_IP(thread, 1);
}

/* WAKE primitive */
void af_prim_wake(af_global_t* global, af_thread_t* thread) {
  af_thread_t* target_thread;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  target_thread = (af_thread_t*)(*thread->data_stack_current++);
  af_wake(global, target_thread);
  AF_ADVANCE_IP(thread, 1);
}

/* RESET primitive */
void af_prim_reset(af_global_t* global, af_thread_t* thread) {
  af_thread_t* target_thread;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  target_thread = (af_thread_t*)(*thread->data_stack_current++);
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
  af_byte_t* name;
  af_cell_t name_length;
  af_word_t* word;
  af_compiled_t* slot;
  if(!af_parse_name_available(global, thread)) {
    af_handle_word_expected(global, thread);
    return;
  }
  name = af_parse_name(global, thread, &name_length);
  if(!(word = af_lookup(global, name, name_length))) {
    af_handle_word_not_found(global, thread);
    return;
  }
  if(!word->is_immediate) {
    if(!(slot = af_allot(global, thread, sizeof(af_compiled_t) * 2))) {
      return;
    }
    slot->compiled_call = global->builtin_postpone_runtime;
    (slot + 1)->compiled_cell = (af_cell_t)word;
  } else {
    if(!(slot = af_allot(global, thread, sizeof(af_compiled_t)))) {
      return;
    }
    slot->compiled_call = word;
  }
  AF_ADVANCE_IP(thread, 1);
}

/* POSTPONE runtime primitive */
void af_prim_postpone_runtime(af_global_t* global, af_thread_t* thread) {
  af_compiled_t* slot = af_allot(global, thread, sizeof(af_compiled_t));
  if(!slot) {
    return;
  }
  slot->compiled_call = *(af_word_t**)(thread->interpreter_pointer + 1);
  AF_ADVANCE_IP(thread, 2);
}

/* BRANCH primitive */
void af_prim_branch(af_global_t* global, af_thread_t* thread) {
  if(thread->interpreter_pointer) {
    thread->interpreter_pointer =
      (af_compiled_t*)((thread->interpreter_pointer + 1)->compiled_cell);
  } else {
    af_handle_not_interactive(global, thread);
  }
}

/* ?BRANCH primitive */
void af_prim_cond_branch(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  if(thread->interpreter_pointer) {
    af_cell_t value = *thread->data_stack_current++;
    if(!value) {
      thread->interpreter_pointer =
	(af_compiled_t*)((thread->interpreter_pointer + 1)->compiled_cell);
    } else {
      AF_ADVANCE_IP(thread, 2);
    }
  } else {
    af_handle_not_interactive(global, thread);
  }
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
  af_cell_t count;
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  count = thread->data_stack_base - thread->data_stack_current;
  *(--thread->data_stack_current) = count;
  AF_ADVANCE_IP(thread, 1);
}

/* CELL-SIZE primitive */
void af_prim_cell_size(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)sizeof(af_cell_t);
  AF_ADVANCE_IP(thread, 1);
}

/* REFILL primitive */
void af_prim_refill(af_global_t* global, af_thread_t* thread) {
  af_refill(global, thread);
  if(!thread->current_input || (thread->current_input->is_closed &&
				!thread->current_input->count)) {
    AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
    *(--thread->data_stack_current) = (af_cell_t)FALSE;
  } else {
    AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
    *(--thread->data_stack_current) = (af_cell_t)TRUE;
  }
}

/* IO-ACTION-DESTROY primitive */
void af_prim_io_action_destroy(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  af_io_action_destroy((af_io_action_t*)(*thread->data_stack_current++));
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
  af_io_action_get_state((af_io_action_t*)(*thread->data_stack_current), state);
  *thread->data_stack_current = (af_cell_t)state;
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
  *(af_byte_t**)thread->data_stack_current =
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

/* IO-OPEN primitive */
void af_prim_io_open(af_global_t* global, af_thread_t* thread) {
  af_io_fd_t fd;
  af_io_error_t error;
  AF_VERIFY_DATA_STACK_READ(global, thread, 4);
  fd = af_io_open((af_byte_t*)(*(thread->data_stack_current + 3)),
		  *(thread->data_stack_current + 2),
		  (af_io_flags_t)(*(thread->data_stack_current + 1)),
		  (af_io_mode_t)(*thread->data_stack_current),
		  &error);
  thread->data_stack_current += 2;
  *(thread->data_stack_current + 1) = (af_cell_t)fd;
  *thread->data_stack_current = (af_cell_t)error;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-PIPE primitive */
void af_prim_io_pipe(af_global_t* global, af_thread_t* thread) {
  af_io_fd_t in;
  af_io_fd_t out;
  af_io_error_t error;
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 3);
  af_io_pipe(&in, &out, &error);
  *(--thread->data_stack_current) = (af_cell_t)in;
  *(--thread->data_stack_current) = (af_cell_t)out;
  *(--thread->data_stack_current) = (af_cell_t)error;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-CLOSE-BLOCK primitive */
void af_prim_io_close_block(af_global_t* global, af_thread_t* thread) {
  af_io_action_t* action;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  action = af_io_close_block(&global->io,
			     (af_io_fd_t)(*thread->data_stack_current),
			     thread);
  *thread->data_stack_current = (af_cell_t)action;
  AF_ADVANCE_IP(thread, 1);
  af_sleep(global, thread);
}

/* IO-CLOSE-ASYNC primitive */
void af_prim_io_close_async(af_global_t* global, af_thread_t* thread) {
  af_io_action_t* action;
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  action = af_io_close_async(&global->io,
			     (af_io_fd_t)(*thread->data_stack_current));
  *thread->data_stack_current = (af_cell_t)action;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-READ-BLOCK primitive */
void af_prim_io_read_block(af_global_t* global, af_thread_t* thread) {
  af_io_action_t* action;
  AF_VERIFY_DATA_STACK_READ(global, thread, 3);
  action = af_io_read_block(&global->io,
			    (af_io_fd_t)(*thread->data_stack_current),
			    (af_byte_t*)(*(thread->data_stack_current + 2)),
			    *(thread->data_stack_current + 1),
			    thread);
  thread->data_stack_current += 2;
  *thread->data_stack_current = (af_cell_t)action;
  AF_ADVANCE_IP(thread, 1);
  af_sleep(global, thread);
}

/* IO-WRITE-BLOCK primitive */
void af_prim_io_write_block(af_global_t* global, af_thread_t* thread) {
  af_io_action_t* action;
  AF_VERIFY_DATA_STACK_READ(global, thread, 3);
  action = af_io_write_block(&global->io,
			     (af_io_fd_t)(*thread->data_stack_current),
			     (af_byte_t*)(*(thread->data_stack_current + 2)),
			     *(thread->data_stack_current + 1),
			     thread);
  thread->data_stack_current += 2;
  *thread->data_stack_current = (af_cell_t)action;
  AF_ADVANCE_IP(thread, 1);
  af_sleep(global, thread);
}

/* IO-READ-ASYNC primitive */
void af_prim_io_read_async(af_global_t* global, af_thread_t* thread) {
  af_io_action_t* action;
  AF_VERIFY_DATA_STACK_READ(global, thread, 3);
  action = af_io_read_async(&global->io,
			    (af_io_fd_t)(*thread->data_stack_current),
			    (af_byte_t*)(*(thread->data_stack_current + 2)),
			    *(thread->data_stack_current + 1));
  thread->data_stack_current += 2;
  *thread->data_stack_current = (af_cell_t)action;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-WRITE-ASYNC primitive */
void af_prim_io_write_async(af_global_t* global, af_thread_t* thread) {
  af_io_action_t* action;
  AF_VERIFY_DATA_STACK_READ(global, thread, 3);
  action = af_io_write_async(&global->io,
			     (af_io_fd_t)(*thread->data_stack_current),
			     (af_byte_t*)(*(thread->data_stack_current + 2)),
			     *(thread->data_stack_current + 1));
  thread->data_stack_current += 2;
  *thread->data_stack_current = (af_cell_t)action;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-READ-NONBLOCK primitive */
void af_prim_io_read_nonblock(af_global_t* global, af_thread_t* thread) {
  af_bool_t again;
  ssize_t size;
  AF_VERIFY_DATA_STACK_READ(global, thread, 3);
  size = af_io_read_nonblock((af_io_fd_t)(*thread->data_stack_current),
			     (af_byte_t*)(*(thread->data_stack_current + 2)),
			     *(thread->data_stack_current + 1),
			     &again);
  thread->data_stack_current++;
  *(af_sign_cell_t*)(thread->data_stack_current + 1) = (af_sign_cell_t)size;
  *(af_bool_t*)thread->data_stack_current = again;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-WRITE-NONBLOCK primitive */
void af_prim_io_write_nonblock(af_global_t* global, af_thread_t* thread) {
  af_bool_t again;
  ssize_t size;
  AF_VERIFY_DATA_STACK_READ(global, thread, 3);
  size = af_io_write_nonblock((af_io_fd_t)(*thread->data_stack_current),
			      (af_byte_t*)(*(thread->data_stack_current + 2)),
			      *(thread->data_stack_current + 1),
			      &again);
  thread->data_stack_current++;
  *(af_sign_cell_t*)(thread->data_stack_current + 1) = (af_sign_cell_t)size;
  *(af_bool_t*)thread->data_stack_current = again;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-STDIN primitive */
void af_prim_io_stdin(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)AF_IO_STDIN;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-STDOUT primitive */
void af_prim_io_stdout(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)AF_IO_STDOUT;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-STDERR primitive */
void af_prim_io_stderr(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)AF_IO_STDERR;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-RDONLY primitive */
void af_prim_io_rdonly(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)AF_IO_RDONLY;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-WRONLY primitive */
void af_prim_io_wronly(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)AF_IO_WRONLY;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-RDWR primitive */
void af_prim_io_rdwr(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)AF_IO_RDWR;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-APPEND primitive */
void af_prim_io_append(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)AF_IO_APPEND;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-TRUNC primitive */
void af_prim_io_trunc(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)AF_IO_TRUNC;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-CREAT primitive */
void af_prim_io_creat(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)AF_IO_CREAT;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-EXCL primitive */
void af_prim_io_excl(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)AF_IO_EXCL;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-IRWXU primitive */
void af_prim_io_irwxu(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)AF_IO_IRWXU;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-IRUSR primitive */
void af_prim_io_irusr(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)AF_IO_IRUSR;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-IWUSR primitive */
void af_prim_io_iwusr(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)AF_IO_IWUSR;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-IXUSR primitive */
void af_prim_io_ixusr(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)AF_IO_IXUSR;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-IRWXG primitive */
void af_prim_io_irwxg(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)AF_IO_IRWXG;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-IRGRP primitive */
void af_prim_io_irgrp(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)AF_IO_IRGRP;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-IWGRP primitive */
void af_prim_io_iwgrp(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)AF_IO_IWGRP;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-IXGRP primitive */
void af_prim_io_ixgrp(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)AF_IO_IXGRP;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-IRWXO primitive */
void af_prim_io_irwxo(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)AF_IO_IRWXO;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-IROTH primitive */
void af_prim_io_iroth(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)AF_IO_IROTH;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-IWOTH primitive */
void af_prim_io_iwoth(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)AF_IO_IWOTH;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-IXOTH primitive */
void af_prim_io_ixoth(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)AF_IO_IXOTH;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-ISUID primitive */
void af_prim_io_isuid(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)AF_IO_ISUID;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-ISGID primitive */
void af_prim_io_isgid(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)AF_IO_ISGID;
  AF_ADVANCE_IP(thread, 1);
}

/* IO-ISVTX primitive */
void af_prim_io_isvtx(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)AF_IO_ISVTX;
  AF_ADVANCE_IP(thread, 1);
}

/* INPUT-SIZE primitive */
void af_prim_input_size(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)sizeof(af_input_t);
  AF_ADVANCE_IP(thread, 1);
}

/* INPUT-NEXT-INPUT primitive */
void af_prim_input_next_input(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  *thread->data_stack_current =
    (af_cell_t)(&((af_input_t*)(*thread->data_stack_current))->next_input);
  AF_ADVANCE_IP(thread, 1);
}

/* INPUT-BUFFER primitive */
void af_prim_input_buffer(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  *thread->data_stack_current =
    (af_cell_t)(&((af_input_t*)(*thread->data_stack_current))->buffer);
  AF_ADVANCE_IP(thread, 1);
}

/* INPUT-COUNT primitive */
void af_prim_input_count(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  *thread->data_stack_current =
    (af_cell_t)(&((af_input_t*)(*thread->data_stack_current))->count);
  AF_ADVANCE_IP(thread, 1);
}

/* INPUT-INDEX primitive */
void af_prim_input_index(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  *thread->data_stack_current =
    (af_cell_t)(&((af_input_t*)(*thread->data_stack_current))->index);
  AF_ADVANCE_IP(thread, 1);
}

/* INPUT-IS-CLOSED primitive */
void af_prim_input_is_closed(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  *thread->data_stack_current =
    (af_cell_t)(&((af_input_t*)(*thread->data_stack_current))->is_closed);
  AF_ADVANCE_IP(thread, 1);
}

/* INPUT-CLEANUP primitive */
void af_prim_input_cleanup(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  *thread->data_stack_current =
    (af_cell_t)(&((af_input_t*)(*thread->data_stack_current))->cleanup);
  AF_ADVANCE_IP(thread, 1);
}

/* INPUT-REFILL primitive */
void af_prim_input_refill(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  *thread->data_stack_current =
    (af_cell_t)(&((af_input_t*)(*thread->data_stack_current))->refill);
  AF_ADVANCE_IP(thread, 1);
}

/* INPUT-ARG primitive */
void af_prim_input_arg(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  *thread->data_stack_current =
    (af_cell_t)(&((af_input_t*)(*thread->data_stack_current))->arg);
  AF_ADVANCE_IP(thread, 1);
}

/* OUTPUT-SIZE primitive */
void af_prim_output_size(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = (af_cell_t)sizeof(af_output_t);
  AF_ADVANCE_IP(thread, 1);
}

/* OUTPUT-NEXT-OUTPUT primitive */
void af_prim_output_next_output(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  *thread->data_stack_current =
    (af_cell_t)(&((af_output_t*)(*thread->data_stack_current))->next_output);
  AF_ADVANCE_IP(thread, 1);
}

/* OUTPUT-WRITE primitive */
void af_prim_output_write(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  *thread->data_stack_current =
    (af_cell_t)(&((af_output_t*)(*thread->data_stack_current))->write);
  AF_ADVANCE_IP(thread, 1);
}

/* OUTPUT-CLEANUP primitive */
void af_prim_output_cleanup(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  *thread->data_stack_current =
    (af_cell_t)(&((af_output_t*)(*thread->data_stack_current))->cleanup);
  AF_ADVANCE_IP(thread, 1);
}

/* OUTPUT-ARG primitive */
void af_prim_output_arg(af_global_t* global, af_thread_t* thread) {
  AF_VERIFY_DATA_STACK_READ(global, thread, 1);
  *thread->data_stack_current =
    (af_cell_t)(&((af_output_t*)(*thread->data_stack_current))->arg);
  AF_ADVANCE_IP(thread, 1);
}
