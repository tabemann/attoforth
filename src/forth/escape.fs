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
WORDLIST CONSTANT ESCAPE-WORDLIST
FORTH-WORDLIST ESCAPE-WORDLIST 2 SET-ORDER
ESCAPE-WORDLIST SET-CURRENT

: CURRENT-CHAR ( c-addr u -- c-addr u c ) OVER C@ ;

: ADVANCE-CHAR ( c-addr u -- c-addr u ) SWAP CHAR+ SWAP 1 >IN +! ;

: REGISTER-CHAR ( c-addr u -- c-addr u ) 1+ ;

: COUNT-CHARS-UNTIL-UNESCAPED ( c -- dest-count )
  SOURCE DROP >IN @ + 0 BEGIN
    OVER SOURCE + < IF
      CURRENT-CHAR [CHAR] \ <> IF
        CURRENT-CHAR 3 PICK <> IF
          REGISTER-CHAR ADVANCE-CHAR FALSE
	ELSE
	  ADVANCE-CHAR TRUE
	THEN
      ELSE
        ADVANCE-CHAR
        OVER SOURCE + < IF
          CURRENT-CHAR CASE
  	    [CHAR] m OF ADVANCE-CHAR REGISTER-CHAR REGISTER-CHAR FALSE ENDOF
  	    [CHAR] x OF
	      ADVANCE-CHAR
  	      OVER SOURCE + 2 - <= IF
  	        OVER C@ TO-UPPER DUP [CHAR] 0 >= OVER [CHAR] 9 <= AND
  	        OVER [CHAR] A >= ROT [CHAR] F <= AND OR IF
  	          OVER 2 ['] PARSE-NUMBER 16 BASE-EXECUTE NIP IF
  	            REGISTER-CHAR
  	          THEN
		  ADVANCE-CHAR ADVANCE-CHAR
  	        THEN
  	        FALSE
  	      ELSE
	        TRUE
              THEN
  	    ENDOF
  	    >R ADVANCE-CHAR REGISTER-CHAR FALSE R>
  	  ENDCASE
        ELSE
          TRUE
        THEN
      THEN
    ELSE
      TRUE
    THEN
  UNTIL
  NIP NIP ;

: CURRENT-CHAR ( c-addr1 c-addr2 u1 u2 -- c-addr1 c-addr2 u1 u2 c )
  3 PICK C@ ;

: ADVANCE-CHAR ( c-addr1 c-addr2 u1 u2 -- c-addr1 c-addr2 u1 u2 )
  3 ROLL CHAR+ 3 ROLL 3 ROLL 3 ROLL ;

: WRITE-CHAR ( c-addr1 c-addr2 u1 u2 c -- c-addr1 c-addr2 u1 u2 )
  3 PICK C! ROT CHAR+ ROT 1- ROT 1+ ;

: PARSE-ESCAPED ( source-addr dest-addr source-count -- dest-count )
  0 BEGIN OVER 0> WHILE
    CURRENT-CHAR [CHAR] \ <> IF
      CURRENT-CHAR WRITE-CHAR
    ELSE
      ADVANCE-CHAR
      OVER 0> IF
        CURRENT-CHAR CASE
	  [CHAR] a OF $07 WRITE-CHAR ENDOF
	  [CHAR] b OF $08 WRITE-CHAR ENDOF
	  [CHAR] e OF $1B WRITE-CHAR ENDOF
	  [CHAR] f OF $0C WRITE-CHAR ENDOF
	  [CHAR] l OF $0A WRITE-CHAR ENDOF
	  [CHAR] m OF $0D WRITE-CHAR $0A WRITE-CHAR ENDOF
	  [CHAR] n OF $0A WRITE-CHAR ENDOF
	  [CHAR] q OF [CHAR] " WRITE-CHAR ENDOF
	  [CHAR] r OF $0D WRITE-CHAR ENDOF
	  [CHAR] t OF $09 WRITE-CHAR ENDOF
	  [CHAR] v OF $0B WRITE-CHAR ENDOF
	  [CHAR] z OF $00 WRITE-CHAR ENDOF
	  [CHAR] " OF [CHAR] " WRITE-CHAR ENDOF
	  [CHAR] \ OF [CHAR] \ WRITE-CHAR ENDOF
	  [CHAR] x OF
	    ADVANCE-CHAR
	    OVER 2 >= IF
	      3 PICK C@ TO-UPPER DUP [CHAR] 0 >= OVER [CHAR] 9 <= AND
	      OVER [CHAR] A >= ROT [CHAR] F <= AND OR IF
	        3 PICK 2 ['] PARSE-NUMBER 16 BASE-EXECUTE IF
	          WRITE-CHAR
		ELSE
		  DROP
	        THEN
	      THEN
	      ADVANCE-CHAR
	    ELSE
	      NIP 0 SWAP
            THEN
	  ENDOF
	  DUP >R WRITE-CHAR R>
	ENDCASE
      THEN
    THEN
    ADVANCE-CHAR
  REPEAT
  2SWAP 2DROP NIP ;

FORTH-WORDLIST SET-CURRENT

: S\" ( compile-time: "ccc<quote>" -- ) ( runtime: -- c-addr u )
  SKIP-WHITESPACE SOURCE DROP >IN @ [CHAR] " COUNT-CHARS-UNTIL-UNESCAPED
  DUP STATE @ IF
    ALLOCATE!
  ELSE
    GET-INTERPRET-STRING-BUFFER
  THEN
  3 ROLL 3 ROLL + OVER 3 ROLL PARSE-ESCAPED STATE @ IF
    SWAP & (LITERAL) , & (LITERAL) ,
  THEN ; IMMEDIATE

: C\" ( compile-time: "ccc<quote>" -- ) ( runtime: -- c-addr )
  SKIP-WHITESPACE SOURCE DROP >IN @ [CHAR] " COUNT-CHARS-UNTIL-UNESCAPED
  DUP 1+ STATE @ IF
    ALLOCATE!
  ELSE
    GET-INTERPRET-STRING-BUFFER
  THEN
  3 ROLL 3 ROLL + OVER CHAR+ 3 ROLL
  PARSE-ESCAPED 255 MIN OVER C! STATE @ IF & (LITERAL) , THEN ; IMMEDIATE

: .\" ( compile-time: "ccc<quote>" -- ) ( runtime: -- )
  SKIP-WHITESPACE SOURCE DROP >IN @ [CHAR] " COUNT-CHARS-UNTIL-UNESCAPED 
  DUP ALLOCATE! 3 ROLL 3 ROLL + OVER 3 ROLL PARSE-ESCAPED
  SWAP & (LITERAL) , & (LITERAL) , & TYPE ; IMMEDIATE

: .\( ( "ccc<quote>" -- )
  SKIP-WHITESPACE SOURCE DROP >IN @ [CHAR] ) COUNT-CHARS-UNTIL-UNESCAPED
  DUP ALLOCATE! 3 ROLL 3 ROLL + OVER 3 ROLL PARSE-ESCAPED
  2DUP TYPE DROP FREE! ; IMMEDIATE

BASE ! SET-CURRENT SET-ORDER
