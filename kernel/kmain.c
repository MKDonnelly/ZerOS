#include <portio.h>
#include <timing.h>
#include <serial/serial.h>
#include <cmos.h>
#include <pic.h>
#include <vga3h/vga3h.h>
#include <vga13h/vga13h.h>
#include <keyboard.h>

#include <string.h>
#include <bcd.h>
#include <types.h>
#include <bitwise.h>
#include <delay.h>
#include <portio.h>

#include <cpu.h>
#include <isr.h>
#include <idt.h>
#include <gdt.h>
#include <paging.h>
#include <kmalloc.h>
#include <multiboot.h>

#include <modeset.h>
#include <vgacommon.h>
#include <vgafont.h>

#include <fs.h>
#include <initrd.h>

void kmain(struct multiboot_info *h){

  //set_vga_mode( vga_3h_mode );
  //write_font( g_8x16_font, 16 );
  init_vga(0);

  init_gdt();
  remap_pic();
  init_interrupts();
  init_timer(1, 0, 0);
  init_keyboard();
  //kbd_enc_send_command( KE_ALL_MAKEBREAK_C );
  enable_ints();

  k_clear_screen();
  k_newline();
  k_newline();
  k_printf("Enter some text: ");
  enable_ints();

  kb_set_leds( 1, 1, 1);
  init_heap();


  k_newline();
  k_printf("Kernel command line: ");
  k_printf( h->cmdline );
  k_newline();

  fs_node_t *initrd = init_initrd( (uint32_t*)(h->mods->start) );
  fs_node_t *file = initrd->finddir( initrd, "first");

  k_printf("Contents of \"first\": ");
  if( file ){
     char buf[20];
     read_fs( file, 0, 20, buf );
     k_printf( buf );
  } else {
     k_printf("File not found");
  }

  k_newline();
  k_printf("Writing to \"first\"");

  char testStr[] = "Wrote to this";
  write_fs(file, 0, 20, testStr);

  char b[20];
  read_fs( file, 0, 20, b );
  k_newline();
  k_printf("Content of \"first\" is now: ");
  k_printf( b );

  kb_set_leds( 1, 1, 1);
  init_paging();

  while(1);
}
