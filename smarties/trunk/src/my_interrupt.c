#include "smarties2.h"
#include "avr/interrupt.h"

/**
 * @file my_interrupt.c 
 * 
 * Alle necessary interrupt routines. Please check the source code of
 * this file for more documentation.
 * 
 */

extern smartie_sorter ss;

ISR (TIMER0_OVF_vect)
// Interrupt routine, executed every millisecond
{	
	// user input (rotary encoder) stuff
	rotary_encoder_stuff();
	
	// display stuff
	// sensor stuff
	// engine stuff
	//engine_stuff();
	
	// shaker stuff
	//shaker_stuff();
	
}
