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
    ": 2DROP DROP DROP ; "
    ": 2DUP OVER OVER ; "
    ": 2SWAP 3 ROLL 3 ROLL ; "
    ": 2OVER 3 PICK 3 PICK ; "
    ": 2ROT 5 ROLL 5 ROLL ; "
    ": 2! SWAP OVER ! CELL+ ! ; "
    ": 2@ DUP @ SWAP CELL+ @ SWAP ; "
    ": , HERE ! CELL-SIZE ALLOT ; "
    ": C, HERE C! 1 ALLOT ; "
    ": LITERAL POSTPONE (LITERAL) , ; IMMEDIATE "
    ": 2, HERE 2! [ 2 CELLS ] LITERAL ALLOT ; "
    ": 2>R R> ROT ROT SWAP >R >R >R ; "
    ": 2R> R> R> R> SWAP ROT >R ; "
    ": 2R@ R> R> R> 2DUP SWAP ROT >R ROT >R ROT >R ; "
    ": COMPILE, , ; "
    ": VARIABLE CREATE 0 , ; "
    ": CONSTANT CREATE , DOES> @ ; "
    ": OFFSET CREATE , DOES> @ + ; "
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
    "DEFER INTERPRET "
    ": BEGIN HERE ; IMMEDIATE "
    ": AGAIN POSTPONE BRANCH , ; IMMEDIATE "
    ": UNTIL POSTPONE ?BRANCH , ; IMMEDIATE "
    ": WHILE POSTPONE ?BRANCH HERE 0 , ; IMMEDIATE "
    ": REPEAT POSTPONE BRANCH SWAP , HERE SWAP ! ; IMMEDIATE "
    ": ?EXECUTE1 DUP IF EXECUTE ELSE 2DROP THEN ; "
    ": ALLOCATE! ALLOCATE IF DROP ABORT THEN ; "
    ": FREE! FREE IF ABORT THEN ; "
    ": FD>OUTPUT-CLEANUP FREE! ; "
    ": FD>OUTPUT-WRITE "
    "  OUTPUT-ARG @ IO-WRITE-ASYNC IO-ACTION-DESTROY ; "
    ": FD>OUTPUT "
    "  OUTPUT-SIZE ALLOCATE! SWAP OVER OUTPUT-ARG ! "
    "  0 OVER OUTPUT-NEXT-OUTPUT ! "
    "  ['] FD>OUTPUT-WRITE OVER OUTPUT-WRITE ! "
    "  ['] FD>OUTPUT-CLEANUP OVER OUTPUT-CLEANUP ! ; "
    ": OLD-SPAWN SPAWN ; "
    ": SPAWN OLD-SPAWN IO-STDOUT FD>OUTPUT OVER >CONSOLE-OUTPUT "
    "  IO-STDERR FD>OUTPUT OVER >CONSOLE-ERROR ; "
    "IO-STDOUT FD>OUTPUT THIS-TASK 2DUP >CONSOLE-OUTPUT >OUTPUT "
    "IO-STDERR FD>OUTPUT THIS-TASK 2DUP >CONSOLE-ERROR >ERROR "
    ": TYPE THIS-TASK OUTPUT> DUP IF "
    "    DUP OUTPUT-WRITE @ ?EXECUTE1 "
    "  ELSE "
    "    DROP "
    "  THEN ; "
    ": . FORMAT-NUMBER TYPE ; "
    ": ERROR-TYPE THIS-TASK ERROR> DUP IF "
    "    DUP OUTPUT-WRITE @ ?EXECUTE1 "
    "  ELSE "
    "    DROP "
    "  THEN ; "
    ": EMIT HERE C! HERE 1 TYPE ; "
    "32 CONSTANT BL "
    "10 CONSTANT NEWLINE "
    ": CR NEWLINE EMIT ; "
    ": SPACE BL EMIT ; "
    ": .S 0 BEGIN DUP DEPTH 2 - < WHILE 1+ DUP PICK . SPACE REPEAT DROP ; "
    ": CHAR PARSE-NAME 1 >= IF C@ ELSE DROP 0 THEN ; "
    ": [CHAR] PARSE-NAME 1 >= IF C@ ELSE DROP 0 THEN POSTPONE (LITERAL) , ; "
    "  IMMEDIATE "
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
    ": ( [CHAR] ) SKIP-CHARS-UNTIL ; IMMEDIATE "
    ": \\ NEWLINE SKIP-CHARS-UNTIL ; IMMEDIATE "
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
    ": LEAVE R> 2R> 2DROP >R ; "
    ": UNLOOP R> 2R> 2DROP R> DROP >R ; "
    ": I R> R@ SWAP >R ; "
    ": J 2R> 2R> R@ 4 ROLL 4 ROLL 4 ROLL 4 ROLL 2>R 2>R ; "
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
    ": CLEANUP-FULL "
    "  DUP CLEANUP DUP CONSOLE-OUTPUT> DUP IF "
    "    DUP OUTPUT-CLEANUP @ ?EXECUTE1 "
    "  ELSE DROP THEN "
    "  CONSOLE-ERROR> DUP IF "
    "    DUP OUTPUT-CLEANUP @ ?EXECUTE1 "
    "  ELSE DROP THEN ; "
    ": SPAWN-SIMPLE-EXECUTE EXECUTE THIS-TASK CLEANUP-FULL ; "
    ": SPAWN-SIMPLE ( ... count xt -- task ) "
    "  SPAWN ROT 1+ DUP 0 ?DO "
    "    DUP 1+ ROLL 2 PICK >DATA "
    "  LOOP "
    "  DROP ['] SPAWN-SIMPLE-EXECUTE OVER >INIT-WORD DUP START ; "
    ": SPAWN-SIMPLE-FREE-DATA-ON-EXIT ( ... count xt -- task ) "
    "  SPAWN TRUE OVER >FREE-DATA-ON-EXIT ROT 1+ DUP 0 ?DO "
    "    DUP 1+ ROLL 2 PICK >DATA "
    "  LOOP "
    "  DROP ['] SPAWN-SIMPLE-EXECUTE OVER >INIT-WORD DUP START ; "
    ": SPAWN-SIMPLE-NO-IO ( ... count xt -- task ) "
    "  OLD-SPAWN SWAP OVER >INIT-WORD SWAP DUP 0 ?DO "
    "    DUP 1+ ROLL 2 PICK >DATA "
    "  LOOP "
    "  DROP DUP START ; "
    ": SPAWN-SIMPLE-NO-IO-FREE-DATA-ON-EXIT ( ... count xt -- task ) "
    "  OLD-SPAWN TRUE OVER >FREE-DATA-ON-EXIT SWAP OVER >INIT-WORD "
    "  SWAP DUP 0 ?DO "
    "    DUP 1+ ROLL 2 PICK >DATA "
    "  LOOP "
    "  DROP DUP START ; "
    ": SPAWN-SIMPLE-NO-DATA ( ... count xt -- task ) "
    "  SPAWN-NO-DATA SWAP OVER >INIT-WORD SWAP DUP 0 ?DO "
    "    DUP 1+ ROLL 2 PICK >DATA "
    "  LOOP "
    "  DROP DUP START ; "
    ": BUFFER: CREATE ALLOT ; "
    ": PARSE SOURCE DROP >IN @ + SWAP COUNT-CHARS-UNTIL ; "
    ": S\" SKIP-WHITESPACE [CHAR] \" PARSE DUP ALLOCATE! 2DUP POSTPONE "
    "  (LITERAL) , POSTPONE (LITERAL) , SWAP MOVE ; IMMEDIATE "
    ": C\" SKIP-WHITESPACE [CHAR] \" PARSE DUP 255 > IF DROP 255 THEN DUP 1+ "
    "  ALLOCATE! DUP POSTPONE (LITERAL) , 2DUP C@ 1+ SWAP MOVE ; IMMEDIATE "
    ": ERASE BEGIN DUP 0 > WHILE 1 - SWAP 0 OVER C! 1 + SWAP REPEAT 2DROP ; "
    ": ABS DUP 0< IF NEGATE THEN ; "
    ": MIN 2DUP > IF SWAP THEN DROP ; "
    ": MAX 2DUP < IF SWAP THEN DROP ; "
    ": SPACES 0 MAX 0 ?DO SPACE LOOP ; "
    ": .\" SKIP-WHITESPACE [CHAR] \" PARSE DUP ALLOCATE! 2DUP POSTPONE "
    "  (LITERAL) , POSTPONE (LITERAL) , SWAP MOVE POSTPONE TYPE ; IMMEDIATE "
    ": .( SKIP-WHITESPACE [CHAR] ) PARSE TYPE ; IMMEDIATE "
    ": ERROR\" SKIP-WHITESPACE [CHAR] \" PARSE DUP ALLOCATE! 2DUP POSTPONE "
    "  (LITERAL) , POSTPONE (LITERAL) , SWAP MOVE POSTPONE "
    "  ERROR-TYPE ; IMMEDIATE "
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
    ": ABORT\" POSTPONE IF SKIP-WHITESPACE [CHAR] \" PARSE DUP ALLOCATE! "
    "  2DUP POSTPONE (LITERAL) , POSTPONE (LITERAL) , SWAP MOVE POSTPONE "
    "  TYPE POSTPONE ABORT POSTPONE THEN ; IMMEDIATE "
    "2 CELLS CONSTANT TERMINAL-INFO-SIZE "
    "0 CELLS OFFSET TERMINAL-FD "
    "1 CELLS OFFSET TERMINAL-BUFFER-SIZE "
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
    "      SPACE OVER TRY-EXPAND-TERMINAL-BUFFER "
    "      OVER INPUT-BUFFER @ ROT INPUT-COUNT DUP @ 1+ DUP ROT ! + 1- C! "
    "      FALSE "
    "    ELSE DUP 127 = IF "
    "      DROP 8 EMIT SPACE 8 EMIT DUP INPUT-COUNT @ 0> IF "
    "        DUP INPUT-COUNT @ 1 - SWAP INPUT-COUNT ! "
    "      ELSE DROP THEN TRUE "
    "    ELSE "
    "      DUP EMIT OVER TRY-EXPAND-TERMINAL-BUFFER "
    "      OVER INPUT-BUFFER @ ROT INPUT-COUNT DUP @ 1+ DUP ROT ! + 1- C! TRUE "
    "    THEN THEN "
    "  ELSE DROP TRUE SWAP INPUT-IS-CLOSED ! FALSE THEN ; "
    ": REFILL-TERMINAL ( source -- ) "
    "  0 OVER INPUT-COUNT ! 0 OVER INPUT-INDEX ! "
    "  BEGIN DUP READ-TERMINAL-CHAR INVERT UNTIL DROP ; "
    "256 CONSTANT DEFAULT-TERMINAL-BUFFER-SIZE "
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
    "3 CELLS CONSTANT FILE-INFO-SIZE "
    "0 CELLS OFFSET FILE-FD "
    "1 CELLS OFFSET FILE-BUFFER-SIZE "
    "2 CELLS OFFSET FILE-DATA-SIZE "
    "256 CONSTANT DEFAULT-FILE-BUFFER-SIZE "
    ": ADVANCE-FILE ( source -- ) "
    "  DUP INPUT-BUFFER @ OVER INPUT-COUNT @ + OVER INPUT-BUFFER @ "
    "  2 PICK INPUT-ARG @ FILE-DATA-SIZE @ 3 PICK INPUT-COUNT @ - MOVE "
    "  DUP INPUT-ARG @ FILE-DATA-SIZE @ OVER INPUT-COUNT @ - "
    "  OVER INPUT-ARG @ FILE-DATA-SIZE ! "
    "  0 SWAP INPUT-COUNT ! ; "
    ": FIND-FILE-NEWLINE ( source -- ) "
    "  DUP INPUT-IS-CLOSED @ INVERT IF "
    "    BEGIN "
    "      DUP INPUT-COUNT @ OVER INPUT-ARG @ FILE-DATA-SIZE @ < IF "
    "        DUP INPUT-BUFFER @ OVER INPUT-COUNT @ + C@ NEWLINE = IF "
    "          DUP INPUT-COUNT @ 1+ SWAP INPUT-COUNT ! TRUE TRUE "
    "        ELSE "
    "          DUP INPUT-COUNT @ 1+ OVER INPUT-COUNT ! FALSE "
    "        THEN "
    "      ELSE "
    "        DROP FALSE TRUE "
    "      THEN "
    "    UNTIL "
    "  ELSE "
    "    DUP INPUT-ARG @ FILE-DATA-SIZE @ SWAP INPUT-COUNT ! TRUE "
    "  THEN ; "
    ": TRY-EXPAND-FILE-BUFFER ( source -- ) "
    "  DUP INPUT-ARG @ FILE-BUFFER-SIZE @ "
    "  OVER INPUT-ARG @ FILE-DATA-SIZE @ = IF "
    "    DUP INPUT-BUFFER @ OVER INPUT-ARG @ FILE-BUFFER-SIZE @ 2 * RESIZE "
    "    OVER INPUT-BUFFER ! "
    "    DUP INPUT-ARG @ FILE-BUFFER-SIZE @ 2 * "
    "    SWAP INPUT-ARG @ FILE-BUFFER-SIZE ! "
    "  ELSE "
    "    DROP "
    "  THEN ; "
    ": TRY-READ-FILE-LINE ( source -- ) "
    "  DUP TRY-EXPAND-FILE-BUFFER "
    "  DUP INPUT-BUFFER @ OVER INPUT-ARG @ FILE-DATA-SIZE @ + "
    "  OVER INPUT-ARG @ DUP FILE-BUFFER-SIZE @ SWAP FILE-DATA-SIZE @ - "
    "  2 PICK INPUT-ARG @ FILE-FD @ IO-READ-BLOCK DUP IO-ACTION-GET-STATE "
    "  SWAP IO-ACTION-DESTROY "
    "  DUP IO-STATE-IS-CLOSED IF "
    "    TRUE 2 PICK INPUT-IS-CLOSED ! "
    "  THEN "
    "  DUP IO-STATE-GET-INDEX 2 ROLL INPUT-ARG @ SWAP OVER "
    "  FILE-DATA-SIZE @ + SWAP FILE-DATA-SIZE ! IO-STATE-DESTROY ; "
    ": REFILL-FILE ( source -- ) "
    "  0 OVER INPUT-INDEX ! "
    "  DUP ADVANCE-FILE BEGIN DUP FIND-FILE-NEWLINE INVERT WHILE "
    "    DUP TRY-READ-FILE-LINE "
    "  REPEAT DROP ; "
    ": FILE>SOURCE-CLEANUP ( source -- ) "
    "  DUP INPUT-ARG @ FILE-FD @ IO-CLOSE-ASYNC IO-ACTION-DESTROY "
    "  DUP INPUT-BUFFER @ FREE! DUP INPUT-ARG @ FREE! FREE! ; "
    ": FILE>SOURCE ( fd -- input ) "
    "  INPUT-SIZE ALLOCATE! FILE-INFO-SIZE ALLOCATE! "
    "  DEFAULT-FILE-BUFFER-SIZE OVER FILE-BUFFER-SIZE ! "
    "  0 OVER FILE-DATA-SIZE ! "
    "  ROT OVER FILE-FD ! OVER INPUT-ARG ! "
    "  DEFAULT-FILE-BUFFER-SIZE ALLOCATE! OVER INPUT-BUFFER ! "
    "  0 OVER INPUT-INDEX ! 0 OVER INPUT-COUNT ! "
    "  FALSE OVER INPUT-IS-CLOSED ! "
    "  ['] FILE>SOURCE-CLEANUP OVER INPUT-CLEANUP ! "
    "  ['] REFILL-FILE OVER INPUT-REFILL ! "
    "  0 OVER INPUT-NEXT-INPUT ! ; "
    ": INCLUDED ( addr count -- ? ) "
    "  IO-RDONLY 0 IO-OPEN ABORT\" Unable to open file \" "
    "  DUP FILE>SOURCE PUSH-INPUT INTERPRET ; "
    ": INCLUDE ( \"filename\" -- ? ) "
    "  PARSE-NAME INCLUDED ; "
    ": ADVANCE-TO-END SOURCE SWAP DROP >IN ! ; "
    ": TIME+ ( s1 ns1 s2 ns2 -- s ns ) "
    "  DUP 3 PICK + 1000000000 > IF "
    "    2 ROLL + 1000000000 - ROT ROT + 1+ SWAP "
    "  ELSE "
    "    2 ROLL + ROT ROT + SWAP "
    "  THEN ; "
    ": FIX-TIME ( s ns -- s ns ) "
    "    DUP 1000000000 / ROT + SWAP 1000000000 MOD ; "
    ": SLEEP ( s ns -- ) IO-GET-MONOTONIC-TIME TIME+ IO-SLEEP ; "
    ": SIMPLE-LOCK ( addr -- ) "
    "  0 1 BEGIN "
    "    0 THIS-TASK 4 PICK CAS IF "
    "      2DROP DROP TRUE "
    "    ELSE "
    "      2DUP SLEEP 2 * SWAP 2 * SWAP FIX-TIME FALSE "
    "    THEN "
    "  UNTIL ; "
    ": SIMPLE-UNLOCK ( addr -- ) THIS-TASK 0 ROT CAS DROP ; "
    "5 CELLS CONSTANT LOCK-SIZE "
    "0 CELLS OFFSET LOCK-PRIMARY "
    "1 CELLS OFFSET LOCK-SECONDARY "
    "2 CELLS OFFSET LOCK-WAITING "
    "3 CELLS OFFSET LOCK-WAITING-COUNT "
    "4 CELLS OFFSET LOCK-WAITING-SIZE "
    "8 CONSTANT DEFAULT-LOCK-WAITING-SIZE "
    ": NEW-LOCK ( -- lock ) "
    "  LOCK-SIZE ALLOCATE! 0 OVER LOCK-PRIMARY ! 0 OVER LOCK-SECONDARY ! "
    "  DEFAULT-LOCK-WAITING-SIZE CELLS ALLOCATE! OVER LOCK-WAITING ! "
    "  0 OVER LOCK-WAITING-COUNT ! "
    "  DEFAULT-LOCK-WAITING-SIZE OVER LOCK-WAITING-SIZE ! ; "
    ": DESTROY-LOCK ( lock -- ) DUP LOCK-WAITING @ FREE! FREE! ; "
    ": LOCK ( lock -- ) "
    "  DUP LOCK-SECONDARY SIMPLE-LOCK "
    "  BEGIN "
    "    0 THIS-TASK 2 PICK LOCK-PRIMARY CAS IF "
    "      LOCK-SECONDARY SIMPLE-UNLOCK TRUE "
    "    ELSE THIS-TASK OVER LOCK-PRIMARY @ = IF "
    "      LOCK-SECONDARY SIMPLE-UNLOCK TRUE "
    "    ELSE "
    "      DUP LOCK-WAITING-COUNT @ OVER LOCK-WAITING-SIZE @ = IF "
    "        DUP LOCK-WAITING-SIZE @ DUP 2 * DUP 3 PICK LOCK-WAITING-SIZE ! "
    "        CELLS ALLOCATE! DUP 3 PICK LOCK-WAITING @ "
    "        SWAP DUP ROT 4 ROLL CELLS MOVE FREE! OVER LOCK-WAITING ! "
    "      THEN "
    "      THIS-TASK OVER LOCK-WAITING-COUNT @ CELLS 2 PICK "
    "      LOCK-WAITING @ + ! DUP LOCK-WAITING-COUNT @ 1+ "
    "      OVER LOCK-WAITING-COUNT ! "
    "      DUP LOCK-SECONDARY SIMPLE-UNLOCK THIS-TASK WAIT "
    "      DUP LOCK-SECONDARY SIMPLE-LOCK FALSE "
    "    THEN THEN "
    "  UNTIL ; "
    ": HAS-LOCK ( task lock -- boolean ) LOCK-PRIMARY @ = ; "
    ": UNLOCK ( lock -- ) "
    "  DUP LOCK-SECONDARY SIMPLE-LOCK "
    "  THIS-TASK OVER HAS-LOCK IF "
    "    DUP LOCK-WAITING-COUNT @ 0= IF "
    "      0 OVER LOCK-PRIMARY ! LOCK-SECONDARY SIMPLE-UNLOCK "
    "    ELSE "
    "      DUP LOCK-WAITING @ DUP @ SWAP DUP CELL+ SWAP 3 PICK "
    "      LOCK-WAITING-COUNT @ 1- DUP 5 PICK LOCK-WAITING-COUNT ! CELLS MOVE "
    "      DUP 2 PICK LOCK-PRIMARY ! WAKE LOCK-SECONDARY SIMPLE-UNLOCK "
    "    THEN "
    "  ELSE "
    "    LOCK-SECONDARY SIMPLE-UNLOCK TRUE "
    "    ABORT\" Attempted to unlock lock not belonging to current task \" "
    "  THEN ; "
    "5 CELLS CONSTANT COND-SIZE "
    "0 CELLS OFFSET COND-PRIMARY "
    "1 CELLS OFFSET COND-SECONDARY "
    "2 CELLS OFFSET COND-WAITING "
    "3 CELLS OFFSET COND-WAITING-COUNT "
    "4 CELLS OFFSET COND-WAITING-SIZE "
    "8 CONSTANT DEFAULT-COND-WAITING-SIZE "    
    ": NEW-COND ( lock -- cond ) "
    "  COND-SIZE ALLOCATE! SWAP OVER COND-PRIMARY ! 0 OVER COND-SECONDARY ! "
    "  DEFAULT-COND-WAITING-SIZE CELLS ALLOCATE! OVER COND-WAITING ! "
    "  0 OVER COND-WAITING-COUNT ! "
    "  DEFAULT-COND-WAITING-SIZE OVER COND-WAITING-SIZE ! ; "
    ": DESTROY-COND ( lock -- ) DUP COND-WAITING @ FREE! FREE! ; "
    ": WAIT-COND ( cond -- ) "
    "  DUP COND-SECONDARY SIMPLE-LOCK "
    "  THIS-TASK OVER COND-PRIMARY @ HAS-LOCK IF "
    "    DUP COND-WAITING-COUNT @ OVER COND-WAITING-SIZE @ = IF "
    "      DUP COND-WAITING-SIZE @ DUP 2 * DUP 3 PICK COND-WAITING-SIZE ! "
    "      CELLS ALLOCATE! DUP 3 PICK COND-WAITING @ "
    "      SWAP DUP ROT 4 ROLL CELLS MOVE FREE! OVER COND-WAITING ! "
    "    THEN "
    "    THIS-TASK OVER COND-WAITING-COUNT @ CELLS 2 PICK "
    "    COND-WAITING @ + ! DUP COND-WAITING-COUNT @ 1+ "
    "    OVER COND-WAITING-COUNT ! "
    "    DUP COND-SECONDARY SIMPLE-UNLOCK "
    "    DUP COND-PRIMARY @ UNLOCK THIS-TASK WAIT COND-PRIMARY @ LOCK "
    "  ELSE "
    "    COND-SECONDARY SIMPLE-UNLOCK TRUE "
    "    ABORT\" Attempted to wait on condition not belonging to current task \" "
    "  THEN ; "
    ": SIGNAL-COND ( cond -- ) "
    "  DUP COND-SECONDARY SIMPLE-LOCK "
    "  DUP COND-WAITING-COUNT @ 0= IF "
    "    COND-SECONDARY SIMPLE-UNLOCK "
    "  ELSE "
    "    DUP COND-WAITING @ DUP @ SWAP DUP CELL+ SWAP 3 PICK "
    "    COND-WAITING-COUNT @ 1- DUP 5 PICK COND-WAITING-COUNT ! CELLS MOVE "
    "    WAKE COND-SECONDARY SIMPLE-UNLOCK "
    "  THEN ; "
    ": BROADCAST-COND ( cond -- ) "
    "  DUP COND-SECONDARY SIMPLE-LOCK "
    "  DUP COND-WAITING-COUNT @ 0 ?DO "
    "    DUP COND-WAITING @ I CELLS + @ WAKE "
    "  LOOP "
    "  0 OVER COND-WAITING-COUNT ! COND-SECONDARY SIMPLE-UNLOCK ; "
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
    "  ELSE "
    "    THIS-TASK DUP CLEANUP-FULL KILL "
    "  THEN ; "
    ":NONAME EMPTY-DATA-STACK QUIT ; ' ABORT DEFER! "
    "' ABORT DUP >DEFAULT-ABORT THIS-TASK >ABORT "
    ": BEGIN-INTERACTIVE ( -- ) "
    "  IO-STDIN TERMINAL>SOURCE THIS-TASK 2DUP >CONSOLE-INPUT >INPUT "
    "  INTERPRET ; "
    "BEGIN-INTERACTIVE ";
  if(!(input = af_new_string_input(global, code, (af_cell_t)strlen(code)))) {
    af_handle_out_of_memory(global, task);
    return;
  }
  task->current_input = input;
  af_interpret(global, task);
}
