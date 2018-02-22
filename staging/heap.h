#pragma once

#include <lib/types.h>
#include <arch/x86/archx86.h>

//#define kmalloc_test( heap_descriptor, size, alignment, phys ) heap_descriptor.malloc( &heap_descriptor, size, alignment, phys )

typedef struct heap{
   uint32_t start;
   uint32_t len;
   uint32_t size_left;

   //The functions to allocate on the heap will be
   //within the heap descriptor. This is to allow
   //many heap allocation algorithms to operate on
   //any heap. Specific algorithms decide what parameters
   //to accept.
   void *(*malloc)();
   void (*free)();
   void (*init_heap)();
   
}heap_t;

//Used by kernel and initilized in kmain
heap_t kernel_heap;

void create_heap(heap_t*,uint32_t,uint32_t,void *(*malloc)(), void (*free)(), void (*init)() );

