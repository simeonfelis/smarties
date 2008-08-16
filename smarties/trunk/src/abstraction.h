#ifndef ABSTRACTION_H_
#define ABSTRACTION_H_

/**
 * @file abstraction.h
 * 
 * All these functions are expected to be called every millisecond by the function 
 * ISR (TIMER0_OVF_vect) in the file my_interrupt.c
 * 
 */

void motor_stuff ();
void rotary_encoder_stuff ();
void lightbarrier_stuff ();
void menu_stuff ();
void sensor_stuff ();
void sensor_adjd_stuff();
void sensor_tcs_stuff();
void shaker_stuff ();


#endif /*ABSTRACTION_H_*/
