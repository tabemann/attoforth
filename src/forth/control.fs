\ Copyright (c) 2018, Travis Bemann
\ All rights reserved.
\ 
\ Redistribution and use in source and binary forms, with or without
\ modification, are permitted provided that the following conditions are met:
\ 
\ 1. Redistributions of source code must retain the above copyright notice,
\    this list of conditions and the following disclaimer.
\ 
\ 2. Redistributions in binary form must reproduce the above copyright notice,
\    this list of conditions and the following disclaimer in the documentation
\    and/or other materials provided with the distribution.
\ 
\ 3. Neither the name of the copyright holder nor the names of its
\    contributors may be used to endorse or promote products derived from
\    this software without specific prior written permission.
\ 
\ THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
\ AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
\ IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
\ ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
\ LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
\ CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
\ SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
\ INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
\ CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
\ ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
\ POSSIBILITY OF SUCH DAMAGE.

GET-ORDER GET-CURRENT BASE @

REQUIRE src/forth/lambda.fs

DECIMAL
FORTH-WORDLIST 1 SET-ORDER
FORTH-WORDLIST SET-CURRENT

REQUIRE src/forth/lambda.fs

FORTH-WORDLIST LAMBDA-WORDLIST 2 SET-ORDER
LAMBDA-WORDLIST SET-CURRENT

: OPTION ( ... flag true-xt -- ... )
  SWAP IF EXECUTE ELSE DROP THEN ;

: CHOOSE ( ... flag true-xt false-xt -- ... )
  ROT IF DROP EXECUTE ELSE NIP EXECUTE THEN ;

: LOOP-UNTIL ( ... xt -- ... )
  >R BEGIN R@ EXECUTE UNTIL R> DROP ;

: WHILE-LOOP ( ... while-xt body-xt -- ... )
  2>R BEGIN 2R@ DROP EXECUTE WHILE R@ EXECUTE REPEAT 2R> 2DROP ;

: COUNT-LOOP ( ... limit init xt -- ... )
  ROT ROT ?DO I SWAP DUP >R EXECUTE R> LOOP DROP ;

: COUNT+LOOP ( ... limit init xt -- ... )
  ROT ROT ?DO I SWAP DUP >R EXECUTE R> SWAP +LOOP DROP ;

BASE ! SET-CURRENT SET-ORDER
