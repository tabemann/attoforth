# Lambdas

These are made available by executing:

    REQUIRE src/forth/lambda.fs

The following words are in the `LAMBDA-WORDLIST` wordlist.

`[:` ( -- xt ) ( in lambda: -- )

Begin a closureless lambda, returning an execution token for the lambda within the word where the lambda is nested. Lambdas created with this word are not allocated and thus do not need to be freed.

`;]` ( -- ) ( in lambda: -- )

End a closureless lambda.

`<:` ( x\*count count -- xt ) ( in lambda: -- x\*count )

Begin a lambda with a closure; take a given number of cells off of the data stack, and put them on the data stack when the lambda is executed, and return an execution token for the lambda in the word the lambda is nested within. Note that the lambda needs to be freed with `FREE-LAMBDA` or else memory may be leaked.

`;>` ( -- ) ( in lambda: -- )

End a lambda with a closure.

`F<:` ( x\*count1 count1 count2 -- xt ) ( F: r\*count2 -- ) ( in lambda: -- x\*count1 ) ( in lambda: F: -- r\*count2 )

Begin a lambda with a closure that encloses values from both the data and float stacks; take a given number of cells off of the data stack and a given number of floats off of the float stack, and put them on the data and float stacks when the lambda is executed, and return an execution token for the lambda in the word the lambda is nested within. Note that the lambda needs to be freed with `FREE-LAMBDA` or else memory may be leaked.

`FREE-LAMBDA` ( xt -- )

Free a lambda created with `<:` or `F<:`.
