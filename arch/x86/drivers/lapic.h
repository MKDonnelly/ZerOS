#pragma once

#define APIC_INT	32

#define APIC_BASE		0xFEE00000
#define APIC_ID			(APIC_BASE + 0x20)
#define APIC_VERSION		(APIC_BASE + 0x30)
#define APIC_TPR		(APIC_BASE + 0x80)
#define APIC_APR		(APIC_BASE + 0x90)
#define APIC_EOI		(APIC_BASE + 0xB0)
#define APIC_RRR		(APIC_BASE + 0xC0)
#define APIC_LDR		(APIC_BASE + 0xD0)
#define APIC_DFR		(APIC_BASE + 0XE0)
#define APIC_SPURIOUS		(APIC_BASE + 0xF0)
   #define ASPUR_ENABLE		(1<<8)
#define APIC_ISR_START		(APIC_BASE + 0x100)
#define APIC_ISR_END		(APIC_BASE + 0x170)
#define APIC_TMP_START		(APIC_BASE + 0x180)
#define APIC_TMR_END		(APIC_BASE + 0x1F0)
#define APIC_IRR_START		(APIC_BASE + 0x200)
#define APIC_IRR_END		(APIC_BASE + 0x270)
#define APIC_ESR		(APIC_BASE + 0x280)
#define APIC_ICR_LOW		(APIC_BASE + 0x300)
#define APIC_ICR_HIGH		(APIC_BASE + 0x310)
#define APIC_TIMER		(APIC_BASE + 0x320)
#define APIC_THERMAL		(APIC_BASE + 0x330)
#define APIC_PCOUNT		(APIC_BASE + 0x340)
#define APIC_INT0		(APIC_BASE + 0x350)
#define APIC_INT1		(APIC_BASE + 0x360)
#define APIC_ERROR		(APIC_BASE + 0x370)
#define APIC_TIMER_ICOUNT	(APIC_BASE + 0x380)
#define APIC_TIMER_CCOUNT	(APIC_BASE + 0x390)
#define APIC_TIMER_DIV		(APIC_BASE + 0x3E0)
   #define DIVIDE_1		0xB
   #define DIVIDE_2		0x0
   #define DIVIDE_4		0x1
   #define DIVIDE_8		0x2
   #define DIVIDE_16		0x3
   #define DIVIDE_32		0x8
   #define DIVIDE_64		0x9
   #define DIVIDE_128		0xA

   #define TIMER_PERIODIC	(1<<17)
#define APIC_EXT_FEAT		(APIC_BASE + 0x400)
#define APIC_EXT_CTRL		(APIC_BASE + 0x410)
#define APIC_SEOI		(APIC_BASE + 0x420)
#define APIC_IER_START		(APIC_BASE + 0x480)
#define APIC_IER_END		(APIC_BASE + 0x4F0)
#define APIC_EXT_REGS_START	(APIC_BASE + 0x500)
#define APIC_EXT_REGS_END	(APIC_BASE + 0x530)

void apic_eoi();
void apic_enable();
