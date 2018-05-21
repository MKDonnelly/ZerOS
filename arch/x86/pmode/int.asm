; One and only one callback function
; This is the master interrupt handler;
; all interrupts must go through it to
; be routed to the correct function.
[extern main_interrupt_handler]

; This will be used to map interrupt
; numbers to interrupt handler when
; they are defined
[extern add_idt_entry]

extern gdt_kernel_data
extern gdt_kernel_code

;Stores the current context during an interrupt.
;The scheduler may request this using get_current_context
;and set this using set_current_context. This is the basic
;method for task switching.
current_context dq 0

global get_current_context
get_current_context:
   mov eax, [current_context]
   ret

global set_current_context
set_current_context:
   push eax

   mov eax, [esp+8]
   mov [current_context], eax

   pop eax
   ret


; This is common to all interrupts.
; The only thing that differs is 
; the error code and interrupt
; number pushed by each. By having
; each interrupt jump here, we cut
; down the size of the compiled file.

; Stack 
;     ;;;;; PUSHED BY CPU ;;;;;;;
;     32 bits EFLAGS
;     32 bits CS
;     32 bits EIP
;
;     ;;;;; PUSHED BY INTERRUPT_COMMON ;;;;;;;;
;     8 bits error code, int #
;     <pushad stuff>
;        32 bits e[a,c,d,b]x, edi, esi, esp, ebp

[extern arch_save_context]
[extern arch_set_context]
interrupt_common:
   pushad

   ;Save segment registers for userland interrupt
   push ds
   push es
   push fs
   push gs

   ;Load in kernel land segment registers
   mov ax, 0x10
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax

   ;Store the current context into "current_context"
   ;for use by the scheduler.
   mov [current_context], esp

   ;TODO make a call here to save the current context
   ;     into the arch_task_t structure (int arch_task.c)
   ;     then decouple the timer all in arch_task.c
   call arch_save_context

   call main_interrupt_handler 

   call arch_set_context

   ;Restore context after scheduler call
   mov esp, [current_context]

   ;Restore userland segment registers
   pop gs
   pop fs
   pop es
   pop ds

   ;Restore register state
   popad

   ;This is to move the stack back
   ;over the error code and interrupt number.
   ;While we only push two bytes worth of
   ;data, the stack is automatically aligned
   ;to 4-byte boundaries. As a result, we need
   ;to jump back 8 bytes. 
   add esp, 8
   iretd


; This defines an interrupt 
; handler that will be automatically
; called when the cpu triggers the isr. 
%macro DEFINE_INTERRUPT 1
global isr%1
isr%1:
   ;Push an error code if none is
   ;provided by the cpu to keep the
   ;stack constant through each interrupt.
   %if %1 < 8 || %1 == 9 || %1 > 14
      push byte 0
   %endif

   push byte %1
   jmp interrupt_common 
%endmacro


; Initilize every interrupt
; to point to the main 
; interrupt handler
%assign i 0
%rep 100 
   DEFINE_INTERRUPT i
%assign i i+1
%endrep

; This handy routine initilizes
; interrupt #x to point to 
; isr#x. Without this, we would
; have to go
;    add_idt_entry(#x, (u32)isr#x)
; 256 times in c!
global init_idt
init_idt:
   push ebp
   mov ebp, esp
   sub esp, 0x8

%assign j 0
%rep 100
   mov eax, isr %+ j
   sub esp, 0x8
   push eax
   push byte j
   call add_idt_entry
   add esp, 0x10
   %assign j j+1
%endrep

   leave
   ret

