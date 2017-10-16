asm("jmp kmain"); //The bootsector immedietelly jumps to the
                  //start of this file and begins executing. This is
		  //undesirable if there is a function above kmain.
		  //add this so that kmain is always called.

#include "../drivers/portio.h"
#include "../drivers/timing.h"
#include "../drivers/print.h"
#include "../cpu/cpu.h"
#include "../lib/string.h"
#include "../drivers/console.h"
#include "../cpu/types.h"

#include "../cpu/isr.h"
#include "../cpu/idt.h"


void kmain(){

  isr_install();

  __asm__ __volatile__("int $2");

  //struct vga_text_console sys_console;

  //initConsole( &sys_console );
  //sys_console.k_print("Testing...");
  
  stop_cpu();

}

