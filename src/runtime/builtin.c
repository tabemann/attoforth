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
    "FORTH-WORDLIST IO-WORDLIST TASK-WORDLIST 3 SET-ORDER "
    "FORTH-WORDLIST SET-CURRENT "
    ": TRUE -1 ; "
    ": FALSE 0 ; "
    ": NEGATE -1 * ; "
    ": 0< 0 < ; "
    ": 0= 0 = ; "
    ": 0> 0 > ; "
    ": 0<> 0 <> ; "
    ": 0<= 0 <= ; "
    ": 0>= 0 >= ; "
    ": 1+ 1 + ; "
    ": 1- 1 - ; "
    ": 2+ 2 + ; "
    ": 2- 2 - ; "
    ": 2* 2 * ; "
    ": 2/ 2 * ; "
    ": , HERE ! CELL-SIZE ALLOT ; "
    ": C, HERE C! 1 ALLOT ; "
    ": COMPILE, , ; "
    ": [COMPILE] ' COMPILE, ; IMMEDIATE "
    ": COMPILE ' ['] (LITERAL) COMPILE, , ['] COMPILE, COMPILE, ; IMMEDIATE "
    ": LITERAL COMPILE (LITERAL) , ; IMMEDIATE "
    ": BIN 2 BASE ! ; "
    ": OCT 8 BASE ! ; "
    ": DEC 10 BASE ! ; "
    ": DECIMAL 10 BASE ! ; "
    ": HEX 16 BASE ! ; "
    ": BASE-EXECUTE BASE @ >R BASE ! EXECUTE R> BASE ! ; "
    ": CELL+ CELL-SIZE + ; "
    ": CELLS CELL-SIZE * ; "
    ": NIP SWAP DROP ; "
    ": TUCK SWAP OVER ; "
    ": 2DROP DROP DROP ; "
    ": 2DUP OVER OVER ; "
    ": 2SWAP 3 ROLL 3 ROLL ; "
    ": 2OVER 3 PICK 3 PICK ; "
    ": 2ROT 5 ROLL 5 ROLL ; "
    ": 2! TUCK ! CELL+ ! ; "
    ": 2@ DUP @ SWAP CELL+ @ SWAP ; "
    ": 2, HERE 2! [ 2 CELLS ] LITERAL ALLOT ; "
    ": 2>R R> ROT ROT SWAP >R >R >R ; "
    ": 2R> R> R> R> SWAP ROT >R ; "
    ": 2R@ R> R> R> 2DUP SWAP ROT >R ROT >R ROT >R ; "
    ": VARIABLE CREATE 0 , ; "
    ": CONSTANT CREATE , DOES> @ ; "
    ": OFFSET CREATE , DOES> @ + ; "
    ": IF COMPILE ?BRANCH HERE 0 , ; IMMEDIATE "
    ": ELSE COMPILE BRANCH HERE 0 , HERE ROT ! ; IMMEDIATE "
    ": THEN HERE SWAP ! ; IMMEDIATE "
    ": POSTPONE ' DUP IS-IMMEDIATE IF "
    "    COMPILE, "
    "  ELSE "
    "    COMPILE (LITERAL) , COMPILE COMPILE, "
    "  THEN ; IMMEDIATE "
    ": & POSTPONE POSTPONE ; IMMEDIATE "
    ": ENDIF & THEN ; IMMEDIATE "
    ": RECURSE LATESTXT COMPILE, ; IMMEDIATE "
    ": CHARS ; "
    ": CHAR+ 1+ ; "
    ": S>D DUP 0< IF 0 ELSE -1 THEN ; "
    ": DEFER-ABORT CREATE 0 , DOES> @ EXECUTE ; "
    "DEFER-ABORT ABORT "
    ": DEFER CREATE ['] ABORT , DOES> @ EXECUTE ; "
    ": DEFER! >BODY ! ; "
    ": DEFER@ >BODY @ ; "
    ": ACTION-OF STATE @ IF & ['] & DEFER@ ELSE ' DEFER@ THEN ; "
    "  IMMEDIATE "
    ": IS STATE @ IF & ['] & DEFER! ELSE ' DEFER! THEN ; "
    "  IMMEDIATE "
    "DEFER INTERPRET "
    ": BEGIN HERE ; IMMEDIATE "
    ": AGAIN & BRANCH , ; IMMEDIATE "
    ": UNTIL & ?BRANCH , ; IMMEDIATE "
    ": WHILE & ?BRANCH HERE 0 , ; IMMEDIATE "
    ": REPEAT & BRANCH SWAP , HERE SWAP ! ; IMMEDIATE "
    ": ?EXECUTE1 DUP IF EXECUTE ELSE 2DROP THEN ; "
    ": ALLOCATE! ALLOCATE IF DROP ABORT THEN ; "
    ": FREE! FREE IF ABORT THEN ; "
    "IO-WORDLIST SET-CURRENT "
    ": FD>OUTPUT-CLEANUP FREE! ; "
    ": FD>OUTPUT-WRITE "
    "  OUTPUT-ARG @ IO-WRITE-ASYNC IO-ACTION-DESTROY ; "
    ": FD>OUTPUT "
    "  OUTPUT-SIZE ALLOCATE! TUCK OUTPUT-ARG ! "
    "  0 OVER OUTPUT-NEXT-OUTPUT ! "
    "  ['] FD>OUTPUT-WRITE OVER OUTPUT-WRITE ! "
    "  ['] FD>OUTPUT-CLEANUP OVER OUTPUT-CLEANUP ! ; "
    "TASK-WORDLIST SET-CURRENT "
    ": OLD-SPAWN SPAWN ; "
    ": SPAWN OLD-SPAWN IO-STDOUT FD>OUTPUT OVER >CONSOLE-OUTPUT "
    "  IO-STDERR FD>OUTPUT OVER >CONSOLE-ERROR ; "
    "IO-STDOUT FD>OUTPUT THIS-TASK 2DUP >CONSOLE-OUTPUT >OUTPUT "
    "IO-STDERR FD>OUTPUT THIS-TASK 2DUP >CONSOLE-ERROR >ERROR "
    "FORTH-WORDLIST SET-CURRENT "
    ": TYPE THIS-TASK OUTPUT> DUP IF "
    "    DUP OUTPUT-WRITE @ ?EXECUTE1 "
    "  ELSE "
    "    DROP "
    "  THEN ; "
    ": . FORMAT-NUMBER TYPE ; "
    ": U. FORMAT-UNSIGNED-NUMBER TYPE ; "
    ": BASE. ['] . SWAP BASE-EXECUTE ; "
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
    ": [CHAR] PARSE-NAME 1 >= IF C@ ELSE DROP 0 THEN & (LITERAL) , ; "
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
    ": COUNT-CHARS-UNTIL-UNESCAPED SOURCE >IN 0 0 BEGIN 3 PICK 3 PICK @ > IF "
    "    2 PICK @ 5 PICK + C@ OVER [CHAR] \\ <>  OVER 8 PICK = AND DUP IF ELSE "
    "      3 ROLL 1+ 3 ROLL 3 ROLL 3 ROLL "
    "    THEN 4 PICK DUP @ 1+ SWAP ! ROT [CHAR] \\ = 2 PICK [CHAR] \\ = AND IF "
    "      NIP 0 SWAP "
    "    THEN "
    "  ELSE "
    "    TRUE "
    "  THEN UNTIL DROP ROT ROT 2DROP ROT ROT 2DROP ; "
    ": ( [CHAR] ) SKIP-CHARS-UNTIL ; IMMEDIATE "
    ": \\ NEWLINE SKIP-CHARS-UNTIL ; IMMEDIATE "
    ": DO & (LITERAL) HERE DUP 0 , & >R & 2>R HERE ; IMMEDIATE "
    ": ?DO & (LITERAL) HERE 0 , & >R & 2DUP & 2>R & <> & ?BRANCH HERE 0 , HERE "
    "  ; IMMEDIATE "
    ": LOOP & 2R> & 1+ & 2DUP & 2>R & = & ?BRANCH , HERE SWAP ! HERE SWAP ! "
    "  & 2R> & 2DROP & R> & DROP ; IMMEDIATE "
    ": +LOOP & 2R> & ROT & DUP & 0>= & IF "
    "    & SWAP & DUP & (LITERAL) 3 , & PICK & < & ROT & ROT & + "
    "    & DUP & (LITERAL) 3 , & PICK & >= & ROT & AND & ROT & ROT & 2>R "
    "  & ELSE "
    "    & SWAP & DUP & (LITERAL) 3 , & PICK & >= & ROT & ROT & + "
    "    & DUP & (LITERAL) 3 , & PICK & < & ROT & AND & ROT & ROT & 2>R "
    "  & THEN "
    "  & ?BRANCH , HERE SWAP ! HERE SWAP ! & 2R> & 2DROP & R> & DROP ; "
    "  IMMEDIATE "
    ": LEAVE R> 2R> 2DROP >R ; "
    ": UNLOOP R> 2R> 2DROP R> DROP >R ; "
    ": I R> R@ SWAP >R ; "
    ": J 2R> 2R> R@ 4 ROLL 4 ROLL 4 ROLL 4 ROLL 2>R 2>R ; "
    ": COUNT ( addr -- addr1 u ) DUP C@ SWAP 1+ SWAP ; "
    ": FILL ( addr count char -- ) "
    "  SWAP 0 ?DO "
    "    DUP 2 PICK C! SWAP CHAR+ SWAP  "
    "  LOOP 2DROP ; "
    ": DISCARD ( x1 ... xn u -- ) 0 ?DO DROP LOOP ; "
    ": DEFINITIONS ( -- ) GET-ORDER SWAP SET-CURRENT DISCARD ; "
    ": FIND DUP COUNT FIND-WORD DUP 0 = IF "
    "    DROP 2DROP 0 "
    "  ELSE "
    "    ROT DROP "
    "  THEN ; "
    ": ALSO ( -- ) GET-ORDER OVER SWAP 1+ SET-ORDER ; "
    ": (WORDLIST) ( wid \"<name>\" -- ) "
    "  CREATE , DOES> @ >R GET-ORDER NIP R> SWAP SET-ORDER ; "
    "FORTH-WORDLIST (WORDLIST) FORTH "
    " : ONLY ( -- ) -1 SET-ORDER ; "
    ": PREVIOUS ( -- ) GET-ORDER NIP 1- SET-ORDER ; "
    ": ORDER ( -- ) GET-ORDER 0 ?DO 16 BASE. SPACE LOOP ; "
    "IO-WORDLIST SET-CURRENT "
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
    "TASK-WORDLIST SET-CURRENT "
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
    "  OLD-SPAWN TUCK >INIT-WORD SWAP DUP 0 ?DO "
    "    DUP 1+ ROLL 2 PICK >DATA "
    "  LOOP "
    "  DROP DUP START ; "
    ": SPAWN-SIMPLE-NO-IO-FREE-DATA-ON-EXIT ( ... count xt -- task ) "
    "  OLD-SPAWN TRUE OVER >FREE-DATA-ON-EXIT TUCK >INIT-WORD "
    "  SWAP DUP 0 ?DO "
    "    DUP 1+ ROLL 2 PICK >DATA "
    "  LOOP "
    "  DROP DUP START ; "
    ": SPAWN-SIMPLE-NO-DATA ( ... count xt -- task ) "
    "  SPAWN-NO-DATA TUCK >INIT-WORD SWAP DUP 0 ?DO "
    "    DUP 1+ ROLL 2 PICK >DATA "
    "  LOOP "
    "  DROP DUP START ; "
    "FORTH-WORDLIST SET-CURRENT "
    ": BUFFER: CREATE ALLOT ; "
    ": PARSE SOURCE DROP >IN @ + SWAP COUNT-CHARS-UNTIL ; "
    ": S\" SKIP-WHITESPACE [CHAR] \" PARSE DUP ALLOCATE! 2DUP & "
    "  (LITERAL) , & (LITERAL) , SWAP MOVE ; IMMEDIATE "
    ": C\" SKIP-WHITESPACE [CHAR] \" PARSE DUP 255 > IF DROP 255 THEN DUP 1+ "
    "  ALLOCATE! DUP & (LITERAL) , 2DUP C@ 1+ SWAP MOVE ; IMMEDIATE "
    ": ERASE BEGIN DUP 0 > WHILE 1 - SWAP 0 OVER C! 1 + SWAP REPEAT 2DROP ; "
    ": ABS DUP 0< IF NEGATE THEN ; "
    ": MIN 2DUP > IF SWAP THEN DROP ; "
    ": MAX 2DUP < IF SWAP THEN DROP ; "
    ": SPACES 0 MAX 0 ?DO SPACE LOOP ; "
    ": .\" SKIP-WHITESPACE [CHAR] \" PARSE DUP ALLOCATE! 2DUP & "
    "  (LITERAL) , & (LITERAL) , SWAP MOVE & TYPE ; IMMEDIATE "
    ": .( SKIP-WHITESPACE [CHAR] ) PARSE TYPE ; IMMEDIATE "
    ": ERROR\" SKIP-WHITESPACE [CHAR] \" PARSE DUP ALLOCATE! 2DUP & "
    "  (LITERAL) , & (LITERAL) , SWAP MOVE & "
    "  ERROR-TYPE ; IMMEDIATE "
    "IO-WORDLIST SET-CURRENT "
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
    "  TUCK INPUT-COUNT ! INPUT-BUFFER ! ; "
    ": SET-STRING-INPUT-DEFAULTS ( input -- ) "
    "  0 OVER INPUT-INDEX ! TRUE OVER INPUT-IS-CLOSED ! "
    "  ['] STRING>SOURCE-CLEANUP OVER INPUT-CLEANUP ! 0 OVER "
    "  INPUT-REFILL ! 0 OVER INPUT-ARG ! -1 OVER INPUT-SOURCE-ID ! "
    "  0 SWAP INPUT-NEXT-INPUT ! ; "
    ": STRING>SOURCE ( addr count -- input ) "
    "  COPY-BUFFER INPUT-SIZE ALLOCATE! DUP 3 ROLL 3 ROLL 3 ROLL "
    "  SET-INPUT-BUFFER DUP SET-STRING-INPUT-DEFAULTS ; "
    ": PUSH-INPUT ( input -- ) "
    "  THIS-TASK INPUT> OVER INPUT-NEXT-INPUT ! THIS-TASK >INPUT ; "
    "FORTH-WORDLIST SET-CURRENT "
    ": EVALUATE ( addr count -- ? ) STRING>SOURCE PUSH-INPUT INTERPRET ; "
    ": BACKSPACE 8 ; "
    ": ABORT\" & IF SKIP-WHITESPACE [CHAR] \" PARSE DUP ALLOCATE! "
    "  2DUP & (LITERAL) , & (LITERAL) , SWAP MOVE & "
    "  TYPE & ABORT & THEN ; IMMEDIATE "
    "IO-WORDLIST SET-CURRENT "
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
    ": REFILL-TERMINAL ( source -- flag ) "
    "  0 OVER INPUT-COUNT ! 0 OVER INPUT-INDEX ! "
    "  BEGIN DUP READ-TERMINAL-CHAR INVERT UNTIL DROP TRUE ; "
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
    "  0 OVER INPUT-SOURCE-ID ! 0 OVER INPUT-NEXT-INPUT ! ; "
    "6 CELLS CONSTANT FILE-SIZE "
    "0 CELLS OFFSET FILE-FD "
    "1 CELLS OFFSET FILE-BUFFER-SIZE "
    "2 CELLS OFFSET FILE-DATA-SIZE "
    "3 CELLS OFFSET FILE-BUFFER "
    "4 CELLS OFFSET FILE-IS-CLOSED "
    "5 CELLS OFFSEt FILE-IS-READ-ONLY "
    "256 CONSTANT DEFAULT-FILE-BUFFER-SIZE "
    "0 CONSTANT NO-ERROR "
    "-1 CONSTANT GENERIC-ERROR "
    ": INIT-FILE ( fd read-only -- fileid ) "
    "  FILE-SIZE ALLOCATE! DEFAULT-FILE-BUFFER-SIZE ALLOCATE! "
    "  OVER FILE-BUFFER ! DEFAULT-FILE-BUFFER-SIZE OVER FILE-BUFFER-SIZE ! "
    "  0 OVER FILE-DATA-SIZE ! FALSE OVER FILE-IS-CLOSED ! ROT OVER "
    "  FILE-FD ! TUCK FILE-IS-READ-ONLY ! ; "
    "FORTH-WORDLIST SET-CURRENT "
    ": BIN ( fam1 -- fam2 ) ; "
    ": FLUSH-FILE ( fileid -- ior ) "
    "  DUP FILE-IS-CLOSED @ INVERT IF "
    "    DUP FILE-IS-READ-ONLY @ INVERT IF "
    "      DUP FILE-BUFFER @ OVER FILE-DATA-SIZE @ 2 PICK FILE-FD @ "
    "      IO-WRITE-ASYNC IO-ACTION-DESTROY "
    "      0 SWAP FILE-DATA-SIZE ! NO-ERROR "
    "    ELSE "
    "      DROP NO-ERROR "
    "    THEN "
    "  ELSE "
    "    DROP GENERIC-ERROR "
    "  THEN ; "
    ": CLOSE-FILE ( fileid -- ) "
    "  DUP FILE-IS-CLOSED @ INVERT IF "
    "    DUP FLUSH-FILE DROP TRUE OVER FILE-IS-CLOSED ! "
    "    DUP FILE-FD @ IO-CLOSE-ASYNC IO-ACTION-DESTROY "
    "    DUP FILE-BUFFER @ FREE! FREE! "
    "  ELSE "
    "    DROP "
    "  THEN ; "
    ": OPEN-FILE ( c-addr u fam -- fileid ior ) "
    "  DUP 3 ROLL 3 ROLL 3 ROLL "
    "  IO-IRUSR IO-IWUSR OR IO-IRGRP OR IO-IWGRP OR IO-IROTH OR IO-IWOTH OR "
    "  IO-OPEN DUP IF "
    "    NIP NIP 0 SWAP "
    "  ELSE "
    "    DROP SWAP DUP IO-WRONLY AND 0= SWAP IO-RDWR AND 0= AND "
    "    INIT-FILE NO-ERROR "
    "  THEN ; "
    ": CREATE-FILE ( c-addr u fam -- fileid ior ) "
    "  IO-CREAT OR IO-TRUNC OR OPEN-FILE ; "
    ": DELETE-FILE ( c-addr u -- ior ) IO-DELETE ; "
    ": FILE-POSITION ( fileid -- ud ior ) "
    "  DUP FILE-IS-CLOSED @ INVERT IF "
    "    DUP FILE-FD @ IO-TELL DUP IO-ACTION-GET-STATE SWAP IO-ACTION-DESTROY "
    "    DUP IO-STATE-HAS-ERROR IF "
    "      IO-STATE-DESTROY DROP 0 GENERIC-ERROR "
    "    ELSE "
    "      DUP IO-STATE-GET-OFFSET SWAP IO-STATE-DESTROY "
    "      SWAP FILE-DATA-SIZE @ + NO-ERROR "
    "    THEN "
    "  ELSE "
    "    0 GENERIC-ERROR "
    "  THEN ; "
    ": REPOSITION-FILE ( ud fileid -- ior ) "
    "  DUP FILE-IS-CLOSED @ INVERT IF "
    "    FILE-FD @ IO-SEEK-SET SWAP IO-SEEK-ASYNC IO-ACTION-DESTROY NO-ERROR "
    "  ELSE "
    "    GENERIC-ERROR "
    "  THEN ; "
    ": RENAME-FILE ( c-addr1 u1 c-addr2 u2 -- ior ) IO-RENAME ; " 
    ": ACTION>ERROR ( action -- flag ) "
    "  DUP IO-ACTION-GET-STATE SWAP IO-ACTION-DESTROY "
    "  DUP IO-STATE-HAS-ERROR SWAP IO-STATE-DESTROY ; "
    ": FILE-SIZE ( fileid -- ud ior )  "
    "  DUP FILE-POSITION DUP INVERT IF "
    "    DROP 0 IO-SEEK-END 3 PICK FILE-FD @ IO-SEEK-BLOCK DUP "
    "    IO-ACTION-GET-STATE SWAP IO-ACTION-DESTROY "
    "    DUP IO-STATE-HAS-ERROR INVERT IF "
    "      DUP IO-STATE-GET-OFFSET SWAP IO-STATE-DESTROY "
    "      SWAP IO-SEEK-SET 3 ROLL FILE-FD @ IO-SEEK-BLOCK ACTION>ERROR "
    "      INVERT IF "
    "        NO-ERROR "
    "      ELSE "
    "        2DROP 0 GENERIC-ERROR "
    "      THEN "
    "    ELSE "
    "      IO-STATE-DESTROY 2DROP 0 GENERIC-ERROR "
    "  ELSE "
    "    2DROP 0 GENERIC-ERROR "
    "  THEN ; "
    ": FILE-STATUS ( fileid -- flag ) FILE-IS-CLOSED @ ; "
    "IO-RDONLY CONSTANT R/O "
    "IO-WRONLY CONSTANT W/O "
    "IO-RDWR CONSTANT R/W "
    ": READ-FILE ( c-addr u1 fileid -- u2 ior ) DEBUGGER "
    "  DUP FLUSH-FILE DUP NO-ERROR = IF "
    "    DROP FILE-FD @ IO-READ-BLOCK DUP IO-ACTION-GET-STATE SWAP "
    "    IO-ACTION-DESTROY DUP IO-STATE-HAS-ERROR IF "
    "      IO-ACTION-DESTROY 0 GENERIC-ERROR "
    "    ELSE "
    "      DUP IO-STATE-GET-INDEX SWAP IO-STATE-DESTROY NO-ERROR "
    "    THEN "
    "  ELSE "
    "    NIP 0 SWAP "
    "  THEN ; "
    ": ADVANCE-BUFFER ( c-addr1 u1 u2 -- c-addr3 u3 ) "
    "  DUP 3 ROLL + ROT ROT - ; "
    ": READ-LINE ( c-addr u1 fileid -- u2 flag ior ) "
    "  0 ROT 0 ?DO "
    "    2 PICK 1 2 PICK READ-FILE DUP NO-ERROR = IF "
    "      DROP 0= IF "
    "        ROT ROT 2DROP NIP DUP 0<> NO-ERROR LEAVE "
    "      ELSE "
    "        2 PICK C@ NEWLINE = IF "
    "          ROT ROT 2DROP 0 TRUE NO-ERROR LEAVE "
    "        ELSE "
    "          1+ "
    "        THEN "
    "      THEN "
    "    ELSE "
    "      ROT ROT 2DROP ROT ROT 2DROP 0 FALSE ROT "
    "    THEN "
    "  LOOP ; "
    ": FILL-BUFFER ( c-addr u1 fileid -- u2 ) "
    "  DUP FILE-BUFFER-SIZE @ OVER FILE-DATA-SIZE @ - 2 PICK < IF "
    "    DUP FILE-BUFFER-SIZE @ OVER FILE-DATA-SIZE @ - ROT DROP SWAP "
    "  THEN "
    "  ROT OVER FILE-BUFFER @ 2 ROLL FILE-DATA-SIZE @ + 2 PICK MOVE ; "
    ": WRITE-FILE ( c-addr u fileid -- ior ) "
    "  DUP FILE-IS-READ-ONLY @ IF "
    "    BEGIN "
    "      2 PICK 2 PICK 2 PICK FILL-BUFFER DUP 2 PICK < IF "
    "        DUP FLUSH-FILE DUP NO-ERROR = IF "
    "          3 ROLL 3 ROLL 2 ROLL ADVANCE-BUFFER 2 ROLL FALSE "
    "        ELSE "
    "          2DROP 2DROP GENERIC-ERROR TRUE "
    "        THEN "
    "      ELSE "
    "        NO-ERROR TRUE "
    "    AGAIN "
    "  ELSE "
    "    2DROP DROP GENERIC-ERROR "
    "  THEN ; "
    ": WRITE-LINE ( c-addr u fileid -- ior ) "
    "  DUP 3 ROLL 3 ROLL 3 ROLL WRITE-FILE DUP NO-ERROR = IF "
    "    DROP NEWLINE HERE C! HERE 1 ROT WRITE-FILE "
    "  THEN ; "
    "IO-WORDLIST SET-CURRENT "
    "3 CELLS CONSTANT FILE-SOURCE-INFO-SIZE "
    "0 CELLS OFFSET FILE-SOURCE-FILE "
    "1 CELLS OFFSET FILE-SOURCE-BUFFER-SIZE "
    "2 CELLS OFFSET FILE-SOURCE-DATA-SIZE "
    "256 CONSTANT DEFAULT-FILE-SOURCE-BUFFER-SIZE "
    ": ADVANCE-FILE ( source -- ) "
    "  DUP INPUT-BUFFER @ OVER INPUT-COUNT @ + OVER INPUT-BUFFER @ "
    "  2 PICK INPUT-ARG @ FILE-SOURCE-DATA-SIZE @ 3 PICK INPUT-COUNT @ - MOVE "
    "  DUP INPUT-ARG @ FILE-SOURCE-DATA-SIZE @ OVER INPUT-COUNT @ - "
    "  OVER INPUT-ARG @ FILE-SOURCE-DATA-SIZE ! "
    "  0 SWAP INPUT-COUNT ! ; "
    ": FIND-FILE-SOURCE-NEWLINE ( source -- ) "
    "  DUP INPUT-IS-CLOSED @ INVERT IF "
    "    BEGIN "
    "      DUP INPUT-COUNT @ OVER INPUT-ARG @ FILE-SOURCE-DATA-SIZE @ < IF "
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
    "    DUP INPUT-ARG @ FILE-SOURCE-DATA-SIZE @ SWAP INPUT-COUNT ! TRUE "
    "  THEN ; "
    ": TRY-EXPAND-FILE-SOURCE-BUFFER ( source -- ) "
    "  DUP INPUT-ARG @ FILE-SOURCE-BUFFER-SIZE @ "
    "  OVER INPUT-ARG @ FILE-SOURCE-DATA-SIZE @ = IF "
    "    DUP INPUT-BUFFER @ OVER INPUT-ARG @ FILE-SOURCE-BUFFER-SIZE @ 2 * RESIZE "
    "    OVER INPUT-BUFFER ! "
    "    DUP INPUT-ARG @ FILE-SOURCE-BUFFER-SIZE @ 2 * "
    "    SWAP INPUT-ARG @ FILE-SOURCE-BUFFER-SIZE ! "
    "  ELSE "
    "    DROP "
    "  THEN ; "
    ": TRY-READ-FILE-SOURCE-LINE ( source -- ) "
    "  DUP TRY-EXPAND-FILE-SOURCE-BUFFER "
    "  DUP INPUT-BUFFER @ OVER INPUT-ARG @ FILE-SOURCE-DATA-SIZE @ + "
    "  OVER INPUT-ARG @ DUP FILE-SOURCE-BUFFER-SIZE @ SWAP FILE-SOURCE-DATA-SIZE @ - "
    "  2 PICK INPUT-ARG @ FILE-SOURCE-FILE @ READ-FILE NO-ERROR <> IF "
    "    DROP TRUE SWAP INPUT-IS-CLOSED ! "
    "  ELSE "
    "    DUP 0= IF "
    "      DROP TRUE SWAP INPUT-IS-CLOSED ! "
    "    ELSE "
    "      OVER INPUT-ARG @ TUCK "
    "      FILE-SOURCE-DATA-SIZE @ + SWAP FILE-SOURCE-DATA-SIZE ! "
    "    THEN "
    "  THEN ; "
    ": REFILL-FILE ( source -- flag ) "
    "  DUP INPUT-INDEX @ OVER INPUT-COUNT @ = OVER INPUT-IS-CLOSED @ AND IF "
    "    DROP FALSE "
    "  ELSE "
    "    0 OVER INPUT-INDEX ! "
    "    DUP ADVANCE-FILE BEGIN DUP FIND-FILE-SOURCE-NEWLINE INVERT WHILE "
    "      DUP TRY-READ-FILE-SOURCE-LINE "
    "    REPEAT DROP TRUE "
    "  THEN ; "
    ": FILE>SOURCE-CLEANUP ( source -- ) "
    "  DUP INPUT-ARG @ FILE-SOURCE-FILE @ CLOSE-FILE "
    "  DUP INPUT-BUFFER @ FREE! DUP INPUT-ARG @ FREE! FREE! ; "
    ": FILE>SOURCE ( fileid -- input ) "
    "  INPUT-SIZE ALLOCATE! FILE-SOURCE-INFO-SIZE ALLOCATE! "
    "  DEFAULT-FILE-SOURCE-BUFFER-SIZE OVER FILE-SOURCE-BUFFER-SIZE ! "
    "  0 OVER FILE-SOURCE-DATA-SIZE ! "
    "  2 PICK OVER FILE-SOURCE-FILE ! "
    "  2 PICK 2 PICK INPUT-SOURCE-ID ! "
    "  OVER INPUT-ARG ! "
    "  DEFAULT-FILE-SOURCE-BUFFER-SIZE ALLOCATE! OVER INPUT-BUFFER ! "
    "  0 OVER INPUT-INDEX ! 0 OVER INPUT-COUNT ! "
    "  FALSE OVER INPUT-IS-CLOSED ! "
    "  ['] FILE>SOURCE-CLEANUP OVER INPUT-CLEANUP ! "
    "  ['] REFILL-FILE OVER INPUT-REFILL ! "
    "  0 OVER INPUT-NEXT-INPUT ! NIP ; "
    "FORTH-WORDLIST SET-CURRENT "
    ": REFILL ( -- flag ) THIS-TASK INPUT> DUP INPUT-REFILL @ DUP IF "
    "    EXECUTE "
    "  ELSE "
    "    2DROP FALSE "
    "  THEN ; "
    ": SOURCE-ID ( -- id ) THIS-TASK INPUT> INPUT-SOURCE-ID @ ; "
    ": INCLUDED ( addr count -- ? ) "
    "  R/O OPEN-FILE ABORT\" Unable to open file \" "
    "  FILE>SOURCE PUSH-INPUT INTERPRET ; "
    ": INCLUDE ( \"filename\" -- ? ) "
    "  PARSE-NAME INCLUDED ; "
    ": ADVANCE-TO-END SOURCE NIP >IN ! ; "
    ": TIME+ ( s1 ns1 s2 ns2 -- s ns ) "
    "  DUP 3 PICK + 1000000000 > IF "
    "    2 ROLL + 1000000000 - ROT ROT + 1+ SWAP "
    "  ELSE "
    "    2 ROLL + ROT ROT + SWAP "
    "  THEN ; "
    ": FIX-TIME ( s ns -- s ns ) "
    "    DUP 1000000000 / ROT + SWAP 1000000000 MOD ; "
    "TASK-WORDLIST SET-CURRENT "
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
    "  COND-SIZE ALLOCATE! TUCK COND-PRIMARY ! 0 OVER COND-SECONDARY ! "
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
    "FORTH-WORDLIST SET-CURRENT "
    ":NONAME "
    "  THIS-TASK INPUT> INPUT-IS-CLOSED @ INVERT BEGIN "
    "    IF REFILL ELSE TRUE THEN IF "
    "      BEGIN "
    "        PARSE-NAME DUP 0<> IF "
    "          STATE @ IF "
    "            FIND-WORD DUP 0<> IF "
    "              1 = IF "
    "                EXECUTE "
    "              ELSE "
    "                COMPILE, "
    "              THEN "
    "            ELSE "
    "              DROP 2DUP PARSE-NUMBER 0= IF "
    "                DROP ADVANCE-TO-END .\" Unable to parse: \" TYPE SPACE "
    "              ELSE "
    "                ROT ROT 2DROP & (LITERAL) , "
    "              THEN "
    "            THEN "
    "          ELSE "
    "            FIND-WORD 0<> IF "
    "              EXECUTE "
    "            ELSE "
    "              2DUP PARSE-NUMBER 0= IF "
    "                DROP ADVANCE-TO-END .\" Unable to parse: \" TYPE SPACE "
    "              ELSE "
    "                ROT ROT 2DROP "
    "              THEN "
    "            THEN "
    "          THEN FALSE"
    "        ELSE "
    "          2DROP THIS-TASK INPUT> THIS-TASK CONSOLE-INPUT> = IF "
    "            STATE @ IF "
    "              SPACE .\" compiled\" CR "
    "            ELSE "
    "              SPACE .\" ok\" CR "
    "            THEN "
    "          THEN TRUE "
    "        THEN "
    "      UNTIL TRUE FALSE "
    "    ELSE "
    "      THIS-TASK DROP-INPUT TRUE "
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
    "FORTH-WORDLIST 1 SET-ORDER "
    "BEGIN-INTERACTIVE ";
  if(!(input = af_new_string_input(global, code, (af_cell_t)strlen(code)))) {
    af_handle_out_of_memory(global, task);
    return;
  }
  task->current_input = input;
  af_interpret(global, task);
}
