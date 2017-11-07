
#pragma once

//Routines to deal with bitfields.
//Note that these can set an arbitrary
//bit in a chunk of memory (regardless
//of the char* type).

void setBit( char*, int );
void clearBit( char*, int );
int getBit( char*, int );


