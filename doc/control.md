# Anonymous Function-Based Control Structures

These are made available by executing:

    REQUIRE src/forth/control.fs

Loading this source file automatically loads `src/forth/lambda.fs`.

The following words are in the `LAMBDA-WORDLIST` wordlist.

`OPTION` ( ... flag true-xt -- ... )

Execute *true-xt* if *flag* is non-zero.

`CHOOSE` ( ... flag true-xt false-xt -- ... )

Execute *true-xt* if *flag* is non-zero, else execute *false-xt*.

`LOOP-UNTIL` ( ... xt -- ... )

Repeatedly execute *xt* until the value popped from the top of the data stack after it executes is non-zero.

`WHILE-LOOP` ( ... while-xt body-xt -- ... )

Repeatedly execute *while-xt* followed by *body-xt* until the value popped from the top of the data stack after the execution of *while-xt* is zero, after which *body-xt* is not executed.

`COUNT-LOOP` ( ... limit init xt -- ... )

For values starting from *init* and ending at *limit* minus one, if *init* does not equal *limit*, place the current value on the top of the data stack and execute *xt*..

`COUNT+LOOP` ( ... limit init xt -- ... )

For values starting with *init* and continuing until the transition between the current value and the next value breaks the boundary between *limit* minus one and *limit*, if *init* does not equal *limit*, place the current value on the top of the data stack and execute *xt*, afterwards taking a value off the top of the stack and adding it to the current value.
