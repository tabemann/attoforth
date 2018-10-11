FORTH-WORDLIST IO-WORDLIST TASK-WORDLIST 3 SET-ORDER

IO-WORDLIST SET-CURRENT

12 CELLS CONSTANT TERMINAL-INFO-SIZE

: INPUT-ARG-OFFSET ( "name" -- ) CREATE , DOES> @ SWAP INPUT-ARG @ + ;

0 CELLS INPUT-ARG-OFFSET TERMINAL-FD
1 CELLS INPUT-ARG-OFFSET TERMINAL-BUFFER-SIZE
2 CELLS INPUT-ARG-OFFSET TERMINAL-INSERT-OFFSET
3 CELLS INPUT-ARG-OFFSET TERMINAL-ROW
4 CELLS INPUT-ARG-OFFSET TERMINAL-COLUMN
5 CELLS INPUT-ARG-OFFSET TERMINAL-WIDTH
6 CELLS INPUT-ARG-OFFSET TERMINAL-HEIGHT
7 CELLS INPUT-ARG-OFFSET TERMINAL-INIT-ROW
8 CELLS INPUT-ARG-OFFSET TERMINAL-INIT-COLUMN
9 CELLS INPUT-ARG-OFFSET TERMINAL-KILL-RING-FIRST
10 CELLS INPUT-ARG-OFFSET TERMINAL-KILL-RING-LAST
11 CELLS INPUT-ARG-OFFSET TERMINAL-PREV-COMMAND-TYPE

4 CELLS CONSTANT KILL-ENTRY-SIZE
0 CELLS OFFSET KILL-ENTRY-PREV
1 CELLS OFFSET KILL-ENTRY-NEXT
2 CELLS OFFSET KILL-ENTRY-DATA-SIZE
3 CELLS OFFSET KILL-ENTRY-BUFFER

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

: DEC. ( n -- ) 10 BASE. ;

: CSI ( -- ) ESCAPE EMIT [CHAR] [ EMIT ;

: ENABLE-WRAP ( -- ) CSI [CHAR] 7 EMIT [CHAR] h EMIT ;

: SHOW-CURSOR ( -- ) CSI [CHAR] ? EMIT 25 DEC. [CHAR] h EMIT ;

: HIDE-CURSOR ( -- ) CSI [CHAR] ? EMIT 25 DEC. [CHAR] l EMIT ;

: SAVE-CURSOR ( -- ) CSI [CHAR] s EMIT ;

: RESTORE-CURSOR ( -- )  CSI [CHAR] u EMIT ;

: GO-TO-COORD ( row column -- )
  SWAP CSI 1+ DEC. [CHAR] ; EMIT 1+ DEC. [CHAR] f EMIT ;

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

: GO-FORWARD ( source u -- )
  DUP 0> IF
    0 ?DO
      DUP TERMINAL-COLUMN @ 1+ OVER TERMINAL-WIDTH @ >= IF
        0 OVER TERMINAL-COLUMN !
        DUP TERMINAL-ROW @ 1+ OVER TERMINAL-ROW !
      ELSE
        DUP TERMINAL-COLUMN @ 1+ OVER TERMINAL-COLUMN !
      THEN
      CSI [CHAR] C EMIT
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

: GO-BACKWARD ( source u -- )
  0 ?DO
    DUP TERMINAL-COLUMN @ 1- DUP 0>= IF
      OVER TERMINAL-COLUMN !
      CSI [CHAR] D EMIT
    ELSE
      DROP DUP TERMINAL-ROW @ 1- DUP 0>= IF
        9999 GO-TO-COORD
        DUP TERMINAL-ROW @ 1- OVER TERMINAL-ROW !
      ELSE
        DROP 0 9999 GO-TO-COORD
        0 OVER TERMINAL-INIT-ROW !
        0 OVER TERMINAL-INIT-COLUMN !
      THEN
      DUP TERMINAL-WIDTH @ 1- OVER TERMINAL-COLUMN !
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

: HANDLE-NEWLINE ( source -- )
  DUP SET-NORMAL-COMMAND
  DUP DUP INPUT-COUNT @ OVER TERMINAL-INSERT-OFFSET @ - GO-FORWARD
  SPACE DUP 1 TRY-EXPAND-TERMINAL-BUFFER
  DUP INPUT-BUFFER @ OVER INPUT-COUNT @ + NEWLINE SWAP C!
  DUP INPUT-COUNT @ 1+ SWAP INPUT-COUNT ! ;

: HANDLE-DELETE ( source -- )
  DUP SET-NORMAL-COMMAND
  DUP TERMINAL-INSERT-OFFSET @ 0> IF
    DUP PRINT-DELETE
    DUP TERMINAL-INSERT-OFFSET @ OVER INPUT-BUFFER @ + DUP 1-
    2 PICK INPUT-COUNT @ 3 PICK TERMINAL-INSERT-OFFSET @ - MOVE
    DUP INPUT-COUNT @ 1- OVER INPUT-COUNT !
    DUP TERMINAL-INSERT-OFFSET @ 1- SWAP TERMINAL-INSERT-OFFSET !
  ELSE DROP THEN ;

: HANDLE-DELETE-FORWARD ( source -- )
  DUP SET-NORMAL-COMMAND
  DUP INPUT-COUNT @ OVER TERMINAL-INSERT-OFFSET @ > IF
    DUP TERMINAL-INSERT-OFFSET @ OVER INPUT-BUFFER @ + 1+ DUP 1-
    2 PICK INPUT-COUNT @ 3 PICK TERMINAL-INSERT-OFFSET @ - 1- MOVE
    DUP INPUT-COUNT @ 1- OVER INPUT-COUNT ! PRINT-DELETE-FORWARD
  ELSE DROP THEN ;

: HANDLE-KILL ( source -- )
  DUP INPUT-COUNT @ OVER TERMINAL-INSERT-OFFSET @ > IF
    DUP PRINT-KILL
    DUP DUP TERMINAL-INSERT-OFFSET @ OVER INPUT-COUNT @ OVER - KILL-FORWARD
    DUP TERMINAL-INSERT-OFFSET @ SWAP INPUT-COUNT !
  ELSE DROP THEN ;

: FIND-PREV-SPACE ( source -- offset count )
  DUP TERMINAL-INSERT-OFFSET @ BEGIN
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
  DUP ROT TERMINAL-INSERT-OFFSET @ SWAP - ;

: HANDLE-KILL-TO-PREV-SPACE ( source -- )
  DUP FIND-PREV-SPACE
  DUP 0> IF
    2 PICK 2 PICK 2 PICK KILL-BACKWARD
    2 PICK INPUT-BUFFER @ 2 PICK + OVER +
    3 PICK INPUT-BUFFER @ 3 PICK +
    4 PICK INPUT-COUNT @ 5 PICK TERMINAL-INSERT-OFFSET @ - MOVE
    2 PICK INPUT-COUNT @ OVER - 3 PICK INPUT-COUNT !
    2 PICK TERMINAL-INSERT-OFFSET @ OVER - 3 PICK TERMINAL-INSERT-OFFSET !
    NIP PRINT-KILL-TO-PREV-SPACE
  ELSE
    2DROP DROP
  THEN ;

: HANDLE-YANK ( source -- )
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
  THEN ;

: HANDLE-YANK-PREV ( source -- )
  DUP TERMINAL-PREV-COMMAND-TYPE @ YANK-COMMAND =
  OVER TERMINAL-KILL-RING-LAST @ 2 PICK TERMINAL-KILL-RING-FIRST @ <> AND IF
    DUP LAST-KILL-ENTRY-DATA-SIZE @
    OVER MOVE-LAST-KILL-ENTRY-TO-FIRST
    OVER LAST-KILL-ENTRY-DATA-SIZE @ OVER - DUP 0> IF
      DUP 3 PICK SWAP TRY-EXPAND-TERMINAL-BUFFER
    THEN
    2 PICK INPUT-BUFFER @ 3 PICK TERMINAL-INSERT-OFFSET @ +
    DUP 2 PICK + 4 PICK INPUT-COUNT @ 5 PICK TERMINAL-INSERT-OFFSET @ - MOVE
    NIP OVER LAST-KILL-ENTRY-BUFFER @
    2 PICK INPUT-BUFFER @ 3 PICK TERMINAL-INSERT-OFFSET @ +
    3 PICK LAST-KILL-ENTRY-DATA-SIZE @ - 3 PICK LAST-KILL-ENTRY-DATA-SIZE @ MOVE
    OVER INPUT-COUNT @ OVER + 2 PICK INPUT-COUNT !
    OVER TERMINAL-INSERT-OFFSET @ + OVER TERMINAL-INSERT-OFFSET !
    PRINT-YANK-PREV
  ELSE
    DROP
  THEN ;

: HANDLE-REFRESH ( source -- ) DUP SET-NORMAL-COMMAND PRINT-REFRESH ;

: HANDLE-FORWARD ( source -- )
  DUP SET-NORMAL-COMMAND
  DUP INPUT-COUNT @ OVER TERMINAL-INSERT-OFFSET @ > IF
    DUP 1 GO-FORWARD DUP TERMINAL-INSERT-OFFSET @ 1+
    SWAP TERMINAL-INSERT-OFFSET !
  ELSE
    DROP
  THEN ;

: HANDLE-BACKWARD ( source -- )
  DUP SET-NORMAL-COMMAND
  DUP TERMINAL-INSERT-OFFSET @ 0> IF
    DUP 1 GO-BACKWARD DUP TERMINAL-INSERT-OFFSET @ 1-
    SWAP TERMINAL-INSERT-OFFSET !
  ELSE
    DROP
  THEN ;

: HANDLE-SPECIAL ( source -- continue )
  DUP READ-CHAR IF
    C@ DUP [CHAR] [ = IF
      DROP DUP READ-CHAR IF
        C@ DUP [CHAR] C = IF
          DROP HANDLE-FORWARD TRUE
        ELSE DUP [CHAR] D = IF
          DROP HANDLE-BACKWARD TRUE
        ELSE [CHAR] 3 = IF
          DUP READ-CHAR IF
            C@ [CHAR] ~ = IF
              HANDLE-DELETE-FORWARD TRUE
            ELSE
              DROP TRUE
            THEN
          ELSE
            TRUE SWAP INPUT-IS-CLOSED ! FALSE
          THEN
        ELSE
          DROP TRUE
        THEN THEN THEN
      ELSE
        DROP TRUE SWAP INPUT-IS-CLOSED ! FALSE
      THEN
    ELSE DUP [CHAR] y = IF
      DROP HANDLE-YANK-PREV TRUE
    ELSE
      HANDLE-CHAR
    THEN THEN
  ELSE
    DROP TRUE SWAP INPUT-IS-CLOSED ! FALSE
  THEN ;

: HANDLE-NORMAL-CHAR ( source c -- )
  OVER SET-NORMAL-COMMAND
  2DUP PRINT-NORMAL-CHAR OVER 1 TRY-EXPAND-TERMINAL-BUFFER
  OVER INPUT-BUFFER @ 2 PICK TERMINAL-INSERT-OFFSET @ + DUP 1+
  3 PICK INPUT-COUNT @ 4 PICK TERMINAL-INSERT-OFFSET @ - MOVE
  OVER INPUT-BUFFER @ 2 PICK TERMINAL-INSERT-OFFSET @ + C!
  DUP INPUT-COUNT @ 1+ OVER INPUT-COUNT !
  DUP TERMINAL-INSERT-OFFSET @ 1+ SWAP TERMINAL-INSERT-OFFSET ! ;

:NONAME ( source c -- continue )
  DUP NEWLINE = IF
    DROP HANDLE-NEWLINE FALSE
  ELSE DUP DELETE = IF
    DROP HANDLE-DELETE TRUE
  ELSE DUP ESCAPE = IF
    DROP HANDLE-SPECIAL
  ELSE DUP CTRL-K = IF
    DROP HANDLE-KILL TRUE
  ELSE DUP CTRL-L = IF
    DROP HANDLE-REFRESH TRUE
  ELSE DUP CTRL-W = IF
    DROP HANDLE-KILL-TO-PREV-SPACE TRUE
  ELSE DUP CTRL-Y = IF
    DROP HANDLE-YANK TRUE
  ELSE DUP $20 < IF
    DROP TRUE
  ELSE
    HANDLE-NORMAL-CHAR TRUE
  THEN THEN THEN THEN THEN THEN THEN THEN ; ' HANDLE-CHAR DEFER!

: READ-TERMINAL-CHAR ( source -- continue )
  DUP READ-CHAR IF
    C@ HANDLE-CHAR
  ELSE
    DROP TRUE SWAP INPUT-IS-CLOSED ! FALSE
  THEN ;

: REFILL-TERMINAL ( source -- flag )
  HIDE-CURSOR
  SAVE-CURSOR
  DUP QUERY-CURSOR-POSITION IF
    DUP 3 PICK TERMINAL-COLUMN !
    2 PICK TERMINAL-INIT-COLUMN !
    DUP 2 PICK TERMINAL-ROW !
    OVER TERMINAL-INIT-ROW !
  ELSE
    2DROP 0 OVER TERMINAL-COLUMN !
    0 OVER TERMINAL-INIT-COLUMN !
    0 OVER TERMINAL-ROW !
    0 OVER TERMINAL-INIT-ROW !
  THEN
  9999 9999 GO-TO-COORD
  DUP QUERY-CURSOR-POSITION IF
    1+ 2 PICK TERMINAL-WIDTH ! 1+ OVER TERMINAL-HEIGHT !
  ELSE
    2DROP 80 OVER TERMINAL-WIDTH ! 24 OVER TERMINAL-HEIGHT !
  THEN
  RESTORE-CURSOR
  SHOW-CURSOR
  0 OVER INPUT-COUNT ! 0 OVER INPUT-INDEX !
  0 OVER TERMINAL-INSERT-OFFSET !
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
  DUP SET-NORMAL-COMMAND
  SWAP OVER TERMINAL-FD !
  DEFAULT-TERMINAL-BUFFER-SIZE ALLOCATE! OVER INPUT-BUFFER !
  0 OVER INPUT-INDEX ! 0 OVER INPUT-COUNT !
  FALSE OVER INPUT-IS-CLOSED !
  ['] TERMINAL>SOURCE-CLEANUP OVER INPUT-CLEANUP !
  ['] REFILL-TERMINAL OVER INPUT-REFILL !
  0 OVER INPUT-SOURCE-ID ! 0 OVER INPUT-NEXT-INPUT ! ;

FORTH-WORDLIST SET-CURRENT

: ACCEPT ( c-addr u1 -- u2 )
  IO-STDIN TERMINAL>SOURCE DUP REFILL-TERMINAL DROP
  DUP INPUT-COUNT @ ROT MIN OVER INPUT-BUFFER @ ( a s u2 b )
  OVER 0> IF
    2DUP + 1- C@ NEWLINE = IF
      SWAP 1- SWAP
    THEN
  THEN
  3 ROLL 2 PICK MOVE SWAP DUP INPUT-CLEANUP @ EXECUTE ;

:NONAME
  IO-STDIN TERMINAL>SOURCE THIS-TASK CLEANUP-INPUT THIS-TASK >CONSOLE-INPUT
  FORTH-WORDLIST DUP 1 SET-ORDER SET-CURRENT ABORT ; EXECUTE