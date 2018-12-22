# Anonymous Function-Based Control Structures

These are made available by executing:

    REQUIRE src/forth/control.fs

Loading this source file automatically loads `src/forth/lambda.fs`.

The following words are in the `LAMBDA-WORDLIST` wordlist.

Note that all the following examples are executed preceded by:

    REQUIRE src/forth/control.fs
    FORTH-WORDLIST LAMBDA-WORDLIST 2 SET-ORDER

`OPTION` ( ... flag true-xt -- ... )

Execute *true-xt* if *flag* is non-zero.

An example of this in action:

    : FOOBAR [: ." foobar" ;] OPTION ;  ok
    FALSE FOOBAR  ok
    TRUE FOOBAR foobar ok

`CHOOSE` ( ... flag true-xt false-xt -- ... )

Execute *true-xt* if *flag* is non-zero, else execute *false-xt*.

An example of this in action:

    : FOOBAR [: ." foo" ;] [: ." bar" ;] CHOOSE ;  ok
    FALSE FOOBAR bar ok
    TRUE FOOBAR foo ok

`LOOP-UNTIL` ( ... xt -- ... )

Repeatedly execute *xt* until the value popped from the top of the data stack after it executes is non-zero.

An example of this in action:

    : FOOBAR 0 [: DUP . 1+ DUP 10 = ;] LOOP-UNTIL DROP ;  ok
    FOOBAR 0 1 2 3 4 5 6 7 8 9  ok

`WHILE-LOOP` ( ... while-xt body-xt -- ... )

Repeatedly execute *while-xt* followed by *body-xt* until the value popped from the top of the data stack after the execution of *while-xt* is zero, after which *body-xt* is not executed.

An example of this in action:

    : FOOBAR 0 [: DUP 10 < ;] [: DUP . 1+ ;] WHILE-LOOP DROP ;  ok
    FOOBAR 0 1 2 3 4 5 6 7 8 9  ok

`COUNT-LOOP` ( ... limit init xt -- ... )

For values starting from *init* and ending at *limit* minus one, if *init* does not equal *limit*, place the current value on the top of the data stack and execute *xt*.

An example of this in action:

    : FOOBAR 10 0 ['] . COUNT-LOOP ;  ok
    FOOBAR 0 1 2 3 4 5 6 7 8 9  ok

`COUNT+LOOP` ( ... limit init xt -- ... )

For values starting with *init* and continuing until the transition between the current value and the next value breaks the boundary between *limit* minus one and *limit*, if *init* does not equal *limit*, place the current value on the top of the data stack and execute *xt*, afterwards taking a value off the top of the stack and adding it to the current value.

An example of this in action:

    : FOOBAR -9 0 [: . -1 ;] COUNT+LOOP ;  ok
    FOOBAR 0 -1 -2 -3 -4 -5 -6 -7 -8 -9  ok
