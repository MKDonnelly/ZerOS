#pragma once
#include <lib/types.h>

//Much of this taken from 
//http://files.osdev.org/mirrors/geezer/osd/graphics/modes.c

//                          VGA 3h mode
#define VGA3H_ROWS 25
#define VGA3H_COLS 80
#define VGA3H_MEM 0xB8000
//Multiply by two since each cell has a character
//and color byte associated with it.
#define VGA3H_MEM_END 0xB8000 + VGA3H_ROWS * VGA3H_COLS * 2

//VGA ports
#define VGA_MISC_P             0x3C2
#define VGA_SEQ_IDX_P          0x3C4
#define VGA_SEQ_DATA_P         0x3C5
#define VGA_CRTC_IDX_P         0x3D4
#define VGA_CRTC_DATA_P        0x3D5
#define VGA_GRAPHICSC_IDX_P    0X3CE
#define VGA_GRAPHICSC_DATA_P   0X3CF
#define VGA_ATTRC_DATA_P 0x3C0

//                          VGA 13h mode
//=========================================================
#define VGA13H_MEM 0xA0000
#define VGA13H_MEM_END   0xA0000 + 320 * 200

#define   VGA13H_BLACK    0,
#define   VGA13H_BLUE     1
#define   VGA13H_GREEN    2
#define   VGA13H_CYAN     3
#define   VGA13H_RED      4
#define   VGA13H_MAGENTA  5
#define   VGA13H_BROWN    6
#define   VGA13H_LGREY    7
#define   VGA13H_DGREY    8
#define   VGA13H_LBLUE    9
#define   VGA13H_LGREEN   10
#define   VGA13H_LCYAN    11
#define   VGA13H_LRED     12
#define   VGA13H_LMAGENTA 13
#define   VGA13H_YELLOW   14
#define   VGA13H_WHITE    15

//This array defines the register state
//for vga mode 3h (80x25 text)
extern unsigned char vga_3h_mode[];

//This array defines the register state
//for vga mode 13h (320x200 graphics)
extern unsigned char vga_13h_mode[];

//This function takes an array of registers
//and implements the given mode.
void set_vga_mode(unsigned char*);

//Set the given vga plane
void set_plane(uint8_t);


