GET-ORDER GET-CURRENT BASE @

DECIMAL
FORTH-WORDLIST 1 SET-ORDER
FORTH-WORDLIST SET-CURRENT

VARIABLE TRACE-INSTRUMENT-ENABLE FALSE TRACE-INSTRUMENT-ENABLE !
VARIABLE TRACE-ENABLE FALSE TRACE-ENABLE !

BEGIN-STRUCTURE TRACE-DATA-SIZE
  FIELD: TRACE-XT
END-STRUCTURE

: SET-TRACE-INSTRUMENT-ENABLE ( f -- ) TRACE-INSTRUMENT-ENABLE ! ;

: GET-TRACE-INSTRUMENT-ENABLE ( -- f ) TRACE-INSTRUMENT-ENABLE @ ;

: SET-TRACE-ENABLE ( f -- ) TRACE-ENABLE ! ;

: GET-TRACE-ENABLE ( -- f ) TRACE-ENABLE @ ;

: COMPILE-TRACE ( -- )
  GET-TRACE-INSTRUMENT-ENABLE IF
    & BRANCH HERE [ TRACE-DATA-SIZE 2 CELLS + ] LITERAL + ,
    0 , LATESTXT ,
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