#ifndef ABSTRACTION_H_
#define ABSTRACTION_H_

/**
 * @file abstraction.h
 * 
 * All these functions are expected to be called every millisecond by the function 
 * ISR (TIMER0_OVF_vect) in the file my_interrupt.c
 * 
 */

/**
 * \brief Takes controll over the catcher and revolver stepper engines
 * 
 * The ramp up is made by linear shrinking the time period t for each step:
 * \code 
 *  Steps
 *  ^
 *  |         4t                 3t       2t   t  t  t  t
 *  *                       *           *     *  *  *  *  *
 *  |  
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-> Time
 *  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18
 * 
 * Symbolic diagram, no real values
 * \endcode
 * 
 * This function is expected to be called every millisecond to work properly.
 * 
 * Important setting values are:
 *  - CATCH_STEP_DURATION
 *  - CATCH_RAMP_DURATION
 *  - REV_STEP_DURATION
 *  - REV_RAMP_DURATION
 * 
 * See below for links to them.
 * 
 */
void mot_stuff ();

/** \brief Takes controll over the rotary encoder (user input) device
 * 
 * This function is expected to be called regulary. It polls the 
 * input pins for the rotary encoder and sets the corresponding 
 * flags of the rotary_encoder struct. 
 */
void rotary_encoder_stuff ();

/**
 *  \brief Take control over the lightbarriers
 * 
 * This function is expected to be called regulary. It polls the
 * input pins of the lightbarrier and sets the corresponding flags 
 * of lightbarrier struct. This function assumes that lightbarriers
 * are always enabled.
 */
void lightbarrier_stuff ();
void display_stuff ();
void sensor_stuff ();

/**
 *  \brief Takes control over the shaker (vibrator) device
 * 
 * This function is expected to be called regulary. It polls the
 * input pins of the shaker and sets the corresponding flags 
 * of the shaker struct. This function assumes that lightbarriers
 * are always enabled.
 */
void shaker_stuff ();


#endif /*ABSTRACTION_H_*/
