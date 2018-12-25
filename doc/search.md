# Search Paths

These are available built into attoforth. The following words are in the `FORTH-WORDLIST` wordlist.

`GET-SEARCH-PATHS` ( -- c-addrn bytesn ... c-addr1 bytes1 count )

Get the search paths for the current task and put them on the stack, placing the address and length of the last search path bottom-most on the stack through the address and length of the first search path on the top of the stack, followed by the number of search paths put on the stack.

`SET-SEARCH-PATHS` ( c-addrn bytesn ... c-addr1 bytes1 count )

Get the number of search paths to set off the top of the stack, followed by the address and length of each search path, with the topmost on the stack being the first entry in the search path and the bottommost on the stack being the last entry in the search path.

`OPEN-FILE-IN-SEARCH-PATH` ( c-addr bytes mode -- file error )

Open a file at the location specified by *c-addr bytes* in one of the search paths, starting with the first search path, with the specified *mode* (one of `R/O`, `W/O`, or `R/W`) and if successful place the file on the stack followed by zero, else place a dummy value on the stack followed by -1.
