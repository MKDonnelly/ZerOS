#pragma once

#include <arch/current_arch>
#include <lib/types.h>
#include <lib/genericll.h>
#include <fs/vfs/vnode.h>

enum TASK_STATE { TASK_RUNNING, TASK_READY, TASK_EXIT, TASK_BLOCKED };

//This is a kernel task. It is designed
//to be used by an abstract linked list
//in the scheduler code.
typedef struct ktask{
   //ktask's are chained together into an
   //intrusive double linked list.
   list_head_t ktask_list;

   //Generic, arch-abstracted structure describing
   //the task's state.
   arch_task_t task_info;

   //Generic information related to every task
   size_t task_stack; //Address of the top of the stack
   void *task_brk;   //Pointer to end of heap
   enum TASK_STATE state;
   void *ret_val;
   int task_id;
   int is_kernel_task;
   char *args;
   char *pwd;

   //Open file nodes
   fs_node_t *open_fs[20];

   //Later, I might add a void* to point to a structure to
   //store scheduler-specific data.
} ktask_t;

//System call
int sys_getpid();

//When creating a task, we pass in the head of the stack.
//k_malloc will return a pointer to the beginning of the memory
//block, but the head of the stack is at the end of the memory block.
//This converts from a pointer from k_malloc to a stack head pointer
#define STACK_HEAD( ptr, stack_size ) (size_t)((char*)ptr + stack_size )

ktask_t *ktask_create(void (*start)(), void *param, void (*exit)());
ktask_t *utask_create(void (*start)(), void *param,
                        void (*exit)(), size_t stack_addr);

ktask_t *utask_from_elf(char *elf_data, int skip);
