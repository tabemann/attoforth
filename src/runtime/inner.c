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
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <ctype.h>
#include <pthread.h>
#include "af/common.h"
#include "af/types.h"
#include "af/cond.h"
#include "af/inner.h"
#include "af/io.h"
#include "af/prim.h"
#include "af/builtin.h"

/* Forward declarations */

void af_free_thread(af_thread_t* thread);

void af_pop_all_inputs(af_thread_t* thread);

void af_inner_loop(af_global_t* global, af_thread_t* thread);

/* Definitions */

af_global_t* af_global_init(void) {
  af_global_t* global = malloc(sizeof(af_global_t));
  if(!global) {
    return NULL;
  }
  global->first_thread = NULL;
  global->threads_active_count = 0;
  if(!af_cond_init(&global->cond)) {
    free(global);
    return NULL;
  }
  if(pthread_mutex_init(&global->mutex, NULL)) {
    af_cond_destroy(&global->cond);
    free(global);
    return NULL;
  }
  global->first_word = NULL;
  global->default_data_stack_count = 16384;
  global->default_return_stack_count = 15872;
  global->min_guaranteed_data_space_size = 8192;
  global->default_data_space_size = 130048;
  global->default_cycles_before_yield = 1024;
  global->builtin_literal_runtime = NULL;
  global->builtin_exit = NULL;
  global->builtin_postpone_runtime = NULL;
  global->default_cleanup = NULL;
  global->default_drop_input = NULL;
  global->default_interactive_endline = NULL;
  if(!(af_io_init(&global->io, global))) {
    pthread_mutex_destroy(&global->mutex);
    af_cond_destroy(&global->cond);
    free(global);
    return NULL;
  }
  return global;
}

void af_global_execute(af_global_t* global) {
  af_thread_t* thread = af_spawn(global);
  if(!thread) {
    return;
  }
  af_register_prims(global, thread);
  af_compile_builtin(global, thread);
  af_start(global, thread);
  af_thread_loop(global);
}

void af_thread_loop(af_global_t* global) {
  pthread_mutex_lock(&global->mutex);
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
	  pthread_mutex_unlock(&global->mutex);
	  pthread_mutex_lock(&global->mutex);
	  af_inner_loop(global, thread);
	  thread = thread->next_thread;
	}
      }
    }
    if(global->first_thread) {
      pthread_mutex_unlock(&global->mutex);
      af_cond_wait(&global->cond);
      pthread_mutex_lock(&global->mutex);
    }
  }
  pthread_mutex_unlock(&global->mutex);
}

void af_inner_loop(af_global_t* global, af_thread_t* thread) {
  thread->current_cycles_left = thread->current_cycles_before_yield;
  if(thread->current_cycles_left && thread->interpreter_pointer) {
    thread->current_interactive_word = NULL;
    thread->repeat_interactive = FALSE;
  }
  if(thread->init_word) {
    thread->init_word->code(global, thread);
  }
  if(thread->current_cycles_left ||
     (thread->current_cycles_before_yield && !thread->interpreter_pointer)) {
    thread->init_word = NULL;
  }
  while(thread->current_cycles_left-- && thread->interpreter_pointer) {
    af_compiled_t* interpreter_pointer = thread->interpreter_pointer;
    interpreter_pointer->compiled_call->code(global, thread);
  }
  if(thread->current_cycles_before_yield && !thread->interpreter_pointer) {
    if(af_parse_name_available(global, thread)) {
      af_cell_t length;
      af_byte_t* text = af_parse_name(global, thread, &length);
      char* print_text = malloc(length + 1);
      memcpy(print_text, text, length);
      print_text[length] = '\0';
      af_word_t* word = af_lookup(global, text, length);
      if(word) {
	if(thread->is_compiling && !word->is_immediate) {
	  printf("COMPILING WORD: %s\n", print_text);	
	  af_compiled_t* slot = af_allot(global, thread,
					 sizeof(af_compiled_t));
	  if(slot) {
	    slot->compiled_call = word;
	  }
	} else {
	  printf("EXECUTING WORD: %s\n", print_text);	
	  thread->current_interactive_word = word;
	  word->code(global, thread);
	}
      } else {
	af_sign_cell_t result;
	if(af_parse_number(global, text, (size_t)length, &result)) {
	  if(thread->is_compiling) {
	    af_compiled_t* slot = af_allot(global, thread,
					   sizeof(af_compiled_t) * 2);
	    if(slot) {
	      slot->compiled_call = global->builtin_literal_runtime;
	      (slot + 1)->compiled_sign_cell = result;
	    }
	  } else {
	    if(--thread->data_stack_current >= thread->data_stack_top) {
	      *thread->data_stack_current = (af_cell_t)result;
	    } else {
	      af_handle_data_stack_overflow(global, thread);
	    }
	  }
	} else {
	  af_handle_parse_error(global, thread);
	}
      }
      free(print_text);
    } else if(thread->current_input && !thread->current_input->is_closed) {
      af_interactive_endline(global, thread);
    } else if(thread->return_stack_current < thread->return_stack_base) {
      thread->interpreter_pointer = *thread->return_stack_current++;
      af_drop_input(global, thread);
    } else {
      af_kill(global, thread);
    }
  }
}

void af_free_thread(af_thread_t* thread) {
  /* af_pop_all_inputs(thread); */
  free(thread->data_stack_top);
  free(thread->return_stack_top);
  free(thread);
}

void af_lock(af_global_t* global) {
  pthread_mutex_lock(&global->mutex);
}

void af_unlock(af_global_t* global) {
  pthread_mutex_unlock(&global->mutex);
}

af_thread_t* af_spawn(af_global_t* global) {
  af_cell_t* data_stack_top;
  af_compiled_t** return_stack_top;
  af_compiled_t* compile_space_base;
  af_compiled_t* data_space_base;
  af_thread_t* thread;
  if(!(data_stack_top = malloc(global->default_data_stack_count *
			       sizeof(af_cell_t)))) {
    return NULL;
  }
  if(!(return_stack_top = malloc(global->default_return_stack_count *
				 sizeof(af_compiled_t*)))) {
    free(data_stack_top);
    return NULL;
  }
  if(!(data_space_base = malloc(global->default_data_space_size *
				sizeof(af_cell_t)))) {
    free(return_stack_top);
    free(data_stack_top);
    return NULL;
  }
  if(!(thread = malloc(sizeof(af_thread_t)))) {
    free(data_space_base);
    free(return_stack_top);
    free(data_stack_top);
    return NULL;
  }
  thread->next_thread = global->first_thread;
  global->first_thread = thread;
  thread->base_cycles_before_yield = 0;
  thread->current_cycles_before_yield = 0;
  thread->current_cycles_left = 0;
  thread->is_compiling = FALSE;
  thread->is_to_be_freed = FALSE;
  thread->interpreter_pointer = NULL;
  thread->data_stack_current = thread->data_stack_base =
    data_stack_top + global->default_data_stack_count;
  thread->return_stack_current = thread->return_stack_base =
    return_stack_top + global->default_return_stack_count;
  thread->data_stack_top = data_stack_top;
  thread->return_stack_top = return_stack_top;
  thread->data_space_current = thread->data_space_base =
    data_space_base;
  thread->data_space_top =
    data_space_base + global->default_data_space_size;
  thread->most_recent_word = NULL;
  thread->console_input = NULL;
  thread->console_output = NULL;
  thread->console_error = NULL;
  thread->current_input = NULL;
  thread->current_output = NULL;
  thread->current_error = NULL;
  thread->base = 10;
  thread->init_word = NULL;
  thread->current_interactive_word = NULL;
  thread->repeat_interactive = FALSE;
  thread->cleanup = global->default_cleanup;
  thread->drop_input = global->default_drop_input;
  thread->interactive_endline = global->default_interactive_endline;
  return thread;
}

void af_set_init_word(af_global_t* global, af_thread_t* thread,
		      af_word_t* word) {
  if(!thread->base_cycles_before_yield && !thread->is_to_be_freed) {
    thread->init_word = word;
  }
}

void af_interpret(af_global_t* global, af_thread_t* thread) {
  if(thread->return_stack_current <= thread->return_stack_top) {
    af_handle_return_stack_overflow(global, thread);
    return;
  }
  *(--thread->return_stack_current) = thread->interpreter_pointer;
  thread->interpreter_pointer = NULL;
}

void af_push_data(af_global_t* global, af_thread_t* thread, af_cell_t data) {
  AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
  *(--thread->data_stack_current) = data;
}

void af_push_return(af_global_t* global, af_thread_t* thread,
		    af_compiled_t* pointer) {
  AF_VERIFY_RETURN_STACK_EXPAND(global, thread, 1);
  *(--thread->return_stack_current) = pointer;
}

void af_drop_input(af_global_t* global, af_thread_t* thread) {
  if(thread->current_input && thread->drop_input) {
    AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
    *(--thread->data_stack_current) = (af_cell_t)thread;
    thread->drop_input->code(global, thread);
  }
}

void af_interactive_endline(af_global_t* global, af_thread_t* thread) {
  if(!thread->interactive_endline) {
    af_refill(global, thread);
    if(thread->current_input) {
      if(thread->current_input->is_closed &&
	 !thread->current_input->count) {
	if(thread->return_stack_current < thread->return_stack_base) {
	  thread->interpreter_pointer = *thread->return_stack_current++;
	  af_drop_input(global, thread);	  
	} else {
	  af_kill(global, thread);
	}
      }
    }
  } else if(thread->current_input) {
    AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
    *(--thread->data_stack_current) = (af_cell_t)thread;
    thread->interactive_endline->code(global, thread);
  }
}

void af_start(af_global_t* global, af_thread_t* thread) {
  if(!thread->base_cycles_before_yield && !thread->is_to_be_freed) {
    thread->base_cycles_before_yield = thread->current_cycles_before_yield =
      thread->current_cycles_left = global->default_cycles_before_yield;
    global->threads_active_count++;
  }
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

void af_sleep(af_global_t* global, af_thread_t* thread) {
  thread->current_cycles_before_yield = 0;
  thread->current_cycles_left = 0;
  global->threads_active_count--;
}

void af_wake(af_global_t* global, af_thread_t* thread) {
  if(!thread->current_cycles_before_yield && !thread->is_to_be_freed) {
    thread->current_cycles_before_yield = thread->base_cycles_before_yield;
    global->threads_active_count++;
  }
}

void af_reset(af_global_t* global, af_thread_t* thread) {
  thread->base_cycles_before_yield = global->default_cycles_before_yield;
  thread->current_cycles_before_yield = global->default_cycles_before_yield;
  thread->is_compiling = FALSE;
  thread->interpreter_pointer = NULL;
  thread->data_stack_current = thread->data_stack_base;
  thread->return_stack_current = thread->return_stack_base;
  thread->most_recent_word = NULL;
  thread->base = 10;
  if(thread->cleanup) {
    AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
    *(--thread->data_stack_current) = (af_cell_t)thread;
    thread->cleanup->code(global, thread);
  }
}

void af_quit(af_global_t* global, af_thread_t* thread) {
  thread->is_compiling = FALSE;
  thread->interpreter_pointer = NULL;
  thread->return_stack_current = thread->return_stack_base;
  if(thread->cleanup) {
    AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
    *(--thread->data_stack_current) = (af_cell_t)thread;
    thread->cleanup->code(global, thread);
  }
}

void af_repeat_interactive(af_global_t* global, af_thread_t* thread) {
  if(!thread->interpreter_pointer) {
    thread->repeat_interactive = TRUE;
  }
}

void af_handle_data_stack_overflow(af_global_t* global, af_thread_t* thread) {
  printf("Data stack overflow\n");
  af_reset(global, thread);
}

void af_handle_return_stack_overflow(af_global_t* global, af_thread_t* thread) {
  printf("Return stack overflow\n");
  af_reset(global, thread);
}

void af_handle_data_stack_underflow(af_global_t* global, af_thread_t* thread) {
  printf("Data stack underflow\n");
  af_reset(global, thread);
}

void af_handle_return_stack_underflow(af_global_t* global,
				      af_thread_t* thread) {
  printf("Return stack underflow\n");
  af_reset(global, thread);
}

void af_handle_word_expected(af_global_t* global, af_thread_t* thread) {
  printf("Word expected\n");
  af_reset(global, thread);
}

void af_handle_data_space_overflow(af_global_t* global, af_thread_t* thread) {
  printf("Data space overflow\n");
  af_reset(global, thread);
}

void af_handle_parse_error(af_global_t* global, af_thread_t* thread) {
  printf("Parse error\n");
  af_reset(global, thread);
}

void af_handle_word_not_found(af_global_t* global, af_thread_t* thread) {
  printf("Word not found\n");
  af_reset(global, thread);
}

void af_handle_out_of_memory(af_global_t* global, af_thread_t* thread) {
  printf("Out of memory\n");
  af_kill(global, thread);
}

void af_handle_divide_by_zero(af_global_t* global, af_thread_t* thread) {
  printf("Divide by zero\n");
  af_reset(global, thread);
}

void af_handle_compile_only(af_global_t* global, af_thread_t* thread) {
  printf("Compile only\n");
  af_reset(global, thread);
}

void af_handle_interpret_only(af_global_t* global, af_thread_t* thread) {
  printf("Interpret only\n");
  af_reset(global, thread);
}

void af_handle_no_word_created(af_global_t* global, af_thread_t* thread) {
  printf("No word created\n");
  af_reset(global, thread);
}

void af_handle_not_interactive(af_global_t* global, af_thread_t* thread) {
  printf("Must not be interactive\n");
  af_reset(global, thread);
}

void* af_guarantee(af_global_t* global, af_thread_t* thread, size_t size) {
  size_t size_remaining = thread->data_space_top - thread->data_space_current;
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

void* af_allot(af_global_t* global, af_thread_t* thread, ssize_t size) {
  ssize_t size_remaining = thread->data_space_top - thread->data_space_current;
  if(size_remaining >= size) {
    void* current = thread->data_space_current;
    thread->data_space_current += size;
    return current;
  } else {
    af_handle_data_space_overflow(global, thread);
    return NULL;
  }
}

af_word_t* af_lookup(af_global_t* global, af_byte_t* name,
		     af_cell_t name_length) {
  af_word_t* current_word = global->first_word;
  while(current_word) {
    af_byte_t current_word_length = AF_WORD_NAME_LEN(current_word);
    if(name_length == current_word_length) {
      af_byte_t* current_name = name;
      af_byte_t* current_word_name = AF_WORD_NAME_DATA(current_word);
      af_byte_t current_name_length = name_length;
      af_bool_t differ = FALSE;
      while(current_name_length--) {
	if(toupper(*current_name++) != toupper(*current_word_name++)) {
	  differ = TRUE;
	  break;
	}
      }
      if(!differ) {
	return current_word;
      }
    }
    current_word = current_word->next_word;
  }
  return NULL;
}

af_bool_t af_parse_number(af_global_t* global, af_byte_t* text,
			  size_t length, af_sign_cell_t* result) {
  char* buffer = malloc(sizeof(af_byte_t) * (length + 1));
  char* end_ptr;
  af_sign_cell_t value;
  if(!buffer) {
    return FALSE;
  }
  memcpy(buffer, (void*)text, length);
  buffer[length] = '\0';
  value = strtoll(buffer, &end_ptr, 0);
  if(*end_ptr == 0) {
    free(buffer);
    *result = value;
    return TRUE;
  } else {
    free(buffer);
    return FALSE;
  }
}

af_bool_t af_word_available(af_global_t* global, af_thread_t* thread,
			    af_byte_t delimiter) {
  af_cell_t current_index;
  af_cell_t count;
  af_byte_t* buffer;
  af_byte_t current_char;
  if(!thread->current_input) {
    return FALSE;
  }
  current_index = thread->current_input->index;
  count = thread->current_input->count;
  buffer = thread->current_input->buffer;
  current_char = *(buffer + current_index);
  while(current_index < count && current_char == delimiter) {
    current_char = *(buffer + ++current_index);
  }
  if(current_index == count) {
    return FALSE;
  }
  if(!thread->current_input->is_closed) {
    while(current_index < count && current_char != delimiter) {
      current_char = *(buffer + ++current_index);
    }
    return current_index < count;
  } else {
    return TRUE;
  }
}

af_byte_t* af_word(af_global_t* global, af_thread_t* thread, af_byte_t delimiter) {
  af_cell_t current_index;
  af_cell_t count;
  af_byte_t* buffer;
  af_byte_t current_char;
  af_cell_t start_index;
  size_t bytes_copied;
  af_byte_t* here;
  if(!thread->current_input) {
    here = (af_byte_t*)af_guarantee(global, thread, 1);
    *here = 0;
    return here;
  }
  current_index = thread->current_input->index;
  count = thread->current_input->count;
  buffer = thread->current_input->buffer;
  current_char = *(buffer + current_index);
  while(current_index < count && current_char == delimiter) {
    current_char = *(buffer + ++current_index);
  }
  if(current_index == count) {
    here = (af_byte_t*)af_guarantee(global, thread, 1);
    *here = 0;
    return here;
  }
  start_index = current_index;
  while(current_index < count && current_char != delimiter) {
    current_char = *(buffer + ++current_index);
  }
  if(!thread->current_input->is_closed && current_index == count) {
    here = (af_byte_t*)af_guarantee(global, thread, 1);
    *here = 0;
    return here;
  }
  thread->current_input->index = current_index;
  if(current_index - start_index <= 255) {
    bytes_copied = current_index - start_index;
  } else {
    bytes_copied = 255;
  }
  here = (af_byte_t*)af_guarantee(global, thread, bytes_copied + 1);
  *here = (af_byte_t)bytes_copied;
  memcpy(here + sizeof(af_byte_t),
	 buffer + (start_index * sizeof(af_byte_t)),
	 sizeof(af_byte_t) * bytes_copied);
  return here;
}

af_bool_t af_parse_name_available(af_global_t* global, af_thread_t* thread) {
  af_cell_t current_index;
  af_cell_t count;
  af_byte_t* buffer;
  af_byte_t current_char;
  if(!thread->current_input) {
    return FALSE;
  }
  current_index = thread->current_input->index;
  count = thread->current_input->count;
  buffer = thread->current_input->buffer;
  current_char = *(buffer + current_index);
  while(current_index < count &&
	(current_char == ' ' || current_char == '\n')) {
    current_char = *(buffer + ++current_index);
  }
  if(current_index == count) {
    return FALSE;
  }
  if(!thread->current_input->is_closed) {
    while(current_index < count &&
	  (current_char != ' ' || current_char != '\n')) {
      current_char = *(buffer + ++current_index);
    }
    return current_index < count;
  } else {
    return TRUE;
  }
}

af_byte_t* af_parse_name(af_global_t* global, af_thread_t* thread,
		       af_cell_t* length) {
  af_cell_t current_index;
  af_cell_t count;
  af_byte_t* buffer;
  af_byte_t current_char;
  af_cell_t start_index;
  size_t bytes_copied;
  if(!thread->current_input) {
    *length = 0;
    return NULL;
  }
  current_index = thread->current_input->index;
  count = thread->current_input->count;
  buffer = thread->current_input->buffer;
  current_char = *(buffer + current_index);
  while(current_index < count &&
	(current_char == ' ' || current_char == '\n')) {
    current_char = *(buffer + ++current_index);
  }
  if(current_index == count) {
    *length = 0;
    return NULL;
  }
  start_index = current_index;
  while(current_index < count &&
	(current_char != ' ' && current_char != '\n')) {
    current_char = *(buffer + ++current_index);
  }
  if(!thread->current_input->is_closed && current_index == count) {
    *length = 0;
    return NULL;
  }
  thread->current_input->index = current_index;
  *length = current_index - start_index;
  return buffer + start_index;
}

void af_refill(af_global_t* global, af_thread_t* thread) {
  if(thread->current_input) {
    if(thread->current_input->refill) {
      AF_VERIFY_RETURN_STACK_EXPAND(global, thread, 1);
      AF_VERIFY_DATA_STACK_EXPAND(global, thread, 1);
      *(--thread->data_stack_current) = (af_cell_t)thread->current_input;
      thread->current_input->refill->code(global, thread);
    } else {
      thread->current_input->is_closed = TRUE;
      thread->current_input->index = 0;
      thread->current_input->count = 0;
    }
  }
}

af_input_t* af_new_string_input(af_global_t* global, af_byte_t* buffer,
				af_cell_t count) {
  af_input_t* input;
  input = malloc(sizeof(af_input_t));
  if(!input) {
    return NULL;
  }
  input->next_input = NULL;
  input->buffer = buffer;
  input->count = count;
  input->index = 0;
  input->is_closed = TRUE;
  input->cleanup = global->builtin_free;
  input->refill = NULL;
  input->arg = 0;
  return input;
}

af_word_t* af_register_prim(af_global_t* global, af_thread_t* thread,
			    af_byte_t* name, af_prim_t prim,
			    af_bool_t is_immediate) {
  void* word_space;
  af_word_t* word;
  if(name) {
    size_t name_length = strlen(name) * sizeof(af_byte_t);
    word_space = af_allocate(global, thread,
			     sizeof(af_word_t) +
			     ((name_length + 1) * sizeof(af_byte_t)));
  } else {
    word_space = af_allocate(global, thread, sizeof(af_word_t));
  }
  word = word_space;
  word->is_immediate = is_immediate;
  if(name) {
    size_t name_length = strlen(name) * sizeof(af_byte_t);
    AF_WORD_NAME_LEN(word) = (af_byte_t)name_length;
    memcpy(AF_WORD_NAME_DATA(word), name, name_length);
    word->next_word = global->first_word;
    global->first_word = word;
  } else {
    word->next_word = NULL;
  }
  thread->most_recent_word = word;
  word->code = prim;
  word->secondary = NULL;
  return word;
}
