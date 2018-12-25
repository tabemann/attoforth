# Memory Access

These are available built into attoforth. The following words are in the `FORTH-WORDLIST` wordlist.

`@` ( addr -- x )

Fetch a cell at a given address.

`!` ( x addr -- )

Store a cell at a given address.

`,` ( x -- )

Allot space for a cell in the current thread's data space and store a cell in it.

`ALIGN` ( -- )

Cell-align the current thread's data space pointer.

`ALIGNED` ( addr -- addr )

Cell-align a pointer.

`CELL-SIZE` ( -- bytes )

Get the size of a cell in bytes.

`CELL+` ( addr -- addr )

Advance a pointer by the size of a cell.

`CELLS` ( count -- bytes )

Get the size of a given number of cells in bytes.

`W@` ( addr -- x )

Fetch a 32-bit value at a given address.

`W!` ( x addr -- )

Store a 32-bit value at a given address.

`W,` ( x -- )

Allot space for a 32-bit value in the current thread's data space and store a 32-bit value in it.

`WALIGN` ( -- )

32-bit-align the current thread's data space pointer.

`WALIGNED` ( addr -- addr )

32-bit-align a pointer.

`WORD-SIZE` ( -- bytes )

Get the size of a 32-bit value in bytes.

`WORD+` ( addr -- addr )

Advance a pointer by the size of a 32-bit value.

`WORDS` ( count -- bytes )

Get the size of a given number of 32-bit values in bytes.

`H@` ( addr -- x )

Fetch a 16-bit value at a given address.

`H!` ( x addr -- )

Store a 16-bit value at a given address.

`H,` ( x -- )

Allot space for a 16-bit value in the current thread's data space and store a 16-bit value in it.

`HALIGN` ( -- )

16-bit-align the current thread's data space pointer.

`HALIGNED` ( addr -- addr )

16-bit-align a pointer.

`HALFWORD-SIZE` ( -- bytes )

Get the size of a 16-bit value in bytes.

`HALFWORD+` ( addr -- addr )

Advance a pointer by the size of a 16-bit value.

`HALFWORDS` ( count -- bytes )

Get the size of a given number of 16-bit values in bytes.

`C@` ( addr -- c )

Fetch a byte at a given address.

`C!` ( c addr -- )

Store a byte at a given address.

`C,` ( c -- )

Allot space for a byte in the current thread's data space and store a byte in it.

`CHAR+` ( addr -- addr )

Advance a pointer by one character (one byte).

`CHARS` ( count -- bytes )

Get the size of a given number of characters (bytes) in bytes. In attoforth this is a no-op.

`F@` ( addr -- ) ( F: -- r )

Fetch a native floating point value at a given address.

`F!` ( addr -- ) ( F: r -- )

Store a native floating point value at a given address.

`F,` ( -- ) ( F: r -- )

Allot space for a native floating point value in the current thread's data space and store a native floating point value in it.

`FALIGN` ( -- )

Native floating point-align the current thread's data space pointer.

`FALIGNED` ( addr -- addr )

Native floating point-align a pointer.

`FLOAT-SIZE` ( -- bytes )

Get the size of a native floating point value in bytes.

`FLOAT+` ( addr -- addr )

Advance a pointer by the size of a native floating point value.

`FLOATS` ( count -- bytes )

Get the size of a given number of native floating point values in bytes.

`SF@` ( addr -- ) ( F: -- r )

Fetch a single-precision floating point value at a given address.

`SF!` ( addr -- ) ( F: r -- )

Store a single-precision floating point value at a given address.

`SF,` ( -- ) ( F: r -- )

Allot space for a single-precision floating point value in the current thread's data space and store a single-precision floating point value in it.

`SFALIGN` ( -- )

Single-Precision floating point-align the current thread's data space pointer.

`SFALIGNED` ( addr -- addr )

Single-Precision floating point-align a pointer.

`SFLOAT-SIZE` ( -- bytes )

Get the size of a single-precision floating point value in bytes.

`SFLOAT+` ( addr -- addr )

Advance a pointer by the size of a single-precision floating point value.

`SFLOATS` ( count -- bytes )

Get the size of a given number of single-precision floating point values in bytes.

`DF@` ( addr -- ) ( F: -- r )

Fetch a double-precision floating point value at a given address.

`DF!` ( addr -- ) ( F: r -- )

Store a double-precision floating point value at a given address.

`DF,` ( -- ) ( F: r -- )

Allot space for a double-precision floating point value in the current thread's data space and store a double-precision floating point value in it.

`DFALIGN` ( -- )

Double-Precision floating point-align the current thread's data space pointer.

`DFALIGNED` ( addr -- addr )

Double-Precision floating point-align a pointer.

`DFLOAT-SIZE` ( -- bytes )

Get the size of a double-precision floating point value in bytes.

`DFLOAT+` ( addr -- addr )

Advance a pointer by the size of a double-precision floating point value.

`DFLOATS` ( count -- bytes )

Get the size of a given number of double-precision floating point values in bytes.

`2@` ( addr -- x2 x1 )

Fetch values in two successive cells; the value of the first cell will be fetched onto the top of the stack and the value of the second cell will be fetched into the next value on the stack.

`2!` ( x2 x1 addr -- )

Store two values in two successive cells; the top value on the stack will take up the first cell and the next value on the stack will take up the second cell.

`2,` ( x2 x1 -- )

Allot space for two cells in the current thread's data space and store two values into it; the top value on the stack will take up the first cell and the next value on the stack will take up the second cell.

`F2@` ( addr -- ) ( F: -- r2 r1 )

Fetch values in two successive native floating point-sized locations; the value of the first native floating point-sized location will be fetched onto the top of the floating point stack and the value of the second native floating point-sized location will be fetched into the next value on the floating point stack.

`F2!` ( addr -- ) ( F: r2 r1 -- )

Store two values in two successive native floating point-sized locations; the top value on the floating point stack will take up the first native floating point-sized location and the next value on the floating point stack will take up the second native floating point-sized location.

`F2,` ( F: r2 r1 -- )

Allot space for two native floating point-sized locations in the current thread's data space and store two values into it; the top value on the floating point stack will take up the first native floating point-sized location and the next value on the floating point stack will take up the second native floating point-sized location.

`ALLOT` ( bytes -- )

Advance the current task's data space pointer by *bytes* bytes. Note that negative values are permitted and have the expected result.

`ALLOCATE` ( bytes -- addr wor )

Allocate a block of memory and return an address on the heap followed by zero, unless allocation fails where then null followed by a non-zero value are returned.

`FREE` ( addr -- wor )

Free a heap-allocated block of memory. Returns zero (a non-zero value would indicate that it failed, but it does not fail except by exiting attoforth abnormally in reality).

`ALLOCATE!` ( bytes -- addr )

Allocate a block of memory and return an address on the heap, unless allocation fails where then an abort takes place.

`FREE!` ( addr -- )

Free a heap-allocated block of memory. It would abort if free could fail, but in the current implementation that is not possible except through exiting attoforth abnormally.
