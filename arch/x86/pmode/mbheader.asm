;Load boot modules on a page boundary
PAGE_ALIGN    equ             1 << 0 

;Provide memory information to kernel
MEMORY_INFO   equ             1 << 1 

;Provide video info to kernel
;(QEMU can't)
;VIDEO_INFO    equ             1 << 2

MULTIBOOT_HEADER_MAGIC equ 0x1BADB002
MULTIBOOT_HEADER_FLAGS equ PAGE_ALIGN | MEMORY_INFO ;| VIDEO_INFO 
KERNEL_VIRT_ADDR equ 0xC0000000
KERNEL_PAGE equ (KERNEL_VIRT_ADDR >> 22)

;Early kernel stack length
EARLY_KERNEL_SLEN equ 3000

[bits 32]

section .boot
align 4
multiboot_header:
    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
;    dd 0, 0, 0, 0, 0 ;Zero out the bytes for AOUT_KLUDGE
;    dd 0 ; 0 = set graphics mode
;    dd 1024, 768, 16 ;Width, height, and depth for display

global arch_start

[extern kmain]
arch_start:

    ;Setup a temporary page directory so that 
    ;we can map and jump to the higher-half kernel.
    ;This is meant to be replaced by a different page 
    ;directory when the higher half starts to run.

    ;CR4.PSE = 1, CR4.PAE = 0, PDE.PS = 1

    ;Identity map first 4M using a single 4M page so that
    ;after we enable paging, this code can still run.
    
    ;Load in address of page directory. remember, we are linked
    ;to KERNEL_VIRT_ADDR and must adjust the memory reference.
    mov eax, page_directory_table
    mov cr3, eax
 
    ;Set CR4.PSE
    mov eax, cr4
    or eax, 0x10
    mov cr4, eax
   
    ;Enable paging
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    ;Have the kernel stack start 4K after the end
    ;of the kernel. Note that the heap will also start
    ;at this same address, so 1 byte is subtracted to 
    ;ensure that they do not overlap.
    mov ebp, early_kernel_stack
    add ebp, EARLY_KERNEL_SLEN
 
    ;Higher-half kernel is mapped to 0xC0000000.
    ;Since paging is enabled, it is alright to push
    ;to this address, since it will loop back to
    ;the real address of early_kernel_stack
    add ebp, KERNEL_VIRT_ADDR 
    ;align the stack on a 4 byte boundary
    and ebp, 0xFFFFFFFC
    mov esp, ebp

    ;Pass the pointer to the multiboot header
    ;GRUB automatically puts it into ebx
    push ebx 
    call kmain

    ;We should never get here, but in the case that kmain
    ;exits, at least we do not crash
stop:
    cli
    hlt
    jmp stop

section .boot
;We waste a lot of space here due to alignment.
; In an attempt to get it back, place the early
; kernel stack here instead of having it go 
; after the kernel image.
;This is a rough estimate: the code above is 
; somewhere 50-100 bytes. To be safe, only
; 3000 bytes is allocated after that for the early
; kernel stack. This could get bigger, if needed.

early_kernel_stack:
times EARLY_KERNEL_SLEN db 0

align 0x1000
;A fixed page table for the early boot environment. This identity
;maps the first 4M and maps 3G to the first 4M as well. This helps
;when transitioning to a higher-half kernel. As soon as kmain is 
;loaded, this page table is no longer used
global page_directory_table
page_directory_table:
   dd 0x00000083
   times (KERNEL_PAGE - 1) dd 0
   dd 0x00000083
   times (1024 - KERNEL_PAGE - 1) dd 0

section .initrd
incbin "./arch/x86/initrd"
