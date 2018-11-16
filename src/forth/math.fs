
: FACTORIAL ( u -- u ) 1 SWAP 1+ 1 ?DO I * LOOP ;

: INV-FACTORIAL ( u -- ) ( F: -- r ) 1E 1+ 1 ?DO 1E I S>F F/ F* LOOP ;

: FASIN-COEFF ( u -- ) ( F: -- r )
  1E 1+ 1 ?DO I S>F FDUP 1E F+ F/ F* 2 +LOOP ;

3.14159265359E FCONSTANT PI

: FSIN-COMPILE ( compile-time: u -- )
  -1 SWAP & FDUP
  3 ?DO
    & FOVER I S>F & (FLITERAL) F, & F**
    I INV-FACTORIAL & (FLITERAL) F, & F* DUP 0< IF & F- ELSE & F+ THEN NEGATE
  2 +LOOP
  & FNIP DROP ; IMMEDIATE

: FSIN ( F: r -- r ) [ 100 ] FSIN-COMPILE ;

: FCOS-COMPILE ( compile-time: u -- )
  -1 SWAP & (FLITERAL) 1E F,
  2 ?DO
    & FOVER I S>F & (FLITERAL) F, & F**
    I INV-FACTORIAL & (FLITERAL) F, & F* DUP 0< IF & F- ELSE & F+ THEN NEGATE
  2 +LOOP
  & FNIP DROP ; IMMEDIATE

: FCOS ( F: r -- r ) [ 100 ] FCOS-COMPILE ;

: FTAN ( F: r -- r ) FDUP FSIN FSWAP FCOS F/ ;

: FASIN-COMPILE ( compile-time: u -- )
  & FDUP
  1 ?DO
    & FOVER I 2 + S>F & (FLITERAL) F, & F**
    I FASIN-COEFF I 2 + S>F F/ & (FLITERAL) F, & F* & F+
  2 +LOOP
  & FNIP ; IMMEDIATE

: FASIN ( F: r -- r ) [ 100 ] FASIN-COMPILE ;

: FACOS ( F: r -- r ) FASIN FNEGATE [ PI 2E F/ ] FLITERAL F+ ;

: FATAN-COMPILE ( compile-time: u -- )
  -1 SWAP & FDUP
  3 ?DO
    & FOVER I S>F & (FLITERAL) F, & F**
    I S>F & (FLITERAL) F, & F/ DUP 0< IF & F- ELSE & F+ THEN NEGATE
  2 +LOOP
  & FNIP DROP ; IMMEDIATE

: FATAN ( F: r -- r ) [ 100 ] FATAN-COMPILE ;

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

\ : FLN-COMPILE ( compile-time: u -- )
\   & (FLITERAL) 1E F, & F- & (FLITERAL) 1E F, & FSWAP & F/
\   & FDUP & (FLITERAL) 2E F, & F* & (FLITERAL) 1E F, & F+
\   & (FLITERAL) 2E F, & FSWAP & F/
\   3 ?DO
\     & FOVER & (FLITERAL) 2E F, & F*
\     & (FLITERAL) 1E F, & F+ I S>F & (FLITERAL) F, & F**
\     I S>F & (FLITERAL) F, & F* & (FLITERAL) 2E F, & FSWAP & F/ & F+
\   2 +LOOP
\   & FNIP ; IMMEDIATE
\ 
\ : FLN ( F: r -- r ) [ 800 ] FLN-COMPILE ;

\ double test(double x, double g) {
\    if closeEnough(x/g, g)
\       return g;
\    else
\       return test(x, betterGuess(x, g));
\ }
\ 
\ boolean closeEnough(double a, double b) {
\    return (Math.abs(a - b) / Math.max(Math.abs(a), Math.abs(b)) < .001);
\ }
\ 
\ double betterGuess(double x, double g) {
\    return ((g + x/g) / 2);
\ }

: FSQRT-CLOSE-ENOUGH ( F: r1 r2 -- ) ( -- f )
  F2DUP F- FABS 2 FROLL FABS 2 FROLL FABS FMAX F/ 0.000001E F< ;

: FSQRT-BETTER-GUESS ( F: r1 r2 -- r3 ) FDUP 2 FROLL 2 FROLL F/ F+ 2E F/ ;

: FSQRT-TEST ( F: r1 r2 -- r3 )
  F2DUP F/ FOVER FSQRT-CLOSE-ENOUGH IF
    FNIP
  ELSE
    F2DUP FSQRT-BETTER-GUESS FNIP RECURSE
  THEN ;

: FSQRT ( F: r -- r ) FDUP 2E F/ FSQRT-TEST ;

: FSINH ( F: r -- r ) FEXPM1 FDUP FDUP 1E F+ F/ F+ 2E F/ ;

: FCOSH ( F: r -- r ) FEXPM1 FDUP FDUP 1E F+ F/ F- 2E F/ 1E F+ ;

: FTANH ( F: r -- r ) FDUP FSINH FSWAP FCOSH F/ ;

: FASINH ( F: r -- r ) FDUP 2E F** 1E F+ FSQRT F+ FLN ;

: FACOSH ( F: r -- r ) FDUP 2E F** 1E F- FSQRT F+ FLN ;

: FATANH ( F: r -- r ) FDUP 1E F+ FSWAP FNEGATE 1E F+ F/ FLN 0.5E F* ;