
#pragma once

#include "types.h"

extern void isr2();

typedef struct {
   u32 ds;
   u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
   u32 int_no, err_code;
   u32 eip, cs ,eflags, useresp, ss;
}registers_t;


void isr_install();
void isr_handler(registers_t r);
