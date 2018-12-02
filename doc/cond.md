# Condition Variables

These are made available by executing:

    REQUIRE src/forth/cond.fs

The following words are in the `TASK-WORDLIST` wordlist.

`NEW-COND` ( lock -- cond )

Create a new condition variable set to use the passed-in lock.

`DESTROY-COND` ( cond -- )

Destroy a condition variable. Do not call when a task is waiting on a condition variable.

`WAIT-COND` ( cond -- )

Wait for a signal or broadcast on a condition variable. This requires the lock for the condition variable to be held by the task attempting to wait, or otherwise an abort will occur. This lock will be freed when waiting, and will be locked again by the task after it is woken up.

`SIGNAL-COND` ( cond -- )

Signal a condition variable to wake up one waiting task.

`BROADCAST-COND` ( cond -- )

Broadcast on a condition variable to wake up all waiting tasks.
