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

WORDLIST CONSTANT LAMBDA-WORDLIST

LAMBDA-WORDLIST (WORDLIST) LAMBDA

GET-ORDER FORTH-WORDLIST LAMBDA-WORDLIST 2 SET-ORDER

LAMBDA-WORDLIST SET-CURRENT

: [: ( -- xt) ( in xt: -- )
  & BRANCH HERE 0 , 0 , LATESTXT :NONAME ; IMMEDIATE

: ;] ( -- ) ( in xt: -- )
  & EXIT 0 , ROT HERE SWAP ! & (LITERAL) , >LATESTXT ; IMMEDIATE

: <: ( x*u u -- xt ) ( in xt: -- x*u )
  & DUP & (LITERAL) 2 , & + & CELLS & HEADER-SIZE & + & ALLOCATE!
  & DUP & >R & CELL+ & HEADER-SIZE & + & 2DUP & ! & CELL+ & SWAP
  & MOVE-FROM-STACK & R> & CREATE-NONAME-AT
  & DOES-AT> HERE 4 CELLS + , & BRANCH HERE 0 , 0 ,
  & DUP & @ & SWAP & CELL+ & SWAP & MOVE-TO-STACK ; IMMEDIATE

: ;> ( -- ) ( in xt: -- ) & EXIT 0 , HERE SWAP ! ; IMMEDIATE

: FREE-LAMBDA ( xt -- ) 1 CELLS - FREE! ;

: 3DUP ( x1 x2 x3 -- x1 x2 x3 x1 x2 x3 ) 2 PICK 2 PICK 2 PICK ;

: F<: ( x*u1 u1 u2 -- xt ) ( F: r*u2 -- ) ( in xt: -- x*u1 )
  ( in xt: F: -- r*u2 )
  & 2DUP & SWAP & (LITERAL) 3 , & + & CELLS & SWAP & FLOATS & +
  & HEADER-SIZE & + & ALLOCATE!
  & DUP & >R & CELL+ & HEADER-SIZE & + & (LITERAL) 2 , & PICK & OVER & !
  & 2DUP & CELL+ & ! & (LITERAL) 2 ,
  & CELLS & + & 3DUP & >R & >R & >R & NIP & SWAP & MOVE-FROM-STACK
  & R> & R> & R> & ROT & CELLS & + & SWAP & MOVE-FROM-FLOAT-STACK
  & R> & CREATE-NONAME-AT
  & DOES-AT> HERE 4 CELLS + , & BRANCH HERE 0 , 0 ,
  & DUP & DUP & >R & @ & DUP & >R & SWAP & (LITERAL) 2 , & CELLS & +
  & SWAP & MOVE-TO-STACK
  & R> & R> & DUP & CELL+ & @ & SWAP & (LITERAL) 2 , & CELLS & +
  & ROT & CELLS & + & SWAP & MOVE-TO-FLOAT-STACK ; IMMEDIATE

SET-ORDER SET-CURRENT
