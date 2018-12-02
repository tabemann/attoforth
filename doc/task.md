# Tasks

These are available built into attoforth. The following words are in the `TASK-WORDLIST` wordlist.

`THIS-TASK` ( -- task )

Get the current task.

`SPAWN` ( -- task )

Create a new, stopped task. No initial word is set for this task, but standard output and standard error are set up. Note that the user needs to execute `CLEANUP-FULL` to cleanup IO and then `THIS-TASK KILL` to terminate the task manually; if the task is not killed when it terminates then Forth will crash.

`SPAWN-NO-DATA` ( -- task )

Create a new, stopped task *without* a data space or configured standard output or standard error. No initial word is set for this task, but standard output and standard error are set up. Note that the user needs to execute `THIS-TASK KILL` to terminate the task manually; if the task is not killed when it terminates then Forth will crash.

`SPAWN-SIMPLE` ( x\*count count xt -- task )

Create a new task, with configured standard output and standard error, which will clean up IO and cleanly kill itself upon exit. The count is the number of arguments on the data stack to push onto the initial data stack of the executed word upon startup.

`SPAWN-SIMPLE-FREE-DATA-ON-EXIT` ( x\*count count xt -- task )

Create a new task, with configured standard output and standard error, which will clean up IO and cleanly kill itself upon exit. The count is the number of arguments on the data stack to push onto the initial data stack of the executed word upon startup. This differs from `SPAWN-SIMPLE` in that the data space it allocates will be freed upon termination, to prevent memory leakage. Note however this means that no words may be created within this task, and no memory allocated in this task's data space may be referenced after termination.

`SPAWN-SIMPLE-NO-IO` ( x\*count count xt -- task )

Create a new task, *without* configured standard output and standard error, which will clean up IO and cleanly kill itself upon exit. The count is the number of arguments on the data stack to push onto the initial data stack of the executed word upon startup.

`SPAWN-SIMPLE-NO-IO-FREE-DATA-ON-EXIT` ( x\*count count xt -- task )

Create a new task, *without* configured standard output and standard error, which will clean up IO and cleanly kill itself upon exit. The count is the number of arguments on the data stack to push onto the initial data stack of the executed word upon startup. This differs from `SPAWN-SIMPLE-NO-IO` in that the data space it allocates will be freed upon termination, to prevent memory leakage. Note however this means that no words may be created within this task, and no memory allocated in this task's data space may be referenced after termination.

`SPAWN-SIMPLE-NO-DATA` ( x\*count count xt -- task )

Create a new task, *without* an allocated data space or configured standard output or standard error, which will cleanly kill itself upon exit. The count is the number of arguments on the data stack to push onto the initial data stack of the executed word upon startup. Note that it is imperative that no words that reference the data space, including words such as `EMIT`, are executed, because no data space exists for tasks created with this word.

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

`WAIT` ( task -- )

Put a task into a waiting state. Note that unused cycles, up to a limit, are saved so they can be assigned to the task upon being woken up.

`WAKE` ( task -- )

Wake up a task from a waiting state. Note that cycles that were unused when the task was put into a waiting state, up to a limit, are given back to the woken up task.

`KILL` ( task -- )

Kill a task. Note that any further use of the word's task address are undefined, as killing a task frees it to be deallocated. Do not execute this word on a task when it is waiting on a lock, condition variable, or channel.

`SLEEP` ( secs nsecs -- )

Put the current task to sleep for a given number of seconds and nanoseconds.

`TASK-LOCAL` ( addr bytes -- offset )

Allocate a given number of bytes of task-local space with the given address containing the beginning of a buffer containing the initial state of this task-local space in each task, and return the offset of the task-local space.

`T@` ( offset task -- x )

Fetch a cell for the given task at a given offset in its task-local space.

`T!` ( x offset task -- )

Store a cell for the given task at a given offset in its task-local space.

`CT@` ( offset task -- c )

Fetch a byte for the given task at a given offset in its task-local space.

`CT!` ( c offset task -- )

Store a byte for the given task at a given offset in its task-local space.