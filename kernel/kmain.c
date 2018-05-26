#include <arch/current_arch>

#include <lib/keyboard.h>
#include <lib/string.h>
#include <lib/types.h>
#include <lib/bitwise.h>
#include <lib/timing.h>
#include <lib/generic_ll.h>
#include <lib/elf32.h>

#include <drivers/ata/ata_pio.h>

#include <kernel/multiboot.h>
#include <kernel/task.h>

#include <kernel/mm/heap_blocklist.h>
#include <kernel/mm/heap_bitmap.h>
#include <kernel/mm/heap.h>

#include <kernel/sched/sched.h>
#include <kernel/sched/round_robin.h>
#include <kernel/syscall.h>
#include <kernel/sched/workqueue.h>

#include <fs/fs.h>
#include <fs/initrd/initrd.h>
#include <lib/assert.h>

void thread1(){
   int t1count = 0;
   while(t1count < 5){
      k_puts_at("1", t1count++, 0);
      for(int i = 0; i < 50000000; i++);
   }
}

void thread2(){
   int t2count = 0;
   while(t2count < 5){
      k_puts_at("2", t2count++, 1);
      for(int i = 0; i < 50000000; i++);
   }
}

//Defined in linker script
extern unsigned int ldscript_text_start;
extern unsigned int ldscript_text_end;
extern unsigned int ldscript_kernel_end;

void kmain(struct multiboot_info *multiboot_info){

   arch_system_init();
   arch_timer_init( timing_main_handler );
   arch_keyboard_init( keyboard_main_handler );
   k_puts("\\c"); //clear screen
   

   //Have the heap start just after the text segment on an aligned boundary
   uint32_t heap_start = ALIGN_ON((int)&ldscript_kernel_end + 0x2000, 
                                  ARCH_PAGE_SIZE);
   //Keep in mind that only the first 4M of memory at the start of the
   //kernel is mapped int. If the size is changed to be larger, this 
   //may cause a page fault.
   heap_create( &global_kernel_heap, heap_start, 0x200000, 
                &blocklist_heap);

   //Initilize vm subsystem. This requires the heap, so we cannot
   //have arch_system_init do this.
   vm_init();
   
   current_scheduler = &rr_scheduler;
   current_scheduler->scheduler_setup();
   syscalls_init();

   workqueue_t *kwq = workqueue_create();
   tasklet_t *first = tasklet_create( thread1, NULL );
   tasklet_t *second = tasklet_create( thread2, NULL );

   workqueue_add(kwq, first);
   workqueue_add(kwq, second);
 
   workqueue_worker_spawn( kwq );
   workqueue_worker_spawn( kwq );


   current_scheduler->scheduler_start();

/*
   char *s1 = k_malloc(kernel_heap, 1024, 0x1000);
   char *s2 = k_malloc(kernel_heap, 1024, 0x1000);
   rr_add_task( k_create_ktask( thread1, NULL, rr_exit_task, STACK_HEAD(s1, 1024)));
   rr_add_task( k_create_ktask( thread2, NULL, rr_exit_task, STACK_HEAD(s2, 1024)));
*/

/*
//Read in first file from initrd (it will contain a test binary)
   char *program_buf = k_malloc( 5000, 0);
   //identity map the lower part to make it easier to grab the initrd.
   map_page_range( 0x0, 0x0, kernel_page_dir, PAGE_RW, PAGE_USR_ACCESS, 0x500000);
   fs_root = init_initrd( ((struct module*)multiboot_info->mods)->start );
   fs_node_t *first = fs_root->readdir( fs_root, 0 );
   first->read( first, 0, first->length, program_buf);

   ktask_t *new_task = k_create_utask_elf(program_buf);
   
   current_scheduler->scheduler_add_task(new_task);

   current_scheduler->scheduler_start();
*/
   while(1) arch_halt_cpu();
}

/*
int y_offset = 0;
int x_offset = 0;

void m( mouse_packet_t p ){
//   k_clear_screen();
   //As we go down the screen, y increases.
   //as we go up the screen, y decreases.
   //But the delta_y is exactly the opposite:
   //negative values go down and positive values
   //go up, so negate delta_y.
   y_offset += (-p.delta_y) / 2;
   x_offset += p.delta_x / 2;
   k_printf_at("*", x_offset, y_offset);
}*/

/*
uint16_t start_state = 0xACF1;
uint16_t lfsr = 0xACF1;
uint16_t bit;
int period = 0;

int random(){
   bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1;
   lfsr = (lfsr >> 1) | (bit << 15);
   ++period;
   return lfsr;
}*/


/*
  //Disable irqs on ata
  portb_write( ATA_PIO_CTRL_P, 0x02 );

  ////////////////////
  uint16_t *identify_data = identify_drive(0, 0);
  uint32_t total_sectors = ( identify_data[61] << 16 ) | identify_data[60];
  k_printf("Total sectors: %d\n", total_sectors);

  //test write 

  uint16_t *data = k_malloc( kernel_heap, sizeof(uint16_t)*256, 0);
  memset( data, sizeof(uint16_t)*256, 7);
  uint8_t sectors = 1;
  uint32_t lba = 0;

  ata_pio_write_sector( sectors, lba, data );

  uint16_t *buf = ata_pio_read_sector( 1, 0 );
  for(int i = 0; i < 10; i++)
     k_printf("%d\n", (uint8_t)buf[i]);
  ///////////////////
*/
