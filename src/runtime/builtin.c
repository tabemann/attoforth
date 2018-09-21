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

void af_compile_builtin(af_global_t* global, af_task_t* task) {
  af_input_t* input;
  uint8_t* code =
    ": TRUE -1 ; "
    ": FALSE 0 ; "
    ": NEGATE -1 * ; "
    ": 0< 0 < ; "
    ": 0= 0 = ; "
    ": 0> 0 > ; "
    ": 0<> 0 <> ; "
    ": 1+ 1 + ; "
    ": 1- 1 - ; "
    ": 2+ 2 + ; "
    ": 2- 2 - ; "
    ": 2* 2 * ; "
    ": 2/ 2 * ; "
    ": CELL+ CELL-SIZE + ; "
    ": CELLS CELL-SIZE * ; "
    ": , HERE ! CELL-SIZE ALLOT ; "
    ": C, HERE C! 1 ALLOT ; "
    ": COMPILE, , ; "
    ": LITERAL POSTPONE (LITERAL) , ; IMMEDIATE "
    ": VARIABLE CREATE 0 , ; "
    ": CONSTANT CREATE , DOES> @ ; "
    ": IF POSTPONE ?BRANCH HERE 0 , ; IMMEDIATE "
    ": ELSE POSTPONE BRANCH HERE 0 , HERE ROT ! ; IMMEDIATE "
    ": THEN HERE SWAP ! ; IMMEDIATE "
    ": ENDIF POSTPONE THEN ; IMMEDIATE "
    ": DEFER-ABORT CREATE 0 , DOES> @ EXECUTE ; "
    "DEFER-ABORT ABORT "
    ": DEFER CREATE ['] ABORT , DOES> @ EXECUTE ; "
    ": DEFER! >BODY ! ; "
    ": DEFER@ >BODY @ ; "
    ": ACTION-OF STATE @ IF POSTPONE ['] POSTPONE DEFER@ ELSE ' DEFER@ THEN ; "
    "  IMMEDIATE "
    ": IS STATE @ IF POSTPONE ['] POSTPONE DEFER! ELSE ' DEFER! THEN ; "
    "  IMMEDIATE "
    "DEFER . "
    "DEFER INTERPRET "
    ": BEGIN HERE ; IMMEDIATE "
    ": AGAIN POSTPONE BRANCH , ; IMMEDIATE "
    ": UNTIL POSTPONE ?BRANCH , ; IMMEDIATE "
    ": WHILE POSTPONE ?BRANCH HERE 0 , ; IMMEDIATE "
    ": REPEAT POSTPONE BRANCH SWAP , HERE SWAP ! ; IMMEDIATE "
    ": DO POSTPONE (LITERAL) HERE DUP 0 , POSTPONE >R POSTPONE 2>R HERE ; "
    "  IMMEDIATE "
    ": ?DO POSTPONE (LITERAL) HERE 0 , POSTPONE >R POSTPONE 2DUP POSTPONE 2>R "
    "  POSTPONE > POSTPONE ?BRANCH HERE 0 , HERE ; IMMEDIATE "
    ": LOOP POSTPONE 2R> POSTPONE 1+ POSTPONE 2DUP POSTPONE 2>R POSTPONE <= "
    "  POSTPONE ?BRANCH , HERE SWAP ! HERE SWAP ! POSTPONE 2R> POSTPONE 2DROP "
    "  POSTPONE R> POSTPONE DROP ; IMMEDIATE "
    ": +LOOP POSTPONE 2R> POSTPONE ROT POSTPONE + POSTPONE 2DUP POSTPONE 2>R "
    "  POSTPONE <= POSTPONE ?BRANCH , HERE SWAP ! HERE SWAP ! POSTPONE 2R> "
    "  POSTPONE 2DROP POSTPONE R> POSTPONE DROP ; IMMEDIATE "
    ": LEAVE 2R> 2DROP EXIT ; "
    ": UNLOOP 2R> 2DROP R> DROP ; "
    ": I R@ ; "
    ": J 2R> R> R@ SWAP >R ROT ROT 2>R ; "
    ": BUFFER: CREATE ALLOT ; "
    ": CHAR PARSE-NAME 1 >= IF C@ ELSE DROP 0 THEN ; "
    ": [CHAR] PARSE-NAME 1 >= IF C@ ELSE DROP 0 THEN POSTPONE (LITERAL) , ; "
    "  IMMEDIATE "
    ": ?EXECUTE1 DUP IF EXECUTE ELSE 2DROP THEN ; "
    ": BL 32 ; "
    ": NEWLINE 10 ; "
    ": SKIP-WHITESPACE BEGIN SOURCE >IN 2DUP @ > IF "
    "    DUP @ 3 PICK + C@ DUP BL = SWAP NEWLINE = OR IF "
    "      DUP @ 1+ SWAP ! 2DROP FALSE "
    "    ELSE 2DROP DROP TRUE THEN "
    "  ELSE 2DROP DROP TRUE THEN UNTIL ; "
    ": SKIP-CHARS-UNTIL SOURCE >IN BEGIN 2DUP @ > IF "
    "    DUP @ 3 PICK + C@ 4 PICK = OVER DUP @ 1+ SWAP ! "
    "  ELSE TRUE THEN UNTIL 2DROP 2DROP ; "
    ": COUNT-CHARS-UNTIL SOURCE >IN 0 BEGIN 2 PICK 2 PICK @ > IF "
    "    OVER @ 4 PICK + C@ 5 PICK = DUP IF ELSE SWAP 1+ SWAP THEN "
    "    2 PICK DUP @ 1+ SWAP ! "
    "  ELSE TRUE THEN UNTIL ROT ROT 2DROP ROT ROT 2DROP ; "
    ": ALLOCATE! ALLOCATE IF DROP ABORT THEN ; "
    ": FREE! FREE IF ABORT THEN ; "
    ": ( [CHAR] ) SKIP-CHARS-UNTIL ; IMMEDIATE "
    ": \\ NEWLINE SKIP-CHARS-UNTIL ; IMMEDIATE "
    ": PARSE SOURCE DROP >IN @ + SWAP COUNT-CHARS-UNTIL ; "
    ": S\" SKIP-WHITESPACE [CHAR] \" PARSE DUP ALLOCATE! 2DUP POSTPONE "
    "  (LITERAL) , POSTPONE (LITERAL) , SWAP MOVE ; IMMEDIATE "
    ": C\" SKIP-WHITESPACE [CHAR] \" PARSE DUP 255 > IF DROP 255 THEN DUP 1+ "
    "  ALLOCATE! DUP POSTPONE (LITERAL) , 2DUP C@ 1+ SWAP MOVE ; IMMEDIATE "
    ": ERASE BEGIN DUP 0 > WHILE 1 - SWAP 0 OVER C! 1 + SWAP REPEAT 2DROP ; "
    ": ABS DUP 0< IF NEGATE THEN ; "
    ": MIN 2DUP > IF SWAP THEN DROP ; "
    ": MAX 2DUP < IF SWAP THEN DROP ; "
    ": TYPE THIS-TASK OUTPUT> DUP OUTPUT-WRITE @ ?EXECUTE1 ; "
    ": EMIT HERE C! HERE 1 TYPE ; "
    ": CR NEWLINE EMIT ; "
    ": SPACE BL EMIT ; "
    ": SPACES 0 MAX 0 ?DO SPACE LOOP ; "
    ": .\" SKIP-WHITESPACE [CHAR] \" PARSE DUP ALLOCATE! 2DUP POSTPONE "
    "  (LITERAL) , POSTPONE (LITERAL) , SWAP MOVE POSTPONE TYPE ; IMMEDIATE "
    ": .( SKIP-WHITESPACE [CHAR] ) PARSE TYPE ; IMMEDIATE "
    ": CLEANUP-INPUT DUP INPUT> BEGIN "
    "    DUP 0 <> 2 PICK CONSOLE-INPUT> 2 PICK <> AND WHILE "
    "      DUP INPUT-CLEANUP @ OVER INPUT-NEXT-INPUT @ ROT ROT ?EXECUTE1 "
    "    REPEAT SWAP >INPUT ; "
    ": CLEANUP-OUTPUT DUP OUTPUT> BEGIN "
    "    DUP 0 <> 2 PICK CONSOLE-OUTPUT> 2 PICK <> AND WHILE "
    "      DUP OUTPUT-CLEANUP @ OVER OUTPUT-NEXT-OUTPUT @ ROT ROT ?EXECUTE1 "
    "    REPEAT SWAP >OUTPUT ; "
    ": CLEANUP-ERROR DUP ERROR> BEGIN "
    "    DUP 0 <> 2 PICK CONSOLE-ERROR> 2 PICK <> AND WHILE "
    "      DUP OUTPUT-CLEANUP @ OVER OUTPUT-NEXT-OUTPUT @ ROT ROT ?EXECUTE1 "
    "    REPEAT SWAP >ERROR ; "
    ": CLEANUP DUP CLEANUP-INPUT DUP CLEANUP-OUTPUT CLEANUP-ERROR ; "
    ": DROP-INPUT DUP INPUT> DUP 0 <> 2 PICK CONSOLE-INPUT> "
    "  2 PICK <> AND IF "
    "    DUP INPUT-NEXT-INPUT @ SWAP DUP INPUT-CLEANUP @ ?EXECUTE1 "
    "  THEN SWAP >INPUT ; "
    ": DROP-OUTPUT DUP OUTPUT> DUP 0 <> "
    "  2 PICK CONSOLE-OUTPUT> 2 PICK <> AND IF "
    "    DUP OUTPUT-NEXT-OUTPUT @ SWAP DUP OUTPUT-CLEANUP @ ?EXECUTE1 "
    "  THEN SWAP >OUTPUT ; "
    ": DROP-ERROR DUP ERROR> DUP 0 <> 2 PICK CONSOLE-ERROR> 2 PICK <> AND IF "
    "    DUP OUTPUT-NEXT-OUTPUT @ SWAP DUP OUTPUT-CLEANUP @ ?EXECUTE1 "
    "  THEN SWAP >ERROR ; "
    ": STRING>SOURCE-CLEANUP ( source -- ) "
    "  DUP INPUT-BUFFER @ FREE! FREE! ; "
    ": COPY-BUFFER ( addr count -- addr count ) "
    "  DUP ALLOCATE! DUP 3 ROLL SWAP 3 PICK MOVE SWAP ; "
    ": SET-INPUT-BUFFER ( addr count input -- ) "
    "  SWAP OVER INPUT-COUNT ! INPUT-BUFFER ! ; "
    ": SET-INPUT-DEFAULTS ( input -- ) "
    "  0 OVER INPUT-INDEX ! TRUE OVER INPUT-IS-CLOSED ! "
    "  ['] STRING>SOURCE-CLEANUP OVER INPUT-CLEANUP ! 0 OVER "
    "  INPUT-REFILL ! 0 OVER INPUT-ARG ! 0 SWAP INPUT-NEXT-INPUT ! ; "
    ": STRING>SOURCE ( addr count -- input ) "
    "  COPY-BUFFER INPUT-SIZE ALLOCATE! DUP 3 ROLL 3 ROLL 3 ROLL "
    "  SET-INPUT-BUFFER DUP SET-INPUT-DEFAULTS ; "
    ": PUSH-INPUT ( input -- ) "
    "  THIS-TASK INPUT> OVER INPUT-NEXT-INPUT ! THIS-TASK >INPUT ; "
    ": EVALUATE ( addr count -- ? ) STRING>SOURCE PUSH-INPUT INTERPRET ; "
    ": BACKSPACE 8 ; "
    ": TERMINAL-INFO-SIZE 2 CELLS ; "
    ": TERMINAL-FD 0 CELLS + ; "
    ": TERMINAL-BUFFER-SIZE 1 CELLS + ; "
    ": EXPAND-TERMINAL-BUFFER ( source -- ) "
    "  DUP INPUT-ARG @ TERMINAL-BUFFER-SIZE @ DUP 2 * DUP ALLOCATE! "
    "  SWAP 3 PICK INPUT-ARG @ TERMINAL-BUFFER-SIZE ! "
    "  2 PICK INPUT-BUFFER @ DUP 2 PICK 4 ROLL MOVE FREE! "
    "  SWAP INPUT-BUFFER ! ; "
    ": TRY-EXPAND-TERMINAL-BUFFER ( source -- ) "
    "  DUP INPUT-COUNT @ OVER INPUT-ARG @ TERMINAL-BUFFER-SIZE @ = IF "
    "    EXPAND-TERMINAL-BUFFER "
    "  ELSE DROP THEN ; "
    ": READ-TERMINAL-CHAR ( source -- continue ) "
    "  HERE DUP 1 3 PICK INPUT-ARG @ TERMINAL-FD @ IO-READ-BLOCK "
    "  DUP IO-ACTION-GET-STATE DUP IO-STATE-GET-INDEX 1 = SWAP "
    "  IO-STATE-DESTROY SWAP IO-ACTION-DESTROY IF "
    "    C@ DUP NEWLINE = IF "
    "      OVER TRY-EXPAND-TERMINAL-BUFFER "
    "      OVER INPUT-BUFFER @ ROT INPUT-COUNT DUP @ 1+ DUP ROT ! + 1- C! "
    "      FALSE "
    "    ELSE DUP BACKSPACE = IF "
    "      DROP DUP INPUT-COUNT @ 0> IF "
    "        DUP INPUT-COUNT @ 1 - SWAP INPUT-COUNT ! "
    "      ELSE DROP THEN TRUE "
    "    ELSE "
    "      OVER TRY-EXPAND-TERMINAL-BUFFER "
    "      OVER INPUT-BUFFER @ ROT INPUT-COUNT DUP @ 1+ DUP ROT ! + 1- C! TRUE "
    "    THEN THEN "
    "  ELSE DROP TRUE SWAP INPUT-IS-CLOSED ! FALSE THEN ; "
    ": REFILL-TERMINAL ( source -- ) "
    "  0 OVER INPUT-COUNT ! 0 OVER INPUT-INDEX ! "
    "  BEGIN DUP READ-TERMINAL-CHAR INVERT UNTIL DROP ; "
    ": DEFAULT-TERMINAL-BUFFER-SIZE 256 ; "
    ": TERMINAL>SOURCE-CLEANUP ( source -- ) "
    "  DUP INPUT-BUFFER @ FREE! DUP INPUT-ARG @ FREE! FREE! ; "
    ": TERMINAL>SOURCE ( fd -- input ) "
    "  INPUT-SIZE ALLOCATE! TERMINAL-INFO-SIZE ALLOCATE! "
    "  DEFAULT-TERMINAL-BUFFER-SIZE OVER TERMINAL-BUFFER-SIZE ! "
    "  ROT OVER TERMINAL-FD ! OVER INPUT-ARG ! "
    "  DEFAULT-TERMINAL-BUFFER-SIZE ALLOCATE! OVER INPUT-BUFFER ! "
    "  0 OVER INPUT-INDEX ! 0 OVER INPUT-COUNT ! "
    "  FALSE OVER INPUT-IS-CLOSED ! "
    "  ['] TERMINAL>SOURCE-CLEANUP OVER INPUT-CLEANUP ! "
    "  ['] REFILL-TERMINAL OVER INPUT-REFILL ! "
    "  0 OVER INPUT-NEXT-INPUT ! ; "
    ": REFILL THIS-TASK INPUT> DUP INPUT-REFILL @ ?EXECUTE1 ; "
    ": FD>OUTPUT-CLEANUP ( output -- ) FREE! ; "
    ": FD>OUTPUT-WRITE ( addr n output -- ) "
    "  OUTPUT-ARG @ IO-WRITE-ASYNC IO-ACTION-DESTROY ; "
    ": FD>OUTPUT ( fd -- output ) "
    "  OUTPUT-SIZE ALLOCATE! SWAP OVER OUTPUT-ARG ! "
    "  0 OVER OUTPUT-NEXT-OUTPUT ! "
    "  ['] FD>OUTPUT-WRITE OVER OUTPUT-WRITE ! "
    "  ['] FD>OUTPUT-CLEANUP OVER OUTPUT-CLEANUP ! ; "
    ": OLD-SPAWN SPAWN ; "
    ": SPAWN OLD-SPAWN IO-STDOUT FD>OUTPUT OVER >CONSOLE-OUTPUT "
    "  IO-STDERR FD>OUTPUT OVER >CONSOLE-ERROR ; "
    ": ADVANCE-TO-END SOURCE SWAP DROP >IN ! ; "
    ":NONAME "
    "  BEGIN "
    "    PARSE-NAME DUP 0<> IF "
    "      STATE @ IF "
    "        FIND-WORD DUP 0<> IF "
    "          1 = IF "
    "            EXECUTE "
    "          ELSE "
    "            COMPILE, "
    "          THEN "
    "        ELSE "
    "          DROP 2DUP PARSE-NUMBER 0= IF "
    "            DROP ADVANCE-TO-END .\" Unable to parse: \" TYPE "
    "          ELSE "
    "            ROT ROT 2DROP POSTPONE (LITERAL) , "
    "          THEN "
    "        THEN "
    "      ELSE "
    "        FIND-WORD 0<> IF "
    "          EXECUTE "
    "        ELSE "
    "          2DUP PARSE-NUMBER 0= IF "
    "            DROP ADVANCE-TO-END .\" Unable to parse: \" TYPE "
    "          ELSE "
    "            ROT ROT 2DROP "
    "          THEN "
    "        THEN "
    "      THEN FALSE"
    "    ELSE "
    "      2DROP THIS-TASK INPUT> THIS-TASK CONSOLE-INPUT> = IF "
    "        STATE @ IF "
    "          SPACE .\" compiled\" CR "
    "        ELSE "
    "          SPACE .\" ok\" CR "
    "        THEN "
    "      THEN "
    "      THIS-TASK INPUT> INPUT-IS-CLOSED @ INVERT IF "
    "        REFILL FALSE "
    "      ELSE "
    "        THIS-TASK DROP-INPUT TRUE "
    "      THEN "
    "    THEN "
    "  UNTIL ; ' INTERPRET DEFER! "
    ": QUIT EMPTY-RETURN-STACK THIS-TASK CLEANUP THIS-TASK INPUT> 0<> IF "
    "    INTERPRET "
    "  THEN ; "
    ":NONAME EMPTY-DATA-STACK QUIT ; ' ABORT DEFER! "
    "' ABORT DUP >DEFAULT-ABORT THIS-TASK >ABORT "
    ": ABORT\" POSTPONE IF SKIP-WHITESPACE [CHAR] \" PARSE DUP ALLOCATE! "
    "  2DUP POSTPONE (LITERAL) , POSTPONE (LITERAL) , SWAP MOVE POSTPONE "
    "  TYPE POSTPONE ABORT POSTPONE THEN ; IMMEDIATE "
    ":NONAME ( n -- ) FORMAT-NUMBER TYPE ; ' . DEFER! "
    ": BEGIN-INTERACTIVE ( -- ) "
    "  IO-STDIN TERMINAL>SOURCE THIS-TASK 2DUP >CONSOLE-INPUT >INPUT "
    "  IO-STDOUT FD>OUTPUT THIS-TASK 2DUP >CONSOLE-OUTPUT >OUTPUT "
    "  IO-STDERR FD>OUTPUT THIS-TASK 2DUP >CONSOLE-ERROR >ERROR "
    "  INTERPRET ; "
    "BEGIN-INTERACTIVE ";
  if(!(input = af_new_string_input(global, code, (af_cell_t)strlen(code)))) {
    af_handle_out_of_memory(global, task);
    return;
  }
  task->current_input = input;
  af_interpret(global, task);
}
