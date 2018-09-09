/* Copyright (c) 2018, Travis Bemann
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include "af/common.h"
#include "af/inner.h"

void af_compile_builtin(af_global_t* global, af_thread_t* thread) {
  uint8_t* code =
    ": NEGATE -1 * ; "
    ": 0< 0 < ; "
    ": 0= 0 = ; "
    ": 0> 0 > ; "
    ": 1+ 1 + ; "
    ": 1- 1 - ; "
    ": 2+ 2 + ; "
    ": 2- 2 - ; "
    ": 2* 2 * ; "
    ": 2/ 2 * ; "
    ": IF POSTPONE 0BRANCH HERE 0 , ; IMMEDIATE "
    ": ELSE POSTPONE BRANCH HERE 0 , SWAP HERE SWAP ! ; IMMEDIATE "
    ": THEN HERE SWAP ! ; IMMEDIATE "
    ": BEGIN HERE ; IMMEDIATE "
    ": UNTIL POSTPONE 0BRANCH , ; IMMEDIATE "
    ": WHILE POSTPONE 0BRANCH HERE 0 , ; IMMEDIATE "
    ": REPEAT POSTPONE BRANCH SWAP , HERE SWAP ! ; IMMEDIATE ";
    ": ABS DUP 0< IF NEGATE THEN ; "
    ": MIN 2DUP > IF SWAP THEN DROP ; "
    ": MAX 2DUP < IF SWAP THEN DROP ; "
  af_evaluate(global, thread, code, (uint64_t)strlen(code));
}
