#include "vgatext.h"

//Array is assumed to be 24x160
void k_screen_cap(int captureArray[][160]){

   for(int row = 0; row < ROWS; row++){
      for(int col = 0; col < COLS; col++){
         captureArray[row][col] = VIDEO_MEMORY[160 * row + col];
      }
   }
}


void k_screen_res(int restoreArray[][160]){

   for(int row = 0; row < ROWS; row++){
      for(int col = 0; col < COLS; col++){
         VIDEO_MEMORY[160 * row + col] = restoreArray[row][col];
      }
   }
}

