\ Copyright (c) 2018, Travis Bemann
\ All rights reserved.
\ 
\ Redistribution and use in source and binary forms, with or without
\ modification, are permitted provided that the following conditions are met:
\ 
\ 1. Redistributions of source code must retain the above copyright notice,
\    this list of conditions and the following disclaimer.
\ 
\ 2. Redistributions in binary form must reproduce the above copyright notice,
\    this list of conditions and the following disclaimer in the documentation
\    and/or other materials provided with the distribution.
\ 
\ 3. Neither the name of the copyright holder nor the names of its
\    contributors may be used to endorse or promote products derived from
\    this software without specific prior written permission.
\ 
\ THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
\ AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
\ IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
\ ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
\ LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
\ CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
\ SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
\ INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
\ CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
\ ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
\ POSSIBILITY OF SUCH DAMAGE.

GET-CURRENT

FORTH-WORDLIST SET-CURRENT

GET-ORDER FORTH-WORDLIST 1 SET-ORDER

: WORDS ( -- )
  GET-ORDER NIPS FIRST-WORD BEGIN DUP 0<> WHILE
    DUP NAME>STRING TYPE SPACE NAME>NEXT
  REPEAT
  DROP ;

: IS-WORD ( xt -- flag )
  ALL-WORDS BEGIN
    DUP 0<> IF
      2DUP = IF
        2DROP TRUE TRUE
      ELSE
        NAME>NEXT-ALL FALSE
      THEN
    ELSE
      2DROP FALSE TRUE
    THEN
  UNTIL ;

: JUMP-IF-NEXT-NULL ( addr -- addr ) DUP CELL+ @ 0= IF @ ELSE CELL+ THEN ;

: COLON-END-ADDRESS ( xt -- addr )
  NAME>SECONDARY BEGIN DUP @ DUP 0<> WHILE
    DUP ['] (LITERAL) = OVER ['] ?BRANCH = OR OVER ['] DOES-AT> = OR IF
      DROP 2 CELLS +
    ELSE DUP ['] (FLITERAL) = IF
      DROP CELL+ FLOAT+
    ELSE ['] BRANCH = IF
      CELL+ JUMP-IF-NEXT-NULL
    ELSE
      CELL+
    THEN THEN THEN
  REPEAT
  DROP ;

: ALLOCATE-BRANCH-DATA ( xt -- addr )
  DUP COLON-END-ADDRESS SWAP NAME>SECONDARY - ALLOCATE! ;

: ADD-TO-BRANCH-DATA ( u addr u -- u )
  2DUP 0 ?DO
    DUP @ 4 PICK = IF
      DROP ROT ROT 2DROP UNLOOP EXIT
    ELSE
      CELL+
    THEN
  LOOP
  DROP DUP CELLS ROT + ROT SWAP ! 1+ ;

: EXTRACT-BRANCH-DATA ( xt -- addr u )
  DUP ALLOCATE-BRANCH-DATA DUP 0 3 ROLL NAME>SECONDARY BEGIN DUP @ DUP 0<> WHILE
    DUP ['] (LITERAL) = OVER ['] DOES-AT> = OR IF
      DROP 2 CELLS +
    ELSE DUP ['] (FLITERAL) = IF
      DROP CELL+ FLOAT+
    ELSE DUP ['] BRANCH = IF
      DROP CELL+ DUP @ 3 ROLL DUP ROT SWAP 4 ROLL ADD-TO-BRANCH-DATA ROT
      JUMP-IF-NEXT-NULL
    ELSE ['] ?BRANCH = IF
      CELL+ DUP @ 3 ROLL DUP ROT SWAP 4 ROLL ADD-TO-BRANCH-DATA ROT CELL+
    ELSE
      CELL+
    THEN THEN THEN THEN
  REPEAT
  2DROP NIP ;

: SEARCH-BRANCH-DATA ( addr u addr -- i flag )
  SWAP 0 ?DO
    SWAP DUP @ 2 PICK = IF
      2DROP I UNLOOP TRUE EXIT
    THEN
    CELL+ SWAP
  LOOP
  2DROP 0 FALSE ;

: DECOMPILE ( xt -- )
  DUP EXTRACT-BRANCH-DATA ROT NAME>SECONDARY BEGIN DUP @ DUP 0<> WHILE
    3 PICK 3 PICK 3 PICK SEARCH-BRANCH-DATA IF
      ." *" (.) ." * "
    ELSE
      DROP
    THEN
    DUP NAME>STRING TYPE SPACE
    DUP ['] (LITERAL) = IF
      DROP CELL+ DUP @ DUP IS-WORD IF
        ." <" NAME>STRING TYPE ." > "
      ELSE
        .
      THEN
      CELL+
    ELSE DUP ['] (FLITERAL) = IF
      DROP CELL+ DUP F@ FS. FLOAT+
    ELSE DUP ['] BRANCH = IF
      DROP CELL+ 2 PICK 2 PICK 2 PICK @ SEARCH-BRANCH-DATA IF
        ." <" (.) ." > "
      ELSE
        DROP DUP @ .
      THEN
      JUMP-IF-NEXT-NULL
    ELSE DUP ['] ?BRANCH = IF
      DROP CELL+ 2 PICK 2 PICK 2 PICK @ SEARCH-BRANCH-DATA IF
        ." <" (.) ." > "
      ELSE
        DROP DUP @ .
      THEN
      CELL+
    ELSE ['] DOES-AT> = IF
      CELL+ DUP @ . CELL+
    ELSE
      CELL+
    THEN THEN THEN THEN THEN
  REPEAT
  2DROP DROP FREE! ;

: SEE ( "<spaces>name" -- )
  ' DUP NAME>CODE DUP PRIM-DOCOL = IF
    DROP DECOMPILE
  ELSE DUP PRIM-PUSH = IF
    2DROP ." <created> "
  ELSE PRIM-DOES = IF
    DROP ." <does> " DECOMPILE
  ELSE
    DROP ." <primitive> "
  THEN THEN THEN ;

SET-ORDER SET-CURRENT
