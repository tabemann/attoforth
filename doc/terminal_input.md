# Terminal Input

This is available in `src/forth/init.fs`, which is automatically loaded upon attoforth startup. The following words are in the `FORTH-WORDLIST` wordlist.

`KEY?` ( -- pressed )

Test whether a byte has been input on standard input, and if it has been, save the byte read so it can be retrieved by a subsequent call to `KEY`. Note that successive calls to `KEY?` after it returns `TRUE` an initial time will return `TRUE` without reading any more characters until `KEY` is called, freeing the buffer.

`KEY` ( -- c )

Get the most recent byte input on standard input. This clears bytes buffered by `KEY?` so that `KEY?` will return `FALSE` after this is called until the user inputs another byte on standard input.

`ACCEPT` ( c-addr bytes -- bytes-input )

Input a line with the line editor, up to the given number of bytes, and store it in the provided buffer, returning the number of bytes entered, up to the numbmer of bytes specified. Note that newlines are not included in the text stored in the buffer and do not contribute to the returned number of bytes.

The line editor used for input by attoforth has the following commands:

* \<forward> or control-F: Move one character forward
* \<backward> or control-B: Move one character backward
* \<up>: Move one line back in the history
* \<down>: Move one line forward in the history
* \<backspace>: Delete a character before the cursor
* \<delete>: Delete a character at the cursor
* control-A: Move to the start of the line
* control-E: Move to the back of the line
* control-K: Kill text from the cursor to the end of the line and put it in the kill ring
* control-L: Refresh the terminal
* control-W or alt-D: Kill text from the cursor to the preceding space before the preceding word and put it in the kill ring
* control-Y: Yank the most recent text from the kill ring and insert it at the cursor
* alt-\<forward> or alt-F: Move to the end of the next word
* alt-\<backward> or alt-B: Move to the start of the previous word
* alt-D: Kill text from the cursor to the end of the next word and put it in the kill ring
* alt-Y: If the last command was a yank, replace the yanked text with the previous text in the kill ring, moving the last yanked text to the end of the kill ring