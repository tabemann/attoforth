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

GET-ORDER GET-CURRENT BASE @

REQUIRE lambda.fs

DECIMAL
FORTH-WORDLIST 1 SET-ORDER
FORTH-WORDLIST SET-CURRENT

FORTH-WORDLIST LAMBDA-WORDLIST 2 SET-ORDER
LAMBDA-WORDLIST SET-CURRENT

: OPTION ( ... flag true-xt -- ... )
  STATE @ IF 
    & SWAP & IF & EXECUTE & ELSE & DROP & THEN
  ELSE
    SWAP IF EXECUTE ELSE DROP THEN
  THEN ; IMMEDIATE SMART

: CHOOSE ( ... flag true-xt false-xt -- ... )
  STATE @ IF
    & ROT & IF & DROP & EXECUTE & ELSE & NIP & EXECUTE & THEN
  ELSE
    ROT IF DROP EXECUTE ELSE NIP EXECUTE THEN
  THEN ; IMMEDIATE SMART

: LOOP-UNTIL ( ... xt -- ... )
  STATE @ IF
    & >R & BEGIN & R@ & EXECUTE & UNTIL & R> & DROP
  ELSE
    >R BEGIN R@ EXECUTE UNTIL R> DROP
  THEN ; IMMEDIATE SMART

: WHILE-LOOP ( ... while-xt body-xt -- ... )
  STATE @ IF
    & 2>R & BEGIN & 2R@ & DROP & EXECUTE & WHILE & R@ & EXECUTE & REPEAT
    & 2R> & 2DROP
  ELSE
    2>R BEGIN 2R@ DROP EXECUTE WHILE R@ EXECUTE REPEAT 2R> 2DROP
  THEN ; IMMEDIATE SMART

: COUNT-LOOP ( ... limit init xt -- ... )
  STATE @ IF
    & ROT & ROT & ?DO & I & SWAP & DUP & >R & EXECUTE & R> & LOOP & DROP
  ELSE
    ROT ROT ?DO I SWAP DUP >R EXECUTE R> LOOP DROP
  THEN ; IMMEDIATE SMART

: COUNT+LOOP ( ... limit init xt -- ... )
  STATE @ IF
    & ROT & ROT & ?DO & I & SWAP & DUP & >R & EXECUTE & R> & SWAP & +LOOP & DROP
  ELSE
    ROT ROT ?DO I SWAP DUP >R EXECUTE R> SWAP +LOOP DROP
  THEN ; IMMEDIATE SMART

: FETCH-ADVANCE ( a-addr1 count1 -- a-addr2 count2 x )
  SWAP DUP @ ROT 1- ROT CELL+ SWAP ROT ;

: (MAP) ( a-addr1 count1 a-addr2 xt -- count2 )
  0
  [: 3 PICK 0> ;]
  [: 4 ROLL 4 ROLL FETCH-ADVANCE 4 PICK EXECUTE
     5 ROLL TUCK ! CELL+ 3 ROLL 1+ 4 ROLL SWAP ;] WHILE-LOOP
  ROT ROT 2DROP ROT ROT 2DROP ;

: MAP ( a-addr1 count1 a-addr2 xt -- a-addr2 count2 )
  3 ROLL 3 ROLL 3 PICK 3 ROLL (MAP) ;

: (FILTER) ( a-addr1 count1 a-addr2 xt -- count2 )
  0
  [: 3 PICK 0> ;]
  [: 4 ROLL 4 ROLL FETCH-ADVANCE DUP 5 PICK EXECUTE
     [: ( a2 xt c2 a1 c1 x ) 5 ROLL TUCK ! CELL+ 3 ROLL 1+ 4 ROLL SWAP ;]
     [: DROP 4 ROLL 4 ROLL 4 ROLL ;] CHOOSE ;] WHILE-LOOP
  ROT ROT 2DROP ROT ROT 2DROP ;

: FILTER ( a-addr1 count1 a-addr2 xt -- a-addr2 count2 )
  3 ROLL 3 ROLL 3 PICK 3 ROLL (FILTER) ;

: (FILTER-MAP) ( a-addr1 count1 a-addr2 xt-filter xt-map -- count2 )
  0
  [: 4 PICK 0> ;]
  [: 5 ROLL 5 ROLL FETCH-ADVANCE DUP 6 PICK EXECUTE
     [: 4 PICK EXECUTE 6 ROLL TUCK ! CELL+ 3 ROLL 1+ 5 ROLL 5 ROLL ROT ;]
     [: DROP 5 ROLL 5 ROLL 5 ROLL 5 ROLL ;] CHOOSE ;] WHILE-LOOP
  ROT ROT 2DROP ROT ROT 2DROP NIP ;

: FILTER-MAP ( a-addr1 count1 a-addr2 xt-filter xt-map -- a-addr2 count2 )
  4 ROLL 4 ROLL 4 PICK 4 ROLL 4 ROLL (FILTER-MAP) ;

BASE ! SET-CURRENT SET-ORDER
