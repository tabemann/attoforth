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
#include "af/cond.h"
#include "af/inner.h"

void af_thread_loop(af_global_t* global) {
  while(global->first_thread) {
    while(global->threads_active_count) {
      af_thread_t* thread = global->first_thread;
      af_thread_t* prev_thread = NULL;
      while(thread) {
	if(thread->is_to_be_freed) {
	  af_thread_t* old_thread = thread;
	  thread = thread->next_thread;
	  if(prev_thread) {
	    prev_thread->next_thread = thread;
	  } else {
	    global->first_thread = thread;
	  }
	  af_free_thread(old_thread);
	} else {
	  af_inner_loop(global, thread);
	  thread = thread->next_thread;
	}
      }
    }
    if(global->first_thread) {
      af_cond_wait(&global->cond);
    }
  }
}

void af_inner_loop(af_global_t* global, af_thread_t* thread) {
  thread->current_cycles_left = thread->current_cycles_before_yield;
  while(thread->current_cycles_left-- && thread->interpreter_pointer) {
    af_compiled_t* interpreter_pointer = thread->interpreter_pointer;
    interpreter_pointer->compiled_call->code(global, thread);
  }
  if(!thread->interpreter_pointer) {
    if(af_word_wait(global, thread, ' ')) {
      uint8_t* text = af_word(global, thread, ' ');
      af_word_t* word = af_lookup(global, text);
      if(word) {
	if(thread->is_compiling) {
	  af_compiled_t* slot = af_allot_compile(global, thread, 1);
	  if(slot) {
	    (slot - 1)->compiled_call = word;
	    slot->compiled_call = global->builtin_exit;
	  }
	} else {
	  word->code(global, thread);
	}
      } else {
	int64_t result;
	if(af_parse_number(global, text, &result)) {
	} else {
	  af_handle_parse_error(global, thread);
	}
      }
    }
  }
}

void af_free_thread(af_thread_t* thread) {
  free(thread->data_stack_top);
  free(thread->return_stack_top);
  free(thread);
}

void af_kill(af_global_t* global, af_thread_t* thread) {
  thread->current_cycles_before_yield = 0;
  thread->current_cycles_left = 0;
  thread->is_to_be_freed = TRUE;
  global->threads_active_count--;
}

void af_yield(af_global_t* global, af_thread_t* thread) {
  thread->current_cycles_left = 0;
}

void af_sleep(af_thread_t* thread) {
  thread->current_cycles_before_yield = 0;
  thread->current_cycles_left = 0;
  global->threads_active_count--;
}

void af_reset(af_global_t* global, af_thread_t* thread) {
  thread->base_cycles_before_yield = global->default_cycles_before_yield;
  thread->current_cycles_before_yield = global->default_cycles_before_yield;
  thread->is_compiling = FALSE;
  thread->interpreter_pointer = NULL;
  thread->data_stack_current = thread->data_stack_base;
  thread->return_stack_current = thread->return_stack_base;
  thread->most_recent_word = NULL;
  thread->text_input_count = 0;
  thread->text_input_index = 0;
  thread->text_input_closed = FALSE;
  thread->input_source_buffer = &thread->text_input_buffer;
  thread->input_source_count = &thread->text_input_count;
  thread->input_source_index = &thread->text_input_index;
  thread->input_source_closed = &thread->text_input_closed;
  thread->base = 10;
}

void af_handle_data_stack_overflow(af_global_t* global, af_thread_t* thread) {
  af_reset(global, thread);
}

void af_handle_return_stack_overflow(af_global_t* global, af_thread_t* thread) {
  af_reset(global, thread);
}

void af_handle_data_stack_underflow(af_global_t* global, af_thread_t* thread) {
  af_reset(global, thread);
}

void af_handle_return_stack_underflow(af_global_t* global,
				      af_thread_t* thread) {
  af_reset(global, thread);
}

void af_handle_unexpected_input_closure(af_global_t* global,
					af_thread_t* thread) {
  af_reset(global, thread);
}

void af_handle_data_space_overflow(af_global_t* global, af_thread_t* thread) {
  af_reset(global, thread);
}

void af_handle_compile_space_overflow(af_global_t* global,
				      af_thread_t* thread) {
  af_reset(global, thread);
}

void af_handle_parse_error(af_global_t* global, af_thread_t* thread) {
  af_reset(global, thread);
}

void* af_guarantee(af_global_t* global, af_thread_t* thread, size_t size) {
  if((size <= size_remaining) &&
     (global->min_guaranteed_data_space_size <= size_remaining)) {
    return thread->data_space_current;
  } else {
    void* new_data_space_base = malloc(global->default_data_space_size);
    thread->data_space_base = new_data_space_base;
    thread->data_space_current = new_data_space_base;
    thread->data_space_top =
      new_data_space_base + global->default_data_space_size;
    return new_data_space_base;
  }
}

void* af_allocate(af_global_t* global, af_thread_t* thread, size_t size) {
  void* base = af_guarantee(global, thread, size);
  thread->data_space_current += size;
  return base;
}

void* af_allot(af_global_t* global, af_thread_t* thread, size_t size) {
  size_t size_remaining = thread->data_space_top - thread->data_space_current;
  if(size_remaining >= size) {
    void* current = thread->data_space_current;
    thread->data_space_current += size;
  } else {
    af_handle_data_space_overflow(global, thread);
    return NULL;
  }
}

af_compiled_t* af_guarantee_compile(af_global_t* global, af_thread_t* thread,
				    uint64_t count) {
  uint64_t count_remaining =
    thread->compile_space_top - thread->compile_space_current;
  if((count * <= count_remaining) &&
     (global->min_guaranteed_compile_space_count <= count_remaining)) {
    return thread->compile_space_current;
  } else {
    af_compiled_t* new_compile_space_base =
      (af_compiled_t*)malloc(global->default_compile_space_count *
			     sizeof(af_compiled_t));
    thread->compile_space_base = new_compile_space_base;
    thread->compile_space_current = new_compile_space_base;
    thread->compile_space_top =
      new_compile_space_base + global->default_compile_space_count;
    return new_compile_space_base;
  }
}

af_compiled_t* af_allocate_compile(af_global_t* global, af_thread_t* thread,
				   uint64_t count) {
  void* base = af_guarantee_compile(global, thread, count);
  thread->compile_space_current += count;
  return base;
}

af_compiled_t* af_allot_compile(af_global_t* global, af_thread_t* thread,
				uint64_t count) {
  uint64_t count_remaining =
    thread->compile_space_top - thread->compile_space_current;
  if(count_remaining >= count) {
    void* current = thread->data_space_current;
    thread->data_space_current += size;
  } else {
    af_handle_data_space_overflow(global, thread);
    return NULL;
  }
}

af_word_t* af_lookup(af_global_t* global, uint8_t* name) {
  uint8_t length = *name;
  af_word_t* current_word = global->first_word;
  while(current_word) {
    uint8_t current_length = *current_word->name;
    if(length == current_length) {
      if(!memcmp(name + 1, current_word->name + 1, length)) {
	return current_word;
      }
    }
    current_word = current_word->next_word;
  }
  return NULL;
}

af_bool_t af_parse_number(af_global_t* global, uint8_t* text, int64_t* result) {
  char buffer[256];
  uint8_t length = *text;
  memmove(buffer, text + 1, length);
  buffer[length] = 0;
  char* end_ptr;
  int64_t value = strtoll(buffer, end_ptr, 0);
  if(*end_ptr == 0) {
    *result = value;
    return TRUE;
  } else {
    return FALSE;
  }
}

af_bool_t af_word_available(af_global_t* global, af_thread_t* thread,
			    uint8_t delimiter) {
  uint64_t current_index = *thread->input_source_index;
  uint64_t count = *thread->input_source_count;
  uint8_t* buffer = *thread->input_source_buffer;
  uint8_t current_char = *(buffer + current_index);
  while(current_index < count &&
	(current_char == delimiter ||
	 current_char == ' ' ||
	 current_char == '\n')) {
    current_char = *(buffer + ++current_index);
  }
  if(current_index == count) {
    return FALSE;
  }
  if(!thread->input_source_closed) {
    while(current_index < count &&
	  current_char != delimiter &&
	  current_char != '\n') {
      current_char = *(buffer + ++current_index);
    }
    return current_index < count;
  } else {
    return TRUE;
  }
}

af_bool_t af_word_wait(af_global_t* global, af_thread_t* thread,
		       uint8_t delimiter) {
  if(!af_thread_available(global, thread, delimiter)) {
    if(!thread->input_source_closed) {
      af_sleep(global, thread);
    } else {
      af_handle_unexpected_input_closure(global, thread);
    }
    return FALSE;
  } else {
    return TRUE;
  }
}

uint8_t* af_word(af_global_t* global, af_thread_t* thread, uint8_t delimiter) {
  uint64_t current_index = *thread->input_source_index;
  uint64_t count = *thread->input_source_count;
  uint8_t* buffer = *thread->input_source_buffer;
  uint8_t current_char = *(buffer + current_index);
  while(current_index < count &&
	(current_char == delimiter ||
	 current_char == ' ' ||
	 current_char == '\n')) {
    current_char = *(buffer + ++current_index);
  }
  if(current_index == count) {
    uint8_t* here = *(uint8_t*)af_guarantee(global, thread, 1);
    *here = 0;
    return here;
  }
  uint64_t start_index = current_index;
  while(current_index < count &&
        current_char != delimiter &&
	current_char != '\n') {
    current_char = *(buffer + ++current_index);
  }
  if(!thread->input_source_closed && current_index == count) {
    uint8_t* here = *(uint8_t*)af_guarantee(global, thread, 1);
    *here = 0;
    return here;
  }
  *thread->input_source_count = current_index;
  size_t bytes_copied;
  if(current_index - start_index <= 255) {
    bytes_copied = current_index - start_index;
  } else {
    bytes_copied = 255;
  }
  uint8_t* here = *(uint8_t*)af_guarantee(global, thread, bytes_copied + 1);
  *here = (uint8_t)bytes_copied;
  memcpy(here, buffer + start_index, bytes_copied);
  return here;
}
