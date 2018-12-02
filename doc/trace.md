# Tracing

This non-debugging mode tracer is made available by executing:

    REQUIRE src/forth/trace.fs

The following words are in the `FORTH-WORDLIST` wordlist.

`SET-TRACE-INSTRUMENT-ENABLE` ( enable -- )

Set whether to instrument words created after this point for tracing. Note that this instrumentation does come with a performance hit, as it requires adding code to each word to carry out tracing, which is quite substantial when tracing is enabled, since it requires writing to standard output for each instrumented word executed. Also, this does not retroactively instrument words created before this is executed. Additionally, this does not instrument words created with `CREATE` and `DOES>`.

`GET-TRACE-INSTRUMENT-ENABLE` ( -- enable )

Get whether words are to be instrumented for tracing at the present.

`SET-TRACE-ENABLE` ( enable -- )

Set whether words executed after this point will update their tracing data.

`GET-TRACE-ENABLE` ( -- enable )

Get whether words executed after this point will update their tracing data.

There is also a debugging mode tracer that is built in which must be enabled by compiling attoforth with:

    make CFLAGS="-DDEBUG"

This tracer has two versions, one global and one task-specific; the global tracer is in the `FORTH-WORDLIST` wordlist and the task-specific tracer is in the `TASK-WORDLIST` wordlist:

`GLOBAL-TRACE` ( enable -- )

Set whether to carry out global tracing, i.e. for all words in every task.

`TASK-TRACE` ( enable task -- )

Set whether to carry out tracing for a particular task, i.e. for all words in that particulr task.

These two tracers have a far more substantial performance hit than the `src/forth/trace.fs` tracer because they trace every single word executed. They also circumvent the terminal control mechanisms involving normal standard output IO, and thus interfere with the user input mechanism; it may not be possible for the user to control attoforth once these are enabled, and thus it is recommended that the user write code for automatically turning off debugging mode tracing once it is not needed since the user may not be able to turn it off manually.