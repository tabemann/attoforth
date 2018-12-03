FORTH-WORDLIST TASK-WORDLIST 2 SET-ORDER

REQUIRE src/forth/cond.fs

NEW-LOCK CONSTANT MY-LOCK
NEW-LOCK CONSTANT MY-COND-LOCK
MY-COND-LOCK NEW-COND CONSTANT MY-COND
VARIABLE ALIVE-COUNT 3 ALIVE-COUNT !

: ATEXIT-MESSAGE ." *** " ;

: DISPLAY-REPEATED"
  SKIP-WHITESPACE SOURCE DROP >IN @ + [CHAR] " COUNT-CHARS-UNTIL
  DUP ALLOCATE! ROT OVER 3 PICK MOVE SWAP CREATE 2,
  DOES> ['] ATEXIT-MESSAGE THIS-TASK ATEXIT 2@ 3 0 ?DO
    MY-COND SIGNAL-COND MY-LOCK LOCK 2DUP TYPE 1 0 SLEEP MY-LOCK UNLOCK 
  LOOP
  -1 ALIVE-COUNT +!
  ALIVE-COUNT @ 0= IF MY-COND BROADCAST-COND THEN
  ." exiting <" 2DUP TYPE ALIVE-COUNT @ (.) ." > " ;

: DISPLAY-COND"
  SKIP-WHITESPACE SOURCE DROP >IN @ + [CHAR] " COUNT-CHARS-UNTIL
  DUP ALLOCATE! ROT OVER 3 PICK MOVE SWAP CREATE 2,
  DOES> 2@ BEGIN ALIVE-COUNT @ 0> WHILE
     2DUP TYPE MY-COND-LOCK LOCK MY-COND WAIT-COND MY-COND-LOCK UNLOCK
  REPEAT
  ." exiting (" 2DUP TYPE ." ) " ;

DISPLAY-REPEATED" foo " FOO
DISPLAY-REPEATED" bar " BAR
DISPLAY-REPEATED" baz " BAZ

DISPLAY-COND" A " A
DISPLAY-COND" B " B
DISPLAY-COND" C " C

0 ' A SPAWN-SIMPLE-FREE-DATA-ON-EXIT DROP
0 ' B SPAWN-SIMPLE-FREE-DATA-ON-EXIT DROP
0 ' C SPAWN-SIMPLE-FREE-DATA-ON-EXIT DROP

0 ' FOO SPAWN-SIMPLE-FREE-DATA-ON-EXIT DROP
0 ' BAR SPAWN-SIMPLE-FREE-DATA-ON-EXIT DROP
0 ' BAZ SPAWN-SIMPLE-FREE-DATA-ON-EXIT DROP
