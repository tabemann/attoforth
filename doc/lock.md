# Locks

These are available built into attoforth. The following words are in the `TASK-WORDLIST` wordlist.

`NEW-LOCK` ( -- lock )

Create a new lock.

`DESTROY-LOCK` ( lock -- )

Destroy a lock. Do not call when a task is holding or waiting on a lock, or when a condition variable is using said lock.

`LOCK` ( lock -- )

Acquire a lock, and if it is currently held, wait until it becomes available and then acquire it.

`UNLOCK` ( lock -- )

Release a lock, and if there is a task currently waiting for it, wake it, pass control of the lock to it, and yield.

`UNLOCK-NO-RELOCK` ( lock -- )

Release a lock, and if there is a task currently waiting for it, wake it, but do not automatically pass control of the lock to it, and do not yield.

`UNLOCK-COUNT-RELOCK` ( count lock -- )

Release a lock, and if there is a task currently waiting for it, wake it, and if a release counter is greater than or equal to the passed in count, pass conrol of the lock to the task and yield, otherwise increment he release counter.