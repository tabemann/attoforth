FORTH-WORDLIST TASK-WORDLIST 2 SET-ORDER

REQUIRE src/forth/bchan.fs

20 NEW-BCHAN CONSTANT MY-BCHAN

VARIABLE TERMINATE FALSE TERMINATE !

: SENDER
  0 BEGIN
    TERMINATE @ 0=
  WHILE
    DUP MY-BCHAN SEND-BCHAN 1+ ." ."
  REPEAT ;

: RECEIVER
  BEGIN
    TERMINATE @ 0=
  WHILE
    MY-BCHAN RECV-BCHAN 16 BASE.
  REPEAT ;

0 ' RECEIVER SPAWN-SIMPLE-FREE-DATA-ON-EXIT DROP
0 ' SENDER SPAWN-SIMPLE-FREE-DATA-ON-EXIT DROP

: DO-TERMINATE KEY DROP TRUE TERMINATE ! ;

DO-TERMINATE
