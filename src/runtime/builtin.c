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
    ": CELL+ 8 + ; "
    ": CELLS 8 * ; "
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
    ": NEWLINE 10 ; "
    ": SKIP-SINGLE-WHITESPACE SOURCE >IN 2DUP @ > IF "
    "    DUP @ 3 PICK + @ DUP 32 = OVER NEWLINE = OR IF "
    "      DUP @ 1+ SWAP ! 2DROP "
    "    ELSE 2DROP DROP THEN "
    "  ELSE 2DROP DROP THEN ; "
    ": SKIP-CHARS-UNTIL SOURCE >IN BEGIN 2DUP @ > IF "
    "    DUP @ 3 PICK + @ 4 PICK = OVER DUP @ 1+ SWAP ! "
    "  ELSE TRUE THEN UNTIL 2DROP 2DROP ; "
    ": COUNT-CHARS-UNTIL SOURCE >IN 0 BEGIN 2 PICK 2 PICK @ > IF "
    "    OVER @ 4 PICK + @ 5 PICK = DUP IF ELSE SWAP 1+ SWAP THEN "
    "    2 PICK DUP @ 1+ SWAP ! "
    "  ELSE TRUE THEN UNTIL ROT ROT 2DROP ROT ROT 2DROP ; "
    ": ( [CHAR] ) SKIP-CHARS-UNTIL ; IMMEDIATE "
    ": \\ NEWLINE SKIP-CHARS-UNTIL ; IMMEDIATE "
    ": S\" SKIP-SINGLE_WHITESPACE SOURCE DROP >IN @ + "
    "  [CHAR] \" COUNT-CHARS-UNTIL DUP ALLOCATE 2DUP POSTPONE (LITERAL) , "
    "  POSTPONE (LITERAL) , SWAP MOVE ; IMMEDIATE "
    ": C\" SKIP-SINGLE_WHITESPACE SOURCE DROP >IN @ + "
    "  [CHAR] \" COUNT-CHARS-UNTIL DUP 255 > IF DROP 255 THEN DUP 1+ ALLOCATE "
    "  DUP POSTPONE (LITERAL) , 2DUP C@ 1+ SWAP MOVE ; IMMEDIATE "
    ": ABS DUP 0< IF NEGATE THEN ; "
    ": MIN 2DUP > IF SWAP THEN DROP ; "
    ": MAX 2DUP < IF SWAP THEN DROP ; ";
  af_evaluate(global, thread, code, (uint64_t)strlen(code));
}
