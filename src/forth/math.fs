
: FACTORIAL ( u -- u ) 1 SWAP 1+ 1 ?DO I * LOOP ;

: INV-FACTORIAL ( u -- ) ( F: -- r ) 1E 1+ 1 ?DO 1E I S>F F/ F* LOOP ;

3.14159265359E FCONSTANT PI

: SIN-COMPILE ( compile-time: u -- )
  -1 SWAP & FDUP
  3 ?DO
    & FOVER I S>F & (FLITERAL) F, & F**
    I INV-FACTORIAL & (FLITERAL) F, & F* DUP 0< IF & F- ELSE & F+ THEN NEGATE
  2 +LOOP
  & FNIP DROP ; IMMEDIATE

: SIN ( F: r -- r ) [ 100 ] SIN-COMPILE ;

: COS-COMPILE ( compile-time: u -- )
  -1 SWAP & (FLITERAL) 1E F,
  2 ?DO
    & FOVER I S>F & (FLITERAL) F, & F**
    I INV-FACTORIAL & (FLITERAL) F, & F* DUP 0< IF & F- ELSE & F+ THEN NEGATE
  2 +LOOP
  & FNIP DROP ; IMMEDIATE

: COS ( F: r -- r ) [ 100 ] COS-COMPILE ;

: SINH-COMPILE ( compile-time: u -- )
  & FDUP
  3 ?DO
    & FOVER I S>F & (FLITERAL) F, & F**
    I INV-FACTORIAL & (FLITERAL) F, & F* & F+
  2 +LOOP
  & FNIP ; IMMEDIATE

: SINH ( F: r -- r ) [ 100 ] SINH-COMPILE ;

: COSH-COMPILE ( compile-time: u -- )
  & (FLITERAL) 1E F,
  2 ?DO
    & FOVER I S>F & (FLITERAL) F, & F**
    I INV-FACTORIAL & (FLITERAL) F, & F* & F+
  2 +LOOP
  & FNIP ; IMMEDIATE

: COSH ( F: r -- r ) [ 100 ] COSH-COMPILE ;

: EXP-COMPILE ( compile-time: u -- )
  & (FLITERAL) 1E F, & FOVER & F+
  2 ?DO
    & FOVER I S>F & (FLITERAL) F, & F**
    I INV-FACTORIAL & (FLITERAL) F, & F* & F+
  LOOP
  & FNIP ; IMMEDIATE

: EXP ( F: r -- r ) [ 100 ] EXP-COMPILE ;

: LN-COMPILE ( compile-time: u -- )
  & (FLITERAL) 1E F, & F- & (FLITERAL) 1E F, & FSWAP & F/
  & FDUP & (FLITERAL) 2E F, & F* & (FLITERAL) 1E F, & F+
  & (FLITERAL) 2E F, & FSWAP & F/
  3 ?DO
    & FOVER & (FLITERAL) 2E F, & F*
    & (FLITERAL) 1E F, & F+ I S>F & (FLITERAL) F, & F**
    I S>F & (FLITERAL) F, & F* & (FLITERAL) 2E F, & FSWAP & F/ & F+
  2 +LOOP
  & FNIP ; IMMEDIATE

: LN ( F: r -- r ) [ 800 ] LN-COMPILE ;
