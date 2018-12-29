# Anonymous Function-Based Control Structures

These are made available by executing:

    REQUIRE control.fs

Loading this source file automatically loads `lambda.fs`.

The following words are in the `LAMBDA-WORDLIST` wordlist.

Note that all the following examples are executed preceded by:

    REQUIRE control.fs  ok
    FORTH-WORDLIST LAMBDA-WORDLIST 2 SET-ORDER  ok

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

`ITER` ( a-addr count xt -- )

Apply *xt* to the value of each cell of the *count* cells starting from *a-addr* and except no return values.

    : WRITE-ARRAY 0 [: CELLS OVER + ROT SWAP ! ;] COUNT-LOOP DROP ;  ok
    10 CELLS BUFFER: TEST-BUFFER  ok
    : ITER-TEST
      0 1 2 3 4 5 6 7 8 9 TEST-BUFFER 10 WRITE-ARRAY
      TEST-BUFFER 10 ['] . ITER ;  ok
    ITER-TEST 9 8 7 6 5 4 3 2 1 0  ok

`ITERI` ( a-addr count xt -- )

Apply *xt* to the cell index starting from *a-addr* followed by the value of each cell of the *count* cells starting from *a-addr* and except no return values.

    : WRITE-ARRAY 0 [: CELLS OVER + ROT SWAP ! ;] COUNT-LOOP DROP ;  ok
    10 CELLS BUFFER: TEST-BUFFER  ok
    : ITERI-TEST
      0 2 4 6 8 10 12 14 16 18 TEST-BUFFER 10 WRITE-ARRAY
      TEST-BUFFER 10 [: . . ;] ITERI ;  ok
    ITERI-TEST 18 0 16 1 14 2 12 3 10 4 8 5 6 6 4 7 2 8 0 9  ok

`MAP` ( a-addr1 count1 a-addr2 xt -- a-addr2 count2 )

Apply *xt* to the value of each cell of the *count1* cells starting from *a-addr1* and write the results in order into an equal number of cells starting from *a-addr2* and return *a-addr2* followed by this count. Note that the two buffers may overlap if *a-addr2* is an equal or lower address relative to *a-addr1*.

    : PRINT-ARRAY 0 [: CELLS OVER + @ . ;] COUNT-LOOP DROP ;  ok
    : WRITE-ARRAY 0 [: CELLS OVER + ROT SWAP ! ;] COUNT-LOOP DROP ;  ok
    10 CELLS BUFFER: TEST-BUFFER  ok
    : MAP-TEST
      0 1 2 3 4 5 6 7 8 9 TEST-BUFFER 10 WRITE-ARRAY
      TEST-BUFFER 10 TEST-BUFFER [: 2 * ;] MAP PRINT-ARRAY ;  ok
    MAP-TEST 18 16 14 12 10 8 6 4 2 0  ok

`FILTER` ( a-addr1 count1 a-addr2 xt - a-addr2 count2 )

Apply *xt* to the value of each cell of the *count1* cells starting from *a-addr1* and, if the value returned is non-zero, and write the value in order into a number of cells less than or equal to *count1* starting from *a-addr2* and return *a-addr2* followed by the total number of cells written. Note that the two buffers may overlap if *a-addr2* is an equal or lower address relative to *a-addr1*.

    : PRINT-ARRAY 0 [: CELLS OVER + @ . ;] COUNT-LOOP DROP ;  ok
    : WRITE-ARRAY 0 [: CELLS OVER + ROT SWAP ! ;] COUNT-LOOP DROP ;  ok
    10 CELLS BUFFER: TEST-BUFFER  ok
    : FILTER-TEST
      0 1 2 3 4 5 6 7 8 9 TEST-BUFFER 10 WRITE-ARRAY
      TEST-BUFFER 10 TEST-BUFFER [: 1 AND 0= ;] FILTER PRINT-ARRAY ;  ok
    FILTER-TEST 8 6 4 2 0  ok

`FILTER-MAP` ( a-addr1 count1 addr-2 xt-filter xt-map a-addr2 count2 )

Apply *xt-filter* to the value of each cell of the *count1* cells starting from *a-addr1* and, if the value returned is non-zero, and write the value with *xt-map* applied to it in order into a number of cells less than or equal to *count1* starting from *a-addr2* and return *a-addr2* followed by the total number of cells written. Note that the two buffers may overlap if *a-addr2* is an equal or lower address relative to *a-addr1*.

    : PRINT-ARRAY 0 [: CELLS OVER + @ . ;] COUNT-LOOP DROP ;  ok
    : WRITE-ARRAY 0 [: CELLS OVER + ROT SWAP ! ;] COUNT-LOOP DROP ;  ok
    10 CELLS BUFFER: TEST-BUFFER  ok
    : FILTER-MAP-TEST
      0 1 2 3 4 5 6 7 8 9 TEST-BUFFER 10 WRITE-ARRAY
      TEST-BUFFER 10 TEST-BUFFER [: 1 AND 0= ;] [: 2 * ;] FILTER-MAP PRINT-ARRAY ;  ok
    FILTER-MAP-TEST 16 12 8 4 0  ok
