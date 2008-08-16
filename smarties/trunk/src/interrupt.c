#include "smarties2.h"
#include "avr/interrupt.h"

/**
 * @file interrupt.c 
 * 
 * Alle necessary interrupt routines. Please check the source code of
 * this file for more documentation.
 * 
 */

extern smartie_sorter ss;

/**
 * \brief Interrupt routine, executed every millisecond
 */
ISR (TIMER0_COMP_vect) {

	/* color sensor TCS stuff */
	sensor_tcs_stuff();

	/* user input (rotary encoder) stuff */
	rotary_encoder_stuff();
	
	/* light barrier stuff */
	lightbarrier_stuff ();
	
	/* stepper motor stuff */
	motor_stuff();
	
	/* color sensor ADJD stuff */
	sensor_adjd_stuff();
	
	/* display and menu stuff */
	menu_stuff ();
	
	// sensor stuff
	// engine stuff
	
	// shaker stuff
	//shaker_stuff();
		
}

/**
 * \brief Interupt routine for measuring frequency of color sensor TCS OUT pin
 */
ISR (INT0_vect) {
	ss.sens_tcs.slopes++;
}
