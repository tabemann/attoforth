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

FORTH-WORDLIST IO-WORDLIST TASK-WORDLIST 3 SET-ORDER

IO-WORDLIST SET-CURRENT

: INPUT-FIELD: CREATE ALIGNED DUP , 1 CELLS + DOES> @ SWAP INPUT-ARG @ + ;

BEGIN-STRUCTURE TERMINAL-INFO-SIZE
  INPUT-FIELD: TERMINAL-FD
  INPUT-FIELD: TERMINAL-BUFFER-SIZE
  INPUT-FIELD: TERMINAL-INSERT-OFFSET
  INPUT-FIELD: TERMINAL-ROW
  INPUT-FIELD: TERMINAL-COLUMN
  INPUT-FIELD: TERMINAL-WIDTH
  INPUT-FIELD: TERMINAL-HEIGHT
  INPUT-FIELD: TERMINAL-INIT-ROW
  INPUT-FIELD: TERMINAL-INIT-COLUMN
  INPUT-FIELD: TERMINAL-KILL-RING-FIRST
  INPUT-FIELD: TERMINAL-KILL-RING-LAST
  INPUT-FIELD: TERMINAL-PREV-COMMAND-TYPE
  INPUT-FIELD: TERMINAL-HISTORY-LAST
  INPUT-FIELD: TERMINAL-HISTORY-CURRENT
  INPUT-FIELD: TERMINAL-PROMPT
  INPUT-FIELD: TERMINAL-COMPLETE-ENABLE
END-STRUCTURE

BEGIN-STRUCTURE KILL-ENTRY-SIZE
  FIELD: KILL-ENTRY-PREV
  FIELD: KILL-ENTRY-NEXT
  FIELD: KILL-ENTRY-DATA-SIZE
  FIELD: KILL-ENTRY-BUFFER
END-STRUCTURE

BEGIN-STRUCTURE HISTORY-ENTRY-SIZE
  FIELD: HISTORY-ENTRY-PREV
  FIELD: HISTORY-ENTRY-NEXT
  FIELD: HISTORY-ENTRY-DATA-SIZE
  FIELD: HISTORY-ENTRY-BUFFER
  FIELD: HISTORY-ENTRY-EDIT-DATA-SIZE
  FIELD: HISTORY-ENTRY-EDIT-BUFFER
END-STRUCTURE

$01 CONSTANT CTRL-A
$02 CONSTANT CTRL-B
$04 CONSTANT CTRL-D
$05 CONSTANT CTRL-E
$06 CONSTANT CTRL-F
$09 CONSTANT TAB
$0B CONSTANT CTRL-K
$0C CONSTANT CTRL-L
$17 CONSTANT CTRL-W
$19 CONSTANT CTRL-Y
$1B CONSTANT ESCAPE
$7F CONSTANT DELETE

0 CONSTANT NORMAL-COMMAND
1 CONSTANT KILL-FORWARD-COMMAND
2 CONSTANT KILL-BACKWARD-COMMAND
3 CONSTANT YANK-COMMAND

: START-VERIFY ( -- ) & SP@ & >R ; IMMEDIATE

: VERIFY" ( u -- ) & SP@ & CELL+ & R> & - & (LITERAL) -1 , & CELLS & / & <> & IF
    SKIP-WHITESPACE [CHAR] " PARSE DUP ALLOCATE! 2DUP & SPACE & (LITERAL) ,
    & (LITERAL) , SWAP MOVE & TYPE & SPACE & .S & QUIT
  & THEN ; IMMEDIATE

: LAST-KILL-ENTRY-DATA-SIZE ( source -- addr )
  TERMINAL-KILL-RING-LAST @ DUP 0= ABORT" Empty kill ring!"
  KILL-ENTRY-DATA-SIZE ;

: LAST-KILL-ENTRY-BUFFER ( source -- c-addr )
  TERMINAL-KILL-RING-LAST @ DUP 0= ABORT" Empty kill ring!"
  KILL-ENTRY-BUFFER ;

: FIRST-KILL-ENTRY-DATA-SIZE ( source -- addr )
  TERMINAL-KILL-RING-FIRST @ DUP 0= ABORT" Empty kill ring!"
  KILL-ENTRY-DATA-SIZE ;

: FIRST-KILL-ENTRY-BUFFER ( source -- c-addr )
  TERMINAL-KILL-RING-FIRST @ DUP 0= ABORT" Empty kill ring!"
  KILL-ENTRY-BUFFER ;

: MOVE-LAST-KILL-ENTRY-TO-FIRST ( source -- )
  DUP TERMINAL-KILL-RING-LAST @ IF
    DUP TERMINAL-KILL-RING-LAST @ OVER TERMINAL-KILL-RING-FIRST @ <> IF
      DUP TERMINAL-KILL-RING-LAST @
      DUP KILL-ENTRY-PREV @
      0 OVER KILL-ENTRY-NEXT !
      2 PICK TERMINAL-KILL-RING-LAST !
      OVER TERMINAL-KILL-RING-FIRST @
      2DUP KILL-ENTRY-PREV !
      OVER KILL-ENTRY-NEXT !
      0 OVER KILL-ENTRY-PREV !
      SWAP TERMINAL-KILL-RING-FIRST !
    ELSE
      DROP
    THEN
  ELSE
    DROP
  THEN ;

: SAVE-HISTORY-EDIT ( source -- )
  DUP TERMINAL-HISTORY-CURRENT @ 0<> IF
    DUP TERMINAL-HISTORY-CURRENT @ HISTORY-ENTRY-EDIT-BUFFER @ 0<> IF
      DUP TERMINAL-HISTORY-CURRENT @ HISTORY-ENTRY-EDIT-BUFFER @ FREE!
    THEN
    DUP INPUT-COUNT @ 0> IF
      DUP INPUT-COUNT @ ALLOCATE!
      OVER INPUT-BUFFER @ OVER 3 PICK INPUT-COUNT @ MOVE
      OVER TERMINAL-HISTORY-CURRENT @ HISTORY-ENTRY-EDIT-BUFFER !
      DUP INPUT-COUNT @ SWAP
      TERMINAL-HISTORY-CURRENT @ HISTORY-ENTRY-EDIT-DATA-SIZE !
    ELSE
      0 OVER TERMINAL-HISTORY-CURRENT @ HISTORY-ENTRY-EDIT-BUFFER !
      0 SWAP TERMINAL-HISTORY-CURRENT @ HISTORY-ENTRY-EDIT-DATA-SIZE !
    THEN
  ELSE
    DROP
  THEN ;

: CLEAR-HISTORY-EDITS ( source -- )
  0 OVER TERMINAL-HISTORY-CURRENT !
  TERMINAL-HISTORY-LAST @ BEGIN DUP 0<> WHILE
    DUP HISTORY-ENTRY-EDIT-BUFFER @ 0<> IF
      DUP HISTORY-ENTRY-EDIT-BUFFER @ FREE!
      0 OVER HISTORY-ENTRY-EDIT-DATA-SIZE !
      0 OVER HISTORY-ENTRY-EDIT-BUFFER !
    THEN
    HISTORY-ENTRY-PREV @
  REPEAT
  DROP ;

: TEST-HISTORY-CURRENT-IN-BUFFER ( source -- in-buffer )
  DUP TERMINAL-HISTORY-LAST @ 0<> IF
    DUP TERMINAL-HISTORY-LAST @ OVER TERMINAL-HISTORY-CURRENT @ =
    OVER TERMINAL-HISTORY-CURRENT @ 0= OR IF
      DUP TERMINAL-HISTORY-LAST @ HISTORY-ENTRY-DATA-SIZE @
      OVER INPUT-COUNT @ = IF
        DUP TERMINAL-HISTORY-LAST @ HISTORY-ENTRY-BUFFER @
        OVER TERMINAL-HISTORY-LAST @ HISTORY-ENTRY-DATA-SIZE @
        2 PICK INPUT-BUFFER @ 3 ROLL INPUT-COUNT @ COMPARE 0=
      ELSE
        DROP FALSE
      THEN
    ELSE
      DROP FALSE
    THEN
  ELSE
    DROP FALSE
  THEN ;

: INSERT-HISTORY-ENTRY ( source entry -- )
  OVER TERMINAL-HISTORY-LAST @ OVER HISTORY-ENTRY-PREV !
  2DUP SWAP TERMINAL-HISTORY-LAST @ DUP 0<> IF
    HISTORY-ENTRY-NEXT !
  ELSE
    2DROP
  THEN
  0 OVER HISTORY-ENTRY-NEXT ! SWAP TERMINAL-HISTORY-LAST ! ;

: FILL-HISTORY-ENTRY ( source entry -- )
  2DUP SWAP INPUT-COUNT @ SWAP HISTORY-ENTRY-DATA-SIZE !
  2DUP SWAP INPUT-COUNT @ ALLOCATE! SWAP HISTORY-ENTRY-BUFFER !
  0 OVER HISTORY-ENTRY-EDIT-DATA-SIZE !
  0 OVER HISTORY-ENTRY-EDIT-BUFFER !
  OVER INPUT-BUFFER @ SWAP HISTORY-ENTRY-BUFFER @ ROT INPUT-COUNT @ MOVE ;

: ADD-HISTORY ( source -- )
  DUP INPUT-COUNT @ 0> IF
    DUP TEST-HISTORY-CURRENT-IN-BUFFER INVERT IF
      HISTORY-ENTRY-SIZE ALLOCATE!
      2DUP INSERT-HISTORY-ENTRY FILL-HISTORY-ENTRY
    ELSE
      DROP
    THEN
  ELSE
    DROP
  THEN ;

: EDIT-HISTORY-WITHOUT-EDIT-BUFFER ( source -- )
  DUP TERMINAL-HISTORY-CURRENT @ HISTORY-ENTRY-BUFFER @
  OVER INPUT-BUFFER @
  2 PICK TERMINAL-HISTORY-CURRENT @ HISTORY-ENTRY-DATA-SIZE @ MOVE
  DUP TERMINAL-HISTORY-CURRENT @ HISTORY-ENTRY-DATA-SIZE @
  SWAP INPUT-COUNT ! ;

: EDIT-HISTORY-WITH-EDIT-BUFFER ( source -- )
  DUP TERMINAL-HISTORY-CURRENT @ HISTORY-ENTRY-EDIT-BUFFER @
  OVER INPUT-BUFFER @
  2 PICK TERMINAL-HISTORY-CURRENT @ HISTORY-ENTRY-EDIT-DATA-SIZE @ MOVE
  DUP TERMINAL-HISTORY-CURRENT @ HISTORY-ENTRY-EDIT-DATA-SIZE @
  SWAP INPUT-COUNT ! ;

: EDIT-HISTORY ( source -- )
  DUP TERMINAL-HISTORY-CURRENT @ 0<> IF
    DUP TERMINAL-HISTORY-CURRENT @ HISTORY-ENTRY-EDIT-BUFFER @ 0= IF
      EDIT-HISTORY-WITHOUT-EDIT-BUFFER
    ELSE
      EDIT-HISTORY-WITH-EDIT-BUFFER
    THEN
  ELSE
    DROP
  THEN ;

: SET-NORMAL-COMMAND ( source -- )
  NORMAL-COMMAND SWAP TERMINAL-PREV-COMMAND-TYPE ! ;

: SET-KILL-FORWARD-COMMAND ( source -- )
  KILL-FORWARD-COMMAND SWAP TERMINAL-PREV-COMMAND-TYPE ! ;

: SET-KILL-BACKWARD-COMMAND ( source -- )
  KILL-BACKWARD-COMMAND SWAP TERMINAL-PREV-COMMAND-TYPE ! ;

: SET-YANK-COMMAND ( source -- )
  YANK-COMMAND SWAP TERMINAL-PREV-COMMAND-TYPE ! ;

DEFER HANDLE-CHAR

: EXPAND-TERMINAL-BUFFER ( source u -- )
  OVER TERMINAL-BUFFER-SIZE @ DUP 2 * OVER 3 ROLL + MAX DUP ALLOCATE!
  SWAP 3 PICK TERMINAL-BUFFER-SIZE !
  2 PICK INPUT-BUFFER @ DUP 2 PICK 4 ROLL MOVE FREE!
  SWAP INPUT-BUFFER ! ;

: TRY-EXPAND-TERMINAL-BUFFER ( source u -- )
  2DUP SWAP INPUT-COUNT @ + 2 PICK TERMINAL-BUFFER-SIZE @ > IF
    EXPAND-TERMINAL-BUFFER
  ELSE 2DROP THEN ;

: READ-CHAR ( source -- c-addr flag )
  HERE DUP 1 3 ROLL TERMINAL-FD @ IO-READ-BLOCK
  DUP IO-ACTION-GET-STATE DUP IO-STATE-GET-INDEX 1 = SWAP
  IO-STATE-DESTROY SWAP IO-ACTION-DESTROY ;

: (DEC.) ( n -- ) 10 (BASE.) ;

: CSI ( -- ) ESCAPE EMIT [CHAR] [ EMIT ;

: ENABLE-WRAP ( -- ) CSI [CHAR] 7 EMIT [CHAR] h EMIT ;

: SHOW-CURSOR ( -- ) CSI [CHAR] ? EMIT 25 (DEC.) [CHAR] h EMIT ;

: HIDE-CURSOR ( -- ) CSI [CHAR] ? EMIT 25 (DEC.) [CHAR] l EMIT ;

: SAVE-CURSOR ( -- ) CSI [CHAR] s EMIT ;

: RESTORE-CURSOR ( -- )  CSI [CHAR] u EMIT ;

: GO-TO-COORD ( row column -- )
  SWAP CSI 1+ (DEC.) [CHAR] ; EMIT 1+ (DEC.) [CHAR] f EMIT ;

: ERASE-END-OF-LINE ( -- ) CSI [CHAR] K EMIT ;

: ERASE-DOWN ( -- ) CSI [CHAR] J EMIT ;

: READ-CHARS ( c-addr u1 c source -- c-addr u2 flag )
  3 PICK 0
  BEGIN
    DUP 5 PICK < IF
      2 PICK READ-CHAR IF
        C@ DUP 5 PICK = IF
          DROP 4 ROLL 4 ROLL 2DROP ROT ROT 2DROP TRUE TRUE
        ELSE
          DUP [CHAR] 0 >= OVER [CHAR] 9 <= AND IF
            ROT DUP ROT SWAP C! 1+ SWAP 1+ FALSE
          ELSE
            DROP 2DROP 2DROP 2DROP 0 0 FALSE TRUE
          THEN
        THEN
      ELSE
        DROP 2DROP 2DROP 2DROP 0 0 FALSE TRUE
      THEN
    ELSE
      2DROP 2DROP 2DROP 0 0 FALSE TRUE
    THEN
  UNTIL ;

: WAIT-CHAR ( source c -- flag )
  BEGIN
    OVER READ-CHAR IF
      C@ OVER = IF
        2DROP TRUE TRUE
      ELSE
        FALSE
      THEN
    ELSE
      DROP 2DROP FALSE TRUE
    THEN
  UNTIL ;    

: QUERY-CURSOR-POSITION ( source -- row column flag )
  BEGIN
    CSI [CHAR] 6 EMIT [CHAR] n EMIT
    DUP ESCAPE WAIT-CHAR IF
      DUP [CHAR] [ WAIT-CHAR IF
        HERE 1+ 10 [CHAR] ; 3 PICK READ-CHARS IF
          ['] PARSE-NUMBER 10 BASE-EXECUTE IF
            1- HERE 1+ 10 [CHAR] R 4 ROLL READ-CHARS IF
              ['] PARSE-NUMBER 10 BASE-EXECUTE IF
                1- TRUE TRUE
              ELSE
                DROP FALSE
              THEN
            ELSE
              2DROP FALSE
            THEN
          ELSE
            DROP FALSE
          THEN
        ELSE
          2DROP FALSE
        THEN
      ELSE
        2DROP 0 0 FALSE TRUE
      THEN
    ELSE
      2DROP 0 0 FALSE TRUE
    THEN
  UNTIL ;

: SET-CURSOR-POSITION ( source row column -- )
  DUP 3 PICK TERMINAL-COLUMN !
  2 PICK TERMINAL-INIT-COLUMN !
  DUP 2 PICK TERMINAL-ROW !
  SWAP TERMINAL-INIT-ROW ! ;

: SET-DEFAULT-CURSOR-POSITION ( source -- )
  2DROP 0 OVER TERMINAL-COLUMN !
  0 OVER TERMINAL-INIT-COLUMN !
  0 OVER TERMINAL-ROW !
  0 SWAP TERMINAL-INIT-ROW ! ;

: FIND-CURSOR-POSITION ( source -- )
  DUP QUERY-CURSOR-POSITION IF
    SET-CURSOR-POSITION
  ELSE
    SET-DEFAULT-CURSOR-POSITION
  THEN ;

: SET-TERMINAL-SIZE ( source row column -- )
  1+ 2 PICK TERMINAL-WIDTH ! 1+ SWAP TERMINAL-HEIGHT ! ;

: SET-DEFAULT-TERMINAL-SIZE ( source -- )
  2DROP 80 OVER TERMINAL-WIDTH ! 24 SWAP TERMINAL-HEIGHT ! ;

: FIND-TERMINAL-SIZE ( source -- )
  9999 9999 GO-TO-COORD
  DUP QUERY-CURSOR-POSITION IF
    SET-TERMINAL-SIZE
  ELSE
    SET-DEFAULT-TERMINAL-SIZE
  THEN ;

: REGISTER-FORWARD ( source u -- )
  DUP 0> IF
    0 ?DO
      DUP TERMINAL-COLUMN @ 1+ OVER TERMINAL-WIDTH @ >= IF
        0 OVER TERMINAL-COLUMN !
        DUP TERMINAL-ROW @ 1+ OVER TERMINAL-ROW !
      ELSE
        DUP TERMINAL-COLUMN @ 1+ OVER TERMINAL-COLUMN !
      THEN
    LOOP
    DUP TERMINAL-ROW @ OVER TERMINAL-HEIGHT @ >= IF
      DUP TERMINAL-ROW @ OVER TERMINAL-HEIGHT @ - 1+
      DUP 2 PICK TERMINAL-INIT-ROW @ >= IF
        OVER TERMINAL-INIT-ROW @ SWAP - OVER TERMINAL-INIT-ROW !
      ELSE
        DROP 0 OVER TERMINAL-INIT-ROW ! 0 OVER TERMINAL-INIT-COLUMN !
      THEN
      DUP TERMINAL-COLUMN @ 0= IF
        ESCAPE EMIT [CHAR] D EMIT
        9999 0 GO-TO-COORD
      THEN
      DUP TERMINAL-HEIGHT @ 1- OVER TERMINAL-ROW !
    ELSE
      DUP TERMINAL-COLUMN @ 0= IF
        DUP TERMINAL-ROW @ 0 GO-TO-COORD
      THEN
    THEN
    DROP
  ELSE
    2DROP
  THEN ;

: GO-FORWARD-ONE-CHAR ( source -- )
  DUP TERMINAL-COLUMN @ 1+ OVER TERMINAL-WIDTH @ >= IF
    0 OVER TERMINAL-COLUMN !
    DUP TERMINAL-ROW @ 1+ SWAP TERMINAL-ROW !
  ELSE
    DUP TERMINAL-COLUMN @ 1+ SWAP TERMINAL-COLUMN !
  THEN
  CSI [CHAR] C EMIT ;

: HANDLE-AT-FIRST-COLUMN-OF-LAST-ROW ( -- )
  ESCAPE EMIT [CHAR] D EMIT 9999 0 GO-TO-COORD ;

: HANDLE-REACHED-TERMINAL-END ( source -- )
  DUP TERMINAL-ROW @ OVER TERMINAL-HEIGHT @ - 1+
  DUP 2 PICK TERMINAL-INIT-ROW @ >= IF
    OVER TERMINAL-INIT-ROW @ SWAP - OVER TERMINAL-INIT-ROW !
  ELSE
    DROP 0 OVER TERMINAL-INIT-ROW ! 0 OVER TERMINAL-INIT-COLUMN !
  THEN
  DUP TERMINAL-COLUMN @ 0= IF HANDLE-AT-FIRST-COLUMN-OF-LAST-ROW THEN
  DUP TERMINAL-HEIGHT @ 1- SWAP TERMINAL-ROW ! ;

: HANDLE-AT-FIRST-COLUMN ( source -- ) TERMINAL-ROW @ 0 GO-TO-COORD ;

: GO-FORWARD ( source u -- )
  DUP 0> IF
    0 ?DO
      DUP GO-FORWARD-ONE-CHAR
      DUP TERMINAL-ROW @ OVER TERMINAL-HEIGHT @ >= IF
        HANDLE-REACHED-TERMINAL-END
      ELSE
        DUP TERMINAL-COLUMN @ 0= IF DUP HANDLE-AT-FIRST-COLUMN THEN
      THEN
    LOOP
    DROP
  ELSE
    2DROP
  THEN ;

: GO-BACKWARD-ONE-COLUMN ( source -- )
  DUP TERMINAL-COLUMN @ 1- SWAP TERMINAL-COLUMN ! CSI [CHAR] D EMIT ;

: GO-BACKWARD-ONE-ROW ( source -- )
  DUP TERMINAL-ROW @ 1- DUP 9999 GO-TO-COORD OVER TERMINAL-ROW !
  DUP TERMINAL-WIDTH @ 1- SWAP TERMINAL-COLUMN ! ;  

: GO-BEFORE-TOP-ROW ( source -- )
  DROP 0 9999 GO-TO-COORD 0 OVER TERMINAL-INIT-ROW !
  0 OVER TERMINAL-INIT-COLUMN ! DUP TERMINAL-WIDTH @ 1- SWAP TERMINAL-COLUMN ! ;

: GO-BACKWARD ( source u -- )
  0 ?DO
    DUP TERMINAL-COLUMN @ 1- 0>= IF
      DUP GO-BACKWARD-ONE-COLUMN
    ELSE
      DUP TERMINAL-ROW @ 1- 0>= IF
        DUP GO-BACKWARD-ONE-ROW
      ELSE
        DUP GO-BEFORE-TOP-ROW
      THEN
    THEN
  LOOP
  DROP ;

: KILL-UNMERGED ( source index count -- )
  DUP ALLOCATE! 3 PICK INPUT-BUFFER @ 3 ROLL + OVER 3 PICK MOVE
  KILL-ENTRY-SIZE ALLOCATE! TUCK KILL-ENTRY-BUFFER !
  TUCK KILL-ENTRY-DATA-SIZE !
  2DUP SWAP TERMINAL-KILL-RING-LAST @ SWAP KILL-ENTRY-PREV !
  0 OVER KILL-ENTRY-NEXT !
  2DUP SWAP TERMINAL-KILL-RING-LAST @ DUP 0<> IF
    KILL-ENTRY-NEXT !
  ELSE
    2DROP 2DUP SWAP TERMINAL-KILL-RING-FIRST !
  THEN
  SWAP TERMINAL-KILL-RING-LAST ! ;

: KILL-MERGE-FORWARD ( source index count -- )
  2 PICK LAST-KILL-ENTRY-DATA-SIZE @ OVER + ALLOCATE!
  3 PICK LAST-KILL-ENTRY-BUFFER @ OVER 5 PICK LAST-KILL-ENTRY-DATA-SIZE @ MOVE
  3 PICK INPUT-BUFFER @ 3 ROLL + 
  OVER 4 PICK LAST-KILL-ENTRY-DATA-SIZE @ + 3 PICK MOVE
  2 PICK LAST-KILL-ENTRY-BUFFER @ FREE!
  2 PICK LAST-KILL-ENTRY-BUFFER !
  OVER LAST-KILL-ENTRY-DATA-SIZE @ + SWAP LAST-KILL-ENTRY-DATA-SIZE ! ;

: KILL-MERGE-BACKWARD ( source index count -- )
  2 PICK LAST-KILL-ENTRY-DATA-SIZE @ OVER + ALLOCATE!
  3 PICK LAST-KILL-ENTRY-BUFFER @ OVER 3 PICK +
  5 PICK LAST-KILL-ENTRY-DATA-SIZE @ MOVE
  3 PICK INPUT-BUFFER @ 3 ROLL + OVER 3 PICK MOVE
  2 PICK LAST-KILL-ENTRY-BUFFER @ FREE!
  2 PICK LAST-KILL-ENTRY-BUFFER !
  OVER LAST-KILL-ENTRY-DATA-SIZE @ + SWAP LAST-KILL-ENTRY-DATA-SIZE ! ;

: KILL-FORWARD ( source index count -- )
  DUP 0> IF
    2 PICK TERMINAL-KILL-RING-LAST @ 0<>
    3 PICK TERMINAL-PREV-COMMAND-TYPE @ KILL-FORWARD-COMMAND = AND IF
      KILL-MERGE-FORWARD
    ELSE
      2 PICK SET-KILL-FORWARD-COMMAND
      KILL-UNMERGED
    THEN
  ELSE
    2DROP DROP
  THEN ;

: KILL-BACKWARD ( source index count -- )
  DUP 0> IF
    2 PICK TERMINAL-KILL-RING-LAST @ 0<>
    3 PICK TERMINAL-PREV-COMMAND-TYPE @ KILL-BACKWARD-COMMAND = AND IF
      KILL-MERGE-BACKWARD
    ELSE
      2 PICK SET-KILL-BACKWARD-COMMAND
      KILL-UNMERGED
    THEN
  ELSE
    2DROP DROP
  THEN ;

: PRINT-REST-INPUT ( source -- )
  DUP TERMINAL-INSERT-OFFSET @ OVER INPUT-BUFFER @ +
  OVER INPUT-COUNT @ 2 PICK TERMINAL-INSERT-OFFSET @ - DUP 0> IF
    DUP ROT SWAP TYPE REGISTER-FORWARD
  ELSE
    2DROP DROP
  THEN ;

: PRINT-OFFSET ( source u -- )
  OVER TERMINAL-INSERT-OFFSET @ 2 PICK INPUT-BUFFER @ + OVER +
  2 PICK INPUT-COUNT @ 3 PICK TERMINAL-INSERT-OFFSET @ - 2 PICK - DUP 0> IF
    DUP ROT SWAP TYPE ROT SWAP REGISTER-FORWARD DROP
  ELSE
    2DROP 2DROP
  THEN ;

: PRINT-ALL-INPUT ( source -- )
  DUP INPUT-BUFFER @ OVER INPUT-COUNT @ DUP 0> IF
    DUP ROT SWAP TYPE REGISTER-FORWARD
  ELSE
    2DROP DROP
  THEN ;

: PRINT-FULL-LINE ( source -- )
  HIDE-CURSOR
  DUP TERMINAL-INIT-ROW @ OVER TERMINAL-INIT-COLUMN @ GO-TO-COORD
  DUP TERMINAL-INIT-ROW @ OVER TERMINAL-ROW !
  DUP TERMINAL-INIT-COLUMN @ OVER TERMINAL-COLUMN !
  PRINT-ALL-INPUT ERASE-END-OF-LINE ERASE-DOWN
  SHOW-CURSOR ;

: PRINT-DELETE ( source -- )
  HIDE-CURSOR
  DUP 1 GO-BACKWARD DUP PRINT-REST-INPUT ERASE-END-OF-LINE ERASE-DOWN
  DUP INPUT-COUNT @ OVER TERMINAL-INSERT-OFFSET @ -
  GO-BACKWARD
  SHOW-CURSOR ;

: PRINT-DELETE-FORWARD ( source -- )
  HIDE-CURSOR
  DUP PRINT-REST-INPUT ERASE-END-OF-LINE ERASE-DOWN
  DUP INPUT-COUNT @ OVER TERMINAL-INSERT-OFFSET @ - GO-BACKWARD
  SHOW-CURSOR ;

: PRINT-KILL ( source -- ) DROP ERASE-END-OF-LINE ERASE-DOWN ;

: PRINT-KILL-TO-NEXT-SPACE ( source -- )
  HIDE-CURSOR
  DUP PRINT-REST-INPUT ERASE-END-OF-LINE ERASE-DOWN
  DUP INPUT-COUNT @ OVER TERMINAL-INSERT-OFFSET @ - GO-BACKWARD
  SHOW-CURSOR ;

: PRINT-KILL-TO-PREV-SPACE ( source count -- )
  HIDE-CURSOR
  OVER SWAP GO-BACKWARD DUP PRINT-REST-INPUT ERASE-END-OF-LINE ERASE-DOWN
  DUP INPUT-COUNT @ OVER TERMINAL-INSERT-OFFSET @ - GO-BACKWARD
  SHOW-CURSOR ;

: PRINT-YANK ( source -- )
  HIDE-CURSOR
  DUP PRINT-REST-INPUT DUP INPUT-COUNT @ OVER TERMINAL-INSERT-OFFSET @ -
  OVER LAST-KILL-ENTRY-DATA-SIZE @ - GO-BACKWARD
  SHOW-CURSOR ;

: PRINT-YANK-PREV ( source -- )
  HIDE-CURSOR
  DUP FIRST-KILL-ENTRY-DATA-SIZE @ OVER SWAP GO-BACKWARD
  DUP DUP LAST-KILL-ENTRY-DATA-SIZE @ NEGATE PRINT-OFFSET
  ERASE-END-OF-LINE ERASE-DOWN DUP
  DUP INPUT-COUNT @ OVER TERMINAL-INSERT-OFFSET @ - GO-BACKWARD
  SHOW-CURSOR ;

: PRINT-INSERT-COMPLETION ( source old-size new-size -- )
  HIDE-CURSOR
  2 PICK 2 ROLL GO-BACKWARD OVER SWAP NEGATE PRINT-OFFSET
  ERASE-END-OF-LINE ERASE-DOWN DUP
  DUP INPUT-COUNT @ OVER TERMINAL-INSERT-OFFSET @ - GO-BACKWARD
  SHOW-CURSOR ;

: PRINT-START ( source -- )
  DUP TERMINAL-INSERT-OFFSET @ GO-BACKWARD ;

: PRINT-END ( source -- )
  DUP INPUT-COUNT @ OVER TERMINAL-INSERT-OFFSET @ - GO-FORWARD ;

: PRINT-REFRESH ( source -- )
  HIDE-CURSOR
  DUP TERMINAL-INIT-ROW @ OVER TERMINAL-INIT-COLUMN @
  2 PICK ROT ROT GO-TO-COORD ERASE-END-OF-LINE ERASE-DOWN PRINT-ALL-INPUT
  DUP INPUT-COUNT @ OVER TERMINAL-INSERT-OFFSET @ - GO-BACKWARD
  SHOW-CURSOR ;

: PRINT-NORMAL-CHAR ( source c -- )
  HIDE-CURSOR
  EMIT DUP 1 REGISTER-FORWARD DUP PRINT-REST-INPUT
  DUP INPUT-COUNT @ OVER TERMINAL-INSERT-OFFSET @ -
  GO-BACKWARD
  SHOW-CURSOR ;

: PRINT-INSERT-NEWLINE ( source -- )
  HIDE-CURSOR DUP TERMINAL-PROMPT @ DUP 0<> IF EXECUTE ELSE DROP THEN
  SAVE-CURSOR DUP FIND-CURSOR-POSITION DUP FIND-TERMINAL-SIZE RESTORE-CURSOR
  SHOW-CURSOR PRINT-REFRESH ;  

: HANDLE-NEWLINE ( source -- )
  LOCK-TERMINAL
  DUP SET-NORMAL-COMMAND DUP CLEAR-HISTORY-EDITS DUP ADD-HISTORY
  DUP DUP INPUT-COUNT @ OVER TERMINAL-INSERT-OFFSET @ - GO-FORWARD
  SPACE DUP 1 TRY-EXPAND-TERMINAL-BUFFER
  DUP INPUT-BUFFER @ OVER INPUT-COUNT @ + NEWLINE SWAP C!
  DUP INPUT-COUNT @ 1+ SWAP INPUT-COUNT !
  UNLOCK-TERMINAL ;

: HANDLE-DELETE ( source -- )
  LOCK-TERMINAL
  DUP SET-NORMAL-COMMAND
  DUP TERMINAL-INSERT-OFFSET @ 0> IF
    DUP PRINT-DELETE
    DUP TERMINAL-INSERT-OFFSET @ OVER INPUT-BUFFER @ + DUP 1-
    2 PICK INPUT-COUNT @ 3 PICK TERMINAL-INSERT-OFFSET @ - MOVE
    DUP INPUT-COUNT @ 1- OVER INPUT-COUNT !
    DUP TERMINAL-INSERT-OFFSET @ 1- SWAP TERMINAL-INSERT-OFFSET !
  ELSE DROP THEN
  UNLOCK-TERMINAL ;

: HANDLE-DELETE-FORWARD ( source -- )
  LOCK-TERMINAL
  DUP SET-NORMAL-COMMAND
  DUP INPUT-COUNT @ OVER TERMINAL-INSERT-OFFSET @ > IF
    DUP TERMINAL-INSERT-OFFSET @ OVER INPUT-BUFFER @ + 1+ DUP 1-
    2 PICK INPUT-COUNT @ 3 PICK TERMINAL-INSERT-OFFSET @ - 1- MOVE
    DUP INPUT-COUNT @ 1- OVER INPUT-COUNT ! PRINT-DELETE-FORWARD
  ELSE DROP THEN
  UNLOCK-TERMINAL ;

: HANDLE-KILL ( source -- )
  LOCK-TERMINAL
  DUP INPUT-COUNT @ OVER TERMINAL-INSERT-OFFSET @ > IF
    DUP PRINT-KILL
    DUP DUP TERMINAL-INSERT-OFFSET @ OVER INPUT-COUNT @ OVER - KILL-FORWARD
    DUP TERMINAL-INSERT-OFFSET @ SWAP INPUT-COUNT !
  ELSE DROP THEN
  UNLOCK-TERMINAL ;

: SKIP-SPACES-FORWARD ( source offset -- offset )
  BEGIN
    DUP 2 PICK INPUT-COUNT @ < IF
      OVER INPUT-BUFFER @ OVER + C@ DUP BL = SWAP TAB = OR IF
        1+ FALSE
      ELSE
        TRUE
      THEN
    ELSE
      TRUE
    THEN
  UNTIL
  NIP ;

: SKIP-TO-SPACE-FORWARD ( source offset -- offset )
  BEGIN
    DUP 2 PICK INPUT-COUNT @ < IF
      OVER INPUT-BUFFER @ OVER + C@ DUP BL = SWAP TAB = OR IF
        TRUE
      ELSE
        1+ FALSE
      THEN
    ELSE
      TRUE
    THEN
  UNTIL
  NIP ;

: FIND-NEXT-SPACE ( source -- offset count )
  DUP DUP TERMINAL-INSERT-OFFSET @ SKIP-SPACES-FORWARD
  OVER SWAP SKIP-TO-SPACE-FORWARD
  SWAP TERMINAL-INSERT-OFFSET @ DUP ROT SWAP - ;

: SKIP-SPACES-BACKWARD ( source offset -- offset )
  BEGIN
    DUP 0> IF
      OVER INPUT-BUFFER @ OVER 1- + C@ DUP BL = SWAP TAB = OR IF
        1- FALSE
      ELSE
        TRUE
      THEN
    ELSE
      TRUE
    THEN
  UNTIL
  NIP ;

: SKIP-TO-SPACE-BACKWARD ( source offset -- offset )
  BEGIN
    DUP 0> IF
      OVER INPUT-BUFFER @ OVER 1- + C@ DUP BL = SWAP TAB = OR IF
        TRUE
      ELSE
        1- FALSE
      THEN
    ELSE
      TRUE
    THEN
  UNTIL
  NIP ;

: FIND-PREV-SPACE ( source -- offset count )
  DUP DUP TERMINAL-INSERT-OFFSET @ SKIP-SPACES-BACKWARD
  OVER SWAP SKIP-TO-SPACE-BACKWARD
  DUP ROT TERMINAL-INSERT-OFFSET @ SWAP - ;

: HANDLE-KILL-TO-NEXT-SPACE ( source -- )
  LOCK-TERMINAL
  DUP FIND-NEXT-SPACE DUP 0> IF
    2 PICK 2 PICK 2 PICK KILL-FORWARD
    2 PICK INPUT-BUFFER @ 3 PICK TERMINAL-INSERT-OFFSET @ + OVER +
    3 PICK INPUT-BUFFER @ 4 PICK TERMINAL-INSERT-OFFSET @ +
    4 PICK INPUT-COUNT @ 5 PICK TERMINAL-INSERT-OFFSET @ - 3 PICK - MOVE
    2 PICK INPUT-COUNT @ OVER - 3 PICK INPUT-COUNT !
    2DROP PRINT-KILL-TO-NEXT-SPACE
  ELSE
    2DROP DROP
  THEN
  UNLOCK-TERMINAL ;

: HANDLE-KILL-TO-PREV-SPACE ( source -- )
  LOCK-TERMINAL
  DUP FIND-PREV-SPACE DUP 0> IF
    2 PICK 2 PICK 2 PICK KILL-BACKWARD
    2 PICK INPUT-BUFFER @ 2 PICK + OVER +
    3 PICK INPUT-BUFFER @ 3 PICK +
    4 PICK INPUT-COUNT @ 5 PICK TERMINAL-INSERT-OFFSET @ - MOVE
    2 PICK INPUT-COUNT @ OVER - 3 PICK INPUT-COUNT !
    2 PICK TERMINAL-INSERT-OFFSET @ OVER - 3 PICK TERMINAL-INSERT-OFFSET !
    NIP PRINT-KILL-TO-PREV-SPACE
  ELSE
    2DROP DROP
  THEN
  UNLOCK-TERMINAL ;

: HANDLE-YANK ( source -- )
  LOCK-TERMINAL
  DUP SET-YANK-COMMAND
  DUP TERMINAL-KILL-RING-LAST @ 0<> IF
    DUP DUP LAST-KILL-ENTRY-DATA-SIZE @ TRY-EXPAND-TERMINAL-BUFFER
    DUP INPUT-BUFFER @ OVER TERMINAL-INSERT-OFFSET @ +
    OVER INPUT-BUFFER @ 2 PICK TERMINAL-INSERT-OFFSET @ +
    2 PICK LAST-KILL-ENTRY-DATA-SIZE @ +
    2 PICK INPUT-COUNT @ 3 PICK TERMINAL-INSERT-OFFSET @ - MOVE
    DUP LAST-KILL-ENTRY-BUFFER @
    OVER INPUT-BUFFER @ 2 PICK TERMINAL-INSERT-OFFSET @ +
    2 PICK LAST-KILL-ENTRY-DATA-SIZE @ MOVE
    DUP INPUT-COUNT @ OVER LAST-KILL-ENTRY-DATA-SIZE @ + OVER INPUT-COUNT !
    DUP PRINT-YANK
    DUP TERMINAL-INSERT-OFFSET @ OVER LAST-KILL-ENTRY-DATA-SIZE @ +
    SWAP TERMINAL-INSERT-OFFSET !
  ELSE
    DROP
  THEN
  UNLOCK-TERMINAL ;

: HANDLE-YANK-PREV ( source -- )
  LOCK-TERMINAL
  DUP TERMINAL-PREV-COMMAND-TYPE @ YANK-COMMAND =
  OVER TERMINAL-KILL-RING-LAST @ 2 PICK TERMINAL-KILL-RING-FIRST @ <> AND IF
    DUP LAST-KILL-ENTRY-DATA-SIZE @
    OVER MOVE-LAST-KILL-ENTRY-TO-FIRST
    OVER LAST-KILL-ENTRY-DATA-SIZE @ OVER - DUP 0> IF
      DUP 3 PICK SWAP TRY-EXPAND-TERMINAL-BUFFER
    THEN
    2 PICK INPUT-BUFFER @ 3 PICK TERMINAL-INSERT-OFFSET @ +
    DUP 2 PICK + 4 PICK INPUT-COUNT @ 5 PICK TERMINAL-INSERT-OFFSET @ - MOVE
    NIP
    OVER TERMINAL-INSERT-OFFSET @ OVER + 2 PICK TERMINAL-INSERT-OFFSET !
    OVER LAST-KILL-ENTRY-BUFFER @
    2 PICK INPUT-BUFFER @ 3 PICK TERMINAL-INSERT-OFFSET @ +
    3 PICK LAST-KILL-ENTRY-DATA-SIZE @ - 3 PICK LAST-KILL-ENTRY-DATA-SIZE @ MOVE
    OVER INPUT-COUNT @ OVER + 2 PICK INPUT-COUNT !
    DROP PRINT-YANK-PREV
  ELSE
    DROP
  THEN
  UNLOCK-TERMINAL ;

: HANDLE-REFRESH ( source -- )
  LOCK-TERMINAL
  DUP SET-NORMAL-COMMAND PRINT-REFRESH
  UNLOCK-TERMINAL ;

: HANDLE-BYE ( source -- ) DROP BYE ;

: HANDLE-START ( source -- )
  LOCK-TERMINAL
  DUP SET-NORMAL-COMMAND
  DUP PRINT-START 0 SWAP TERMINAL-INSERT-OFFSET !
  UNLOCK-TERMINAL ;

: HANDLE-END ( source -- )
  LOCK-TERMINAL
  DUP SET-NORMAL-COMMAND
  DUP PRINT-END DUP INPUT-COUNT @ SWAP TERMINAL-INSERT-OFFSET !
  UNLOCK-TERMINAL ;

: HANDLE-FORWARD ( source -- )
  LOCK-TERMINAL
  DUP SET-NORMAL-COMMAND
  DUP INPUT-COUNT @ OVER TERMINAL-INSERT-OFFSET @ > IF
    DUP 1 GO-FORWARD DUP TERMINAL-INSERT-OFFSET @ 1+
    SWAP TERMINAL-INSERT-OFFSET !
  ELSE
    DROP
  THEN
  UNLOCK-TERMINAL ;

: HANDLE-BACKWARD ( source -- )
  LOCK-TERMINAL
  DUP SET-NORMAL-COMMAND
  DUP TERMINAL-INSERT-OFFSET @ 0> IF
    DUP 1 GO-BACKWARD DUP TERMINAL-INSERT-OFFSET @ 1-
    SWAP TERMINAL-INSERT-OFFSET !
  ELSE
    DROP
  THEN
  UNLOCK-TERMINAL ;

: HANDLE-FORWARD-WORD ( source -- )
  LOCK-TERMINAL
  DUP SET-NORMAL-COMMAND
  DUP FIND-NEXT-SPACE DUP 0> IF
    DUP 3 PICK SWAP  GO-FORWARD + SWAP TERMINAL-INSERT-OFFSET !
  ELSE
    2DROP DROP
  THEN
  UNLOCK-TERMINAL ;

: HANDLE-BACKWARD-WORD ( source -- )
  LOCK-TERMINAL
  DUP SET-NORMAL-COMMAND
  DUP FIND-PREV-SPACE DUP 0> IF
    DUP 3 PICK SWAP GO-BACKWARD DROP SWAP TERMINAL-INSERT-OFFSET !
  ELSE
    2DROP DROP
  THEN
  UNLOCK-TERMINAL ;

: ACTUALLY-HANDLE-UP ( source -- )
  DUP EDIT-HISTORY DUP INPUT-COUNT @ OVER TERMINAL-INSERT-OFFSET !
  PRINT-FULL-LINE ;

: TEST-WORD-INITIAL ( c-addr u xt -- matches )
  NAME>STRING 2 PICK SWAP <= IF
    OVER COMPARE-CASE 0=
  ELSE
    DROP 2DROP FALSE
  THEN ;

: TEST-WORDLIST-REST-COMPLETION ( c-addr u xt -- xt duplicate )
  DUP NAME>NEXT BEGIN
    DUP 0<> IF
      3 PICK 3 PICK 2 PICK TEST-WORD-INITIAL IF
        2DROP 2DROP 0 TRUE TRUE
      ELSE
        NAME>NEXT FALSE
      THEN
    ELSE
      DROP ROT ROT 2DROP FALSE TRUE
    THEN
  UNTIL ;

: TEST-WORDLIST-COMPLETION ( c-addr u wid -- xt duplicate )
  FIRST-WORD BEGIN
    DUP 0<> IF
      2 PICK 2 PICK 2 PICK TEST-WORD-INITIAL IF
        TEST-WORDLIST-REST-COMPLETION TRUE
      ELSE
        NAME>NEXT FALSE
      THEN
    ELSE
      DROP 2DROP 0 FALSE TRUE
    THEN
  UNTIL ;

: TEST-REST-COMPLETION ( x*u2 c-addr u1 u2 xt -- xt duplicate )
  BEGIN
    OVER 0> IF
      1- 3 PICK 3 PICK 6 ROLL TEST-WORDLIST-COMPLETION SWAP 0<> AND IF
        DROP 2 + DROPS 0 TRUE TRUE
      ELSE
	FALSE
      THEN
    ELSE
      NIP NIP NIP FALSE TRUE
    THEN
  UNTIL ;

: TEST-ONLY-COMPLETION ( c-addr u -- xt duplicate )
  2>R GET-ORDER 2R> ROT BEGIN
    DUP 0> IF
      1- 2 PICK 2 PICK 5 ROLL TEST-WORDLIST-COMPLETION IF
        DROP 2 + DROPS 0 TRUE TRUE
      ELSE DUP 0<> IF
        TEST-REST-COMPLETION TRUE
      ELSE
        DROP FALSE
      THEN THEN
    ELSE
      DROP 2DROP 0 FALSE TRUE
    THEN
  UNTIL ;

: INSERT-COMPLETION ( source offset u1 c-addr2 u2 -- )
  4 PICK OVER 3 PICK - TRY-EXPAND-TERMINAL-BUFFER
  4 PICK INPUT-BUFFER @ 4 PICK + 3 PICK +
  5 PICK INPUT-BUFFER @ 5 PICK + 2 PICK +
  6 PICK INPUT-COUNT @ 6 PICK - 5 PICK - MOVE
  4 PICK INPUT-BUFFER @ 4 PICK + ROT SWAP 2 PICK MOVE
  3 PICK TERMINAL-INSERT-OFFSET @ 2 PICK - OVER +
  4 PICK TERMINAL-INSERT-OFFSET !
  3 PICK INPUT-COUNT @ 2 PICK - OVER + 4 PICK INPUT-COUNT !
  ROT DROP PRINT-INSERT-COMPLETION ;

: DISPLAY-WORDLIST-COMPLETIONS ( c-addr u wid -- )
  FIRST-WORD BEGIN
    DUP 0<> IF
      2 PICK 2 PICK 2 PICK TEST-WORD-INITIAL IF
        DUP NAME>STRING TYPE SPACE
      THEN
      NAME>NEXT FALSE
    ELSE
      DROP 2DROP TRUE
    THEN
  UNTIL ;

: DISPLAY-COMPLETIONS ( source c-addr u -- )
  CR
  >R >R >R GET-ORDER R> R> R> 3 ROLL BEGIN
    DUP 0> IF
      1- 2 PICK 2 PICK 6 ROLL DISPLAY-WORDLIST-COMPLETIONS FALSE
    ELSE
      DROP 2DROP TRUE
    THEN
  UNTIL
  CR
  PRINT-INSERT-NEWLINE ;

: HANDLE-TAB ( source -- )
  LOCK-TERMINAL
  DUP TERMINAL-COMPLETE-ENABLE @ IF
    DUP FIND-PREV-SPACE DUP 0> IF
      2 PICK INPUT-BUFFER @ 2 PICK + OVER TEST-ONLY-COMPLETION INVERT IF
        DUP 0<> IF
          NAME>STRING INSERT-COMPLETION
	ELSE
	  2DROP 2DROP
	THEN
      ELSE
        DROP 2 PICK INPUT-BUFFER @ ROT + SWAP DISPLAY-COMPLETIONS
      THEN
    ELSE
      2DROP DROP
    THEN
  ELSE
    DROP
  THEN
  UNLOCK-TERMINAL ;

: HANDLE-UP ( source -- )
  LOCK-TERMINAL
  DUP SET-NORMAL-COMMAND
  DUP TERMINAL-HISTORY-LAST @ 0<> IF
    DUP TERMINAL-HISTORY-CURRENT @ 0<> IF
      DUP TERMINAL-HISTORY-CURRENT @ HISTORY-ENTRY-PREV @ 0<> IF
        DUP SAVE-HISTORY-EDIT
        DUP TERMINAL-HISTORY-CURRENT @ HISTORY-ENTRY-PREV @
        OVER TERMINAL-HISTORY-CURRENT !
        ACTUALLY-HANDLE-UP
      ELSE
        DROP
      THEN
    ELSE
      DUP ADD-HISTORY
      DUP TERMINAL-HISTORY-LAST @ OVER TERMINAL-HISTORY-CURRENT !
      ACTUALLY-HANDLE-UP
    THEN
  ELSE
    DROP
  THEN
  UNLOCK-TERMINAL ;

: HANDLE-DOWN ( source -- )
  LOCK-TERMINAL
  DUP SET-NORMAL-COMMAND
  DUP TERMINAL-HISTORY-CURRENT @ 0<> IF
    DUP SAVE-HISTORY-EDIT
    DUP TERMINAL-HISTORY-CURRENT @ HISTORY-ENTRY-NEXT @
    OVER TERMINAL-HISTORY-CURRENT !
    DUP TERMINAL-HISTORY-CURRENT @ 0<> IF
      DUP EDIT-HISTORY DUP INPUT-COUNT @ OVER TERMINAL-INSERT-OFFSET !
      PRINT-FULL-LINE
    ELSE
      0 OVER INPUT-COUNT ! 0 OVER TERMINAL-INSERT-OFFSET ! PRINT-FULL-LINE
    THEN
  ELSE
    DUP ADD-HISTORY
    0 OVER INPUT-COUNT ! 0 OVER TERMINAL-INSERT-OFFSET ! PRINT-FULL-LINE
  THEN
  UNLOCK-TERMINAL ;

: HANDLE-CTRL-SPECIAL ( source -- continue )
  DUP READ-CHAR IF
    C@ [CHAR] ; = IF
      DUP READ-CHAR IF
        C@ [CHAR] 5 = IF
          DUP READ-CHAR IF
            C@ CASE
              [CHAR] C OF HANDLE-FORWARD-WORD TRUE ENDOF
              [CHAR] D OF HANDLE-BACKWARD-WORD TRUE ENDOF
              NIP TRUE SWAP
            ENDCASE
          ELSE
            TRUE SWAP INPUT-IS-CLOSED ! FALSE
          THEN
        ELSE
          DROP TRUE
        THEN
      ELSE
        TRUE SWAP INPUT-IS-CLOSED ! FALSE
      THEN
    ELSE
      DROP TRUE
    THEN
  ELSE
    TRUE SWAP INPUT-IS-CLOSED ! FALSE
  THEN ;

: HANDLE-SPECIAL ( source -- continue )
  DUP READ-CHAR IF
    C@ CASE
      [CHAR] C OF HANDLE-FORWARD TRUE ENDOF
      [CHAR] D OF HANDLE-BACKWARD TRUE ENDOF
      [CHAR] A OF HANDLE-UP TRUE ENDOF
      [CHAR] B OF HANDLE-DOWN TRUE ENDOF
      [CHAR] 3 OF
        DUP READ-CHAR IF
          C@ [CHAR] ~ = IF
            HANDLE-DELETE-FORWARD TRUE
          ELSE
            DROP TRUE
          THEN
        ELSE
          TRUE SWAP INPUT-IS-CLOSED ! FALSE
        THEN
      ENDOF
      [CHAR] 1 OF HANDLE-CTRL-SPECIAL ENDOF
      NIP TRUE SWAP
    ENDCASE
  ELSE
    DROP TRUE SWAP INPUT-IS-CLOSED ! FALSE
  THEN ;

: HANDLE-ESCAPE ( source -- continue )
  DUP READ-CHAR IF
    C@ CASE
      [CHAR] [ OF HANDLE-SPECIAL ENDOF
      [CHAR] f OF HANDLE-FORWARD-WORD TRUE ENDOF
      [CHAR] b OF HANDLE-BACKWARD-WORD TRUE ENDOF
      [CHAR] y OF HANDLE-YANK-PREV TRUE ENDOF
      [CHAR] d OF HANDLE-KILL-TO-NEXT-SPACE TRUE ENDOF
      DELETE OF HANDLE-KILL-TO-PREV-SPACE TRUE ENDOF
      DUP >R HANDLE-CHAR R>
    ENDCASE
  ELSE
    DROP TRUE SWAP INPUT-IS-CLOSED ! FALSE
  THEN ;

: HANDLE-NORMAL-CHAR ( source c -- )
  LOCK-TERMINAL
  OVER SET-NORMAL-COMMAND
  2DUP PRINT-NORMAL-CHAR OVER 1 TRY-EXPAND-TERMINAL-BUFFER
  OVER INPUT-BUFFER @ 2 PICK TERMINAL-INSERT-OFFSET @ + DUP 1+
  3 PICK INPUT-COUNT @ 4 PICK TERMINAL-INSERT-OFFSET @ - MOVE
  OVER INPUT-BUFFER @ 2 PICK TERMINAL-INSERT-OFFSET @ + C!
  DUP INPUT-COUNT @ 1+ OVER INPUT-COUNT !
  DUP TERMINAL-INSERT-OFFSET @ 1+ SWAP TERMINAL-INSERT-OFFSET !
  UNLOCK-TERMINAL ;

:NONAME ( source c -- continue )
  CASE
    NEWLINE OF HANDLE-NEWLINE FALSE ENDOF
    DELETE OF HANDLE-DELETE TRUE ENDOF
    TAB OF HANDLE-TAB TRUE ENDOF
    ESCAPE OF HANDLE-ESCAPE ENDOF
    CTRL-A OF HANDLE-START TRUE ENDOF
    CTRL-D OF HANDLE-BYE TRUE ENDOF
    CTRL-E OF HANDLE-END TRUE ENDOF
    CTRL-F OF HANDLE-FORWARD TRUE ENDOF
    CTRL-B OF HANDLE-BACKWARD TRUE ENDOF
    CTRL-K OF HANDLE-KILL TRUE ENDOF
    CTRL-L OF HANDLE-REFRESH TRUE ENDOF
    CTRL-W OF HANDLE-KILL-TO-PREV-SPACE TRUE ENDOF
    CTRL-Y OF HANDLE-YANK TRUE ENDOF
    DUP $20 < IF NIP TRUE SWAP ELSE TUCK HANDLE-NORMAL-CHAR TRUE SWAP THEN
  ENDCASE ; IS HANDLE-CHAR

: READ-TERMINAL-CHAR ( source -- continue )
  DUP READ-CHAR IF
    C@ HANDLE-CHAR
  ELSE
    DROP TRUE SWAP INPUT-IS-CLOSED ! FALSE
  THEN ;

: RESET-TERMINAL-INPUT ( source -- )
  0 OVER INPUT-COUNT ! 0 OVER INPUT-INDEX !
  0 SWAP TERMINAL-INSERT-OFFSET ! ;

: REFILL-TERMINAL ( source -- flag )
  LOCK-TERMINAL HIDE-CURSOR
  DUP TERMINAL-PROMPT @ DUP 0<> IF EXECUTE ELSE DROP THEN
  SAVE-CURSOR 0 OVER TERMINAL-HISTORY-CURRENT !
  DUP FIND-CURSOR-POSITION DUP FIND-TERMINAL-SIZE RESTORE-CURSOR SHOW-CURSOR
  UNLOCK-TERMINAL DUP RESET-TERMINAL-INPUT
  BEGIN DUP READ-TERMINAL-CHAR INVERT UNTIL DROP TRUE ;

256 CONSTANT DEFAULT-TERMINAL-BUFFER-SIZE

: TERMINAL>SOURCE-CLEANUP ( source -- )
  BEGIN
    DUP TERMINAL-KILL-RING-LAST @ DUP 0<> IF
      DUP KILL-ENTRY-NEXT @ 2 PICK TERMINAL-KILL-RING-LAST !
      DUP KILL-ENTRY-BUFFER @ FREE! FREE! FALSE
    ELSE
      DROP TRUE
    THEN
  UNTIL
  DUP INPUT-BUFFER @ FREE! DUP INPUT-ARG @ FREE! FREE! ;

: TERMINAL>SOURCE ( fd -- input )
  ENABLE-WRAP
  INPUT-SIZE ALLOCATE! TERMINAL-INFO-SIZE ALLOCATE! OVER INPUT-ARG !
  DEFAULT-TERMINAL-BUFFER-SIZE OVER TERMINAL-BUFFER-SIZE !
  0 OVER TERMINAL-INSERT-OFFSET !
  0 OVER TERMINAL-KILL-RING-FIRST !
  0 OVER TERMINAL-KILL-RING-LAST !
  0 OVER TERMINAL-HISTORY-LAST !
  0 OVER TERMINAL-HISTORY-CURRENT !
  0 OVER TERMINAL-PROMPT !
  FALSE OVER TERMINAL-COMPLETE-ENABLE !
  DUP SET-NORMAL-COMMAND
  SWAP OVER TERMINAL-FD !
  DEFAULT-TERMINAL-BUFFER-SIZE ALLOCATE! OVER INPUT-BUFFER !
  0 OVER INPUT-INDEX ! 0 OVER INPUT-COUNT !
  FALSE OVER INPUT-IS-CLOSED !
  ['] TERMINAL>SOURCE-CLEANUP OVER INPUT-CLEANUP !
  ['] REFILL-TERMINAL OVER INPUT-REFILL !
  0 OVER INPUT-SOURCE-ID ! 0 OVER INPUT-NEXT-INPUT ! ;

FORTH-WORDLIST SET-CURRENT

0 HERE ! HERE 1 CELLS TASK-LOCAL CONSTANT STORED-KEY
FALSE HERE ! HERE 1 CELLS TASK-LOCAL CONSTANT KEY-IS-STORED

: KEY? ( -- flag )
  KEY-IS-STORED TT@ 0= IF
    HERE DUP 1 CHARS IO-STDIN IO-READ-NONBLOCK 0= IF
      IF
        2DROP FALSE
      ELSE
        0<> IF
	  C@ STORED-KEY TT!
	  TRUE KEY-IS-STORED TT!
	  TRUE
	ELSE
	  DROP TRUE ABORT" end of input"
	THEN
      THEN
    ELSE
      DROP TRUE ABORT" error reading standard input"
    THEN
  ELSE
    TRUE
  THEN ;

: KEY ( -- c )
  KEY-IS-STORED TT@ 0= IF
    HERE DUP 1 IO-STDIN IO-READ-BLOCK DUP IO-ACTION-GET-STATE DUP
    IO-STATE-GET-INDEX 1 = SWAP IO-STATE-DESTROY SWAP IO-ACTION-DESTROY
    0= ABORT" end of input" C@
  ELSE
    FALSE KEY-IS-STORED TT!
    STORED-KEY TT@
  THEN ;

0 HERE ! HERE 1 CELLS TASK-LOCAL CONSTANT ACCEPT-INPUT

: CLEANUP-ACCEPT-INPUT ( -- )
  ACCEPT-INPUT TT@ DUP 0<> IF
    DUP INPUT-CLEANUP @ DUP 0<> IF EXECUTE ELSE 2DROP THEN
  ELSE
    DROP
  THEN ;

: GET-ACCEPT-INPUT ( -- input )
  ACCEPT-INPUT TT@ DUP 0= IF
    DROP IO-STDIN TERMINAL>SOURCE DUP ACCEPT-INPUT TT!
    ['] CLEANUP-ACCEPT-INPUT THIS-TASK ATEXIT
  THEN ;

: ACCEPT ( c-addr u1 -- u2 )
  GET-ACCEPT-INPUT DUP REFILL-TERMINAL DROP
  DUP INPUT-COUNT @ ROT MIN OVER INPUT-BUFFER @
  OVER 0> IF
    2DUP + 1- C@ NEWLINE = IF
      SWAP 1- SWAP
    THEN
  THEN
  3 ROLL 2 PICK MOVE NIP ;

0 HERE ! HERE 1 CELLS TASK-LOCAL CONSTANT REPL

:NONAME
  IO-STDIN TERMINAL>SOURCE DUP REPL TT!
  TRUE REPL TT@ TERMINAL-COMPLETE-ENABLE !
  THIS-TASK CLEANUP-INPUT
  THIS-TASK >CONSOLE-INPUT FORTH-WORDLIST DUP 1 SET-ORDER SET-CURRENT
  ." ready" CR QUIT ; EXECUTE