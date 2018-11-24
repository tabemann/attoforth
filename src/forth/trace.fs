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
FORTH-WORDLIST 1 SET-ORDER
FORTH-WORDLIST SET-CURRENT

VARIABLE TRACE-INSTRUMENT-ENABLE FALSE TRACE-INSTRUMENT-ENABLE !
VARIABLE TRACE-ENABLE FALSE TRACE-ENABLE !

: SET-TRACE-INSTRUMENT-ENABLE ( f -- ) TRACE-INSTRUMENT-ENABLE ! ;

: GET-TRACE-INSTRUMENT-ENABLE ( -- f ) TRACE-INSTRUMENT-ENABLE @ ;

: SET-TRACE-ENABLE ( f -- ) TRACE-ENABLE ! ;

: GET-TRACE-ENABLE ( -- f ) TRACE-ENABLE @ ;

: COMPILE-TRACE ( -- )
  GET-TRACE-INSTRUMENT-ENABLE IF
    & GET-TRACE-ENABLE & IF
      & CR & RP0 & RP@ & - & DUP & 0>= & IF
        & (LITERAL) [ 1 CELLS ] LITERAL , & / & (LITERAL) 2 , & * & SPACES
      & ELSE
        & DROP
      & THEN
      LATESTXT & (LITERAL) , & DUP & NAME>STRING & TYPE & SPACE
      & (LITERAL) 16 , & BASE. & .S & F.S
    & THEN
  THEN ;

: :NONAME ( -- xt ) :NONAME COMPILE-TRACE ;

: : ( "name" -- ) : COMPILE-TRACE ;

BASE ! SET-CURRENT SET-ORDER