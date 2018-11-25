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

DECIMAL
FORTH-WORDLIST TASK-WORDLIST 2 SET-ORDER
TASK-WORDLIST SET-CURRENT

REQUIRE src/forth/cond.fs

BEGIN-STRUCTURE CHAN-SIZE
  FIELD: CHAN-LOCK
  FIELD: CHAN-RECV-COND
  FIELD: CHAN-QUEUE
  FIELD: CHAN-QUEUE-COUNT
  FIELD: CHAN-QUEUE-SIZE
  FIELD: CHAN-ENQUEUE-INDEX
  FIELD: CHAN-DEQUEUE-INDEX
END-STRUCTURE

8 CONSTANT CHAN-QUEUE-DEFAULT-SIZE

: CHAN. ( chan -- )
  CR ." CHAN-LOCK: " DUP CHAN-LOCK @ .
  CR ." CHAN-RECV-COND: " DUP CHAN-RECV-COND @ .
  CR ." CHAN-QUEUE: " DUP CHAN-QUEUE @ .
  CR ." CHAN-QUEUE-COUNT: " DUP CHAN-QUEUE-COUNT @ .
  CR ." CHAN-QUEUE-SIZE: " DUP CHAN-QUEUE-SIZE @ .
  CR ." CHAN-ENQUEUE-INDEX: " CHAN-ENQUEUE-INDEX @ .
  CR ." CHAN-DEQUEUE-INDEX: " CHAN-DEQUEUE-INDEX @ . CR ;

: NEW-CHAN ( -- chan )
  CHAN-SIZE ALLOCATE!
  CHAN-QUEUE-DEFAULT-SIZE OVER CHAN-QUEUE-SIZE !
  0 OVER CHAN-QUEUE-COUNT !
  0 OVER CHAN-ENQUEUE-INDEX !
  0 OVER CHAN-DEQUEUE-INDEX !
  CHAN-QUEUE-DEFAULT-SIZE CELLS ALLOCATE! OVER CHAN-QUEUE !
  NEW-LOCK OVER CHAN-LOCK !
  DUP CHAN-LOCK @ NEW-COND OVER CHAN-RECV-COND ! ;

: DESTROY-CHAN ( chan -- )
  DUP CHAN-RECV-COND @ DESTROY-COND
  DUP CHAN-LOCK @ DESTROY-LOCK
  DUP CHAN-QUEUE @ FREE!
  FREE! ;


: DEQUEUE-CHAN ( chan -- x )
  DUP CHAN-QUEUE @ OVER CHAN-DEQUEUE-INDEX @ CELLS + @
  OVER CHAN-QUEUE-COUNT @ 1- 2 PICK CHAN-QUEUE-COUNT !
  OVER CHAN-DEQUEUE-INDEX @ 1+ 2 PICK CHAN-QUEUE-SIZE @ MOD
  ROT CHAN-DEQUEUE-INDEX ! ;

: EXPAND-CHAN ( chan -- )
  DUP CHAN-QUEUE-SIZE @ 2 *
  DUP CELLS ALLOCATE!
  DUP 3 PICK CHAN-QUEUE-COUNT @ 0 ?DO
    3 PICK CHAN-QUEUE @ 4 PICK CHAN-DEQUEUE-INDEX @ CELLS + @
    OVER ! CELL+
    3 PICK CHAN-DEQUEUE-INDEX @ 1+ 4 PICK CHAN-QUEUE-SIZE @ MOD
    4 PICK CHAN-DEQUEUE-INDEX !
  LOOP
  DROP
  2 PICK CHAN-QUEUE FREE!
  2 PICK CHAN-QUEUE !
  OVER CHAN-QUEUE-SIZE !
  0 OVER CHAN-DEQUEUE-INDEX !
  DUP CHAN-QUEUE-COUNT @ SWAP CHAN-ENQUEUE-INDEX ! ;

: ENQUEUE-CHAN ( x chan -- )
  DUP CHAN-QUEUE-COUNT @ OVER CHAN-QUEUE-SIZE @ >= IF DUP EXPAND-CHAN THEN
  TUCK CHAN-QUEUE @ 2 PICK CHAN-ENQUEUE-INDEX @ CELLS + !
  DUP CHAN-ENQUEUE-INDEX @ 1+ OVER CHAN-QUEUE-SIZE @ MOD
  SWAP CHAN-ENQUEUE-INDEX ! ;

\ : DEQUEUE-CHAN ( chan -- x )
\   DUP CHAN-QUEUE @ @
\   OVER CHAN-QUEUE-COUNT @ 1- 2 PICK CHAN-QUEUE-COUNT !
\   OVER CHAN-QUEUE @ CELL+ 2 PICK CHAN-QUEUE @
\   3 ROLL CHAN-QUEUE-COUNT @ CELLS MOVE ;
\ 
\ : ENQUEUE-CHAN ( x chan -- )
\   DUP CHAN-QUEUE-COUNT @ OVER CHAN-QUEUE-SIZE @ >= IF
\     DUP CHAN-QUEUE-SIZE @ 2 *
\     OVER CHAN-QUEUE @ OVER CELLS RESIZE DROP 2 PICK CHAN-QUEUE !
\     OVER CHAN-QUEUE-SIZE !
\   THEN
\   TUCK CHAN-QUEUE @ 2 PICK CHAN-QUEUE-COUNT @ CELLS + !
\   1 SWAP CHAN-QUEUE-COUNT +! ;

: SEND-CHAN ( x chan -- )
  DUP CHAN-LOCK @ LOCK
  TUCK ENQUEUE-CHAN
  DUP CHAN-RECV-COND @ SIGNAL-COND
  DUP CHAN-QUEUE-COUNT @ OVER CHAN-QUEUE-SIZE @ >= IF
    CHAN-LOCK @ UNLOCK
  ELSE
    \ CHAN-LOCK @ UNLOCK-NO-RELOCK
    32 SWAP CHAN-LOCK @ UNLOCK-COUNT-RELOCK
  THEN ;

: RECV-CHAN ( chan -- x )
  DUP CHAN-LOCK @ LOCK
  BEGIN DUP CHAN-QUEUE-COUNT @ 0= WHILE
    DUP CHAN-RECV-COND @ WAIT-COND
  REPEAT
  DUP DEQUEUE-CHAN
  OVER CHAN-QUEUE-SIZE @ 0= IF
    SWAP CHAN-LOCK @ UNLOCK
  ELSE
    SWAP CHAN-LOCK @ UNLOCK-NO-RELOCK
  THEN ;

: PEEK-CHAN ( chan -- x )
  DUP CHAN-LOCK @ LOCK
  BEGIN DUP CHAN-QUEUE-COUNT @ 0= WHILE
    DUP CHAN-RECV-COND @ WAIT-COND
  REPEAT
  DUP CHAN-QUEUE @ @
  OVER CHAN-QUEUE-SIZE @ 0= IF
    SWAP CHAN-LOCK @ UNLOCK
  ELSE
    SWAP CHAN-LOCK @ UNLOCK-NO-RELOCK
  THEN ;

: TRY-RECV-CHAN ( chan -- x found )
  DUP CHAN-LOCK @ LOCK
  DUP CHAN-QUEUE-COUNT @ 0> IF
    DUP DEQUEUE-CHAN
    OVER CHAN-QUEUE-COUNT @ 0> IF
      \ SWAP CHAN-LOCK @ UNLOCK-NO-RELOCK
      32 ROT CHAN-LOCK @ UNLOCK-COUNT-RELOCK
    ELSE
      SWAP CHAN-LOCK @ UNLOCK
    THEN
    TRUE
  ELSE
    CHAN-LOCK @ UNLOCK
    0 FALSE
  THEN ;

: TRY-PEEK-CHAN ( chan -- x found )
  DUP CHAN-LOCK @ LOCK
  DUP CHAN-QUEUE-COUNT @ 0> IF
    DUP CHAN-QUEUE @ @
    OVER CHAN-QUEUE-COUNT @ 0> IF
      SWAP CHAN-LOCK @ UNLOCK-NO-RELOCK
    ELSE
      SWAP CHAN-LOCK @ UNLOCK
    THEN
    TRUE
  ELSE
    CHAN-LOCK @ UNLOCK
    0 FALSE
  THEN ;

: COUNT-CHAN ( chan -- u ) CHAN-QUEUE-COUNT @ ;

BASE ! SET-CURRENT SET-ORDER