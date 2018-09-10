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
#include <stdint.h>
#include <stddef.h>
#include <pthread.h>
#include "af/common.h"
#include "af/types.h"
#include "af/cond.h"
#include "af/inner.h"

void af_cond_init(af_cond_t* cond) {
  pthread_cond_init(&cond->cond);
  pthread_mutex_init(&cond->mutex);
  cond->count = 0;
}

void af_cond_destroy(af_cond_t* cond) {
  pthread_mutex_destroy(&cond->mutex);
  pthread_cond_destroy(&cond->cond);
}

void af_cond_wait(af_cond_t* cond) {
  pthread_cond_lock(&cond->mutex);
  if(!cond->count) {
    pthread_cond_wait(&cond->cond, &cond->mutex);
  }
  cond->count = 0;
  pthread_cond_unlock(&cond->mutex);
}

void af_cond_signal(af_cond_t* cond) {
  pthread_cond_lock(&cond->mutex);
  cond->count++;
  pthread_cond_signal(&cond->cond);
  pthread_cond_unlock(&cond->mutex);
}
