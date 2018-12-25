# Command-line Arguments and Environment Variables

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

`GETENV` ( c-addr1 bytes1 -- c-addr2 bytes2 found )

Read the environment variable specified by *c-addr1 bytes1*, and return its value as *c-addr2 bytes2* followed by `TRUE` if it exists; otherwise return two zeros followed by `FALSE`.

`SETENV` ( c-addr1 bytes1 c-addr2 bytes2 overwrite -- error )

Set the environment variable specified by *c-addr2 bytes2* to the value specified by *c-addr1 bytes1* if it is not set or if *overwrite* is non-zero and return `0`, unless the environment variable name is empty or contains `=`, where then a non-zero error code is returned, or if the environment variable is already set and *overwrite* is zero, where then `0` is returned but the environment variable is not set.

`UNSETENV` ( c-addr bytes -- error )

Unset the environment variable specified by *c-addr bytes* and return `0`, unless the environment variable name is empty or contains `=`, where then a non-zero error code is returned.
