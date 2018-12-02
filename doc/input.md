# Input

This is available built into attoforth. The following words are in the `IO-WORDLIST` wordlist.

`>INPUT` ( input task -- )

Set the input structure at the top of the input stack for a given task.

`INPUT>` ( task -- input )

Get the input structure at the top of the input statck for a given task.

`INPUT-SIZE` ( -- bytes )

Get the size in bytes of an input structure.

`INPUT-NEXT-INPUT` ( input -- addr )

Get the address of the next-input field of an input structure. The content of this field either points to the next input structure in the input stack for a given task or is 0 if an input structure is at the bottom of said input stack.

`INPUT-BUFFER` ( input -- addr )

Get the address of the buffer field of an input structure. The content of this field points to a buffer containing input data, which would be returned by `SOURCE` were this input structure at the top of the input stack for the current task.

`INPUT-COUNT` ( input -- addr )

Get the address of the count field of an input structure. The content of this field is the number of bytes of input data currently stored in the buffer for that input structure, which would be returned by `SOURCE` were this input structure at the top of the input stack for the current task.

`INPUT-INDEX` ( input -- addr )

Get the address of the index field of an input structure. The content of this field is the current index into the input data currently stored in the buffer for that input structure, and must be between 0 and the content of the field pointed to by count field of the input structure minus 1. When this input structure is at the top of the input stack for the current task, the address returned by this word is the same as the address returned by `>IN`.

`INPUT-IS-CLOSED` ( input -- addr )

Get the address of the is-closed field of an input structure. The content of this field is a boolean value that, if `TRUE`, indicates that `REFILL`, if this input structure is at the top of the input stack for the current task, will fail next time it called (even though its being `FALSE` does not indicate that `REFILL` will succeed next time it is called).

`INPUT-CLEANUP` ( input -- addr )

Get the address of the cleanup field of an input structure. The content of this field is the execution token of a word with the signature ( input -- ). This word frees the input structure and any other associated allocated memory such as the buffer when popping the input structure from the input stack of a task. If the execution token is 0, then no cleanup takes place.

`INPUT-REFILL` ( input -- addr )

Get the address of the refill field of an input structure. The content of this field is the execution token of a word with the signature ( input -- success ). This word refills the buffer associated with the input structure, sets the count field to the number of bytes the buffer is refilled with, and sets the index field to zero. If the execution token is 0, then refilling will automatically fail (e.g. for an input structure that corresponds to a fixed string in memory).

`INPUT-SOURCE-ID` ( input -- addr )

Get the address of the source-id field of an input structure. The content of this field is the value returned by `SOURCE-ID` when this input structure is at the top of the input stack for the current task.

`INPUT-ARG` ( input - addr )

Get the address of the arg field of an input structure. The content of this field is up to the user, and is often used to hold the address of a data structure associated with the input structure.