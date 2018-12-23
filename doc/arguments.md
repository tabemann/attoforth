# Command-line Arguments

These are available built into attoforth. The following words are in the `FORTH-WORDLIST` wordlist.

`ARGC` ( -- count )

Get the number of arguments.

`ARG` ( index -- c-addr bytes )

Get argument *index*.

`CONSUME-ARGS` ( count -- )

Remove `count` arguments from the start of the arguments made available by `ARG` and `ARGC` within the current thread. Other threads are not affected.

`BASE-ARGC` ( -- count )

Get the full number of arguments, including the attoforth process name and any source file specified to be executed by attoforth, and ignore arguments' being removed by `CONSUME-ARGS`.

`BASE-ARG` ( index -- c-addr u )

Get argument *index* out of the full array of arguments, including the attoforth process name and any source file specified to be executed by attoforth, and ignore arguments' being removed by `CONSUME-ARGS`.
