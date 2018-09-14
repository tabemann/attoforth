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

void af_compile_builtin(af_global_t* global, af_thread_t* thread) {
  af_input_t* input;
  uint8_t* code =
    ": TRUE -1 ; "
    ": FALSE 0 ; "
    ": NEGATE -1 * ; "
    ": 0< 0 < ; "
    ": 0= 0 = ; "
    ": 0> 0 > ; "
    ": 1+ 1 + ; "
    ": 1- 1 - ; "
    ": 2+ 2 + ; "
    ": 2- 2 - ; "
    ": 2* 2 * ; "
    ": 2/ 2 * ; "
    ": CELL+ CELL-SIZE + ; "
    ": CELLS CELL-SIZE * ; "
    ": COMPILE, , ; "
    ": LITERAL POSTPONE (LITERAL) , ; IMMEDIATE "
    ": VARIABLE CREATE 0 , ; "
    ": CONSTANT CREATE , DOES> @ ; "
    ": IF POSTPONE 0BRANCH HERE 0 , ; IMMEDIATE "
    ": ELSE POSTPONE BRANCH HERE 0 , SWAP HERE SWAP ! ; IMMEDIATE "
    ": THEN HERE SWAP ! ; IMMEDIATE "
    ": ENDIF POSTPONE THEN ; IMMEDIATE "
    ": DEFER CREATE ['] ABORT , DOES> @ EXECUTE ; "
    ": DEFER! >BODY ! ; "
    ": DEFER@ >BODY @ ; "
    ": ACTION-OF STATE @ IF POSTPONE ['] POSTPONE DEFER@ ELSE ' DEFER@ THEN ; "
    "  IMMEDIATE "
    ": IS STATE @ IF POSTPONE ['] POSTPONE DEFER! ELSE ' DEFER! THEN ; "
    "  IMMEDIATE "
    ": BEGIN HERE ; IMMEDIATE "
    ": AGAIN POSTPONE BRANCH , ; IMMEDIATE "
    ": UNTIL POSTPONE 0BRANCH , ; IMMEDIATE "
    ": WHILE POSTPONE 0BRANCH HERE 0 , ; IMMEDIATE "
    ": REPEAT POSTPONE BRANCH SWAP , HERE SWAP ! ; IMMEDIATE "
    ": DO POSTPONE (LITERAL) HERE DUP 0 , POSTPONE >R POSTPONE 2>R HERE ; "
    "  IMMEDIATE "
    ": ?DO POSTPONE (LITERAL) HERE 0 , POSTPONE >R POSTPONE 2DUP POSTPONE 2>R "
    "  POSTPONE > POSTPONE 0BRANCH HERE 0 , HERE ; IMMEDIATE "
    ": LOOP POSTPONE 2R> POSTPONE 1+ POSTPONE 2DUP POSTPONE 2>R POSTPONE <= "
    "  POSTPONE 0BRANCH , HERE SWAP ! HERE SWAP ! POSTPONE 2R> POSTPONE 2DROP "
    "  POSTPONE R> POSTPONE DROP ; IMMEDIATE "
    ": +LOOP POSTPONE 2R> POSTPONE ROT POSTPONE + POSTPONE 2DUP POSTPONE 2>R "
    "  POSTPONE <= POSTPONE 0BRANCH , HERE SWAP ! HERE SWAP ! POSTPONE 2R> "
    "  POSTPONE 2DROP POSTPONE R> POSTPONE DROP ; IMMEDIATE "
    ": LEAVE 2R> 2DROP EXIT ; "
    ": UNLOOP 2R> 2DROP R> DROP ; "
    ": I R@ ; "
    ": J 2R> R> R@ SWAP >R ROT ROT 2>R ; "
    ": BUFFER: CREATE ALLOT ; "
    ": CHAR PARSE-NAME 1 = IF @ ELSE 0 THEN ; "
    ": [CHAR] PARSE-NAME 1 = IF @ ELSE 0 THEN POSTPONE (LITERAL) , THEN ; "
    "  IMMEDIATE "
    ": BL 32 ; "
    ": NEWLINE 10 ; "
    ": SKIP-WHITESPACE BEGIN SOURCE >IN 2DUP @ > IF "
    "    DUP @ 3 PICK + @ DUP BL = OVER NEWLINE = OR IF "
    "      DUP @ 1+ SWAP ! 2DROP FALSE "
    "    ELSE 2DROP DROP TRUE THEN "
    "  ELSE 2DROP DROP TRUE THEN UNTIL ; "
    ": SKIP-CHARS-UNTIL SOURCE >IN BEGIN 2DUP @ > IF "
    "    DUP @ 3 PICK + @ 4 PICK = OVER DUP @ 1+ SWAP ! "
    "  ELSE TRUE THEN UNTIL 2DROP 2DROP ; "
    ": COUNT-CHARS-UNTIL SOURCE >IN 0 BEGIN 2 PICK 2 PICK @ > IF "
    "    OVER @ 4 PICK + @ 5 PICK = DUP IF ELSE SWAP 1+ SWAP THEN "
    "    2 PICK DUP @ 1+ SWAP ! "
    "  ELSE TRUE THEN UNTIL ROT ROT 2DROP ROT ROT 2DROP ; "
    ": ( [CHAR] ) SKIP-CHARS-UNTIL ; IMMEDIATE "
    ": \\ NEWLINE SKIP-CHARS-UNTIL ; IMMEDIATE "
    ": PARSE SOURCE DROP >IN @ + SWAP COUNT-CHARS-UNTIL ; "
    ": S\" SKIP-WHITESPACE [CHAR] \" PARSE DUP ALLOCATE 2DUP POSTPONE "
    "  (LITERAL) , POSTPONE (LITERAL) , SWAP MOVE ; IMMEDIATE "
    ": C\" SKIP-WHITESPACE [CHAR] \" PARSE DUP 255 > IF DROP 255 THEN DUP 1+ "
    "  ALLOCATE DUP POSTPONE (LITERAL) , 2DUP C@ 1+ SWAP MOVE ; IMMEDIATE "
    ": ERASE BEGIN DUP 0 > WHILE 1 - SWAP 0 OVER C! 1 + SWAP REPEAT 2DROP ; "
    ": ABS DUP 0< IF NEGATE THEN ; "
    ": MIN 2DUP > IF SWAP THEN DROP ; "
    ": MAX 2DUP < IF SWAP THEN DROP ; "
    ": CLEANUP-INPUT DUP INPUT> BEGIN "
    "    DUP 0 <> 2 PICK CONSOLE-INPUT> 2 PICK <> AND WHILE "
    "      DUP INPUT-CLEANUP @ OVER INPUT-NEXT-INPUT @ ROT ROT EXECUTE "
    "    REPEAT SWAP >INPUT ; "
    ": CLEANUP-OUTPUT DUP OUTPUT> BEGIN "
    "    DUP 0 <> 2 PICK CONSOLE-OUTPUT> 2 PICK <> AND WHILE "
    "      DUP OUTPUT-CLEANUP @ OVER OUTPUT-NEXT-OUTPUT @ ROT ROT EXECUTE "
    "    REPEAT SWAP >OUTPUT ; "
    ": CLEANUP-ERROR DUP ERROR> BEGIN "
    "    DUP 0 <> 2 PICK CONSOLE-ERROR> 2 PICK <> AND WHILE "
    "      DUP OUTPUT-CLEANUP @ OVER OUTPUT-NEXT-OUTPUT @ ROT ROT EXECUTE "
    "    REPEAT SWAP >ERROR ; "
    ": CLEANUP DUP CLEANUP-INPUT DUP CLEANUP-OUTPUT CLEANUP-ERROR ; "
    "' CLEANUP DUP >DEFAULT-CLEANUP THIS-THREAD >CLEANUP "
    ": DROP-INPUT DUP INPUT> DUP 0 <> 2 PICK CONSOLE-INPUT> 2 PICK <> AND IF "
    "    DUP INPUT-CLEANUP @ OVER INPUT-NEXT-INPUT @ ROT ROT EXECUTE "
    "  THEN SWAP >INPUT ; "
    ": DROP-OUTPUT DUP OUTPUT> DUP 0 <> "
    "  2 PICK CONSOLE-OUTPUT> 2 PICK <> AND IF "
    "    DUP OUTPUT-CLEANUP @ OVER OUTPUT-NEXT-OUTPUT @ ROT ROT EXECUTE "
    "  THEN SWAP >OUTPUT ; "
    ": DROP-ERROR DUP ERROR> DUP 0 <> 2 PICK CONSOLE-ERROR> 2 PICK <> AND IF "
    "    DUP OUTPUT-CLEANUP @ OVER OUTPUT-NEXT-OUTPUT @ ROT ROT EXECUTE "
    "  THEN SWAP >ERROR ; "
    "' DROP-INPUT DUP >DEFAULT-DROP-INPUT THIS-THREAD >DROP-INPUT "
    ": TYPE THIS-THREAD CONSOLE-OUT> OUTPUT-WRITE @ EXECUTE ; "
    ": EMIT HERE C! HERE 1 TYPE ; "
    ": .\" SKIP-WHITESPACE [CHAR] \" PARSE DUP ALLOCATE 2DUP POSTPONE "
    "  (LITERAL) , POSTPONE (LITERAL) , SWAP MOVE POSTPONE TYPE ; IMMEDIATE "
    ": .( SKIP-WHITESPACE [CHAR] ) PARSE TYPE ; IMMEDIATE "
    ": INTERPRET THIS-THREAD >INTERPRET ; "
    ": STRING>SOURCE ( count addr -- input ) DUP ALLOCATE DUP 3 ROLL SWAP "
    "  3 PICK MOVE ( count addr ) INPUT-SIZE ALLOCATE DUP 3 ROLL SWAP "
    "  INPUT-COUNT ! ( addr input )  DUP ROT SWAP INPUT-BUFFER ! ( input ) "
    "  0 OVER INPUT-INDEX ! TRUE OVER INPUT-IS-CLOSED ! TRUE OVER "
    "  INPUT-IS-FREEABLE ! TRUE OVER INPUT-IS-BUFFER-FREEABLE ! 0 OVER "
    "  INPUT-REFILL ! 0 OVER INPUT-ARG ! 0 OVER INPUT-NEXT-INPUT ! ( input ) ; "
    ": EVALUATE ( count addr -- ? ) STRING>SOURCE >SOURCE INTERPRET ; "
    "
  if(!(input = af_new_string_input(code, (af_cell_t)strlen(code)))) {
    af_handle_out_of_memory(global, thread);
    return;
  }
  af_push_input(global, thread, input);
  af_interpret(global, thread);
}
