#pragma once

#include <arch/x86/pmode/isr.h>
#include <arch/x86/frame.h>
#include <arch/x86/pmode/pagingasm.h>
#include <arch/x86/cpu.h>

#include <kernel/mm/heap.h>

#include <lib/memory.h>
#include <lib/types.h>

#define ARCH_PAGE_SIZE  0x1000
#define TABLE_SIZE 0x1000

#define PAGE_INTERRUPT 14

//Masks for page error
#define PAGE_PRESENT_M 0x1
#define PAGE_RW_M      0x2
#define PAGE_USR_M     0x4
#define PAGE_RES_M     0x8
#define PAGE_ID_M      0x10

//PD = Page directory
//PDE = Page directory entry
//PT = Page Table
//PTE = Page Table Entry
#define PDE_IN_PD 1024
#define PTE_IN_PT 1024

#define KERNEL_VBASE 0xC0000000

//The upper 10 bits of the address specifies the 
//offset within the page directory.
#define PD_INDEX(vaddr) (vaddr >> 22)

//After the first 10 bits, the next 10 bits specifies
//the offset within the page table.
#define PT_INDEX(vaddr) ((vaddr >> 12) & 0x3ff)

//Used to convert Virtual <=> Physical addresses. This may
//be insufficient later. I assume that all of these will be called
//with kernel addresses
#define PHYS_TO_VIRT(paddr) ( (uint32_t)paddr + KERNEL_VBASE)
#define VIRT_TO_PHYS(vaddr) ( (uint32_t)vaddr - KERNEL_VBASE)

//This represents an individual page<->frame allocation.
struct pte{
   uint8_t present          : 1; //Present in memory
   uint8_t rw               : 1; //Read-write if set
   uint8_t user             : 1; //Ring 0 if clear
   uint8_t write_through    : 1; //
   uint8_t cache_disable    : 1; //Has the page been written to?
   uint8_t accessed         : 1; //Has page been accessed?
   uint8_t dirty            : 1; 
   uint8_t zero_not_used    : 1;
   uint8_t global_page      : 1;
   uint8_t available        : 3; 

   //Since a page MUST be aligned to 4K, only the upper 20 bits
   //matter. This is the frame address >> 12 
   uint32_t frame_addr      : 20; 
} __attribute__((packed));
typedef struct pte pte_t;

//A page table is composed of page table entires.
//Each of these page entries maps a virtual to physical
//address using a page_table_entry_t.
struct pt{

   pte_t pt_entries[ PTE_IN_PT ];

}__attribute__((packed));
typedef struct pt pt_t;


struct pde{
   int8_t present          : 1;
   int8_t rw               : 1;
   int8_t user_access      : 1; //1 = user access, 0 = ring0 only
   int8_t write_through    : 1;
   int8_t cache_disabled   : 1;
   int8_t accessed         : 1;
   int8_t not_used         : 1;
   int8_t page_size        : 1;
   int8_t global_page      : 1;
   int8_t available        : 3;

   int32_t table_addr      : 20;

} __attribute__((packed));
typedef struct pde pde_t;


//A page directory is composed of page table descriptors.
struct pd{

   pde_t pd_entries[PDE_IN_PD];

   //pd_entries is used by the hardware paging translation mechanism.
   //it can only work with physical addresses. However, we as the OS
   //can only work with virtual addresses.

}__attribute__((packed));
typedef struct pd pd_t;

///////////////////////////////////////////////

//This points to the current page directory being used
//for address translation. It is null if not page table
//is in effect.
extern pd_t *kernel_page_dir;
extern pd_t *current_page_dir;

/////////////////Get information from paging structures
//Given a page directory and a virtual address, return the 
//pte corresponding to it. If create_pt is set, create any
//page tables needed. If create_pt is not set and the pt
//is not allocated, return null
//pte_t *get_page( uint32_t vaddr, bool create_pt, pd_t *page_directory); 
//////////////////////////////////////////////////

//Map a physical to virtual address in the paging structure
void map_page(uint32_t vaddr, uint32_t paddr, pd_t *page_directory);
void quick_map(uint32_t vaddr, uint32_t paddr, pd_t *page_directory);
///////////////////////////////////////

/////////////////Cloning page directory
pd_t *clone_pd(pd_t *page_directory);


//Given a buffer, length, and physical address, copy the
//buffer to the physical address.
void copy_to_physical(char *vbuf, uint32_t paddr, uint32_t len);

//Initilized the paging structure
void init_paging();

//Handles page interrupts
void page_int_handler(registers_t);