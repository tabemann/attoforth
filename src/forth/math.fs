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

: FACTORIAL ( u -- u ) 1 SWAP 1+ 1 ?DO I * LOOP ;

: INV-FACTORIAL ( u -- ) ( F: -- r ) 1E 1+ 1 ?DO 1E I S>F F/ F* LOOP ;

3.141592653589793E FCONSTANT PI

: FSQRT-CLOSE-ENOUGH ( F: r1 r2 -- ) ( -- f )
  F2DUP F- FABS 2 FROLL FABS 2 FROLL FABS FMAX F/ 0.000000000000001E F< ;

: FSQRT-BETTER-GUESS ( F: r1 r2 -- r3 ) FDUP 2 FROLL 2 FROLL F/ F+ 2E F/ ;

: FSQRT-TEST ( F: r1 r2 -- r3 )
  F2DUP F/ FOVER FSQRT-CLOSE-ENOUGH IF
    FNIP
  ELSE
    F2DUP FSQRT-BETTER-GUESS FNIP RECURSE
  THEN ;

: FSQRT ( F: r -- r ) FDUP 2E F/ FSQRT-TEST ;

: FSIN-COMPILE ( compile-time: u -- )
  -1 SWAP & FDUP
  3 ?DO
    & FOVER I S>F & (FLITERAL) F, & F**
    I INV-FACTORIAL & (FLITERAL) F, & F* DUP 0< IF & F- ELSE & F+ THEN NEGATE
  2 +LOOP
  & FNIP DROP ; IMMEDIATE

: FSIN ( F: r -- r ) [ 30 ] FSIN-COMPILE ;

: FCOS-COMPILE ( compile-time: u -- )
  -1 SWAP & (FLITERAL) 1E F,
  2 ?DO
    & FOVER I S>F & (FLITERAL) F, & F**
    I INV-FACTORIAL & (FLITERAL) F, & F* DUP 0< IF & F- ELSE & F+ THEN NEGATE
  2 +LOOP
  & FNIP DROP ; IMMEDIATE

: FCOS ( F: r -- r ) [ 30 ] FCOS-COMPILE ;

: FTAN ( F: r -- r ) FDUP FSIN FSWAP FCOS F/ ;

: FATAN ( F: r -- r )
  1E
  1 40 DO
    FOVER I S>F F* 2E F** FSWAP F/ I 2 * 1- S>F F+
  -1 +LOOP
  F/ ;

: FASIN ( F: r -- r )
  FDUP 2E F** 1E F< IF
    1E FOVER 2E F** F- FSQRT F/ FATAN
  ELSE FDUP F0> IF
    FDROP [ PI 2E F/ ] FLITERAL
  ELSE
    FDROP [ PI -2E F/ ] FLITERAL
  THEN THEN ;

: FACOS ( F: r -- r ) FASIN FNEGATE [ PI 2E F/ ] FLITERAL F+ ;

: FATAN2 ( F: r1 r2 -- r3 ) \ ( F: y x -- angle )
  FDUP F0> IF
    F/ FATAN
  ELSE F2DUP F0< FSWAP F0>= AND IF
    F/ FATAN PI F+
  ELSE F2DUP F0< FSWAP F0< AND IF
    F/ FATAN PI F-
  ELSE F2DUP F0= FSWAP F0> AND IF
    F2DROP PI 2E F/
  ELSE F2DUP F0= FSWAP F0< AND IF
    F2DROP PI -2E F/
  ELSE
    F2DROP 0E F2DUP F/
  THEN THEN THEN THEN THEN ;

: FSINCOS ( F: r1 -- r2 r3) \ ( F: angle -- sin cos )
  FDUP FSIN FSWAP FCOS ;

: FEXPM1-COMPILE ( compile-time: u -- )
  & FDUP
  2 ?DO
    & FOVER I S>F & (FLITERAL) F, & F**
    I INV-FACTORIAL & (FLITERAL) F, & F* & F+
  LOOP
  & FNIP ; IMMEDIATE

: FEXPM1 ( F: r -- r ) [ 155 ] FEXPM1-COMPILE ;

\ : FEXPM1-COMPILE ( compile-time: -- )
\   & FDUP & (FLITERAL) 2E F, & F** & (FLITERAL) 1E F,
\   6 70 DO
\     & FOVER & FSWAP & F/ I S>F & (FLITERAL) F, & F+
\   -4 +LOOP
\   & F/ & (FLITERAL) 2E F, & F+ & FOVER & F- & FSWAP
\   & (FLITERAL) 2E F, & F* & FSWAP & F/ ; IMMEDIATE
\ 
\ : FEXPM1 ( F: r -- r ) FEXPM1-COMPILE ;

: FEXP ( F: r -- r ) FEXPM1 1E F+ ;

1E FEXP FCONSTANT E

0.6931471805599453E FCONSTANT FLN2

: ARITH-GEOM-MEAN ( F: r1 r2 -- r3 )
  BEGIN
    F2DUP F- FABS 0.000000000000001E F>
  WHILE
    F2DUP F+ 2E F/ FROT FROT F* FSQRT
  REPEAT
  FDROP ;

\ x2^m = 2^(p/2)
\ 2^(log2(x))2^m = 2^(p/2)
\ log2(x) + m = p/2
\ m = p/2 - log2(x)

: ROUGH-LOG2 ( d -- u )
  0 BEGIN
    ROT ROT 1 DRSHIFT 2DUP D0> IF ROT 1+ FALSE ELSE 2DROP TRUE THEN
  UNTIL ;

: FLN-PRECISION ( F: r -- r )
  [ 54 2 / S>F ] FLITERAL FSWAP F>D ROUGH-LOG2 S>F F- ;

: FLNP1 ( F: r -- r )
  1E F+ FDUP FLN-PRECISION
  PI 1E 2E 2E 4 FPICK F- F** 4 FROLL F/ ARITH-GEOM-MEAN 2E F* F/
  FSWAP FLN2 F* F- ;

: FLN ( F: r -- r ) 1E F- FLNP1 ;

: FLOG ( F: r -- r ) FLN [ 10E FLN ] FLITERAL F/ ;

: FSINH ( F: r -- r ) FEXPM1 FDUP FDUP 1E F+ F/ F+ 2E F/ ;

: FCOSH ( F: r -- r ) FEXPM1 FDUP FDUP 1E F+ F/ F- 2E F/ 1E F+ ;

: FTANH ( F: r -- r ) FDUP FSINH FSWAP FCOSH F/ ;

: FASINH ( F: r -- r ) FDUP 2E F** 1E F+ FSQRT F+ FLN ;

: FACOSH ( F: r -- r ) FDUP 2E F** 1E F- FSQRT F+ FLN ;

: FATANH ( F: r -- r ) FDUP 1E F+ FSWAP FNEGATE 1E F+ F/ FLN 0.5E F* ;

BASE ! SET-CURRENT SET-ORDER