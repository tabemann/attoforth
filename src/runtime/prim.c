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
void af_prim_docol(af_global_t* global, af_task_t* task);

/* EXIT primitive */
void af_prim_exit(af_global_t* global, af_task_t* task);

/* Push the data address of a word primitive */
void af_prim_push_data(af_global_t* global, af_task_t* task);

/* Runtime of DOES> primitve */
void af_prim_do_does(af_global_t* global, af_task_t* task);

/* (LITERAL) primitive */
void af_prim_literal_runtime(af_global_t* global, af_task_t* task);

/* CREATE primitive */
void af_prim_create(af_global_t* global, af_task_t* task);

/* : primitive */
void af_prim_colon(af_global_t* global, af_task_t* task);

/* :NONAME primitive */
void af_prim_colon_noname(af_global_t* global, af_task_t* task);

/* ; primitive - immediate */
void af_prim_semi(af_global_t* global, af_task_t* task);

/* IMMEDIATE primitive */
void af_prim_immediate(af_global_t* global, af_task_t* task);

/* IS-IMMEDIATE primitive */
void af_prim_is_immediate(af_global_t* global, af_task_t* task);

/* DUP primitive */
void af_prim_dup(af_global_t* global, af_task_t* task);

/* DROP primitive */
void af_prim_drop(af_global_t* global, af_task_t* task);

/* SWAP primitive */
void af_prim_swap(af_global_t* global, af_task_t* task);

/* OVER primitive */
void af_prim_over(af_global_t* global, af_task_t* task);

/* ROT primitive */
void af_prim_rot(af_global_t* global, af_task_t* task);

/* PICK primitive */
void af_prim_pick(af_global_t* global, af_task_t* task);

/* ROLL primitive */
void af_prim_roll(af_global_t* global, af_task_t* task);

/* + primitive */
void af_prim_add(af_global_t* global, af_task_t* task);

/* - primitive */
void af_prim_sub(af_global_t* global, af_task_t* task);

/* * primitive */
void af_prim_mul(af_global_t* global, af_task_t* task);

/* / primitive */
void af_prim_div(af_global_t* global, af_task_t* task);

/* MOD primitive */
void af_prim_mod(af_global_t* global, af_task_t* task);

/* INVERT primitive */
void af_prim_invert(af_global_t* global, af_task_t* task);

/* AND primitive */
void af_prim_and(af_global_t* global, af_task_t* task);

/* OR primitive */
void af_prim_or(af_global_t* global, af_task_t* task);

/* XOR primitive */
void af_prim_xor(af_global_t* global, af_task_t* task);

/* < primitive */
void af_prim_lt(af_global_t* global, af_task_t* task);

/* <= primitive */
void af_prim_lte(af_global_t* global, af_task_t* task);

/* > primitive */
void af_prim_gt(af_global_t* global, af_task_t* task);

/* >= primitive */
void af_prim_gte(af_global_t* global, af_task_t* task);

/* = primitive */
void af_prim_eq(af_global_t* global, af_task_t* task);

/* <> primitive */
void af_prim_ne(af_global_t* global, af_task_t* task);

/* @ primitive */
void af_prim_fetch(af_global_t* global, af_task_t* task);

/* ! primitive */
void af_prim_store(af_global_t* global, af_task_t* task);

/* +! primitive */
void af_prim_add_store(af_global_t* global, af_task_t* task);

/* C@ primitive */
void af_prim_c_fetch(af_global_t* global, af_task_t* task);

/* C! primitive */
void af_prim_c_store(af_global_t* global, af_task_t* task);

/* >R primitive */
void af_prim_to_r(af_global_t* global, af_task_t* task);

/* R> primitive */
void af_prim_from_r(af_global_t* global, af_task_t* task);

/* R@ primitive */
void af_prim_r_fetch(af_global_t* global, af_task_t* task);

/* HERE primitive */
void af_prim_here(af_global_t* global, af_task_t* task);

/* DOES> primitive */
void af_prim_does(af_global_t* global, af_task_t* task);

/* >BODY primitive */
void af_prim_to_body(af_global_t* global, af_task_t* task);

/* SP@ primitive */
void af_prim_sp_fetch(af_global_t* global, af_task_t* task);

/* SP0 primitive */
void af_prim_sp0(af_global_t* global, af_task_t* task);

/* LITTLE-ENDIAN primitive */
void af_prim_little_endian(af_global_t* global, af_task_t* task);

/* ' primitve */
void af_prim_tick(af_global_t* global, af_task_t* task);

/* ['] primitive - immediate */
void af_prim_bracket_tick(af_global_t* global, af_task_t* task);

/* EXECUTE primitive */
void af_prim_execute(af_global_t* global, af_task_t* task);

/* STATE primitive */
void af_prim_state(af_global_t* global, af_task_t* task);

/* LATESTXT primitive */
void af_prim_latestxt(af_global_t* global, af_task_t* task);

/* >IN primitive */
void af_prim_to_in(af_global_t* global, af_task_t* task);

/* SOURCE primitive */
void af_prim_source(af_global_t* global, af_task_t* task);

/* >INPUT primitive */
void af_prim_to_input(af_global_t* global, af_task_t* task);

/* INPUT> primitive */
void af_prim_from_input(af_global_t* global, af_task_t* task);

/* >OUTPUT primitive */
void af_prim_to_output(af_global_t* global, af_task_t* task);

/* OUTPUT> primitive */
void af_prim_from_output(af_global_t* global, af_task_t* task);

/* >ERROR primitive */
void af_prim_to_error(af_global_t* global, af_task_t* task);

/* ERROR> primitive */
void af_prim_from_error(af_global_t* global, af_task_t* task);

/* >DATA primitive */
void af_prim_to_data(af_global_t* global, af_task_t* task);

/* >RETURN primitive */
void af_prim_to_return(af_global_t* global, af_task_t* task);

/* >ABORT primitive */
void af_prim_to_abort(af_global_t* global, af_task_t* task);

/* ABORT> primitive */
void af_prim_from_abort(af_global_t* global, af_task_t* task);

/* >DEFAULT-ABORT primitive */
void af_prim_to_default_abort(af_global_t* global, af_task_t* task);

/* DEFAULT-ABORT> primitive */
void af_prim_from_default_abort(af_global_t* global, af_task_t* task);

/* ALLOCATE primitive */
void af_prim_allocate(af_global_t* global, af_task_t* task);

/* FREE primitive */
void af_prim_free(af_global_t* global, af_task_t* task);

/* RESIZE primitive */
void af_prim_resize(af_global_t* global, af_task_t* task);

/* ALIGN primitive */
void af_prim_align(af_global_t* global, af_task_t* task);

/* ALLOT primitive */
void af_prim_allot(af_global_t* global, af_task_t* task);

/* UNUSED primitive */
void af_prim_unused(af_global_t* global, af_task_t* task);

/* WORD primitive */
void af_prim_word(af_global_t* global, af_task_t* task);

/* PARSE-NAME primitive */
void af_prim_parse_name(af_global_t* global, af_task_t* task);

/* PARSE-NUMBER primitive */
void af_prim_parse_number(af_global_t* global, af_task_t* task);

/* FORMAT-NUMBER primitive */
void af_prim_format_number(af_global_t* global, af_task_t* task);

/* BASE primitive */
void af_prim_base(af_global_t* global, af_task_t* task);

/* FIND-WORD primitive */
void af_prim_find_word(af_global_t* global, af_task_t* task);

/* CAS primitive */
void af_prim_cas(af_global_t* global, af_task_t* task);

/* 2CAS primitive */
void af_prim_2cas(af_global_t* global, af_task_t* task);

/* THIS-TASK primitive */
void af_prim_this_task(af_global_t* global, af_task_t* task);

/* SPAWN primitive */
void af_prim_spawn(af_global_t* global, af_task_t* task);

/* SPAWN-NO-DATA primitive */
void af_prim_spawn_no_data(af_global_t* global, af_task_t* task);

/* >FREE-DATA-ON-EXIT primitive */
void af_prim_free_data_on_exit(af_global_t* global, af_task_t* task);

/* >CONSOLE-IN primitive */
void af_prim_to_console_input(af_global_t* global, af_task_t* task);

/* >CONSOLE-OUT primitive */
void af_prim_to_console_output(af_global_t* global, af_task_t* task);

/* >CONSOLE-ERR primitive */
void af_prim_to_console_error(af_global_t* global, af_task_t* task);

/* CONSOLE-INPUT> primitive */
void af_prim_from_console_input(af_global_t* global, af_task_t* task);

/* CONSOLE-OUT> primitive */
void af_prim_from_console_output(af_global_t* global, af_task_t* task);

/* CONSOLE-ERR> primitive */
void af_prim_from_console_error(af_global_t* global, af_task_t* task);

/* >INIT-WORD primitive */
void af_prim_to_init_word(af_global_t* global, af_task_t* task);

/* START primitive */
void af_prim_start(af_global_t* global, af_task_t* task);

/* KILL primitive */
void af_prim_kill(af_global_t* global, af_task_t* task);

/* YIELD primitive */
void af_prim_yield(af_global_t* global, af_task_t* task);

/* WAIT primitive */
void af_prim_wait(af_global_t* global, af_task_t* task);

/* WAKE primitive */
void af_prim_wake(af_global_t* global, af_task_t* task);

/* RESET primitive */
void af_prim_reset(af_global_t* global, af_task_t* task);

/* [ primitive - immediate */
void af_prim_open_bracket(af_global_t* global, af_task_t* task);

/* ] primitive */
void af_prim_close_bracket(af_global_t* global, af_task_t* task);

/* BRANCH primitive */
void af_prim_branch(af_global_t* global, af_task_t* task);

/* ?BRANCH primitive */
void af_prim_cond_branch(af_global_t* global, af_task_t* task);

/* MOVE primitive */
void af_prim_move(af_global_t* global, af_task_t* task);

/* DEPTH primitive */
void af_prim_depth(af_global_t* global, af_task_t* task);

/* CELL-SIZE primitive */
void af_prim_cell_size(af_global_t* global, af_task_t* task);

/* REFILL primitive */
void af_prim_refill(af_global_t* global, af_task_t* task);

/* IO-ACTION-DESTROY primitive */
void af_prim_io_action_destroy(af_global_t* global, af_task_t* task);

/* IO-ACTION-GET-STATE primitive */
void af_prim_io_action_get_state(af_global_t* global, af_task_t* task);

/* IO-STATE-DESTROY primitive */
void af_prim_io_state_destroy(af_global_t* global, af_task_t* task);

/* IO-STATE-IS-DONE primitive */
void af_prim_io_state_is_done(af_global_t* global, af_task_t* task);

/* IO-STATE-IS-CLOSED primitive */
void af_prim_io_state_is_closed(af_global_t* global, af_task_t* task);

/* IO-STATE-HAS-HANGUP primitive */
void af_prim_io_state_has_hangup(af_global_t* global, af_task_t* task);

/* IO-STATE-HAS-ERROR primitive */
void af_prim_io_state_has_error(af_global_t* global, af_task_t* task);

/* IO-STATE-GET-BUFFER primitive */
void af_prim_io_state_get_buffer(af_global_t* global, af_task_t* task);

/* IO-STATE-GET-INDEX primitive */
void af_prim_io_state_get_index(af_global_t* global, af_task_t* task);

/* IO-STATE-GET-COUNT primitive */
void af_prim_io_state_get_count(af_global_t* global, af_task_t* task);

/* IO-OPEN primitive */
void af_prim_io_open(af_global_t* global, af_task_t* task);

/* IO-PIPE primitive */
void af_prim_io_pipe(af_global_t* global, af_task_t* task);

/* IO-GET-MONOTONIC-TIME primitive */
void af_prim_io_get_monotonic_time(af_global_t* global, af_task_t* task);

/* IO-SLEEP primitive */
void af_prim_io_sleep(af_global_t* global, af_task_t* task);

/* IO-CLOSE-BLOCK primitive */
void af_prim_io_close_block(af_global_t* global, af_task_t* task);

/* IO-CLOSE-ASYNC primitive */
void af_prim_io_close_async(af_global_t* global, af_task_t* task);

/* IO-SEEK-BLOCK primitive */
void af_prim_io_seek_block(af_global_t* global, af_task_t* task);

/* IO-SEEK-ASYNC primitive */
void af_prim_io_seek_async(af_global_t* global, af_task_t* task);

/* IO-READ-BLOCK primitive */
void af_prim_io_read_block(af_global_t* global, af_task_t* task);

/* IO-WRITE-BLOCK primitive */
void af_prim_io_write_block(af_global_t* global, af_task_t* task);

/* IO-READ-ASYNC primitive */
void af_prim_io_read_async(af_global_t* global, af_task_t* task);

/* IO-WRITE-ASYNC primitive */
void af_prim_io_write_async(af_global_t* global, af_task_t* task);

/* IO-READ-NONBLOCK primitive */
void af_prim_io_read_nonblock(af_global_t* global, af_task_t* task);

/* IO-WRITE-NONBLOCK primitive */
void af_prim_io_write_nonblock(af_global_t* global, af_task_t* task);

/* IO-STDIN primitive */
void af_prim_io_stdin(af_global_t* global, af_task_t* task);

/* IO-STDOUT primitive */
void af_prim_io_stdout(af_global_t* global, af_task_t* task);

/* IO-STDERR primitive */
void af_prim_io_stderr(af_global_t* global, af_task_t* task);

/* IO-RDONLY primitive */
void af_prim_io_rdonly(af_global_t* global, af_task_t* task);

/* IO-WRONLY primitive */
void af_prim_io_wronly(af_global_t* global, af_task_t* task);

/* IO-RDWR primitive */
void af_prim_io_rdwr(af_global_t* global, af_task_t* task);

/* IO-APPEND primitive */
void af_prim_io_append(af_global_t* global, af_task_t* task);

/* IO-TRUNC primitive */
void af_prim_io_trunc(af_global_t* global, af_task_t* task);

/* IO-CREAT primitive */
void af_prim_io_creat(af_global_t* global, af_task_t* task);

/* IO-EXCL primitive */
void af_prim_io_excl(af_global_t* global, af_task_t* task);

/* IO-IRWXU primitive */
void af_prim_io_irwxu(af_global_t* global, af_task_t* task);

/* IO-IRUSR primitive */
void af_prim_io_irusr(af_global_t* global, af_task_t* task);

/* IO-IWUSR primitive */
void af_prim_io_iwusr(af_global_t* global, af_task_t* task);

/* IO-IXUSR primitive */
void af_prim_io_ixusr(af_global_t* global, af_task_t* task);

/* IO-IRWXG primitive */
void af_prim_io_irwxg(af_global_t* global, af_task_t* task);

/* IO-IRGRP primitive */
void af_prim_io_irgrp(af_global_t* global, af_task_t* task);

/* IO-IWGRP primitive */
void af_prim_io_iwgrp(af_global_t* global, af_task_t* task);

/* IO-IXGRP primitive */
void af_prim_io_ixgrp(af_global_t* global, af_task_t* task);

/* IO-IRWXO primitive */
void af_prim_io_irwxo(af_global_t* global, af_task_t* task);

/* IO-IROTH primitive */
void af_prim_io_iroth(af_global_t* global, af_task_t* task);

/* IO-IWOTH primitive */
void af_prim_io_iwoth(af_global_t* global, af_task_t* task);

/* IO-IXOTH primitive */
void af_prim_io_ixoth(af_global_t* global, af_task_t* task);

/* IO-ISUID primitive */
void af_prim_io_isuid(af_global_t* global, af_task_t* task);

/* IO-ISGID primitive */
void af_prim_io_isgid(af_global_t* global, af_task_t* task);

/* IO-ISVTX primitive */
void af_prim_io_isvtx(af_global_t* global, af_task_t* task);

/* IO-SEEK-SET primitive */
void af_prim_io_seek_set(af_global_t* global, af_task_t* task);

/* IO-SEEK-CUR primitive */
void af_prim_io_seek_cur(af_global_t* global, af_task_t* task);

/* IO-SEEK-END primitive */
void af_prim_io_seek_end(af_global_t* global, af_task_t* task);

/* INPUT-SIZE primitive */
void af_prim_input_size(af_global_t* global, af_task_t* task);

/* INPUT-NEXT-INPUT primitive */
void af_prim_input_next_input(af_global_t* global, af_task_t* task);

/* INPUT-BUFFER primitive */
void af_prim_input_buffer(af_global_t* global, af_task_t* task);

/* INPUT-COUNT primitive */
void af_prim_input_count(af_global_t* global, af_task_t* task);

/* INPUT-INDEX primitive */
void af_prim_input_index(af_global_t* global, af_task_t* task);

/* INPUT-IS-CLOSED primitive */
void af_prim_input_is_closed(af_global_t* global, af_task_t* task);

/* INPUT-CLEANUP primitive */
void af_prim_input_cleanup(af_global_t* global, af_task_t* task);

/* INPUT-REFILL primitive */
void af_prim_input_refill(af_global_t* global, af_task_t* task);

/* INPUT-ARG primitive */
void af_prim_input_arg(af_global_t* global, af_task_t* task);

/* OUTPUT-SIZE primitive */
void af_prim_output_size(af_global_t* global, af_task_t* task);

/* OUTPUT-NEXT-OUTPUT primitive */
void af_prim_output_next_output(af_global_t* global, af_task_t* task);

/* OUTPUT-WRITE primitive */
void af_prim_output_write(af_global_t* global, af_task_t* task);

/* OUTPUT-CLEANUP primitive */
void af_prim_output_cleanup(af_global_t* global, af_task_t* task);

/* OUTPUT-ARG primitive */
void af_prim_output_arg(af_global_t* global, af_task_t* task);

/* EMPTY-DATA-STACK primitive */
void af_prim_empty_data_stack(af_global_t* global, af_task_t* task);

/* EMPTY-RETURN-STACK primitive */
void af_prim_empty_return_stack(af_global_t* global, af_task_t* task);

/* Function definitions */

/* Register primitives */
void af_register_prims(af_global_t* global, af_task_t* task) {
  global->builtin_literal_runtime =
    af_register_prim(global, task, "(LITERAL)", af_prim_literal_runtime,
		     FALSE);
  global->builtin_exit =
    af_register_prim(global, task, "EXIT", af_prim_exit, FALSE);
  af_register_prim(global, task, "CREATE", af_prim_create, FALSE);
  af_register_prim(global, task, ":", af_prim_colon, FALSE);
  af_register_prim(global, task, ":NONAME", af_prim_colon_noname, FALSE);
  af_register_prim(global, task, ";", af_prim_semi, TRUE);
  af_register_prim(global, task, "IMMEDIATE", af_prim_immediate, FALSE);
  af_register_prim(global, task, "IS-IMMEDIATE", af_prim_is_immediate, FALSE);
  af_register_prim(global, task, "DUP", af_prim_dup, FALSE);
  af_register_prim(global, task, "DROP", af_prim_drop, FALSE);
  af_register_prim(global, task, "SWAP", af_prim_swap, FALSE);
  af_register_prim(global, task, "OVER", af_prim_over, FALSE);
  af_register_prim(global, task, "ROT", af_prim_rot, FALSE);
  af_register_prim(global, task, "PICK", af_prim_pick, FALSE);
  af_register_prim(global, task, "ROLL", af_prim_roll, FALSE);
  af_register_prim(global, task, "+", af_prim_add, FALSE);
  af_register_prim(global, task, "-", af_prim_sub, FALSE);
  af_register_prim(global, task, "*", af_prim_mul, FALSE);
  af_register_prim(global, task, "/", af_prim_div, FALSE);
  af_register_prim(global, task, "MOD", af_prim_mod, FALSE);
  af_register_prim(global, task, "INVERT", af_prim_invert, FALSE);
  af_register_prim(global, task, "AND", af_prim_and, FALSE);
  af_register_prim(global, task, "OR", af_prim_or, FALSE);
  af_register_prim(global, task, "XOR", af_prim_xor, FALSE);
  af_register_prim(global, task, "<", af_prim_lt, FALSE);
  af_register_prim(global, task, "<=", af_prim_lte, FALSE);
  af_register_prim(global, task, ">", af_prim_gt, FALSE);
  af_register_prim(global, task, ">=", af_prim_gte, FALSE);
  af_register_prim(global, task, "=", af_prim_eq, FALSE);
  af_register_prim(global, task, "<>", af_prim_ne, FALSE);
  af_register_prim(global, task, "@", af_prim_fetch, FALSE);
  af_register_prim(global, task, "!", af_prim_store, FALSE);
  af_register_prim(global, task, "+!", af_prim_add_store, FALSE);
  af_register_prim(global, task, "C@", af_prim_c_fetch, FALSE);
  af_register_prim(global, task, "C!", af_prim_c_store, FALSE);
  af_register_prim(global, task, ">R", af_prim_to_r, FALSE);
  af_register_prim(global, task, "R>", af_prim_from_r, FALSE);
  af_register_prim(global, task, "R@", af_prim_r_fetch, FALSE);
  af_register_prim(global, task, "HERE", af_prim_here, FALSE);
  af_register_prim(global, task, "DOES>", af_prim_does, FALSE);
  af_register_prim(global, task, ">BODY", af_prim_to_body, FALSE);
  af_register_prim(global, task, "SP@", af_prim_sp_fetch, FALSE);
  af_register_prim(global, task, "SP0", af_prim_sp0, FALSE);
  af_register_prim(global, task, "LITTLE-ENDIAN", af_prim_little_endian, FALSE);
  af_register_prim(global, task, "'", af_prim_tick, FALSE);
  af_register_prim(global, task, "[']", af_prim_bracket_tick, TRUE);
  af_register_prim(global, task, "EXECUTE", af_prim_execute, FALSE);
  af_register_prim(global, task, "STATE", af_prim_state, FALSE);
  af_register_prim(global, task, "LATESTXT", af_prim_latestxt, FALSE);
  af_register_prim(global, task, ">IN", af_prim_to_in, FALSE);
  af_register_prim(global, task, "SOURCE", af_prim_source, FALSE);
  af_register_prim(global, task, ">INPUT", af_prim_to_input, FALSE);
  af_register_prim(global, task, "INPUT>", af_prim_from_input, FALSE);
  af_register_prim(global, task, ">OUTPUT", af_prim_to_output, FALSE);
  af_register_prim(global, task, "OUTPUT>", af_prim_from_output, FALSE);
  af_register_prim(global, task, ">ERROR", af_prim_to_error, FALSE);
  af_register_prim(global, task, "ERROR>", af_prim_from_error, FALSE);
  af_register_prim(global, task, ">DATA", af_prim_to_data, FALSE);
  af_register_prim(global, task, ">RETURN", af_prim_to_return, FALSE);
  af_register_prim(global, task, ">ABORT", af_prim_to_abort, FALSE);
  af_register_prim(global, task, "ABORT>", af_prim_from_abort, FALSE);
  af_register_prim(global, task, ">DEFAULT-ABORT",
		   af_prim_to_default_abort, FALSE);
  af_register_prim(global, task, "DEFAULT-ABORT>",
		   af_prim_from_default_abort, FALSE);
  af_register_prim(global, task, "ALLOCATE", af_prim_allocate, FALSE);
  global->builtin_free =
    af_register_prim(global, task, "FREE", af_prim_free, FALSE);
  af_register_prim(global, task, "RESIZE", af_prim_resize, FALSE);
  af_register_prim(global, task, "ALIGN", af_prim_align, FALSE);
  af_register_prim(global, task, "ALLOT", af_prim_allot, FALSE);
  af_register_prim(global, task, "WORD", af_prim_word, FALSE);
  af_register_prim(global, task, "PARSE-NAME", af_prim_parse_name, FALSE);
  af_register_prim(global, task, "PARSE-NUMBER", af_prim_parse_number, FALSE);
  af_register_prim(global, task, "FORMAT-NUMBER", af_prim_format_number, FALSE);
  af_register_prim(global, task, "BASE", af_prim_base, FALSE);
  af_register_prim(global, task, "FIND-WORD", af_prim_find_word, FALSE);
  af_register_prim(global, task, "CAS", af_prim_cas, FALSE);
  af_register_prim(global, task, "2CAS", af_prim_2cas, FALSE);
  af_register_prim(global, task, "THIS-TASK", af_prim_this_task, FALSE);
  af_register_prim(global, task, "SPAWN", af_prim_spawn, FALSE);
  af_register_prim(global, task, "SPAWN-NO-DATA", af_prim_spawn_no_data, FALSE);
  af_register_prim(global, task, ">FREE-DATA-ON-EXIT",
		   af_prim_free_data_on_exit, FALSE);
  af_register_prim(global, task, ">CONSOLE-INPUT", af_prim_to_console_input,
		   FALSE);
  af_register_prim(global, task, ">CONSOLE-OUTPUT", af_prim_to_console_output,
		   FALSE);
  af_register_prim(global, task, ">CONSOLE-ERROR", af_prim_to_console_error,
		   FALSE);
  af_register_prim(global, task, "CONSOLE-INPUT>",
		   af_prim_from_console_input, FALSE);
  af_register_prim(global, task, "CONSOLE-OUTPUT>",
		   af_prim_from_console_output, FALSE);
  af_register_prim(global, task, "CONSOLE-ERROR>",
		   af_prim_from_console_error, FALSE);
  af_register_prim(global, task, ">INIT-WORD", af_prim_to_init_word, FALSE);
  af_register_prim(global, task, "START", af_prim_start, FALSE);
  af_register_prim(global, task, "KILL", af_prim_kill, FALSE);
  af_register_prim(global, task, "YIELD", af_prim_yield, FALSE);
  af_register_prim(global, task, "WAIT", af_prim_wait, FALSE);
  af_register_prim(global, task, "WAKE", af_prim_wake, FALSE);
  af_register_prim(global, task, "RESET", af_prim_reset, FALSE);
  af_register_prim(global, task, "[", af_prim_open_bracket, TRUE);
  af_register_prim(global, task, "]", af_prim_close_bracket, FALSE);
  af_register_prim(global, task, "BRANCH", af_prim_branch, FALSE);
  af_register_prim(global, task, "?BRANCH", af_prim_cond_branch, FALSE);
  af_register_prim(global, task, "MOVE", af_prim_move, FALSE);
  af_register_prim(global, task, "DEPTH", af_prim_depth, FALSE);
  af_register_prim(global, task, "CELL-SIZE", af_prim_cell_size, FALSE);
  af_register_prim(global, task, "REFILL", af_prim_refill, FALSE);
  af_register_prim(global, task, "IO-ACTION-DESTROY",
		   af_prim_io_action_destroy, FALSE);
  af_register_prim(global, task, "IO-ACTION-GET-STATE",
		   af_prim_io_action_get_state, FALSE);
  af_register_prim(global, task, "IO-STATE-DESTROY", af_prim_io_state_destroy,
		   FALSE);
  af_register_prim(global, task, "IO-STATE-IS-DONE",
		   af_prim_io_state_is_done, FALSE);
  af_register_prim(global, task, "IO-STATE-IS-CLOSED",
		   af_prim_io_state_is_closed, FALSE);
  af_register_prim(global, task, "IO-STATE-HAS-HANGUP",
		   af_prim_io_state_has_hangup, FALSE);
  af_register_prim(global, task, "IO-STATE-HAS-ERROR",
		   af_prim_io_state_has_error, FALSE);
  af_register_prim(global, task, "IO-STATE-GET-BUFFER",
		   af_prim_io_state_get_buffer, FALSE);
  af_register_prim(global, task, "IO-STATE-GET-INDEX",
		   af_prim_io_state_get_index, FALSE);
  af_register_prim(global, task, "IO-STATE-GET-COUNT",
		   af_prim_io_state_get_count, FALSE);
  af_register_prim(global, task, "IO-OPEN", af_prim_io_open, FALSE);
  af_register_prim(global, task, "IO-PIPE", af_prim_io_pipe, FALSE);
  af_register_prim(global, task, "IO-GET-MONOTONIC-TIME",
		   af_prim_io_get_monotonic_time, FALSE);
  af_register_prim(global, task, "IO-SLEEP", af_prim_io_sleep, FALSE);
  af_register_prim(global, task, "IO-CLOSE-BLOCK", af_prim_io_close_block,
		   FALSE);
  af_register_prim(global, task, "IO-CLOSE-ASYNC", af_prim_io_close_async,
		   FALSE);
  af_register_prim(global, task, "IO-SEEK-BLOCK", af_prim_io_seek_block,
		   FALSE);
  af_register_prim(global, task, "IO-SEEK-ASYNC", af_prim_io_seek_async,
		   FALSE);
  af_register_prim(global, task, "IO-READ-BLOCK", af_prim_io_read_block,
		   FALSE);
  af_register_prim(global, task, "IO-WRITE-BLOCK", af_prim_io_write_block,
		   FALSE);
  af_register_prim(global, task, "IO-READ-ASYNC", af_prim_io_read_async,
		   FALSE);
  af_register_prim(global, task, "IO-WRITE-ASYNC", af_prim_io_write_async,
		   FALSE);
  af_register_prim(global, task, "IO-READ-NONBLOCK", af_prim_io_read_nonblock,
		   FALSE);
  af_register_prim(global, task, "IO-WRITE-NONBLOCK",
		   af_prim_io_write_nonblock, FALSE);
  af_register_prim(global, task, "IO-STDIN", af_prim_io_stdin, FALSE);
  af_register_prim(global, task, "IO-STDOUT", af_prim_io_stdout, FALSE);
  af_register_prim(global, task, "IO-STDERR", af_prim_io_stderr, FALSE);
  af_register_prim(global, task, "IO-RDONLY", af_prim_io_rdonly, FALSE);
  af_register_prim(global, task, "IO-WRONLY", af_prim_io_wronly, FALSE);
  af_register_prim(global, task, "IO-RDWR", af_prim_io_rdwr, FALSE);
  af_register_prim(global, task, "IO-APPEND", af_prim_io_append, FALSE);
  af_register_prim(global, task, "IO-TRUNC", af_prim_io_trunc, FALSE);
  af_register_prim(global, task, "IO-CREAT", af_prim_io_creat, FALSE);
  af_register_prim(global, task, "IO-EXCL", af_prim_io_excl, FALSE);
  af_register_prim(global, task, "IO-IRWXU", af_prim_io_irwxu, FALSE);
  af_register_prim(global, task, "IO-IRUSR", af_prim_io_irusr, FALSE);
  af_register_prim(global, task, "IO-IWUSR", af_prim_io_iwusr, FALSE);
  af_register_prim(global, task, "IO-IXUSR", af_prim_io_ixusr, FALSE);
  af_register_prim(global, task, "IO-IRWXG", af_prim_io_irwxg, FALSE);
  af_register_prim(global, task, "IO-IRGRP", af_prim_io_irgrp, FALSE);
  af_register_prim(global, task, "IO-IWGRP", af_prim_io_iwgrp, FALSE);
  af_register_prim(global, task, "IO-IXGRP", af_prim_io_ixgrp, FALSE);
  af_register_prim(global, task, "IO-IRWXO", af_prim_io_irwxo, FALSE);
  af_register_prim(global, task, "IO-IROTH", af_prim_io_iroth, FALSE);
  af_register_prim(global, task, "IO-IWOTH", af_prim_io_iwoth, FALSE);
  af_register_prim(global, task, "IO-IXOTH", af_prim_io_ixoth, FALSE);
  af_register_prim(global, task, "IO-ISUID", af_prim_io_isuid, FALSE);
  af_register_prim(global, task, "IO-ISGID", af_prim_io_isgid, FALSE);
  af_register_prim(global, task, "IO-ISVTX", af_prim_io_isvtx, FALSE);
  af_register_prim(global, task, "IO-SEEK-SET",
		   af_prim_io_seek_set, FALSE);
  af_register_prim(global, task, "IO-SEEK-CUR",
		   af_prim_io_seek_cur, FALSE);
  af_register_prim(global, task, "IO-SEEK-END",
		   af_prim_io_seek_end, FALSE);
  af_register_prim(global, task, "INPUT-SIZE", af_prim_input_size, FALSE);
  af_register_prim(global, task, "INPUT-NEXT-INPUT", af_prim_input_next_input,
		   FALSE);
  af_register_prim(global, task, "INPUT-BUFFER", af_prim_input_buffer, FALSE);
  af_register_prim(global, task, "INPUT-COUNT", af_prim_input_count, FALSE);
  af_register_prim(global, task, "INPUT-INDEX", af_prim_input_index, FALSE);
  af_register_prim(global, task, "INPUT-IS-CLOSED", af_prim_input_is_closed,
		   FALSE);
  af_register_prim(global, task, "INPUT-CLEANUP", af_prim_input_cleanup,
		   FALSE);
  af_register_prim(global, task, "INPUT-REFILL", af_prim_input_refill, FALSE);
  af_register_prim(global, task, "INPUT-ARG", af_prim_input_arg, FALSE);
  af_register_prim(global, task, "OUTPUT-SIZE", af_prim_output_size, FALSE);
  af_register_prim(global, task, "OUTPUT-NEXT-OUTPUT",
		   af_prim_output_next_output, FALSE);
  af_register_prim(global, task, "OUTPUT-WRITE", af_prim_output_write, FALSE);
  af_register_prim(global, task, "OUTPUT-CLEANUP", af_prim_output_cleanup,
		   FALSE);
  af_register_prim(global, task, "OUTPUT-ARG", af_prim_output_arg, FALSE);
  af_register_prim(global, task, "EMPTY-DATA-STACK",
		   af_prim_empty_data_stack, FALSE);
  af_register_prim(global, task, "EMPTY-RETURN-STACK",
		   af_prim_empty_return_stack, FALSE);
}

/* Docol primitive */
void af_prim_docol(af_global_t* global, af_task_t* task) {
  if(task->return_stack_current > task->return_stack_top) {
    *(--task->return_stack_current) =
      task->interpreter_pointer ? task->interpreter_pointer + 1 : NULL;
    task->interpreter_pointer = task->current_word->secondary;
  } else {
    af_handle_return_stack_overflow(global, task);
  }
}

/* Exit primitive */
void af_prim_exit(af_global_t* global, af_task_t* task) {
  if(task->return_stack_current < task->return_stack_base) {
    task->interpreter_pointer = *task->return_stack_current++;
  } else {
    af_handle_return_stack_underflow(global, task);
  }
}

/* Push the data address of a word primitive */
void af_prim_push_data(af_global_t* global, af_task_t* task) {
  if(task->data_stack_current > task->data_stack_top) {
    *(--task->data_stack_current) =
      (af_cell_t)task->current_word->data;
    AF_ADVANCE_IP(task, 1);
  } else {
    af_handle_data_stack_overflow(global, task);
  }
}

/* Runtime of DOES> primitve */
void af_prim_do_does(af_global_t* global, af_task_t* task) {
  if(task->data_stack_current > task->data_stack_top) {
    if(task->return_stack_current > task->return_stack_top) {
      *(--task->data_stack_current) = (af_cell_t)task->current_word->data;
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
void af_prim_literal_runtime(af_global_t* global, af_task_t* task) {
  if(--task->data_stack_current >= task->data_stack_top) {
    *task->data_stack_current =
      (task->interpreter_pointer + 1)->compiled_cell;
    AF_ADVANCE_IP(task, 2);
  } else {
    af_handle_data_stack_overflow(global, task);
  }
}

/* LITERAL primitive - immediate */
void af_prim_literal(af_global_t* global, af_task_t* task) {
  af_compiled_t* slot;
  AF_VERIFY_COMPILING(global, task);
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  slot = af_allot(global, task, sizeof(af_compiled_t) * 2);
  if(slot) {
    slot->compiled_call = global->builtin_literal_runtime;
    (slot + 1)->compiled_cell = *task->data_stack_current++;
    AF_ADVANCE_IP(task, 1);
  }
}

/* CREATE primitive */
void af_prim_create(af_global_t* global, af_task_t* task) {
  af_byte_t* name;
  af_cell_t name_length;
  size_t name_size;
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
  if(!(word_space = af_allocate(global, task, sizeof(af_word_t) + name_size))) {
    return;
  }
  word = word_space;
  AF_WORD_NAME_LEN(word) = (af_byte_t)name_length;
  memmove(AF_WORD_NAME_DATA(word), name, name_length);
  word->is_immediate = FALSE;
  word->code = af_prim_push_data;
  word->data = word_space + sizeof(af_word_t) + name_size;
  word->secondary = NULL;
  task->most_recent_word = word;
  word->next_word = global->first_word;
  global->first_word = word;
  AF_ADVANCE_IP(task, 1);
}

/* : primitive */
void af_prim_colon(af_global_t* global, af_task_t* task) {
  af_byte_t* name;
  af_cell_t name_length;
  size_t name_size;
  void* word_space;
  af_word_t* word;
  if(!af_parse_name_available(global, task)) {
    af_handle_word_expected(global, task);
    return;
  }
  name = af_parse_name(global, task, &name_length);
  name_length = name_length < 256 ? name_length : 255;

  /*af_byte_t* buffer = malloc(name_length + 1);
  memcpy(buffer, name, name_length);
  buffer[name_length] = 0;
  fprintf(stderr, "Compiling word: \"%s\"\n", buffer);
  free(buffer);*/

  name_size = (name_length + 1) * sizeof(af_byte_t);
  if(!(word_space = af_allocate(global, task, sizeof(af_word_t) + name_size))) {
    return;
  }
  word = word_space;
  AF_WORD_NAME_LEN(word) = (af_byte_t)name_length;
  memmove(AF_WORD_NAME_DATA(word), name, name_length);

  /* TESTING */
  /*  char* buffer = malloc(name_length + 1);
  memcpy(buffer, name, name_length);
  buffer[name_length] = '\0';
  printf("STARTING COMPILING: %s\n", buffer);
  free(buffer);*/


  word->next_word = global->first_word;
  word->is_immediate = FALSE;
  word->code = af_prim_docol;
  word->data = NULL;
  word->secondary = word_space + sizeof(af_word_t) + name_size;
  task->most_recent_word = word;
  global->first_word = word;
  task->is_compiling = TRUE;
  AF_ADVANCE_IP(task, 1);
}

/* :NONAME primitive - immediate */
void af_prim_colon_noname(af_global_t* global, af_task_t* task) {
  void* word_space;
  af_word_t* word;
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  if(!(word_space = af_allocate(global, task,
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
  task->most_recent_word = word;
  task->is_compiling = TRUE;
  *(--task->data_stack_current) = (af_cell_t)word;
  AF_ADVANCE_IP(task, 1);
}

/* ; primitive - immediate */
void af_prim_semi(af_global_t* global, af_task_t* task) {
  void* space;
  AF_VERIFY_COMPILING(global, task);
  if(!(space = af_allot(global, task, sizeof(af_compiled_t)))) {
    return;
  }
  ((af_compiled_t*)space)->compiled_call = global->builtin_exit;
  task->is_compiling = FALSE;
  AF_ADVANCE_IP(task, 1);
}

/* IMMEDIATE primitive */
void af_prim_immediate(af_global_t* global, af_task_t* task) {
  AF_VERIFY_WORD_CREATED(global, task);
  task->most_recent_word->is_immediate = TRUE;
  AF_ADVANCE_IP(task, 1);
}

/* IS-IMMEDIATE primitive */
void af_prim_is_immediate(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  *task->data_stack_current =
    (af_bool_t)((af_word_t*)(*task->data_stack_current))->is_immediate;
  AF_ADVANCE_IP(task, 1);
}

/* DUP primitive */
void af_prim_dup(af_global_t* global, af_task_t* task) {
  af_cell_t value;
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  value = *task->data_stack_current;
  *(--task->data_stack_current) = value;
  AF_ADVANCE_IP(task, 1);
}

/* DROP primitive */
void af_prim_drop(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  task->data_stack_current++;
  AF_ADVANCE_IP(task, 1);
}

/* SWAP primitive */
void af_prim_swap(af_global_t* global, af_task_t* task) {
  af_cell_t value;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  value = *task->data_stack_current;
  *task->data_stack_current = *(task->data_stack_current + 1);
  *(task->data_stack_current + 1) = value;
  AF_ADVANCE_IP(task, 1);
}

/* OVER primitive */
void af_prim_over(af_global_t* global, af_task_t* task) {
  af_cell_t value;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  value = *(task->data_stack_current + 1);
  *(--task->data_stack_current) = value;
  AF_ADVANCE_IP(task, 1);
}

/* ROT primitive */
void af_prim_rot(af_global_t* global, af_task_t* task) {
  af_cell_t value;
  AF_VERIFY_DATA_STACK_READ(global, task, 3);
  value = *(task->data_stack_current + 2);
  *(task->data_stack_current + 2) = *(task->data_stack_current + 1);
  *(task->data_stack_current + 1) = *task->data_stack_current;
  *task->data_stack_current = value;
  AF_ADVANCE_IP(task, 1);
}

/* PICK primitive */
void af_prim_pick(af_global_t* global, af_task_t* task) {
  af_cell_t index;
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  index = *task->data_stack_current;
  AF_VERIFY_DATA_STACK_READ(global, task, index + 2);
  *task->data_stack_current = *(task->data_stack_current + index + 1);
  AF_ADVANCE_IP(task, 1);
}

/* ROLL primitive */
void af_prim_roll(af_global_t* global, af_task_t* task) {
  af_cell_t index;
  af_cell_t value;
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  index = *task->data_stack_current++;
  AF_VERIFY_DATA_STACK_READ(global, task, index);
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
void af_prim_add(af_global_t* global, af_task_t* task) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  value0 = *((af_sign_cell_t*)task->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)task->data_stack_current;
  *(++task->data_stack_current) = (af_cell_t)(value0 + value1);
  AF_ADVANCE_IP(task, 1);
}

/* - primitive */
void af_prim_sub(af_global_t* global, af_task_t* task) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  value0 = *((af_sign_cell_t*)task->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)task->data_stack_current;
  *(++task->data_stack_current) = (af_cell_t)(value0 - value1);
  AF_ADVANCE_IP(task, 1);
}

/* * primitive */
void af_prim_mul(af_global_t* global, af_task_t* task) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  value0 = *((af_sign_cell_t*)task->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)task->data_stack_current;
  *(++task->data_stack_current) = (af_cell_t)(value0 * value1);
  AF_ADVANCE_IP(task, 1);
}

/* / primitive */
void af_prim_div(af_global_t* global, af_task_t* task) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
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
void af_prim_mod(af_global_t* global, af_task_t* task) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  value0 = *((af_sign_cell_t*)task->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)task->data_stack_current;
  if(value1 == 0) {
    af_handle_divide_by_zero(global, task);
    return;
  }
  *(++task->data_stack_current) = (af_cell_t)(value0 % value1);
  AF_ADVANCE_IP(task, 1);
}

/* INVERT primitive */
void af_prim_invert(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  *task->data_stack_current = ~(*task->data_stack_current);
  AF_ADVANCE_IP(task, 1);
}

/* AND primitive */
void af_prim_and(af_global_t* global, af_task_t* task) {
  af_cell_t value0;
  af_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  value0 = *(task->data_stack_current + 1);
  value1 = *task->data_stack_current;
  *(++task->data_stack_current) = value0 & value1;
  AF_ADVANCE_IP(task, 1);
}

/* OR primitive */
void af_prim_or(af_global_t* global, af_task_t* task) {
  af_cell_t value0;
  af_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  value0 = *(task->data_stack_current + 1);
  value1 = *task->data_stack_current;
  *(++task->data_stack_current) = value0 | value1;
  AF_ADVANCE_IP(task, 1);
}

/* XOR primitive */
void af_prim_xor(af_global_t* global, af_task_t* task) {
  af_cell_t value0;
  af_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  value0 = *(task->data_stack_current + 1);
  value1 = *task->data_stack_current;
  *(++task->data_stack_current) = value0 ^ value1;
  AF_ADVANCE_IP(task, 1);
}

/* < primitive */
void af_prim_lt(af_global_t* global, af_task_t* task) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  value0 = *((af_sign_cell_t*)task->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)task->data_stack_current;
  *(++task->data_stack_current) = (af_cell_t)(value0 < value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* <= primitive */
void af_prim_lte(af_global_t* global, af_task_t* task) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  value0 = *((af_sign_cell_t*)task->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)task->data_stack_current;
  *(++task->data_stack_current) = (af_cell_t)(value0 <= value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* > primitive */
void af_prim_gt(af_global_t* global, af_task_t* task) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  value0 = *((af_sign_cell_t*)task->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)task->data_stack_current;
  *(++task->data_stack_current) = (af_cell_t)(value0 > value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* >= primitive */
void af_prim_gte(af_global_t* global, af_task_t* task) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  value0 = *((af_sign_cell_t*)task->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)task->data_stack_current;
  *(++task->data_stack_current) = (af_cell_t)(value0 >= value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* = primitive */
void af_prim_eq(af_global_t* global, af_task_t* task) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  value0 = *((af_sign_cell_t*)task->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)task->data_stack_current;
  *(++task->data_stack_current) = (af_cell_t)(value0 == value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* <> primitive */
void af_prim_ne(af_global_t* global, af_task_t* task) {
  af_sign_cell_t value0;
  af_sign_cell_t value1;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  value0 = *((af_sign_cell_t*)task->data_stack_current + 1);
  value1 = *(af_sign_cell_t*)task->data_stack_current;
  *(++task->data_stack_current) = (af_cell_t)(value0 != value1 ? -1 : 0);
  AF_ADVANCE_IP(task, 1);
}

/* @ primitive */
void af_prim_fetch(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  *task->data_stack_current = *(af_cell_t*)(*task->data_stack_current);
  AF_ADVANCE_IP(task, 1);
}

/* ! primitive */
void af_prim_store(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  *(af_cell_t*)(*task->data_stack_current) = *(task->data_stack_current + 1);
  task->data_stack_current += 2;
  AF_ADVANCE_IP(task, 1);
}

/* +! primitive */
void af_prim_add_store(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  *(af_cell_t*)(*task->data_stack_current) +=
    *(task->data_stack_current + 1);
  task->data_stack_current += 2;
  AF_ADVANCE_IP(task, 1);
}

/* C@ primitive */
void af_prim_c_fetch(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  *task->data_stack_current = *(af_byte_t*)(*task->data_stack_current);
  AF_ADVANCE_IP(task, 1);
}

/* C! primitive */
void af_prim_c_store(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  *(af_byte_t*)(*task->data_stack_current) =
    *(task->data_stack_current + 1) & 0xFF;
  task->data_stack_current += 2;
  AF_ADVANCE_IP(task, 1);
}

/* >R primitive */
void af_prim_to_r(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  AF_VERIFY_RETURN_STACK_EXPAND(global, task, 1);
  *(af_cell_t*)(--task->return_stack_current) = *task->data_stack_current++;
  AF_ADVANCE_IP(task, 1);
}

/* R> primitive */
void af_prim_from_r(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  AF_VERIFY_RETURN_STACK_READ(global, task, 1);
  *(--task->data_stack_current) =
    *(af_cell_t*)(task->return_stack_current++);
  AF_ADVANCE_IP(task, 1);
}

/* R@ primitive */
void af_prim_r_fetch(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  AF_VERIFY_RETURN_STACK_READ(global, task, 1);
  *(--task->data_stack_current) =
    *(af_cell_t*)(task->return_stack_current);
  AF_ADVANCE_IP(task, 1);
}

/* HERE primitive */
void af_prim_here(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)task->data_space_current;
  AF_ADVANCE_IP(task, 1);
}

/* DOES> primitive */
void af_prim_does(af_global_t* global, af_task_t* task) {
  AF_VERIFY_WORD_CREATED(global, task);
  AF_VERIFY_NOT_INTERACTIVE(global, task);
  AF_VERIFY_RETURN_STACK_READ(global, task, 1);
  task->most_recent_word->code = af_prim_do_does;
  task->most_recent_word->secondary = task->interpreter_pointer + 1;
  task->interpreter_pointer = *task->return_stack_current++;
}

/* >BODY primitive */
void af_prim_to_body(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  *task->data_stack_current =
    (af_cell_t)(((af_word_t*)(*task->data_stack_current))->data);
  AF_ADVANCE_IP(task, 1);
}

/* SP@ primitive */
void af_prim_sp_fetch(af_global_t* global, af_task_t* task) {
  af_cell_t* addr;
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  addr = task->data_stack_current;
  *(--task->data_stack_current) = (af_cell_t)addr;
  AF_ADVANCE_IP(task, 1);
}

/* SP0 primitive */
void af_prim_sp0(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)task->data_stack_base;
  AF_ADVANCE_IP(task, 1);
}

/* LITTLE-ENDIAN primitive */
void af_prim_little_endian(af_global_t* global, af_task_t* task) {
  af_cell_t value = 1;
  AF_VERIFY_DATA_STACK_EXPAND(global, task,  1);
  *(--task->data_stack_current) = *(af_byte_t*)(&value) ? TRUE : FALSE;
  AF_ADVANCE_IP(task, 1);
}

/* ' primitve */
void af_prim_tick(af_global_t* global, af_task_t* task) {
  af_byte_t* name;
  af_cell_t name_length;
  af_word_t* word;
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  if(!af_parse_name_available(global, task)) {
    af_handle_word_expected(global, task);
    return;
  }
  name = af_parse_name(global, task, &name_length);

  /*af_byte_t* buffer = malloc(name_length + 1);
  memcpy(buffer, name, name_length);
  buffer[name_length] = 0;
  fprintf(stderr, "Got name: \"%s\"\n", buffer);
  free(buffer);*/

  if(!(word = af_lookup(global, name, name_length))) {
    af_handle_word_not_found(global, task);
    return;
  }
  *(--task->data_stack_current) = (af_cell_t)word;
  AF_ADVANCE_IP(task, 1);
}

/* ['] primitive - immediate */
void af_prim_bracket_tick(af_global_t* global, af_task_t* task) {
  af_byte_t* name;
  af_cell_t name_length;
  af_word_t* word;
  af_compiled_t* code;
  AF_VERIFY_COMPILING(global, task);
  AF_VERIFY_WORD_CREATED(global, task);
  if(!af_parse_name_available(global, task)) {
    af_handle_word_expected(global, task);
    return;
  }
  if(!(code = af_allot(global, task, sizeof(af_compiled_t) * 2))) {
    return;
  }
  name = af_parse_name(global, task, &name_length);
  if(!(word = af_lookup(global, name, name_length))) {
    af_handle_word_not_found(global, task);
    return;
  }
  code->compiled_call = global->builtin_literal_runtime;
  (code + 1)->compiled_cell = (af_cell_t)word;
  AF_ADVANCE_IP(task, 1);
}

/* EXECUTE primitive */
void af_prim_execute(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  AF_WORD_EXECUTE(global, task, (af_word_t*)(*task->data_stack_current++));
}

/* STATE primitive */
void af_prim_state(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)(&task->is_compiling);
  AF_ADVANCE_IP(task, 1);
}

/* LATESTXT primitive */
void af_prim_latestxt(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)task->most_recent_word;
  AF_ADVANCE_IP(task, 1);
}

/* >IN primitive */
void af_prim_to_in(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)(&task->current_input->index);
  AF_ADVANCE_IP(task, 1);
}

/* SOURCE primitive */
void af_prim_source(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 2);
  *(--task->data_stack_current) = (af_cell_t)task->current_input->buffer;
  *(--task->data_stack_current) = task->current_input->count;
  AF_ADVANCE_IP(task, 1);
}

/* >INPUT primitive */
void af_prim_to_input(af_global_t* global, af_task_t* task) {
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  target_task = (af_task_t*)(*task->data_stack_current++);
  target_task->current_input = (af_input_t*)(*task->data_stack_current++);
  AF_ADVANCE_IP(task, 1);
}

/* INPUT> primitive */
void af_prim_from_input(af_global_t* global, af_task_t* task) {
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  target_task = (af_task_t*)(*task->data_stack_current);
  *task->data_stack_current = (af_cell_t)target_task->current_input;
  AF_ADVANCE_IP(task, 1);
}

/* >OUTPUT primitive */
void af_prim_to_output(af_global_t* global, af_task_t* task) {
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  target_task = (af_task_t*)(*task->data_stack_current++);
  target_task->current_output = (af_output_t*)(*task->data_stack_current++);
  AF_ADVANCE_IP(task, 1);
}

/* OUTPUT> primitive */
void af_prim_from_output(af_global_t* global, af_task_t* task) {
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  target_task = (af_task_t*)(*task->data_stack_current);
  *task->data_stack_current = (af_cell_t)target_task->current_output;
  AF_ADVANCE_IP(task, 1);
}

/* >ERROR primitive */
void af_prim_to_error(af_global_t* global, af_task_t* task) {
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  target_task = (af_task_t*)(*task->data_stack_current++);
  target_task->current_error = (af_output_t*)(*task->data_stack_current++);
  AF_ADVANCE_IP(task, 1);
}

/* ERROR> primitive */
void af_prim_from_error(af_global_t* global, af_task_t* task) {
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  target_task = (af_task_t*)(*task->data_stack_current);
  *task->data_stack_current = (af_cell_t)target_task->current_error;
  AF_ADVANCE_IP(task, 1);
}

/* >DATA primitive */
void af_prim_to_data(af_global_t* global, af_task_t* task) {
  af_task_t* target_task;
  af_cell_t data;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  target_task = (af_task_t*)(*task->data_stack_current++);
  data = *task->data_stack_current++;
  af_push_data(global, target_task, data);
  AF_ADVANCE_IP(task, 1);
}

/* >RETURN primitive */
void af_prim_to_return(af_global_t* global, af_task_t* task) {
  af_task_t* target_task;
  af_compiled_t* pointer;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  target_task = (af_task_t*)(*task->data_stack_current++);
  pointer = (af_compiled_t*)(*task->data_stack_current++);
  af_push_return(global, target_task, pointer);
  AF_ADVANCE_IP(task, 1);
}

/* >ABORT primitive */
void af_prim_to_abort(af_global_t* global, af_task_t* task) {
  af_task_t* target_task;
  af_word_t* word;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  target_task = (af_task_t*)(*task->data_stack_current++);
  target_task->abort = (af_word_t*)(*task->data_stack_current++);
  AF_ADVANCE_IP(task, 1);
}

/* ABORT> primitive */
void af_prim_from_abort(af_global_t* global, af_task_t* task) {
  af_task_t* target_task;
  af_word_t* word;
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  target_task = (af_task_t*)(*task->data_stack_current);
  *task->data_stack_current = (af_cell_t)target_task->abort;
  AF_ADVANCE_IP(task, 1);
}

/* >DEFAULT-ABORT primitive */
void af_prim_to_default_abort(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  global->default_abort = (af_word_t*)(*task->data_stack_current++);
  AF_ADVANCE_IP(task, 1);
}

/* DEFAULT-ABORT> primitive */
void af_prim_from_default_abort(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)global->default_abort;
  AF_ADVANCE_IP(task, 1);
}

/* ALLOCATE primitive */
void af_prim_allocate(af_global_t* global, af_task_t* task) {
  void* data;
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
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
void af_prim_free(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  free((void*)(*task->data_stack_current));
  *task->data_stack_current = 0;
  AF_ADVANCE_IP(task, 1);
}

/* RESIZE primitive */
void af_prim_resize(af_global_t* global, af_task_t* task) {
  void* data;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
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

/* ALIGN primitive */
void af_prim_align(af_global_t* global, af_task_t* task) {
  af_cell_t extra = (af_cell_t)task->data_space_current % sizeof(af_cell_t);
  if(extra) {
    if(!af_allot(global, task, sizeof(af_cell_t) - extra)) {
      return;
    }
  }
  AF_ADVANCE_IP(task, 1);
}

/* ALLOT primitive */
void af_prim_allot(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  if(!af_allot(global, task, (ssize_t)(*task->data_stack_current++))) {
    return;
  }
  AF_ADVANCE_IP(task, 1);
}

/* UNUSED primitive */
void af_prim_unused(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) =
    (af_cell_t)(task->data_stack_top - task->data_stack_current);
  AF_ADVANCE_IP(task, 1);
}

/* WORD primitive */
void af_prim_word(af_global_t* global, af_task_t* task) {
  af_byte_t delimiter;
  af_byte_t* text;
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  delimiter = (af_byte_t)(*task->data_stack_current & 0xFF);
  text = af_word(global, task, delimiter);
  *task->data_stack_current = (af_cell_t)text;
  AF_ADVANCE_IP(task, 1);
}

/* PARSE-NAME primitive */
void af_prim_parse_name(af_global_t* global, af_task_t* task) {
  af_byte_t* text;
  af_cell_t length;
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 2);
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
void af_prim_parse_number(af_global_t* global, af_task_t* task) {
  af_byte_t* text;
  af_cell_t length;
  af_sign_cell_t result = 0;
  af_bool_t success = TRUE;
  af_bool_t negative = FALSE;
  af_cell_t base = task->base;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  text = (af_byte_t*)(*(task->data_stack_current + 1));
  length = *task->data_stack_current;
  if(length > 0) {
    if(*text == '%') {
      base = 2;
      text++;
      length--;
    } else if(*text == '/') {
      base = 8;
      text++;
      length--;
    } else if(*text == '#') {
      base = 10;
      text++;
      length--;
    } else if(*text == '$') {
      base = 16;
      text++;
      length--;
    }
    if(length > 0 && base >= 2 && base <= 36) {
      if(*text == '-') {
	negative = TRUE;
	text++;
	length--;
      }
      if(length > 0) {
	while(length--) {
	  result *= base;
	  if(*text >= '0' && *text <= '9') {
	    if(*text - '0' < base) {
	      result += *text++ - '0';
	    } else {
	      result = 0;
	      success = FALSE;
	      break;
	    }
	  } else if(*text >= 'a' && *text <= 'z') {
	    if(*text - 'a' < base - 10) {
	      result += (*text++ - 'a') + 10;
	    } else {
	      result = 0;
	      success = FALSE;
	      break;
	    }
	  } else if(*text >= 'A' && *text <= 'Z') {
	    if(*text - 'A' < base - 10) {
	      result += (*text++ - 'A') + 10;
	    } else {
	      result = 0;
	      success = FALSE;
	      break;
	    }
	  } else {
	    result = 0;
	    success = FALSE;
	    break;
	  }
	}
	if(success && negative) {
	  result = -result;
	}
      } else {
	result = 0;
	success = FALSE;
      }
    } else {
      result = 0;
      success = FALSE;
    }
  } else {
    result = 0;
    success = FALSE;
  }
  *(task->data_stack_current + 1) = (af_cell_t)result;
  *task->data_stack_current = (af_cell_t)success;
  AF_ADVANCE_IP(task, 1);
}

/* FORMAT-NUMBER primitive */
void af_prim_format_number(af_global_t* global, af_task_t* task) {
  af_byte_t buffer[64];
  af_byte_t* current = buffer + sizeof(buffer);
  af_cell_t length = 0;
  af_cell_t value;
  af_bool_t negative = FALSE;
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  value = *task->data_stack_current;
  if(task->base == 10) {
    af_sign_cell_t signed_value = (af_sign_cell_t)value;
    if(signed_value < 0) {
      negative = TRUE;
      signed_value = -signed_value;
    }
    while(signed_value > 0) {
      af_byte_t part = signed_value % 10;
      part += '0';
      signed_value /= 10;
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

/* BASE primitive */
void af_prim_base(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)(&task->base);
  AF_ADVANCE_IP(task, 1);
}

/* FIND-WORD primitive */
void af_prim_find_word(af_global_t* global, af_task_t* task) {
  af_byte_t* text;
  af_cell_t length;
  af_word_t* word;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  text = (af_byte_t*)(*(task->data_stack_current + 1));
  length = *task->data_stack_current;
  word = af_lookup(global, text, length);
  if(!word) {
    AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
    *(--task->data_stack_current) = 0;
  } else {
    *(task->data_stack_current + 1) = (af_cell_t)word;
    *task->data_stack_current = word->is_immediate ? 1 : (af_cell_t)(-1);
  }
  AF_ADVANCE_IP(task, 1);
}

/* CAS primitive */
void af_prim_cas(af_global_t* global, af_task_t* task) {
  af_cell_t* addr;
  AF_VERIFY_DATA_STACK_READ(global, task, 3);
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
void af_prim_2cas(af_global_t* global, af_task_t* task) {
  af_cell_t* addr;
  AF_VERIFY_DATA_STACK_READ(global, task, 5);
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
void af_prim_this_task(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)task;
  AF_ADVANCE_IP(task, 1);
}

/* SPAWN primitive */
void af_prim_spawn(af_global_t* global, af_task_t* task) {
  af_task_t* new_task;
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  if(!(new_task = af_spawn(global))) {
    return;
  }
  *(--task->data_stack_current) = (af_cell_t)new_task;
  AF_ADVANCE_IP(task, 1);
}

/* SPAWN-NO-DATA primitive */
void af_prim_spawn_no_data(af_global_t* global, af_task_t* task) {
  af_task_t* new_task;
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  if(!(new_task = af_spawn_no_data(global))) {
    return;
  }
  *(--task->data_stack_current) = (af_cell_t)new_task;
  AF_ADVANCE_IP(task, 1);
}

/* >FREE-DATA-ON-EXIT primitive */
void af_prim_free_data_on_exit(af_global_t* global, af_task_t* task) {
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  target_task = (af_task_t*)(*task->data_stack_current++);
  target_task->free_data_on_exit = (af_bool_t)(*task->data_stack_current++);
  AF_ADVANCE_IP(task, 1);
}

/* >CONSOLE-INPUT primitive */
void af_prim_to_console_input(af_global_t* global, af_task_t* task) {
  af_task_t* target_task;
  af_input_t* current_input;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  target_task = (af_task_t*)(*task->data_stack_current++);
  target_task->console_input = (af_input_t*)(*task->data_stack_current++);
  current_input = target_task->current_input;
  while(current_input && current_input->next_input) {
    current_input = current_input->next_input;
  }
  if(current_input) {
    current_input->next_input = target_task->console_input;
  } else {
    target_task->current_input = target_task->console_input;
  }
  AF_ADVANCE_IP(task, 1);
}

/* >CONSOLE-OUTPUT primitive */
void af_prim_to_console_output(af_global_t* global, af_task_t* task) {
  af_task_t* target_task;
  af_output_t* current_output;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  target_task = (af_task_t*)(*task->data_stack_current++);
  target_task->console_output = (af_output_t*)(*task->data_stack_current++);
  current_output = target_task->current_output;
  while(current_output && current_output->next_output) {
    current_output = current_output->next_output;
  }
  if(current_output) {
    current_output->next_output = target_task->console_output;
  } else {
    target_task->current_output = target_task->console_output;
  }
  AF_ADVANCE_IP(task, 1);
}

/* >CONSOLE-ERROR primitive */
void af_prim_to_console_error(af_global_t* global, af_task_t* task) {
  af_task_t* target_task;
  af_output_t* current_error;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  target_task = (af_task_t*)(*task->data_stack_current++);
  target_task->console_error = (af_output_t*)(*task->data_stack_current++);
  current_error = target_task->current_error;
  while(current_error && current_error->next_output) {
    current_error = current_error->next_output;
  }
  if(current_error) {
    current_error->next_output = target_task->console_error;
  } else {
    target_task->current_error = target_task->console_error;
  }
  AF_ADVANCE_IP(task, 1);
}

/* CONSOLE-INPUT> primitive */
void af_prim_from_console_input(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  *task->data_stack_current =
    (af_cell_t)((af_task_t*)(*task->data_stack_current))->console_input;
  AF_ADVANCE_IP(task, 1);
}

/* CONSOLE-OUTPUT> primitive */
void af_prim_from_console_output(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  *task->data_stack_current =
    (af_cell_t)((af_task_t*)(*task->data_stack_current))->console_output;
  AF_ADVANCE_IP(task, 1);
}

/* CONSOLE-ERROR> primitive */
void af_prim_from_console_error(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  *task->data_stack_current =
    (af_cell_t)((af_task_t*)(*task->data_stack_current))->console_error;
  AF_ADVANCE_IP(task, 1);
}

/* >INIT-WORD primitive */
void af_prim_to_init_word(af_global_t* global, af_task_t* task) {
  af_task_t* target_task;
  af_word_t* word;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  target_task = (af_task_t*)(*task->data_stack_current++);
  word = (af_word_t*)(*task->data_stack_current++);
  af_set_init_word(global, target_task, word);
  AF_ADVANCE_IP(task, 1);
}

/* START primitive */
void af_prim_start(af_global_t* global, af_task_t* task) {
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  target_task = (af_task_t*)(*task->data_stack_current++);
  af_start(global, target_task);
  AF_ADVANCE_IP(task, 1);
}

/* KILL primitive */
void af_prim_kill(af_global_t* global, af_task_t* task) {
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  target_task = (af_task_t*)(*task->data_stack_current++);
  af_kill(global, target_task);
  AF_ADVANCE_IP(task, 1);
}

/* YIELD primitive */
void af_prim_yield(af_global_t* global, af_task_t* task) {
  af_yield(global, task);
  AF_ADVANCE_IP(task, 1);
}

/* WAIT primitive */
void af_prim_wait(af_global_t* global, af_task_t* task) {
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  target_task = (af_task_t*)(*task->data_stack_current++);
  af_wait(global, target_task);
  AF_ADVANCE_IP(task, 1);
}

/* WAKE primitive */
void af_prim_wake(af_global_t* global, af_task_t* task) {
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  target_task = (af_task_t*)(*task->data_stack_current++);
  af_wake(global, target_task);
  AF_ADVANCE_IP(task, 1);
}

/* RESET primitive */
void af_prim_reset(af_global_t* global, af_task_t* task) {
  af_task_t* target_task;
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  target_task = (af_task_t*)(*task->data_stack_current++);
  af_reset(global, target_task);
  AF_ADVANCE_IP(task, 1);
}

/* [ primitive - immediate */
void af_prim_open_bracket(af_global_t* global, af_task_t* task) {
  AF_VERIFY_COMPILING(global, task);
  task->is_compiling = FALSE;
  AF_ADVANCE_IP(task, 1);
}

/* ] primitive */
void af_prim_close_bracket(af_global_t* global, af_task_t* task) {
  AF_VERIFY_WORD_CREATED(global, task);
  task->is_compiling = TRUE;
  AF_ADVANCE_IP(task, 1);
}

/* BRANCH primitive */
void af_prim_branch(af_global_t* global, af_task_t* task) {
  if(task->interpreter_pointer) {
    task->interpreter_pointer =
      (af_compiled_t*)((task->interpreter_pointer + 1)->compiled_cell);
  } else {
    af_handle_not_interactive(global, task);
  }
}

/* ?BRANCH primitive */
void af_prim_cond_branch(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
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
void af_prim_move(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 3);
  memmove((void*)(*(task->data_stack_current + 1)),
	  (void*)(*(task->data_stack_current + 2)),
	  (size_t)(*task->data_stack_current));
  task->data_stack_current += 3;
  AF_ADVANCE_IP(task, 1);
}

/* DEPTH primitive */
void af_prim_depth(af_global_t* global, af_task_t* task) {
  af_cell_t count;
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  count = task->data_stack_base - task->data_stack_current;
  *(--task->data_stack_current) = count;
  AF_ADVANCE_IP(task, 1);
}

/* CELL-SIZE primitive */
void af_prim_cell_size(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)sizeof(af_cell_t);
  AF_ADVANCE_IP(task, 1);
}

/* REFILL primitive */
void af_prim_refill(af_global_t* global, af_task_t* task) {
  af_refill(global, task);
  if(!task->current_input || (task->current_input->is_closed &&
				!task->current_input->count)) {
    AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
    *(--task->data_stack_current) = (af_cell_t)FALSE;
  } else {
    AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
    *(--task->data_stack_current) = (af_cell_t)TRUE;
  }
}

/* IO-ACTION-DESTROY primitive */
void af_prim_io_action_destroy(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  af_io_action_destroy((af_io_action_t*)(*task->data_stack_current++));
  AF_ADVANCE_IP(task, 1);
}

/* IO-ACTION-GET-STATE primitive */
void af_prim_io_action_get_state(af_global_t* global, af_task_t* task) {
  af_io_state_t* state;
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  if(!(state = malloc(sizeof(af_io_state_t)))) {
    af_handle_out_of_memory(global, task);
    return;
  }
  af_io_action_get_state((af_io_action_t*)(*task->data_stack_current), state);
  *task->data_stack_current = (af_cell_t)state;
  AF_ADVANCE_IP(task, 1);
}

/* IO-STATE-DESTROY primitive */
void af_prim_io_state_destroy(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  free((void*)(*task->data_stack_current++));
  AF_ADVANCE_IP(task, 1);
}

/* IO-STATE-IS-DONE primitive */
void af_prim_io_state_is_done(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  *(af_bool_t*)task->data_stack_current =
    af_io_state_is_done((af_io_state_t*)(*task->data_stack_current));
  AF_ADVANCE_IP(task, 1);
}

/* IO-STATE-IS-CLOSED primitive */
void af_prim_io_state_is_closed(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  *(af_bool_t*)task->data_stack_current =
    af_io_state_is_closed((af_io_state_t*)(*task->data_stack_current));
  AF_ADVANCE_IP(task, 1);
}

/* IO-STATE-HAS-HANGUP primitive */
void af_prim_io_state_has_hangup(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  *(af_bool_t*)task->data_stack_current =
    af_io_state_has_hangup((af_io_state_t*)(*task->data_stack_current));
  AF_ADVANCE_IP(task, 1);
}

/* IO-STATE-HAS-ERROR primitive */
void af_prim_io_state_has_error(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  *(af_bool_t*)task->data_stack_current =
    af_io_state_has_error((af_io_state_t*)(*task->data_stack_current));
  AF_ADVANCE_IP(task, 1);
}

/* IO-STATE-GET-BUFFER primitive */
void af_prim_io_state_get_buffer(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  *(af_byte_t**)task->data_stack_current =
    af_io_state_get_buffer((af_io_state_t*)(*task->data_stack_current));
  AF_ADVANCE_IP(task, 1);
}

/* IO-STATE-GET-INDEX primitive */
void af_prim_io_state_get_index(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  *task->data_stack_current =
    af_io_state_get_index((af_io_state_t*)(*task->data_stack_current));
  AF_ADVANCE_IP(task, 1);
}

/* IO-STATE-GET-COUNT primitive */
void af_prim_io_state_get_count(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  *task->data_stack_current =
    af_io_state_get_count((af_io_state_t*)(*task->data_stack_current));
  AF_ADVANCE_IP(task, 1);
}

/* IO-OPEN primitive */
void af_prim_io_open(af_global_t* global, af_task_t* task) {
  af_io_fd_t fd;
  af_io_error_t error;
  AF_VERIFY_DATA_STACK_READ(global, task, 4);
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
void af_prim_io_pipe(af_global_t* global, af_task_t* task) {
  af_io_fd_t in;
  af_io_fd_t out;
  af_io_error_t error;
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 3);
  af_io_pipe(&in, &out, &error);
  *(--task->data_stack_current) = (af_cell_t)in;
  *(--task->data_stack_current) = (af_cell_t)out;
  *(--task->data_stack_current) = (af_cell_t)error;
  AF_ADVANCE_IP(task, 1);
}

/* IO-GET-MONOTONIC-TIME primitive */
void af_prim_io_get_monotonic_time(af_global_t* global, af_task_t* task) {
  af_time_t monotonic_time;
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 2);
  af_io_get_monotonic_time(&monotonic_time);
  *(--task->data_stack_current) = monotonic_time.sec;
  *(--task->data_stack_current) = monotonic_time.nsec;
  AF_ADVANCE_IP(task, 1);
}

/* IO-SLEEP primitive */
void af_prim_io_sleep(af_global_t* global, af_task_t* task) {
  af_time_t sleep_until;
  af_io_action_t* action;
  AF_VERIFY_DATA_STACK_READ(global, task, 2);
  sleep_until.nsec = *task->data_stack_current++;
  sleep_until.sec = *task->data_stack_current++;
  action = af_io_sleep(&global->io, &sleep_until, task);
  af_io_action_destroy(action);
  AF_ADVANCE_IP(task, 1);
  af_wait(global, task);
}

/* IO-CLOSE-BLOCK primitive */
void af_prim_io_close_block(af_global_t* global, af_task_t* task) {
  af_io_action_t* action;
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  action = af_io_close_block(&global->io,
			     (af_io_fd_t)(*task->data_stack_current),
			     task);
  *task->data_stack_current = (af_cell_t)action;
  AF_ADVANCE_IP(task, 1);
  af_wait(global, task);
}

/* IO-CLOSE-ASYNC primitive */
void af_prim_io_close_async(af_global_t* global, af_task_t* task) {
  af_io_action_t* action;
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  action = af_io_close_async(&global->io,
			     (af_io_fd_t)(*task->data_stack_current));
  *task->data_stack_current = (af_cell_t)action;
  AF_ADVANCE_IP(task, 1);
}

/* IO-SEEK-BLOCK primitive */
void af_prim_io_seek_block(af_global_t* global, af_task_t* task) {
  af_io_action_t* action;
  af_io_fd_t fd;
  af_io_off_t offset;
  af_io_whence_t whence;
  AF_VERIFY_DATA_STACK_READ(global, task, 3);
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
void af_prim_io_seek_async(af_global_t* global, af_task_t* task) {
  af_io_action_t* action;
  af_io_fd_t fd;
  af_io_off_t offset;
  af_io_whence_t whence;
  AF_VERIFY_DATA_STACK_READ(global, task, 3);
  fd = *task->data_stack_current;
  whence = *(task->data_stack_current + 1);
  offset = *(af_sign_cell_t*)(task->data_stack_current + 2);
  action = af_io_seek_async(&global->io, fd, offset, whence);
  task->data_stack_current += 2;
  *task->data_stack_current = (af_cell_t)action;
  AF_ADVANCE_IP(task, 1);
}

/* IO-READ-BLOCK primitive */
void af_prim_io_read_block(af_global_t* global, af_task_t* task) {
  af_io_action_t* action;
  AF_VERIFY_DATA_STACK_READ(global, task, 3);
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
void af_prim_io_write_block(af_global_t* global, af_task_t* task) {
  af_io_action_t* action;
  AF_VERIFY_DATA_STACK_READ(global, task, 3);
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
void af_prim_io_read_async(af_global_t* global, af_task_t* task) {
  af_io_action_t* action;
  AF_VERIFY_DATA_STACK_READ(global, task, 3);
  action = af_io_read_async(&global->io,
			    (af_io_fd_t)(*task->data_stack_current),
			    (af_byte_t*)(*(task->data_stack_current + 2)),
			    *(task->data_stack_current + 1));
  task->data_stack_current += 2;
  *task->data_stack_current = (af_cell_t)action;
  AF_ADVANCE_IP(task, 1);
}

/* IO-WRITE-ASYNC primitive */
void af_prim_io_write_async(af_global_t* global, af_task_t* task) {
  af_io_action_t* action;
  AF_VERIFY_DATA_STACK_READ(global, task, 3);
  action = af_io_write_async(&global->io,
			     (af_io_fd_t)(*task->data_stack_current),
			     (af_byte_t*)(*(task->data_stack_current + 2)),
			     *(task->data_stack_current + 1));
  task->data_stack_current += 2;
  *task->data_stack_current = (af_cell_t)action;
  AF_ADVANCE_IP(task, 1);
}

/* IO-READ-NONBLOCK primitive */
void af_prim_io_read_nonblock(af_global_t* global, af_task_t* task) {
  af_bool_t again;
  ssize_t size;
  AF_VERIFY_DATA_STACK_READ(global, task, 3);
  size = af_io_read_nonblock((af_io_fd_t)(*task->data_stack_current),
			     (af_byte_t*)(*(task->data_stack_current + 2)),
			     *(task->data_stack_current + 1),
			     &again);
  task->data_stack_current++;
  *(af_sign_cell_t*)(task->data_stack_current + 1) = (af_sign_cell_t)size;
  *(af_bool_t*)task->data_stack_current = again;
  AF_ADVANCE_IP(task, 1);
}

/* IO-WRITE-NONBLOCK primitive */
void af_prim_io_write_nonblock(af_global_t* global, af_task_t* task) {
  af_bool_t again;
  ssize_t size;
  AF_VERIFY_DATA_STACK_READ(global, task, 3);
  size = af_io_write_nonblock((af_io_fd_t)(*task->data_stack_current),
			      (af_byte_t*)(*(task->data_stack_current + 2)),
			      *(task->data_stack_current + 1),
			      &again);
  task->data_stack_current++;
  *(af_sign_cell_t*)(task->data_stack_current + 1) = (af_sign_cell_t)size;
  *(af_bool_t*)task->data_stack_current = again;
  AF_ADVANCE_IP(task, 1);
}

/* IO-STDIN primitive */
void af_prim_io_stdin(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_STDIN;
  AF_ADVANCE_IP(task, 1);
}

/* IO-STDOUT primitive */
void af_prim_io_stdout(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_STDOUT;
  AF_ADVANCE_IP(task, 1);
}

/* IO-STDERR primitive */
void af_prim_io_stderr(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_STDERR;
  AF_ADVANCE_IP(task, 1);
}

/* IO-RDONLY primitive */
void af_prim_io_rdonly(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_RDONLY;
  AF_ADVANCE_IP(task, 1);
}

/* IO-WRONLY primitive */
void af_prim_io_wronly(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_WRONLY;
  AF_ADVANCE_IP(task, 1);
}

/* IO-RDWR primitive */
void af_prim_io_rdwr(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_RDWR;
  AF_ADVANCE_IP(task, 1);
}

/* IO-APPEND primitive */
void af_prim_io_append(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_APPEND;
  AF_ADVANCE_IP(task, 1);
}

/* IO-TRUNC primitive */
void af_prim_io_trunc(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_TRUNC;
  AF_ADVANCE_IP(task, 1);
}

/* IO-CREAT primitive */
void af_prim_io_creat(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_CREAT;
  AF_ADVANCE_IP(task, 1);
}

/* IO-EXCL primitive */
void af_prim_io_excl(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_EXCL;
  AF_ADVANCE_IP(task, 1);
}

/* IO-IRWXU primitive */
void af_prim_io_irwxu(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_IRWXU;
  AF_ADVANCE_IP(task, 1);
}

/* IO-IRUSR primitive */
void af_prim_io_irusr(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_IRUSR;
  AF_ADVANCE_IP(task, 1);
}

/* IO-IWUSR primitive */
void af_prim_io_iwusr(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_IWUSR;
  AF_ADVANCE_IP(task, 1);
}

/* IO-IXUSR primitive */
void af_prim_io_ixusr(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_IXUSR;
  AF_ADVANCE_IP(task, 1);
}

/* IO-IRWXG primitive */
void af_prim_io_irwxg(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_IRWXG;
  AF_ADVANCE_IP(task, 1);
}

/* IO-IRGRP primitive */
void af_prim_io_irgrp(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_IRGRP;
  AF_ADVANCE_IP(task, 1);
}

/* IO-IWGRP primitive */
void af_prim_io_iwgrp(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_IWGRP;
  AF_ADVANCE_IP(task, 1);
}

/* IO-IXGRP primitive */
void af_prim_io_ixgrp(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_IXGRP;
  AF_ADVANCE_IP(task, 1);
}

/* IO-IRWXO primitive */
void af_prim_io_irwxo(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_IRWXO;
  AF_ADVANCE_IP(task, 1);
}

/* IO-IROTH primitive */
void af_prim_io_iroth(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_IROTH;
  AF_ADVANCE_IP(task, 1);
}

/* IO-IWOTH primitive */
void af_prim_io_iwoth(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_IWOTH;
  AF_ADVANCE_IP(task, 1);
}

/* IO-IXOTH primitive */
void af_prim_io_ixoth(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_IXOTH;
  AF_ADVANCE_IP(task, 1);
}

/* IO-ISUID primitive */
void af_prim_io_isuid(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_ISUID;
  AF_ADVANCE_IP(task, 1);
}

/* IO-ISGID primitive */
void af_prim_io_isgid(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_ISGID;
  AF_ADVANCE_IP(task, 1);
}

/* IO-ISVTX primitive */
void af_prim_io_isvtx(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_ISVTX;
  AF_ADVANCE_IP(task, 1);
}

/* IO-SEEK-SET primitive */
void af_prim_io_seek_set(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_SEEK_SET;
  AF_ADVANCE_IP(task, 1);
}

/* IO-SEEK-CUR primitive */
void af_prim_io_seek_cur(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_SEEK_CUR;
  AF_ADVANCE_IP(task, 1);
}

/* IO-SEEK-END primitive */
void af_prim_io_seek_end(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)AF_IO_SEEK_END;
  AF_ADVANCE_IP(task, 1);
}


/* INPUT-SIZE primitive */
void af_prim_input_size(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)sizeof(af_input_t);
  AF_ADVANCE_IP(task, 1);
}

/* INPUT-NEXT-INPUT primitive */
void af_prim_input_next_input(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  *task->data_stack_current =
    (af_cell_t)(&((af_input_t*)(*task->data_stack_current))->next_input);
  AF_ADVANCE_IP(task, 1);
}

/* INPUT-BUFFER primitive */
void af_prim_input_buffer(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  *task->data_stack_current =
    (af_cell_t)(&((af_input_t*)(*task->data_stack_current))->buffer);
  AF_ADVANCE_IP(task, 1);
}

/* INPUT-COUNT primitive */
void af_prim_input_count(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  *task->data_stack_current =
    (af_cell_t)(&((af_input_t*)(*task->data_stack_current))->count);
  AF_ADVANCE_IP(task, 1);
}

/* INPUT-INDEX primitive */
void af_prim_input_index(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  *task->data_stack_current =
    (af_cell_t)(&((af_input_t*)(*task->data_stack_current))->index);
  AF_ADVANCE_IP(task, 1);
}

/* INPUT-IS-CLOSED primitive */
void af_prim_input_is_closed(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  *task->data_stack_current =
    (af_cell_t)(&((af_input_t*)(*task->data_stack_current))->is_closed);
  AF_ADVANCE_IP(task, 1);
}

/* INPUT-CLEANUP primitive */
void af_prim_input_cleanup(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  *task->data_stack_current =
    (af_cell_t)(&((af_input_t*)(*task->data_stack_current))->cleanup);
  AF_ADVANCE_IP(task, 1);
}

/* INPUT-REFILL primitive */
void af_prim_input_refill(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  *task->data_stack_current =
    (af_cell_t)(&((af_input_t*)(*task->data_stack_current))->refill);
  AF_ADVANCE_IP(task, 1);
}

/* INPUT-ARG primitive */
void af_prim_input_arg(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  *task->data_stack_current =
    (af_cell_t)(&((af_input_t*)(*task->data_stack_current))->arg);
  AF_ADVANCE_IP(task, 1);
}

/* OUTPUT-SIZE primitive */
void af_prim_output_size(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = (af_cell_t)sizeof(af_output_t);
  AF_ADVANCE_IP(task, 1);
}

/* OUTPUT-NEXT-OUTPUT primitive */
void af_prim_output_next_output(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  *task->data_stack_current =
    (af_cell_t)(&((af_output_t*)(*task->data_stack_current))->next_output);
  AF_ADVANCE_IP(task, 1);
}

/* OUTPUT-WRITE primitive */
void af_prim_output_write(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  *task->data_stack_current =
    (af_cell_t)(&((af_output_t*)(*task->data_stack_current))->write);
  AF_ADVANCE_IP(task, 1);
}

/* OUTPUT-CLEANUP primitive */
void af_prim_output_cleanup(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  *task->data_stack_current =
    (af_cell_t)(&((af_output_t*)(*task->data_stack_current))->cleanup);
  AF_ADVANCE_IP(task, 1);
}

/* OUTPUT-ARG primitive */
void af_prim_output_arg(af_global_t* global, af_task_t* task) {
  AF_VERIFY_DATA_STACK_READ(global, task, 1);
  *task->data_stack_current =
    (af_cell_t)(&((af_output_t*)(*task->data_stack_current))->arg);
  AF_ADVANCE_IP(task, 1);
}

/* EMPTY-DATA-STACK primitive */
void af_prim_empty_data_stack(af_global_t* global, af_task_t* task) {
  task->data_stack_current = task->data_stack_base;
  AF_ADVANCE_IP(task, 1);
}

/* EMPTY-RETURN-STACK primitive */
void af_prim_empty_return_stack(af_global_t* global, af_task_t* task) {
  task->return_stack_current = task->return_stack_base;
  AF_ADVANCE_IP(task, 1);
}
