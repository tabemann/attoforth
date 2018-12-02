# Bounded Channels

These are made available by executing:

    REQUIRE src/forth/bchan.fs

These following words are in the `TASK-WORDLIST` wordlist.

`BCHAN.` ( chan -- )

Display statistics on an bounded channel.

`NEW-BCHAN` ( queue-size -- chan )

Create a new bounded channel with a maximum queue size of the given number of cells.

`DESTROY-BCHAN` ( -- chan )

Destroy an bounded channel. Do not call when a task is waiting on an bounded channel.

`SEND-BCHAN` ( x chan -- )

Send a message on an bounded channel, waiting of the bounded channel already contains a maximum number of messages until at least one of those messages are dequeued.

`TRY-SEND-BCHAN` ( x chan -- success )

Send a message on a bounded channel and return `TRUE` if it does not already contain a maximum number of messages, else return `FALSE`.

`RECV-BCHAN` ( chan -- x )

Receive with dequeuing a message from an bounded channel, waiting for one to become available if none are available. If there is a task waiting for space to become available in the queue, wake that task up.

`PEEK-BCHAN` ( chan -- x )

Receive without dequeuing a message from an bounded channel, waiting for one to become available if none are available.

`TRY-RECV-BCHAN` ( chan -- x found )

Receive with dequeuing a message from an bounded channel and return a flag `TRUE` if one is available, otherwise return a flag `FALSE` and a dummy value is none are available. If there is a task waiting for space to become available in the queue, wake that task up.

`TRY-PEEK-BCHAN` ( chan -- x found )

Receive without dequeuing a message from an bounded channel and return a flag `TRUE` if one is available, otherwise return a flag `FALSE` and a dummy value is none are available.

`COUNT-BCHAN` ( chan -- u )

Get the number of queued messages in an bounded channel.
