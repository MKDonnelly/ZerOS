OUTPUT_FORMAT(elf32-i386)
ENTRY(main)

SECTIONS{
   /*Where to place the text*/
   . = 0x1000;
   .text : { *(.text) }
}
