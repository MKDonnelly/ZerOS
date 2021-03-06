#pragma once

#include <lib/types.h>

#define I8253_CH0_P   0x40 //This can generate an interrupt
//CH1 @ 0x41 is not implemented
#define I8253_CH2_P   0x42 //This controls the PC speaker

#define I8253_CTRL_P  0x43
#define I8253_CTRL_CH0_M (0b00 << 6)
#define I8253_CTRL_LHBYTE_M (0b11 << 4)
#define I8253_CTRL_MODE3_M ( 0b011 << 1 )
#define I8253_CTRL_BIN_M ( 0 << 0 )

#define TIMER_INTERRUPT 32
#define TIMER_IRQ        0

//Configuration bits for port 0x43:
//(from wiki.osdev.or/Programmable_Interval_Timer)
//Bits           Use
//6-7            Channel select:
//                 0 0 = Channel 0
//                 0 1 = Channel 1
//                 1 0 = Channel 2
//                 1 1 = Read-back command
//4-5            Access mode
//                 0 0 = Latch count value command
//                 0 1 = lowbyte only
//                 1 0 = highbyte only
//                 1 1 = lowbyte/highbyte
//1-3            Timer Mode
//                 0 0 0 = Interrupt on terminal count
//                 0 0 1 = Hardware re-triggerable one-shot
//                         (only works on CH 2)
//                 0 1 0 = Rate generator
//                 0 1 1 = Square wave generator
//                 1 0 0 = Software triggered strobe
//                 1 0 1 = Hardware triggered strobe
//                         (only works on CH 2)
//                 1 1 0 = same as 0 1 0
//                 1 1 1 = same as 0 1 1
//0              Binary (0) or BCD (1) mode

#define I8253_FREQ 1193182
#define I8253_10MS_COUNTER 11932 
#define ARCH_TIMER_MS_PERIOD 10

//Initilize the timer
//Enable its IRQ and sets timer_callback
//as its handler.
void arch_timer_init( void (*timer_callback)() );
void set_timer_count(uint16_t);
