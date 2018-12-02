# Unbounded Channels

These are made available by executing:

    REQUIRE src/forth/chan.fs

These following words are in the `TASK-WORDLIST` wordlist.

`CHAN.` ( chan -- )

Display statistics on an unbounded channel.

`NEW-CHAN` ( -- chan )

Create a new unbounded channel.

`DESTROY-CHAN` ( -- chan )

Destroy an unbounded channel. Do not call when a task is waiting on an unbounded channel.

`SEND-CHAN` ( x chan -- )

Send a message on an unbounded channel.

`RECV-CHAN` ( chan -- x )

Receive with dequeuing a message from an unbounded channel, waiting for one to become available if none are available.

`PEEK-CHAN` ( chan -- x )

Receive without dequeuing a message from an unbounded channel, waiting for one to become available if none are available.

`TRY-RECV-CHAN` ( chan -- x found )

Receive with dequeuing a message from an unbounded channel and return a flag `TRUE` if one is available, otherwise return a flag `FALSE` and a dummy value is none are available.

`TRY-PEEK-CHAN` ( chan -- x found )

Receive without dequeuing a message from an unbounded channel and return a flag `TRUE` if one is available, otherwise return a flag `FALSE` and a dummy value is none are available.

`COUNT-CHAN` ( chan -- u )

Get the number of queued messages in an unbounded channel.
