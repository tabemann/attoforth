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

FORTH-WORDLIST TASK-WORDLIST IO-WORDLIST 3 SET-ORDER
FORTH-WORDLIST SET-CURRENT

REQUIRE src/forth/chan.fs

256 CONSTANT TASK-COUNT
1024 CONSTANT CYCLES

TASK-COUNT CELLS BUFFER: CHANS

NEW-CHAN CONSTANT END-CHAN

: INIT-CHANS ( -- ) TASK-COUNT 0 ?DO NEW-CHAN CHANS I CELLS + ! LOOP ;

: RING-TASK ( u -- )
  DUP TASK-COUNT 1- < IF
    CYCLES 0 ?DO
      DUP CELLS CHANS + @ RECV-CHAN DROP
      DUP 1+ CELLS CHANS + @ 0 SWAP SEND-CHAN
    LOOP
    DROP
  ELSE
    CYCLES 0 ?DO
      DUP CELLS CHANS + @ RECV-CHAN DROP
      0 CELLS CHANS + @ 0 SWAP SEND-CHAN
    LOOP
    0 CELLS CHANS + @ RECV-CHAN DROP
    0 END-CHAN SEND-CHAN
    DROP
  THEN ;  

: INIT-TASKS ( -- )
  TASK-COUNT 0 ?DO I 1 ['] RING-TASK SPAWN-SIMPLE-FREE-DATA-ON-EXIT DROP LOOP ;

: DO-RING ( -- )
  INIT-CHANS
  IO-GET-MONOTONIC-TIME CONVERT-TO-NS
  INIT-TASKS
  0 0 CELLS CHANS + @ SEND-CHAN
  END-CHAN RECV-CHAN DROP
  IO-GET-MONOTONIC-TIME CONVERT-TO-NS 2SWAP D- 1000000. D/
  CR ." Completed " CYCLES . ." cycles with " TASK-COUNT . ." tasks in "
  D. ." ms" ;

DO-RING