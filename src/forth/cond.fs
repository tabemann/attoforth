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

BEGIN-STRUCTURE COND-SIZE
  FIELD: COND-PRIMARY
  FIELD: COND-SECONDARY
  FIELD: COND-WAITING
  FIELD: COND-WAITING-COUNT
  FIELD: COND-WAITING-SIZE
END-STRUCTURE

8 CONSTANT DEFAULT-COND-WAITING-SIZE

: NEW-COND ( lock -- cond )
  COND-SIZE ALLOCATE! TUCK COND-PRIMARY ! 0 OVER COND-SECONDARY !
  DEFAULT-COND-WAITING-SIZE CELLS ALLOCATE! OVER COND-WAITING !
  0 OVER COND-WAITING-COUNT !
  DEFAULT-COND-WAITING-SIZE OVER COND-WAITING-SIZE ! ;

: DESTROY-COND ( lock -- ) DUP COND-WAITING @ FREE! FREE! ;

: WAIT-COND ( cond -- )
  DUP COND-SECONDARY SIMPLE-LOCK
  THIS-TASK OVER COND-PRIMARY @ HAS-LOCK IF
    DUP COND-WAITING-COUNT @ OVER COND-WAITING-SIZE @ = IF
      DUP COND-WAITING-SIZE @ DUP 2 * DUP 3 PICK COND-WAITING-SIZE !
      CELLS ALLOCATE! DUP 3 PICK COND-WAITING @
      SWAP DUP ROT 4 ROLL CELLS MOVE FREE! OVER COND-WAITING !
    THEN
    THIS-TASK OVER COND-WAITING-COUNT @ CELLS 2 PICK
    COND-WAITING @ + ! DUP COND-WAITING-COUNT @ 1+
    OVER COND-WAITING-COUNT !
    DUP COND-SECONDARY SIMPLE-UNLOCK
    DUP COND-PRIMARY @ UNLOCK THIS-TASK WAIT COND-PRIMARY @ LOCK
  ELSE
    COND-SECONDARY SIMPLE-UNLOCK TRUE
    ABORT" Attempted to wait on condition not belonging to current task "
  THEN ;

: SIGNAL-COND ( cond -- )
  DUP COND-SECONDARY SIMPLE-LOCK
  DUP COND-WAITING-COUNT @ 0= IF
    COND-SECONDARY SIMPLE-UNLOCK
  ELSE
    DUP COND-WAITING @ DUP @ SWAP DUP CELL+ SWAP 3 PICK
    COND-WAITING-COUNT @ 1- DUP 5 PICK COND-WAITING-COUNT ! CELLS MOVE
    WAKE COND-SECONDARY SIMPLE-UNLOCK
  THEN ;

: BROADCAST-COND ( cond -- )
  DUP COND-SECONDARY SIMPLE-LOCK
  DUP COND-WAITING-COUNT @ 0 ?DO
    DUP COND-WAITING @ I CELLS + @ WAKE
  LOOP
  0 OVER COND-WAITING-COUNT ! COND-SECONDARY SIMPLE-UNLOCK ;

BASE ! SET-CURRENT SET-ORDER