# Escaped Strings

These are made available by executing:

    REQUIRE src/forth/escape.fs

The following words are in the `FORTH-WORDLIST` wordlist.

These provide escaped strings, which are similar to strings in C. After the initial whitespace which is skipped over after the introducing word, the continue until an unescaped `"`, or in the case of `.\(`, `)` character is reached. The allowed escape sequences are:

* `\a`, i.e. BEL or alert, ASCII 7
* `\b`, i.e. BS or backspace, ASCII 8
* `\e`, i.e. ESC or escape, ASCII 27
* `\f`, i.e. FF or form feed, ASCII 12
* `\l`, i.e. LF or line feed, ASCII 10
* `\m`, i.e. CR/LF pair, ASCII 13, 10
* `\n`, i.e. newline (in this implementation LF), ASCII 10
* `\q`, i.e. double-quote, ASCII 34
* `\r`, i.e. CR or carriage return, ASCII 13
* `\t`, i.e. HT or horizontal tab, ASCII 9
* `\v`, i.e. VT or vertical tab, ASCII 11
* `\z`, i.e. NUL or null, ASCII 0
* `\"`, i.e. double-quote, ASCII 34
* `\)`, i.e. close-paren, ASCII 41
* `\\`, i.e. backslash, ASCII 92
* `\x<hexdigit><hexdigit>`, i.e. the result of parsing two hexadecimal digits

`S\"` ( compile-time: "ccc\<quote>" -- ) ( runtime: -- c-addr u )

This starts an embedded escaped string within a compiled word.

`C\"` ( compile-time: "ccc\<quote>" -- ) ( runtime: -- c-addr )

This starts an embedded escaped counted string within a compiled word.

`.\"` ( compile-time: "ccc\<quote>" -- ) ( runtime: -- )

This starts a printed escaped string within a compiled word.

`.\(` ( immediate: "ccc\<close-paren>" -- )

This starts an immediate printed escaped string.