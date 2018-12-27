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

void af_free_task(af_task_t* task);

void af_inner_loop(af_global_t* global, af_task_t* task);

void af_print_current_return_stack(af_global_t* global, af_task_t* task);

/* Definitions */

af_global_t* af_global_init(int argc, char** argv) {
  af_global_t* global = malloc(sizeof(af_global_t));
  if(!global) {
    return NULL;
  }
  if(!(global->forth_wordlist = malloc(sizeof(af_wordlist_t)))) {
    free(global);
    return NULL;
  }
  if(!(global->io_wordlist = malloc(sizeof(af_wordlist_t)))) {
    free(global);
    return NULL;
  }
  if(!(global->task_wordlist = malloc(sizeof(af_wordlist_t)))) {
    free(global);
    return NULL;
  }
  global->first_task = NULL;
  global->current_task = NULL;
  global->tasks_active_count = 0;
  global->task_count = 0;
  if(!af_cond_init(&global->cond)) {
    free(global->task_wordlist);
    free(global->io_wordlist);
    free(global->forth_wordlist);
    free(global);
    return NULL;
  }
  if(pthread_mutex_init(&global->mutex, NULL)) {
    af_cond_destroy(&global->cond);
    free(global->task_wordlist);
    free(global->io_wordlist);
    free(global->forth_wordlist);
    free(global);
    return NULL;
  }
  global->first_of_all_words = NULL;
  global->forth_wordlist->first_word = NULL;
  global->io_wordlist->first_word = NULL;
  global->task_wordlist->first_word = NULL;
  global->default_data_stack_count = 1024;
  global->default_float_stack_count = 1024;
  global->default_return_stack_count = 1024;
  global->default_data_stack_base_room_count = 16;
  global->default_float_stack_base_room_count = 16;
  global->default_return_stack_base_room_count = 16;
  global->min_guaranteed_data_space_size = 1024;
  global->default_data_space_size = 130048;
  global->default_cycles_before_yield = 1024 * 16;
  global->max_cycles_before_yield = 1024 * 64;
  global->max_extra_cycles = 1024 * 128;
  global->default_wordlist_order_max_count = 128;
  global->task_local_space_size = 512;
  global->task_local_space_size_allocated = 0;
  global->do_trace = FALSE;
  if(!(global->default_task_local_space_base =
       malloc(global->task_local_space_size * sizeof(af_byte_t)))) {
    pthread_mutex_destroy(&global->mutex);
    af_cond_destroy(&global->cond);
    free(global->task_wordlist);
    free(global->io_wordlist);
    free(global->forth_wordlist);
    free(global);
    return NULL;
  }
  global->builtin_literal_runtime = NULL;
  global->builtin_f_literal_runtime = NULL;
  global->builtin_exit = NULL;
  global->default_abort = NULL;
  if(!(af_io_init(&global->io, global))) {
    free(global->default_task_local_space_base);
    pthread_mutex_destroy(&global->mutex);
    af_cond_destroy(&global->cond);
    free(global->task_wordlist);
    free(global->io_wordlist);
    free(global->forth_wordlist);
    free(global);
    return NULL;
  }
  global->argc = argc;
  global->argv = (af_byte_t**)argv;
  global->atomic_task = NULL;
  global->handler_task = NULL;
  return global;
}

void af_global_execute(af_global_t* global) {
  af_task_t* task = af_spawn(global, NULL);
  if(!task) {
    return;
  }
  af_register_prims(global, task);
  af_compile_builtin(global, task);
  task->interpreter_pointer = &global->base_interpreter_code[0];
  af_start(global, task);
  af_task_loop(global);
  af_io_cleanup_stdin();
}

void af_task_loop(af_global_t* global) {
  pthread_mutex_lock(&global->mutex);
  while(global->first_task) {
    while(global->tasks_active_count) {
      af_task_t* task = global->first_task;
      af_task_t* prev_task = NULL;
      while(task) {
	if(task->is_to_be_freed) {
	  af_task_t* old_task = task;
	  task = task->next_task;
	  if(prev_task) {
	    prev_task->next_task = task;
	  } else {
	    global->first_task = task;
	  }
	  if(global->atomic_task == old_task) {
	    global->atomic_task = NULL;
	  }
	  af_free_task(old_task);
	} else {
	  if(!global->atomic_task || global->atomic_task == task) {
	    pthread_mutex_unlock(&global->mutex);
	    pthread_mutex_lock(&global->mutex);
	    af_inner_loop(global, task);
	  }
	  prev_task = task;
	  task = task->next_task;
	}
      }
      task = global->first_task;
      prev_task = NULL;
      while(task) {
	if(task->is_to_be_freed) {
	  af_task_t* old_task = task;
	  task = task->next_task;
	  if(prev_task) {
	    prev_task->next_task = task;
	  } else {
	    global->first_task = task;
	  }
	  if(global->atomic_task == old_task) {
	    global->atomic_task = NULL;
	  }
	  af_free_task(old_task);
	} else {
	  prev_task = task;
	  task = task->next_task;
	}
      }
    }
    if(global->first_task) {
      pthread_mutex_unlock(&global->mutex);
      af_cond_wait(&global->cond);
      pthread_mutex_lock(&global->mutex);
    }
  }
  pthread_mutex_unlock(&global->mutex);
}

void af_inner_loop(af_global_t* global, af_task_t* task) {
  if(!task->current_cycles_before_yield) {
    return;
  }
  global->current_task = task;
  task->current_cycles_left =
    task->current_cycles_before_yield + task->extra_cycles;
  if(task->current_cycles_left > global->max_cycles_before_yield) {
    task->extra_cycles =
      task->current_cycles_left - global->max_cycles_before_yield;
    task->current_cycles_left = global->max_cycles_before_yield;
    if(task->extra_cycles > global->max_extra_cycles) {
      task->extra_cycles = global->max_extra_cycles;
    }
  }
  if(task->init_word) {
    AF_WORD_EXECUTE(global, task, task->init_word);
  }
  task->init_word = NULL;
  while(task->current_cycles_left--) {
    af_compiled_t* interpreter_pointer = task->interpreter_pointer;
    af_word_t* word = interpreter_pointer->compiled_call;
    AF_WORD_EXECUTE(global, task, word);
  }
  task->yields++;
  global->current_task = NULL;
}

void af_free_task(af_task_t* task) {
  if(task->free_data_on_exit) {
    free(task->data_space_base);
  }
  free(task->task_local_space_base);
  free(task->wordlist_order);
  free(task->data_stack_top);
  free(task->float_stack_top);
  free(task->return_stack_top);
  free(task);
}

void af_lock(af_global_t* global) {
  pthread_mutex_lock(&global->mutex);
}

void af_unlock(af_global_t* global) {
  pthread_mutex_unlock(&global->mutex);
}

void af_bye(af_global_t* global) {
  af_task_t* task = global->first_task;
  while(task) {
    af_terminate(global, task);
    task = task->next_task;
  }
}

void af_print_state(af_global_t* global, af_task_t* task) {
  af_byte_t length = AF_WORD_NAME_LEN(task->current_word);
  af_byte_t* buffer = malloc((length + 1) * sizeof(af_byte_t));
  af_cell_t* data_stack = task->data_stack_current;
  af_cell_t return_count = task->return_stack_base - task->return_stack_current;
  af_cell_t stack_count = 4;
  memcpy(buffer, AF_WORD_NAME_DATA(task->current_word),
	 length * sizeof(af_byte_t));
  buffer[length] = 0;
  fprintf(stderr, "Entering: ");
  while(return_count--) {
    fprintf(stderr, "  ");
  }
  fprintf(stderr, "%s", buffer);
  while(stack_count && data_stack < task->data_stack_base) {
    fprintf(stderr, " %lld", *data_stack++);
    stack_count--;
  }
  if(!stack_count && data_stack != task->data_stack_base) {
    fprintf(stderr, " ...");
  }
  fprintf(stderr, "\n");
  free(buffer);
}

void af_print_current_return_stack(af_global_t* global, af_task_t* task) {
  /*  af_byte_t length = AF_WORD_NAME_LEN(task->current_word);
  af_byte_t* buffer = malloc(length * sizeof(af_byte_t));
  memcpy(buffer, AF_WORD_NAME_DATA(task->current_word),
	 length * sizeof(af_byte_t));
  buffer[length] = 0;
  printf("Current word: %s\n", buffer);
  free(buffer);
  af_compiled_t** return_stack = task->return_stack_current;
  while(return_stack < task->return_stack_base) {
    af_word_t* word = (*return_stack++)->compiled_call;
    length = AF_WORD_NAME_LEN(word);
    buffer = malloc(length * sizeof(af_byte_t));
    memcpy(buffer, AF_WORD_NAME_DATA(word), length * sizeof(af_byte_t));
    buffer[length] = 0;
    printf("Next word: %s\n", buffer);
    free(buffer);
    }*/
}

af_task_t* af_spawn(af_global_t* global, af_task_t* parent_task) {
  af_cell_t* data_stack_top;
  af_float_t* float_stack_top;
  af_compiled_t** return_stack_top;
  af_cell_t* data_space_base;
  af_wordlist_t** wordlist_order;
  void* task_local_space_base;
  af_task_t* task;
  
  if(!(data_stack_top = malloc(global->default_data_stack_count *
			       sizeof(af_cell_t)))) {
    return NULL;
  }
  if(!(float_stack_top = malloc(global->default_float_stack_count *
				sizeof(af_float_t)))) {
    free(data_stack_top);
    return NULL;
  }
  if(!(return_stack_top = malloc(global->default_return_stack_count *
				 sizeof(af_compiled_t*)))) {
    free(float_stack_top);
    free(data_stack_top);
    return NULL;
  }
  if(!(data_space_base = malloc(global->default_data_space_size *
				sizeof(af_cell_t)))) {
    free(return_stack_top);
    free(float_stack_top);
    free(data_stack_top);
    return NULL;
  }
  if(!(wordlist_order = malloc(global->default_wordlist_order_max_count *
			       sizeof(af_wordlist_t*)))) {
    free(data_space_base);
    free(return_stack_top);
    free(float_stack_top);
    free(data_stack_top);
    return NULL;
  }
  if(!(task_local_space_base = malloc(global->task_local_space_size *
					sizeof(af_byte_t)))) {
    free(wordlist_order);
    free(data_space_base);
    free(return_stack_top);
    free(float_stack_top);
    free(data_stack_top);
    return NULL;
  }
  if(!(task = malloc(sizeof(af_task_t)))) {
    free(task_local_space_base);
    free(wordlist_order);
    free(data_space_base);
    free(return_stack_top);
    free(float_stack_top);
    free(data_stack_top);
    return NULL;
  }
  memcpy(task_local_space_base, global->default_task_local_space_base,
	 global->task_local_space_size * sizeof(af_byte_t));
  task->next_task = global->first_task;
  global->first_task = task;
  task->base_cycles_before_yield = 0;
  task->extra_cycles = 0;
  task->current_cycles_before_yield = 0;
  task->current_cycles_left = 0;
  task->yields = 0;
  task->is_compiling = FALSE;
  task->is_to_be_freed = FALSE;
  task->interpreter_pointer = NULL;
  task->data_stack_current = task->data_stack_base =
    (data_stack_top + global->default_data_stack_count) -
    global->default_data_stack_base_room_count;
  task->float_stack_current = task->float_stack_base =
    (float_stack_top + global->default_float_stack_count) -
    global->default_float_stack_base_room_count;
  task->return_stack_current = task->return_stack_base =
    (return_stack_top + global->default_return_stack_count) -
    global->default_return_stack_base_room_count;
  task->data_stack_top = data_stack_top;
  task->float_stack_top = float_stack_top;
  task->return_stack_top = return_stack_top;
  task->data_space_current = task->data_space_base =
    data_space_base;
  task->data_space_top =
    data_space_base + global->default_data_space_size;
  task->task_local_space_base = task_local_space_base;
  task->wordlist_order_max_count = global->default_wordlist_order_max_count;
  task->wordlist_order = wordlist_order;
  if(parent_task) {
    memcpy(task->wordlist_order, parent_task->wordlist_order,
	  parent_task->wordlist_order_count);
    task->wordlist_order_count = parent_task->wordlist_order_count;
    task->current_wordlist = parent_task->current_wordlist;
  } else {
    *task->wordlist_order = global->forth_wordlist;
    task->wordlist_order_count = 1;
    task->current_wordlist = global->forth_wordlist;
  }
  task->most_recent_word = NULL;
  task->current_input = NULL;
  task->base = 10;
  task->init_word = NULL;
  task->current_word = NULL;
  task->abort = global->default_abort;
  task->terminate = NULL;
  task->free_data_on_exit = FALSE;
  task->do_trace = FALSE;
  task->nesting_level = -1;
  global->task_count++;
  af_handle_spawn(global, task);
  return task;
}

af_task_t* af_spawn_no_data(af_global_t* global, af_task_t* parent_task) {
  af_cell_t* data_stack_top;
  af_float_t* float_stack_top;
  af_compiled_t** return_stack_top;
  af_cell_t* data_space_base;
  af_wordlist_t** wordlist_order;
  void* task_local_space_base;
  af_task_t* task;
  
  if(!(data_stack_top = malloc(global->default_data_stack_count *
			       sizeof(af_cell_t)))) {
    return NULL;
  }
  if(!(float_stack_top = malloc(global->default_float_stack_count *
				sizeof(af_float_t)))) {
    free(data_stack_top);
    return NULL;
  }
  if(!(return_stack_top = malloc(global->default_return_stack_count *
				 sizeof(af_compiled_t*)))) {
    free(float_stack_top);
    free(data_stack_top);
    return NULL;
  }
  if(!(wordlist_order = malloc(global->default_wordlist_order_max_count *
			       sizeof(af_wordlist_t*)))) {
    free(return_stack_top);
    free(float_stack_top);
    free(data_stack_top);
    return NULL;
  }
  if(!(task_local_space_base = malloc(global->task_local_space_size *
					sizeof(af_byte_t)))) {
    free(wordlist_order);
    free(return_stack_top);
    free(float_stack_top);
    free(data_stack_top);
    return NULL;
  }
  if(!(task = malloc(sizeof(af_task_t)))) {
    free(task_local_space_base);
    free(wordlist_order);
    free(return_stack_top);
    free(float_stack_top);
    free(data_stack_top);
    return NULL;
  }
  memcpy(task_local_space_base, global->default_task_local_space_base,
	 global->task_local_space_size * sizeof(af_byte_t));
  task->next_task = global->first_task;
  global->first_task = task;
  task->base_cycles_before_yield = 0;
  task->extra_cycles = 0;
  task->current_cycles_before_yield = 0;
  task->current_cycles_left = 0;
  task->yields = 0;
  task->is_compiling = FALSE;
  task->is_to_be_freed = FALSE;
  task->interpreter_pointer = NULL;
  task->data_stack_current = task->data_stack_base =
    (data_stack_top + global->default_data_stack_count) -
    global->default_data_stack_base_room_count;
  task->float_stack_current = task->float_stack_base =
    (float_stack_top + global->default_float_stack_count) -
    global->default_float_stack_base_room_count;
  task->return_stack_current = task->return_stack_base =
    (return_stack_top + global->default_return_stack_count) -
    global->default_return_stack_base_room_count;
  task->data_stack_top = data_stack_top;
  task->float_stack_top = float_stack_top;
  task->return_stack_top = return_stack_top;
  task->data_space_base = NULL;
  task->data_space_current = NULL;
  task->data_space_top = NULL;
  task->task_local_space_base = task_local_space_base;
  task->wordlist_order_max_count = global->default_wordlist_order_max_count;
  task->wordlist_order = wordlist_order;
  if(parent_task) {
    memcpy(task->wordlist_order, parent_task->wordlist_order,
	  parent_task->wordlist_order_count);
    task->wordlist_order_count = parent_task->wordlist_order_count;
    task->current_wordlist = parent_task->current_wordlist;
  } else {
    *task->wordlist_order = global->forth_wordlist;
    task->wordlist_order_count = 1;
    task->current_wordlist = global->forth_wordlist;
  }
  task->most_recent_word = NULL;
  task->current_input = NULL;
  task->base = 10;
  task->init_word = NULL;
  task->current_word = NULL;
  task->abort = global->default_abort;
  task->terminate = NULL;
  task->free_data_on_exit = FALSE;
  task->do_trace = FALSE;
  task->nesting_level = -1;
  global->task_count++;
  af_handle_spawn(global, task);
  return task;
}

void af_set_init_word(af_global_t* global, af_task_t* task,
		      af_word_t* word) {
  if(!task->base_cycles_before_yield && !task->is_to_be_freed) {
    task->init_word = word;
  }
}


void af_push_data(af_global_t* global, af_task_t* task, af_cell_t value) {
  AF_VERIFY_DATA_STACK_EXPAND(global, task, 1);
  *(--task->data_stack_current) = value;
}

void af_push_float(af_global_t* global, af_task_t* task, af_float_t value) {
  AF_VERIFY_FLOAT_STACK_EXPAND(global, task, 1);
  *(--task->float_stack_current) = value;
}

void af_push_return(af_global_t* global, af_task_t* task,
		    af_compiled_t* pointer) {
  AF_VERIFY_RETURN_STACK_EXPAND(global, task, 1);
  *(--task->return_stack_current) = pointer;
}

void af_start(af_global_t* global, af_task_t* task) {
  if(!task->base_cycles_before_yield && !task->is_to_be_freed) {
    task->base_cycles_before_yield = task->current_cycles_before_yield =
      task->current_cycles_left = global->default_cycles_before_yield;
    af_schedule(global, task);
  }
}

void af_terminate(af_global_t* global, af_task_t* task) {
  if(!task->is_to_be_freed) {
    if(task->terminate) {
      if(!task->current_cycles_before_yield) {
	task->current_cycles_before_yield =
	  global->default_cycles_before_yield;
	af_schedule(global, task);
      }
      AF_WORD_EXECUTE(global, task, task->terminate);
    } else {
      af_kill(global, task);
    }
  }
}

void af_kill(af_global_t* global, af_task_t* task) {
  task->current_cycles_before_yield = 0;
  task->current_cycles_left = 0;
  task->is_to_be_freed = TRUE;
  global->task_count--;
  af_deschedule(global, task);
  af_handle_kill(global, task);
}

void af_yield(af_global_t* global, af_task_t* task) {
  task->current_cycles_left = 0;
  task->extra_cycles = 0;
}

void af_wait(af_global_t* global, af_task_t* task) {
  task->current_cycles_before_yield = 0;
  task->extra_cycles += task->current_cycles_left;
  if(task->extra_cycles > global->max_extra_cycles) {
    task->extra_cycles = global->max_extra_cycles;
  }
  task->current_cycles_left = 0;
  af_deschedule(global, task);
}

void af_end_atomic_and_wait(af_global_t* global, af_task_t* task) {
  af_end_atomic(global, task);
  af_wait(global, task);
}

void af_wake(af_global_t* global, af_task_t* task) {
  if(!task->current_cycles_before_yield && !task->is_to_be_freed) {
    task->current_cycles_before_yield = task->base_cycles_before_yield;
    af_schedule(global, task);
  }
}

void af_begin_atomic(af_global_t* global, af_task_t* task) {
  if(!global->atomic_task) {
    task->nesting_level = -1;
    global->atomic_task = task;
    if(global->current_task != global->atomic_task) {
      af_yield(global, global->current_task);
      af_wake(global, global->atomic_task);
    }
  } else if(task->nesting_level < 0 && task != global->atomic_task) {
    af_sign_cell_t level = af_get_nesting_level(global);
    level = level < 0 ? 0 : level + 1;
    task->nesting_level = level;
  }
}

void af_end_atomic(af_global_t* global, af_task_t* task) {
  af_task_t* next_atomic_task;
  if(task == global->atomic_task) {
    next_atomic_task = af_get_nested_task(global);
    if(next_atomic_task && global->current_task != next_atomic_task) {
      next_atomic_task->nesting_level = -1;
      global->atomic_task = next_atomic_task;
      af_yield(global, global->current_task);
      af_wake(global, global->atomic_task);
    } else {
      global->atomic_task = NULL;
    }
  }
}

void af_reset(af_global_t* global, af_task_t* task) {
  task->base_cycles_before_yield = global->default_cycles_before_yield;
  task->extra_cycles = 0;
  task->current_cycles_before_yield = global->default_cycles_before_yield;
  task->is_compiling = FALSE;
  task->interpreter_pointer = NULL;
  task->data_stack_current = task->data_stack_base;
  task->float_stack_current = task->float_stack_base;
  task->return_stack_current = task->return_stack_base;
  task->most_recent_word = NULL;
  task->base = 10;
  if(task->abort) {
    AF_WORD_EXECUTE(global, task, task->abort);
  } else {
    af_kill(global, task);
  }
}

void af_schedule(af_global_t* global, af_task_t* task) {
  global->tasks_active_count++;
}

void af_deschedule(af_global_t* global, af_task_t* task) {
  global->tasks_active_count--;
}

void af_interpret(af_global_t* global, af_task_t* task) {
  task->interpreter_pointer = global->base_interpreter_code;
}

void af_handle_spawn(af_global_t* global, af_task_t* task) {
  if(global->handler_task && !global->handler_task->is_to_be_freed) {
    af_begin_atomic(global, global->handler_task);
  }
}

void af_handle_kill(af_global_t* global, af_task_t* task) {
  if(global->handler_task && !global->handler_task->is_to_be_freed) {
    af_begin_atomic(global, global->handler_task);
  }
}

af_task_t* af_get_nested_task(af_global_t* global) {
  af_task_t* current_task = global->first_task;
  af_task_t* nested_task = NULL;
  af_sign_cell_t level = current_task->nesting_level;
  while(current_task) {
    if(!current_task->is_to_be_freed) {
      if(((level > -1) && (current_task->nesting_level < level)) ||
	 ((level < 0) && (current_task->nesting_level > -1))) {
	nested_task = current_task;
	level = current_task->nesting_level;
      }
    }
    current_task = current_task->next_task;
  }
  return nested_task;
}

af_sign_cell_t af_get_nesting_level(af_global_t* global) {
  af_task_t* current_task = global->first_task;
  af_sign_cell_t level = current_task->nesting_level;
  while(current_task) {
    if(!current_task->is_to_be_freed && level < current_task->nesting_level ) {
      level = current_task->nesting_level;
    }
    current_task = current_task->next_task;
  }
  return level;
}

void af_handle_data_stack_overflow(af_global_t* global, af_task_t* task) {
  af_print_current_return_stack(global, task);
  printf("Data stack overflow\n");
  af_reset(global, task);
}

void af_handle_float_stack_overflow(af_global_t* global, af_task_t* task) {
  af_print_current_return_stack(global, task);
  printf("Float stack overflow\n");
  af_reset(global, task);
}

void af_handle_return_stack_overflow(af_global_t* global, af_task_t* task) {
  af_print_current_return_stack(global, task);
  printf("Return stack overflow\n");
  af_reset(global, task);
}

void af_handle_data_stack_underflow(af_global_t* global, af_task_t* task) {
  af_print_current_return_stack(global, task);
  printf("Data stack underflow\n");
  af_reset(global, task);
}

void af_handle_float_stack_underflow(af_global_t* global, af_task_t* task) {
  af_print_current_return_stack(global, task);
  printf("Float stack underflow\n");
  af_reset(global, task);
}

void af_handle_return_stack_underflow(af_global_t* global, af_task_t* task) {
  af_print_current_return_stack(global, task);
  printf("Return stack underflow\n");
  af_reset(global, task);
}

void af_handle_word_expected(af_global_t* global, af_task_t* task) {
  printf("Word expected\n");
  af_reset(global, task);
}

void af_handle_data_space_overflow(af_global_t* global, af_task_t* task) {
  af_print_current_return_stack(global, task);
  printf("Data space overflow\n");
  af_reset(global, task);
}

void af_handle_parse_error(af_global_t* global, af_task_t* task) {
  printf("Parse error\n");
  af_reset(global, task);
}

void af_handle_word_not_found(af_global_t* global, af_task_t* task) {
  printf("Word not found\n");
  af_reset(global, task);
}

void af_handle_out_of_memory(af_global_t* global, af_task_t* task) {
  printf("Out of memory\n");
  af_kill(global, task);
}

void af_handle_divide_by_zero(af_global_t* global, af_task_t* task) {
  printf("Divide by zero\n");
  af_reset(global, task);
}

void af_handle_compile_only(af_global_t* global, af_task_t* task) {
  printf("Compile only\n");
  af_reset(global, task);
}

void af_handle_interpret_only(af_global_t* global, af_task_t* task) {
  printf("Interpret only\n");
  af_reset(global, task);
}

void af_handle_no_word_created(af_global_t* global, af_task_t* task) {
  printf("No word created\n");
  af_reset(global, task);
}

void af_handle_not_interactive(af_global_t* global, af_task_t* task) {
  printf("Must not be interactive\n");
  af_reset(global, task);
}

void af_handle_wordlist_too_large(af_global_t* global, af_task_t* task) {
  printf("Wordlist too large\n");
  af_reset(global, task);
}

void* af_guarantee(af_global_t* global, af_task_t* task, size_t size) {
  size_t size_remaining = task->data_space_top - task->data_space_current;
  if((size <= size_remaining) &&
     (global->min_guaranteed_data_space_size <= size_remaining ||
      task->free_data_on_exit)) {
    return task->data_space_current;
  } else if(!task->free_data_on_exit) {
    void* new_data_space_base = malloc(global->default_data_space_size);
    task->data_space_base = new_data_space_base;
    task->data_space_current = new_data_space_base;
    task->data_space_top =
      new_data_space_base + global->default_data_space_size;
    return new_data_space_base;
  } else {
    af_handle_data_space_overflow(global, task);
    return NULL;
  }
}

void* af_allocate(af_global_t* global, af_task_t* task, size_t size) {
  void* base = af_guarantee(global, task, size);
  if(base) {
    task->data_space_current += size;
  }
  return base;
}

void* af_allot(af_global_t* global, af_task_t* task, ssize_t size) {
  ssize_t size_remaining = task->data_space_top - task->data_space_current;
  if(size_remaining >= size) {
    void* current = task->data_space_current;
    task->data_space_current += size;
    return current;
  } else {
    af_handle_data_space_overflow(global, task);
    return NULL;
  }
}

af_word_t* af_search_wordlist(af_wordlist_t* wordlist, af_byte_t* name,
			      af_cell_t name_length) {
  af_word_t* current_word = wordlist->first_word;
  while(current_word) {
    af_byte_t current_word_length = AF_WORD_NAME_LEN(current_word);
    if(name_length == current_word_length &&
       !(current_word->flags & AF_WORD_HIDDEN)) {
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

af_word_t* af_lookup(af_global_t* global, af_task_t* task, af_byte_t* name,
		     af_cell_t name_length) {
  af_wordlist_t** wordlist = task->wordlist_order;
  af_cell_t wordlist_count = task->wordlist_order_count;
  while(wordlist_count--) {
    af_word_t* word = af_search_wordlist(*wordlist++, name, name_length);
    if(word) {
      return word;
    }
  }
  return NULL;
}

af_bool_t af_parse_number(af_global_t* global, af_cell_t base,  af_byte_t* text,
			  size_t length, af_sign_cell_t* result) {
  af_sign_cell_t value = 0;
  af_bool_t success = TRUE;
  af_bool_t negative = FALSE;
  if(length > 0) {
    if(*text == '%') {
      base = 2;
      text++;
      length--;
    } else if(*text == '/') {
      base = 8;
      text++;
      length--;
    } else if(*text == '#') {
      base = 10;
      text++;
      length--;
    } else if(*text == '$') {
      base = 16;
      text++;
      length--;
    }
    if(length > 0 && base >= 2 && base <= 36) {
      if(*text == '-') {
	negative = TRUE;
	text++;
	length--;
      }
      if(length > 0) {
	while(length--) {
	  value *= base;
	  if(*text >= '0' && *text <= '9') {
	    if(*text - '0' < base) {
	      value += *text++ - '0';
	    } else {
	      value = 0;
	      success = FALSE;
	      break;
	    }
	  } else if(*text >= 'a' && *text <= 'z') {
	    if(*text - 'a' < base - 10) {
	      value += (*text++ - 'a') + 10;
	    } else {
	      value = 0;
	      success = FALSE;
	      break;
	    }
	  } else if(*text >= 'A' && *text <= 'Z') {
	    if(*text - 'A' < base - 10) {
	      value += (*text++ - 'A') + 10;
	    } else {
	      value = 0;
	      success = FALSE;
	      break;
	    }
	  } else {
	    value = 0;
	    success = FALSE;
	    break;
	  }
	}
	if(success && negative) {
	  value = -value;
	}
      } else {
	value = 0;
	success = FALSE;
      }
    } else {
      value = 0;
      success = FALSE;
    }
  } else {
    value = 0;
    success = FALSE;
  }
  *result = value;
  return success;
}

af_bool_t af_parse_float(af_global_t* global, af_byte_t* text,
			 size_t length, af_float_t* result) {
  char* buffer;
  char* end_ptr;
  af_float_t value;
  af_bool_t found = FALSE;
  for(af_cell_t i = 0; i < length; i++) {
    if(text[i] == 'e' || text[i] == 'E') {
      found = TRUE;
      if(i == length - 1) {
	length--;
      }
      break;
    }
  }
  if(!found) {
    return FALSE;
  }
  if(!(buffer = malloc(sizeof(af_byte_t) * (length + 1)))) {
    return FALSE;
  }
  memcpy(buffer, (void*)text, length);
  buffer[length] = '\0';
  
  value = strtod(buffer, &end_ptr);
  if(*end_ptr == 0) {
    free(buffer);
    *result = value;
    return TRUE;
  } else {
    free(buffer);
    return FALSE;
  }
}

af_bool_t af_parse_2number(af_global_t* global, af_cell_t base,
			   af_byte_t* text, size_t length,
			   af_sign_2cell_t* result) {
  af_sign_2cell_t value = 0;
  af_bool_t success = FALSE;
  af_bool_t negative = FALSE;
  if(length > 0) {
    if(*text == '%') {
      base = 2;
      text++;
      length--;
    } else if(*text == '/') {
      base = 8;
      text++;
      length--;
    } else if(*text == '#') {
      base = 10;
      text++;
      length--;
    } else if(*text == '$') {
      base = 16;
      text++;
      length--;
    }
    if(length > 0 && base >= 2 && base <= 36) {
      if(*text == '-') {
	negative = TRUE;
	text++;
	length--;
      }
      if(length > 0) {
	while(length--) {
	  if(*text == '.' && length == 0) {
	    success = TRUE;
	    break;
	  }
	  value *= base;
	  if(*text >= '0' && *text <= '9') {
	    if(*text - '0' < base) {
	      value += *text++ - '0';
	    } else {
	      value = 0;
	      success = FALSE;
	      break;
	    }
	  } else if(*text >= 'a' && *text <= 'z') {
	    if(*text - 'a' < base - 10) {
	      value += (*text++ - 'a') + 10;
	    } else {
	      value = 0;
	      success = FALSE;
	      break;
	    }
	  } else if(*text >= 'A' && *text <= 'Z') {
	    if(*text - 'A' < base - 10) {
	      value += (*text++ - 'A') + 10;
	    } else {
	      value = 0;
	      success = FALSE;
	      break;
	    }
	  } else {
	    value = 0;
	    success = FALSE;
	    break;
	  }
	}
	if(success && negative) {
	  value = -value;
	}
      } else {
	value = 0;
	success = FALSE;
      }
    } else {
      value = 0;
      success = FALSE;
    }
  } else {
    value = 0;
    success = FALSE;
  }
  *result = value;
  return success;
}

af_bool_t af_word_available(af_global_t* global, af_task_t* task,
			    af_byte_t delimiter) {
  af_cell_t current_index;
  af_cell_t count;
  af_byte_t* buffer;
  af_byte_t current_char;
  if(!task->current_input) {
    return FALSE;
  }
  current_index = task->current_input->index;
  count = task->current_input->count;
  buffer = task->current_input->buffer;
  current_char = *(buffer + current_index);
  while(current_index < count && current_char == delimiter) {
    current_char = *(buffer + ++current_index);
  }
  if(current_index == count) {
    return FALSE;
  }
  if(!task->current_input->is_closed) {
    while(current_index < count && current_char != delimiter) {
      current_char = *(buffer + ++current_index);
    }
    return current_index < count;
  } else {
    return TRUE;
  }
}

af_byte_t* af_word(af_global_t* global, af_task_t* task, af_byte_t delimiter) {
  af_cell_t current_index;
  af_cell_t count;
  af_byte_t* buffer;
  af_byte_t current_char;
  af_cell_t start_index;
  size_t bytes_copied;
  af_byte_t* here;
  if(!task->current_input) {
    here = (af_byte_t*)af_guarantee(global, task, 1);
    *here = 0;
    return here;
  }
  current_index = task->current_input->index;
  count = task->current_input->count;
  buffer = task->current_input->buffer;
  current_char = *(buffer + current_index);
  while(current_index < count && current_char == delimiter) {
    current_char = *(buffer + ++current_index);
  }
  if(current_index == count) {
    here = (af_byte_t*)af_guarantee(global, task, 1);
    *here = 0;
    return here;
  }
  start_index = current_index;
  while(current_index < count && current_char != delimiter) {
    current_char = *(buffer + ++current_index);
  }
  if(!task->current_input->is_closed && current_index == count) {
    here = (af_byte_t*)af_guarantee(global, task, 1);
    *here = 0;
    return here;
  }
  task->current_input->index = current_index;
  if(current_index - start_index <= 255) {
    bytes_copied = current_index - start_index;
  } else {
    bytes_copied = 255;
  }
  here = (af_byte_t*)af_guarantee(global, task, bytes_copied + 1);
  *here = (af_byte_t)bytes_copied;
  memcpy(here + sizeof(af_byte_t),
	 buffer + (start_index * sizeof(af_byte_t)),
	 sizeof(af_byte_t) * bytes_copied);
  return here;
}

af_bool_t af_parse_name_available(af_global_t* global, af_task_t* task) {
  af_cell_t current_index;
  af_cell_t count;
  af_byte_t* buffer;
  af_byte_t current_char;
  if(!task->current_input) {
    return FALSE;
  }
  current_index = task->current_input->index;
  count = task->current_input->count;
  buffer = task->current_input->buffer;
  current_char = *(buffer + current_index);
  while(current_index < count &&
	(current_char == ' ' || current_char == '\t' || current_char == '\n')) {
    current_char = *(buffer + ++current_index);
  }
  if(current_index == count) {
    return FALSE;
  }
  if(!task->current_input->is_closed) {
    while(current_index < count &&
	  (current_char != ' ' && current_char != '\t' &&
	   current_char != '\n')) {
      current_char = *(buffer + ++current_index);
    }
    return current_index < count;
  } else {
    return TRUE;
  }
}

af_byte_t* af_parse_name(af_global_t* global, af_task_t* task,
			 af_cell_t* length) {
  af_cell_t current_index;
  af_cell_t count;
  af_byte_t* buffer;
  af_byte_t current_char;
  af_cell_t start_index;
  size_t bytes_copied;
  if(!task->current_input) {
    *length = 0;
    return NULL;
  }
  current_index = task->current_input->index;
  count = task->current_input->count;
  buffer = task->current_input->buffer;
  current_char = *(buffer + current_index);
  while(current_index < count &&
	(current_char == ' ' || current_char == '\t' || current_char == '\n')) {
    current_char = *(buffer + ++current_index);
  }
  if(current_index == count) {
    *length = 0;
    return NULL;
  }
  start_index = current_index;
  while(current_index < count &&
	(current_char != ' ' && current_char != '\t' && current_char != '\n')) {
    current_char = *(buffer + ++current_index);
  }
  if(!task->current_input->is_closed && current_index == count) {
    *length = 0;
    return NULL;
  }
  task->current_input->index = current_index;
  *length = current_index - start_index;
  return buffer + start_index;
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
  input->cleanup = NULL;
  input->refill = NULL;
  input->source_id = -1;
  input->arg = 0;
  return input;
}

af_word_t* af_register_prim(af_global_t* global, af_task_t* task,
			    af_byte_t* name, af_prim_t prim,
			    af_cell_t flags, af_wordlist_t* wordlist) {
  void* word_space;
  af_word_t* word;
  af_cell_t name_length;
  size_t data_space_current = (size_t)task->data_space_current;
  size_t name_size;
  size_t pad_size;
  if(name) {
    name_length = strlen(name);
  } else {
    name_length = 0;
  }
  name_size = (name_length + 1) * sizeof(af_byte_t);
  pad_size = ((data_space_current + name_size) % sizeof(af_cell_t)) == 0 ?
    0 * sizeof(af_byte_t) :
    sizeof(af_cell_t) - ((data_space_current + name_size) % sizeof(af_cell_t));
  word_space = af_allocate(global, task,
			   sizeof(af_word_t) + name_size + pad_size);
  word = word_space + name_size + pad_size;
  word->flags = flags;
  AF_WORD_NAME_LEN(word) = (af_byte_t)name_length;
  if(name) {
    memcpy(AF_WORD_NAME_DATA(word), name, name_length);
    word->next_word = wordlist->first_word;
    wordlist->first_word = word;
  } else {
    word->next_word = NULL;
  }
  word->next_of_all_words = global->first_of_all_words;
  global->first_of_all_words = word;
  task->most_recent_word = word;
  word->code = prim;
  word->secondary = NULL;
  return word;
}
