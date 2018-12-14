4 chars constant line-buffer-size
line-buffer-size buffer: line-buffer

: test-create-file
  s" test_file" r/w create-file dup if
    ." create-file error: " . drop abort
  else drop then
  ." A" cr
  s" foo " 2 pick write-file dup if
    ." write-file error: " . abort
  else drop then
  ." B" cr
  s" bar" 2 pick write-line dup if
    ." write-line error: " . abort
  else drop then
  ." C" cr
  s" baz " 2 pick write-file dup if
    ." write-file error: " . abort
  else drop then
  ." D" cr
  s" qux" 2 pick write-line dup if
    ." write-line error: " . abort
  else drop then
  ." E" cr
  dup file-position dup if
    ." file-position error: " . drop abort
  else drop then
  ." F" cr
  ." position: " drop . cr
  4 0 2 pick reposition-file dup if
    ." reposition-file error: " . abort
  else drop then
  ." G" cr
  s" BAR" 2 pick write-file dup if
    ." write-file error: " . abort
  else drop then
  ." H" cr
  0 0 2 pick reposition-file dup if
    ." reposition-file error: " . abort
  else drop then
  ." I" cr
  dup file-position dup if
    ." file-position error: " . drop abort
  else drop then
  ." J" cr
  ." position: " drop . cr
  line-buffer line-buffer-size 2 pick read-line dup if
    ." read-line error: " . drop drop abort
  else drop then
  ." K" cr
  ." flag: " . space ." length: " dup . space
  ." text: " line-buffer swap type cr
  line-buffer line-buffer-size 2 pick read-line dup if
    ." read-line error: " . drop drop abort
  else drop then
  ." L" cr
  ." flag: " . space ." length: " dup . space
  ." text: " line-buffer swap type cr
  line-buffer line-buffer-size 2 pick read-line dup if
    ." read-line error: " . drop drop abort
  else drop then
  ." M" cr
  ." flag: " . space ." length: " dup . space
  ." text: " line-buffer swap type cr
  close-file
;

