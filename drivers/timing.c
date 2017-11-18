#include "timing.h"

void init_timer(){

   //Set the timer to operate at 100HZ
   set_timer_freq( I8253_10MS_COUNTER );

   //Enable the timer interrupt and 
   //set the interrupt handler
   enable_irq( 0 );
   register_interrupt( TIMER_INTERRUPT, timer_int_handler );

   //Zero out the members of the sys_time struct
   //to be safe
   system_time.days = 0;
   system_time.hours = 0;
   system_time.minutes = 0;
   system_time.seconds = 0;
   system_time.milliseconds = 0;
}

void set_timer_freq(int freq){
   udword opFreq = freq;

   //Tell the timer we are going to initilize it
   portb_write( I8253_CTRL_P, I8253_LOAD_C );

   //Write the frequency to the timer
   ubyte lowerByte = (ubyte)(opFreq & 0xFF );
   ubyte higherByte = (ubyte)( (opFreq >> 8) & 0xFF );
   portb_write( I8253_CH0_P, lowerByte );
   portb_write( I8253_CH0_P, higherByte );
}


//This timer callback handles the system clock
void timer_int_handler( struct registers r){

   //This will be used if we should update
   //time uptime on the screen
   ubyte secondOverflow = 0;

   //This is hardcoded, so, for now, we will assume
   //that we have set the clock to use .01 second
   //interrupt intervals
   system_time.milliseconds += 10;

   //Miliseconds overflowed
   if( system_time.milliseconds >= 1000 ){
      system_time.seconds += 1;
      system_time.milliseconds -= 1000;
      secondOverflow = 1;
   }
   //Seconds overflowed
   if( system_time.seconds >= 60 ){
      system_time.minutes++;
      system_time.seconds -= 60;
   }
   //Minutes overflowed
   if( system_time.minutes >= 60 ){
      system_time.hours++;
      system_time.minutes -= 60;
   }

   if( system_time.hours >= 24 ){
      system_time.hours -= 24;
      system_time.days++;
   }

   //Update the clock if needed on the screen
   if( secondOverflow ){
      char hours[9];
      char minutes[9];
      char seconds[9];

      itoa( system_time.hours, hours);
      itoa( system_time.minutes, minutes);
      itoa( system_time.seconds, seconds);

      int clockx = 5;
      int clocky = 1;
      k_print_at( "System uptime: ", 0, 0);
      k_print_at( hours, clockx+1, clocky);
      k_print_at( ":", clockx+2, clocky);
      k_print_at( minutes, clockx+4, clocky);
      k_print_at( ":", clockx+5, clocky);
      k_print_at( seconds, clockx+7, clocky);
   }
   
}


/*
unsigned int i8253_get(){

   unsigned int lower_byte, higher_byte;

   portb_write( I8253_CMD, I8253_CMD_LATCH );
   lower_byte = portb_read( I8253_CH0 );
   higher_byte = portb_read( I8253_CH0 );

   return lower_byte + (higher_byte << 8);
}
void i8253_set( unsigned int c ){

   unsigned char counter;

   portb_write( I8253_CMD, I8253_CMD_LOAD );

   counter = (unsigned char) ( c & 0x00ff );
   portb_write( I8253_CH0, counter );

   counter = (unsigned char) ( ( c >> 8 ) & 0x00ff );
   portb_write( I8253_CH0, counter);
}*/
