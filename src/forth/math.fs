
: FACTORIAL ( u -- u ) 1 SWAP 1+ 1 ?DO I * LOOP ;

: INV-FACTORIAL ( u -- ) ( F: -- r ) 1E 1+ 1 ?DO 1E I S>F F/ F* LOOP ;

3.141592653589793E FCONSTANT PI

: FSQRT-CLOSE-ENOUGH ( F: r1 r2 -- ) ( -- f )
  F2DUP F- FABS 2 FROLL FABS 2 FROLL FABS FMAX F/ 0.0000000000000001E F< ;

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

\ : FSINH-COMPILE ( compile-time: u -- )
\   & FDUP
\   3 ?DO
\     & FOVER I S>F & (FLITERAL) F, & F**
\     I INV-FACTORIAL & (FLITERAL) F, & F* & F+
\   2 +LOOP
\   & FNIP ; IMMEDIATE
\ 
\ : FSINH ( F: r -- r ) [ 100 ] FSINH-COMPILE ;

\ : FCOSH-COMPILE ( compile-time: u -- )
\   & (FLITERAL) 1E F,
\   2 ?DO
\     & FOVER I S>F & (FLITERAL) F, & F**
\     I INV-FACTORIAL & (FLITERAL) F, & F* & F+
\   2 +LOOP
\   & FNIP ; IMMEDIATE
\ 
\ : FCOSH ( F: r -- r ) [ 100 ] FCOSH-COMPILE ;

: FEXPM1-COMPILE ( compile-time: u -- )
  & FDUP
  2 ?DO
    & FOVER I S>F & (FLITERAL) F, & F**
    I INV-FACTORIAL & (FLITERAL) F, & F* & F+
  LOOP
  & FNIP ; IMMEDIATE

: FEXPM1 ( F: r -- r ) [ 100 ] FEXPM1-COMPILE ;

: FEXP ( F: r -- r ) FEXPM1 1E F+ ;

1E FEXP FCONSTANT E

: FLNP1 ( F: r -- r )
  1E FSWAP F/
  FDUP 2E F* 1E F+ 2E FSWAP F/
  1600 3 ?DO
    FOVER 2E F* 1E F+ I S>F FDUP FROT FSWAP F** F* 2E FSWAP F/ F+
  2 +LOOP
  FNIP ;

: FLN ( F: r -- r ) 1E F- FLNP1 ;

: FSINH ( F: r -- r ) FEXPM1 FDUP FDUP 1E F+ F/ F+ 2E F/ ;

: FCOSH ( F: r -- r ) FEXPM1 FDUP FDUP 1E F+ F/ F- 2E F/ 1E F+ ;

: FTANH ( F: r -- r ) FDUP FSINH FSWAP FCOSH F/ ;

: FASINH ( F: r -- r ) FDUP 2E F** 1E F+ FSQRT F+ FLN ;

: FACOSH ( F: r -- r ) FDUP 2E F** 1E F- FSQRT F+ FLN ;

: FATANH ( F: r -- r ) FDUP 1E F+ FSWAP FNEGATE 1E F+ F/ FLN 0.5E F* ;