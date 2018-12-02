# Profiling

This is made available by executing:

    REQUIRE src/forth/profile.fs

The following words are in the `forth-wordlist` wordlist.

`SET-PROFILE-INSTRUMENT-ENABLE` ( enable -- )

Set whether to instrument words created after this point for profiling. Note that this instrumentation does come with a performance hit, as it requires adding code to each word to carry out profiling. Also, this does not retroactively instrument words created before this is executed.

`GET-PROFILE-INSTRUMENT-ENABLE` ( -- enable )

Get whether words are to be instrumented for profiling at the present.

`SET-PROFILE-ENABLE` ( enable -- )

Set whether words executed after this point will update their profiling data.

`GET-PROFILE-ENABLE` ( -- enable )

Get whether words executed after this point will update their profiling data.

`PROFILE.` ( -- )

Output the collected profiling data for all the words instrumented for profiling.

`RESET-PROFILE` ( -- )

Reset the profiling data for all the words instrumented for profiling so that their execution counts are returned to zero.
