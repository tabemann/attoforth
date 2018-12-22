# Tasks

These are available built into attoforth. The following words are in the `TASK-WORDLIST` wordlist.

`THIS-TASK` ( -- task )

Get the current task.

`SPAWN` ( -- task )

Create a new, stopped task. No initial word is set for this task, but standard output, standard error, and at exit handling are set up. On task exit the user needs to execute `DIE` to execute at exit handlers, including one to clean up standard output and standard error, clean up after at exit handling, and cleanly kill the task.

`SPAWN-NO-IO` ( -- task )

Create a new, stopped task. No initial word is set for this task, and standard output and standard error are not set up, but at exit handling is set up. On task exit the user needs to execute `DIE` to execute at exit handlers, clean up after at exit handling, and cleanly kill the task.

`SPAWN-NO-DATA` ( -- task )

Create a new, stopped task *without* a data space, or configured standard output or standard error, but with configured exit handling. No initial word is set for this task. Note that the user needs to execute `DIE` to execute at exit handlers, clean up after at exit handling, and cleanly kill the task.

`SPAWN-SIMPLE` ( x\*count count xt -- task )

Create a new task, with configured standard output, standard error, and at exit handling, which will execute at exit handlers and clean up after at exit handling, clean up IO, cleanly kill itself upon exit. The count is the number of arguments on the data stack to push onto the initial data stack of the executed word upon startup.

`SPAWN-SIMPLE-FREE-DATA-ON-EXIT` ( x\*count count xt -- task )

Create a new task, with configured standard output, standard error, and at exit handling, which will execute at exit handlers and clean up after at exit handling, clean up IO, cleanly kill itself upon exit. The count is the number of arguments on the data stack to push onto the initial data stack of the executed word upon startup. This differs from `SPAWN-SIMPLE` in that the data space it allocates will be freed upon termination, to prevent memory leakage. Note however this means that no words may be created within this task, and no memory allocated in this task's data space may be referenced after termination.

`SPAWN-SIMPLE-NO-IO` ( x\*count count xt -- task )

Create a new task, *without* configured standard output and standard error, but with configured at exit handling, which will execute at exit handlers and clean up after at exit handling, and cleanly kill itself upon exit. The count is the number of arguments on the data stack to push onto the initial data stack of the executed word upon startup.

`SPAWN-SIMPLE-NO-IO-FREE-DATA-ON-EXIT` ( x\*count count xt -- task )

Create a new task, *without* configured standard output and standard error, but with configured at exit handling, which will execute at exit handlers and clean up after at exit handling, and cleanly kill itself upon exit. The count is the number of arguments on the data stack to push onto the initial data stack of the executed word upon startup. This differs from `SPAWN-SIMPLE-NO-IO` in that the data space it allocates will be freed upon termination, to prevent memory leakage. Note however this means that no words may be created within this task, and no memory allocated in this task's data space may be referenced after termination.

`SPAWN-SIMPLE-NO-DATA` ( x\*count count xt -- task )

Create a new task, *without* an allocated data space or configured standard output or standard error, but with configured at exit handling, which will execute at exit handlers and cleanly kill itself upon exit. The count is the number of arguments on the data stack to push onto the initial data stack of the executed word upon startup. Note that it is imperative that no words that reference the data space, including words such as `EMIT`, are executed, because no data space exists for tasks created with this word.

`>INIT-WORD` ( xt task -- )

Set an initial word for a new, stopped task.

`>DATA` ( x task -- )

Push a value onto the initial data stack of a new, stopped task.

`>FLOAT` ( task -- ) ( F: r -- )

Push a value onto the initial float stack of a new, stopped task.

`>RETURN` ( x task -- )

Push a value onto the initial return stack of a new, stopped task.

`START` ( task -- )

Initially start a new, stopped task.

`YIELD` ( -- )

Yield control of the processor for the current task. Note that extra cycles assigned to the current task are discarded.

`DIE` ( -- )

Cleanly terminate the current task, after executing at exit handlers.

`WAIT` ( task -- )

Put a task into a waiting state. Note that unused cycles, up to a limit, are saved so they can be assigned to the task upon being woken up.

`WAKE` ( task -- )

Wake up a task from a waiting state. Note that cycles that were unused when the task was put into a waiting state, up to a limit, are given back to the woken up task.

`KILL` ( task -- )

Kill a task. Note that any further use of the word's task address are undefined, as killing a task frees it to be deallocated. Do not execute this word on a task when it is waiting on a lock, condition variable, or channel.

`TERMINATE` ( task -- )

Activate a termination handler for a task, or kill the task if no termination handler is set. Note however that most tasks by default have a termination handler that calls `DIE`, so as to enable executing exit handlers and then killing themselves cleanly.

`SLEEP` ( secs nsecs -- )

Put the current task to sleep for a given number of seconds and nanoseconds.

`TASK-LOCAL` ( addr bytes -- offset )

Allocate a given number of bytes of task-local space with the given address containing the beginning of a buffer containing the initial state of this task-local space in each task, and return the offset of the task-local space.

`T@` ( offset task -- x )

Fetch a cell for the given task at a given offset in its task-local space.

`T!` ( x offset task -- )

Store a cell for the given task at a given offset in its task-local space.

`WT@` ( offset task -- x )

Fetch a 32-bit value for the given task at a given offset in its task-local space.

`WT!` ( x offset task -- )

Store a 32-bit value for the given task at a given offset in its task-local space.

`HT@` ( offset task -- x )

Fetch a 16-bit value for the given task at a given offset in its task-local space.

`HT!` ( x offset task -- )

Store a 16-bit value for the given task at a given offset in its task-local space.

`CT@` ( offset task -- c )

Fetch a byte for the given task at a given offset in its task-local space.

`CT!` ( c offset task -- )

Store a byte for the given task at a given offset in its task-local space.

`FT@` ( offset task -- ) ( F: -- r )

Fetch a native floating point value for the given task at a given offset in its task-local space.

`FT!` ( offset task -- ) ( F: r -- )

Store a native floating point value for the given task at a given offset in its task-local space.

`SFT@` ( offset task -- ) ( F: -- r )

Fetch a single-precision floating point value for the given task at a given offset in its task-local space.

`SFT!` ( offset task -- ) ( F: r -- )

Store a single-precision floating point value for the given task at a given offset in its task-local space.

`DFT@` ( offset task -- ) ( F: -- r )

Fetch a double-precision floating point value for the given task at a given offset in its task-local space.

`DFT!` ( offset task -- ) ( F: r -- )

Store a double-precision floating point value for the given task at a given offset in its task-local space.

`TT@` ( offset -- x )

Fetch a cell for the current task at a given offset in its task-local space.

`TT!` ( x offset -- )

Store a cell for the current task at a given offset in its task-local space.

`TWT@` ( offset -- x )

Fetch a 32-bit value for the current task at a given offset in its task-local space.

`TWT!` ( x offset -- )

Store a 32-bit value for the current task at a given offset in its task-local space.

`THT@` ( offset -- x )

Fetch a 16-bit value for the current task at a given offset in its task-local space.

`THT!` ( x offset -- )

Store a 16-bit value for the current task at a given offset in its task-local space.

`TCT@` ( offset -- c )

Fetch a byte for the current task at a given offset in its task-local space.

`TCT!` ( c offset -- )

Store a byte for the current task at a given offset in its task-local space.

`TFT@` ( offset -- ) ( F: -- r )

Fetch a native floating point value for the current task at a given offset in its task-local space.

`TFT!` ( offset -- ) ( F: r -- )

Store a native floating point value for the current task at a given offset in its task-local space.

`TSFT@` ( offset -- ) ( F: -- r )

Fetch a single-precision floating point value for the current task at a given offset in its task-local space.

`TSFT!` ( offset -- ) ( F: r -- )

Store a single-precision floating point value for the current task at a given offset in its task-local space.

`TDFT@` ( offset -- ) ( F: -- r )

Fetch a double-precision floating point value for the current task at a given offset in its task-local space.

`TDFT!` ( offset -- ) ( F: r -- )

Store a double-precision floating point value for the current task at a given offset in its task-local space.

`ATEXIT` ( xt task -- )

Register an execution token as an at exit handler for a given task. Note that at exit handlers are executed in the opposite order of that in which they are registered when `EXECUTE-ATEXIT` is executed (which is done automatically for any tasks spawned with the `SPAWN-SIMPLE-*` words or when `DIE` is called).

`EXECUTE-ATEXIT` ( -- )

Execute and remove all the at exit handlers for the current task, in the order opposite to that in which they were registered. Calling this manually is only necessary for tasks not spawned with the `SPAWN-SIMPLE-*` words when the task is not terminated with `DIE`.

`DESTROY-ATEXIT` ( -- )

Destroy the allocated data for at exit handling for the current task. If `ATEXIT` is called for the current task after this has been called, and before `INIT-ATEXIT` is called again, attoforth will crash. Calling this manually is only necessary for tasks not spawned with the `SPAWN-SIMPLE-*` words when the task is not terminated with `DIE`.

`>TERMINATE` ( xt task -- )

Set a termination handler for a task.

`TERMINATE>` ( task -- xt )

Get a termination handler for a task.
