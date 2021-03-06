#pragma once

//CPU Features: Use cpuid code 0x1
enum{
        CPUID_EDX_FPU   =       1 << 0,
        CPUID_EDX_VME   =       1 << 1,
        CPUID_EDX_DE    =       1 << 2,
        CPUID_EDX_PSE   =       1 << 3,
        CPUID_EDX_TSC   =       1 << 4,
        CPUID_EDX_MSR   =       1 << 5,
        CPUID_EDX_PAE   =       1 << 6,
        CPUID_EDX_MCE   =       1 << 7,
        CPUID_EDX_CX8   =       1 << 8,
        CPUID_EDX_APIC  =       1 << 9,
        CPUID_EDX_SEP   =       1 << 11,
        CPUID_EDX_MTRR  =       1 << 12,
        CPUID_EDX_PGE   =       1 << 13,
        CPUID_EDX_MCA   =       1 << 14,
        CPUID_EDX_CMOV  =       1 << 15,
        CPUID_EDX_PAT   =       1 << 16,
        CPUID_EDX_PSE36 =       1 << 17,
        CPUID_EDX_PSN   =       1 << 18,
        CPUID_EDX_CLF   =       1 << 19,
        CPUID_EDX_DTES  =       1 << 21,
        CPUID_EDX_ACPI  =       1 << 22,
        CPUID_EDX_MMX   =       1 << 23,
        CPUID_EDX_FXSR  =       1 << 24,
        CPUID_EDX_SSE   =       1 << 25,
        CPUID_EDX_SSE2  =       1 << 26,
        CPUID_EDX_SS    =       1 << 27,
        CPUID_EDX_HTT   =       1 << 28,
        CPUID_EDX_TM1   =       1 << 29,
        CPUID_EDX_IA64  =       1 << 30,
        CPUID_EDX_PBE   =       1 << 31,
};

typedef struct{
   int eax;
   int ebx;
   int ecx;
   int edx;
}cpuid_t;

static inline void cpuid(int code, cpuid_t *cpu){
   asm volatile("cpuid" : "=a"(cpu->eax), "=b"(cpu->ebx),
           "=c"(cpu->ecx), "=d" (cpu->edx) : "a"(code) :);
}

int cpuid_avail();

