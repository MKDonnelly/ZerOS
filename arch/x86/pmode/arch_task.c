#include <arch/x86/pmode/arch_task.h>

#include <arch/x86/pmode/paging.h>
#include <arch/x86/pmode/loader.h>
#include <lib/assert.h>
#include <kernel/mm/heap.h>
#include <arch/x86/pmode/frame.h>

#include <kernel/sched/sched.h>
#include <kernel/task.h>

//Default userland stack address when loading through
//ELF image.
#define USERLAND_STACK 0xB0000000

arch_task_t arch_ktask_create(void (*start)(), void *param,
                                   void (*exit)(), size_t stack_addr){
   KASSERT( stack_addr != 0 );

   arch_task_t kernel_task;
   kernel_task.callstack.current_stack = 0;

   //Create the context
   kernel_task.callstack.stacks[0] = 
                arch_kcontext_create(start, param, exit, stack_addr);
   kernel_task.callstack.stacks[1] = NULL;
   
   //Kernel tasks will always inherit from kernel_page_dir
   kernel_task.task_pd = kernel_page_dir;

   //No interrupt stack needed for system calls since this is a
   //kernel-level stack
   kernel_task.interrupt_stack = NULL;

   return kernel_task;
}

//NOTE: This is used to create a userland task from a function in
//the kernel code. This is for testing only, since all userland
//programs will be loaded from and ELF file.
arch_task_t arch_utask_create(void (*start)(), void *param,
                                   void (*exit)(), size_t stack_addr){
   KASSERT( stack_addr != 0 );

   arch_task_t user_task;
   user_task.callstack.current_stack = 0;

   //Create context
   user_task.callstack.stacks[0] = arch_ucontext_create(start, param, 
                                                        exit, stack_addr);
   user_task.callstack.stacks[1] = NULL;

   //Userland tasks will clone the page directory of kernel_page_dir
   user_task.task_pd = vm_pdir_clone( kernel_page_dir );

   //Setup a system call stack of 1KB. Align on page boundary
   user_task.interrupt_stack = k_malloc( 1024, 0x1000);

   return user_task;
}


arch_task_t arch_utask_from_elf(char *elf_file_buffer, int skip ){

   arch_task_t user_task;

   //Userland task will inherit the pd from kernel_page_dir
   //Current page directory will be saved in current_page_dir
   user_task.task_pd = vm_pdir_clone( kernel_page_dir );
   pd_t *temp = current_page_dir;
   load_pd( (void*)VIRT_TO_PHYS(user_task.task_pd) );
   current_page_dir = user_task.task_pd;

   uint32_t start_addr = arch_create_from_elf((Elf32_Ehdr*)elf_file_buffer,
                                               user_task.task_pd, skip);
   
   //Map in a stack for the task
   uint32_t ustack = framepool_first_free();

   //Subtract the size of the page, since the stack head will be AT
   //USERLAND_STACK.
   vm_pmap(USERLAND_STACK-ARCH_PAGE_SIZE, ustack, user_task.task_pd, 1, 1 );

   //Create context. In this case, we do not need to subtract 
   //ARCH_PAGE_SIZE since arch_create_ucontext assumes that is is
   //passed the head of the stack.
   user_task.callstack.stacks[0] = arch_ucontext_create((void*)start_addr, 
                                             NULL, NULL, USERLAND_STACK );
   user_task.callstack.stacks[1] = NULL;

   //Setup a syscall stack of 4K aligned on page boundary
   user_task.interrupt_stack = k_malloc( 1024, 0x1000 );

   //Restore previous page directory from current_page_dir
   current_page_dir = temp;
   load_pd( (void*)VIRT_TO_PHYS(current_page_dir) );

   return user_task;
}
