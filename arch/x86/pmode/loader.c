#include <arch/x86/pmode/loader.h>

#include <lib/string.h>
#include <arch/x86/pmode/frame.h>

//TODO move elf stuff into generic lib file

uint8_t elf_verify_magic(Elf32_Ehdr *elf_header){
   return strncmp( (char*)elf_header, ELF_MAGIC, ELF_MAGIC_LEN ) == 0;
}

int8_t elf_can_exec(Elf32_Ehdr *elf_header){
   if( elf_header->e_ident[EI_CLASS] != ELFCLASS32  || 
       elf_header->e_ident[EI_DATA]  != ELFDATA2LSB ||
       elf_header->e_machine         != MACHINE_X86 || 
       elf_header->e_version != 1                   ||
       (elf_header->e_type != ET_REL && elf_header->e_type != ET_EXEC) ){
      return 0;
   }
   return 1;
}

//Returns starting address of program.
uint32_t arch_create_from_elf(Elf32_Ehdr *elf_data, pd_t *task_pd, int skip){


   //Make sure we can execute the ELF file.
   if( !elf_verify_magic(elf_data) || !elf_can_exec(elf_data) ){
      k_printf("Cannot execute ELF!\n");
      return 0;
   }

   Elf32_Phdr *elf_pheader = (Elf32_Phdr*)( (char*)elf_data  
                                            + elf_data->e_phoff);

   for(int i = 0; i < elf_data->e_phnum; i++){
      elf_pheader = (Elf32_Phdr*)( (char*)elf_data + elf_data->e_phoff +
                                    i * elf_data->e_phentsize);

      if( elf_pheader->p_type == PT_LOAD ){
         uint32_t frame = framepool_first_free(); 
         vm_pmap( ALIGN_DOWN(elf_pheader->p_vaddr, 0x1000), 
                  frame, task_pd, 1, 1);

         vm_copy_to_physical( (char*)elf_data + elf_pheader->p_offset, 
                         frame + ((uint32_t)elf_pheader->p_vaddr & 0xfff),
                         elf_pheader->p_filesize);
      }
   }
   return elf_data->e_entry;
}
