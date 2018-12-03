# Output Implementation

This is available built into attoforth. The following words are in the `IO-WORDLIST` wordlist.

`>OUTPUT` ( output task -- )

Set the output structure at the top of the output stack for a given task.

`OUTPUT>` ( task -- output )

Get the output structure at the top of the output stack for a given task.

`>ERROR` ( output task -- )

Set the output structure at the top of the error stack for a given task.

`ERROR>` ( task -- output )

Get the output structure at the top of the error stack for a given task.

`>CONSOLE-OUTPUT` ( output task -- )

Set the output structure at the bottom of the output stack for a given task.

`CONSOLE-OUTPUT>` ( task -- output )

Get the output structure at the bottom of the output stack for a given task.

`>CONSOLE-ERROR` ( output task -- )

Set the output structure at the bottom of the error stack for a given task.

`CONSOLE-ERROR>` ( task -- output )

Get the output structure at the bottom of the error stack for a given task.

`OUTPUT-SIZE` ( -- bytes )

Get the size in bytes of an output structure.

`OUTPUT-NEXT-OUTPUT` ( output -- addr )

Get the address of the next-output field of an output structure. The content of this field either points to the next output structure in the output or error stack for a given task or is 0 if an output structure is at the bottom of said output or error stack.

`OUTPUT-WRITE` ( output -- addr )

Get the address of the write field of an output structure. The content of this field is the execution token of a word with the signature ( c-addr bytes output -- ). This word prints the specified bytes of text to either output or error, and the output structure should be at the top of the proper output or error stack of the current task. If the execution token is 0, then no printing takes place.

`OUTPUT-CLEANUP` ( output -- addr )

Get the address of the cleanup field of an output structure. The content of this field is the execution token of a word with the signature ( output -- ). This word frees the output structure and any other associated allocated memory such as the buffer when popping the output structure from the output or error stack of a task. If the execution token is 0, then no cleanup takes place.

`OUTPUT-ARG` ( output - addr )

Get the address of the arg field of an output structure. The content of this field is up to the user, and is often used to hold the address of a data structure associated with the output structure.