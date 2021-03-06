#pragma once

#include <lib/types.h>

#define MASTER_PIC_CTRL_P 0x20
#define MASTER_PIC_DATA_P 0x21

#define SLAVE_PIC_CTRL_P 0xA0
#define SLAVE_PIC_DATA_P 0xA1

//This must be sent to the master
//pic (the slave pic also if the interrupt
//came from the slave) after handling the
//interrupt or else the PIC will not 
//trigger any interrupts.
#define PIC_EOI_C 0x20

#define PIC_ICW1_ENABLE_ICW4_C 0x01   //Enable ICW4 control register
#define PIC_ICW1_CASCADE_C     0x02   //Enable cascade mode
#define PIC_ICW1_INTERVAL4_C   0x04   //Call address interval 4 (8)
#define PIC_ICW1_LELVEL_C      0x08   //Level triggered (edge) mode
#define PIC_ICW1_INIT_C        0x10   //Initilize the PIC

#define PIC_ICW4_8086_MODE_C   0x01
#define PIC_ICW4_AUTO_EOI_C    0x02

//Each pic has 8 interrupt lines
#define PIC_INTERRUPT_LINES 8
#define PIC_MASTER_START 0x20
#define PIC_MASTER_END   0x27
#define PIC_SLAVE_START  0x28
#define PIC_SLAVE_END    0x30

//Remap the PIC to non-handware
//interrupt numbers
void pic_init();

//Enable/disable an IRQ on the PIC by number
void irq_enable(uint8_t);
void irq_disable(uint8_t);
