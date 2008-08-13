#include "abstraction.h"
#include "smarties2.h"

#define TESTING_RAMPS 	1 /* temporarly only */

extern smartie_sorter ss;

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
 */
void motor_stuff ()
{
	/**************************************************************************
	 *                           C A T C H E R                                *
	 *************************************************************************/

	if (ss.mot_catcher.status != stat_idle)
		ss.mot_catcher.cycle_counter++;
		
#if TESTING_RAMPS
	/* start and ramp up */
	if (ss.mot_catcher.status == stat_start_working) {
		/* if we just started to rotate, start and prepare ramp up */
		if (ss.mot_catcher.status_tmp == stat_idle) {
			CATCH_ENABLE;
			ss.mot_catcher.status_tmp = stat_start_working;
			ss.mot_catcher.rampup_steps = CATCH_RAMP_DURATION; /* will be decreased during ramp up */
			ss.mot_catcher.cycle_counter = 0;
		}
		
		/* do the ramp up */
		if (ss.mot_catcher.cycle_counter
				== (CATCH_STEP_DURATION * ss.mot_catcher.rampup_steps)) {
			ss.mot_catcher.cycle_counter = 0;
			ss.mot_catcher.rampup_steps--;
			if (ss.mot_catcher.rampdown_steps == 0) {
				ss.mot_catcher.status = stat_working;
				ss.mot_catcher.status_tmp = stat_start_working;
			}
			CATCH_MOVE_STEP;
		}
	}
#endif /* Testing ramps */

#if !TESTING_RAMPS
	/* start rotating */
	if (ss.mot_catcher.status == stat_start_working) {
		ss.mot_catcher.status_tmp = stat_start_working;
		ss.mot_catcher.status = stat_working;
		CATCH_ENABLE;
	}
#endif 
	
	/* just rotate */
	if (ss.mot_catcher.status == stat_working) {
		if (ss.mot_catcher.status_tmp == stat_start_working) {
			ss.mot_catcher.status_tmp = stat_working;
			ss.mot_catcher.cycle_counter = 0;
		}
		if (ss.mot_catcher.cycle_counter == REV_STEP_DURATION) {
			CATCH_MOVE_STEP;
			ss.mot_catcher.cycle_counter = 0;
		}
		/* count the passes on the lightbarrier. One pass, one position */
		if (ss.lb_catcher.passes > 0) {
			ss.mot_catcher.currentPos++;
			ss.lb_catcher.passes--;
		}
		/* before we reach the target position do the ramp down */
		if ( ss.mot_catcher.currentPos == (ss.mot_catcher.targetPos-1) ) 
			ss.mot_catcher.status = stat_stop_working;
	}
	
#if TESTING_RAMPS
	/* ramp down */
	if (ss.mot_catcher.status == stat_stop_working) {
		if (ss.mot_catcher.status_tmp == stat_working) {
			ss.mot_catcher.status_tmp = stat_stop_working;
			ss.mot_catcher.cycle_counter = 0;
			ss.mot_catcher.rampdown_steps = 1;
		}
		
		/* do the ramp down */
		if (ss.mot_catcher.cycle_counter 
				== (CATCH_STEP_DURATION * ss.mot_catcher.rampdown_steps)) {
			ss.mot_catcher.cycle_counter = 0;
			ss.mot_catcher.rampdown_steps++;
			if (ss.mot_catcher.rampdown_steps == CATCH_RAMP_DURATION) {
				ss.mot_catcher.status_tmp = stat_stop_working;
				ss.mot_catcher.status = stat_finished;
			}
			CATCH_MOVE_STEP;
		}
	}
#endif
	
#if !TESTING_RAMPS
	/* stop working */
	if (ss.mot_catcher.status == stat_stop_working) {
		ss.mot_catcher.status_tmp = stat_stop_working;
		ss.mot_catcher.status = stat_finished;
	}
#endif
	
	/* stop */
	if (ss.mot_catcher.status == stat_finished) {
		if (ss.mot_catcher.status_tmp == stat_stop_working) {
			ss.mot_catcher.status_tmp = stat_finished;
			ss.mot_catcher.cycle_counter = 0;
		}
		/* go on rotating until we meet the end position, indicated by the lightbarrier */
		/* but rotate slowly */
		if (ss.mot_catcher.cycle_counter
				== (CATCH_STEP_DURATION * REV_RAMP_DURATION)) {
			ss.mot_catcher.cycle_counter = 0;
			if (IS_LB_CATCHER) {
				ss.mot_catcher.status = stat_idle;
				CATCH_DISABLE;				
			}
		}
	}
	
	/**************************************************************************
	 *                           R E V O L V E R                              *
	 *************************************************************************/

	if (ss.mot_revolver.status != stat_idle)
		ss.mot_revolver.cycle_counter++;
		
#if TESTING_RAMPS
	/* start and ramp up */
	if (ss.mot_revolver.status == stat_start_working) {
		/* if we just started to rotate, prepare ramp up */
		if (ss.mot_revolver.status_tmp == stat_idle) {
			REV_ENABLE;
			ss.mot_revolver.status_tmp = stat_start_working;
			ss.mot_revolver.rampup_steps = REV_RAMP_DURATION; /* will be decreased during ramp up */
			ss.mot_revolver.cycle_counter = 0;
		}
		
		/* do the ramp up */
		if (ss.mot_revolver.cycle_counter
				== (REV_STEP_DURATION * ss.mot_revolver.rampup_steps)) {
			ss.mot_revolver.cycle_counter = 0;
			ss.mot_revolver.rampup_steps--;
			if (ss.mot_revolver.rampdown_steps == 0) {
				ss.mot_revolver.status = stat_working;
				ss.mot_revolver.status_tmp = stat_start_working;
			}
			REV_MOVE_STEP;
		}
	}
#endif

#if !TESTING_RAMPS
	/* start rotating */
	if (ss.mot_revolver.status == stat_start_working) {
		ss.mot_revolver.status_tmp = stat_start_working;
		ss.mot_revolver.status = stat_working;
		REV_ENABLE;
	}
#endif 
	
	/* just rotate */
	if (ss.mot_revolver.status == stat_working) {
		if (ss.mot_revolver.status_tmp == stat_start_working) {
			ss.mot_revolver.status_tmp = stat_working;
			ss.mot_revolver.cycle_counter = 0;
		}
		if (ss.mot_revolver.cycle_counter == REV_STEP_DURATION) {
			REV_MOVE_STEP;
			ss.mot_revolver.cycle_counter = 0;
		}
		/* count the passes on the lightbarrier. One pass, one position */
		if (ss.lb_revolver.passes > 0) {
			ss.mot_revolver.currentPos++;
			ss.lb_revolver.passes--;
		}
		/* before we reach the target position do the ramp down */
		if ( ss.mot_revolver.currentPos == (ss.mot_revolver.targetPos-1) ) 
			ss.mot_revolver.status = stat_stop_working;
	}
	
#if TESTING_RAMPS
	/* ramp down */
	if (ss.mot_revolver.status == stat_stop_working) {
		if (ss.mot_revolver.status_tmp == stat_working) {
			ss.mot_revolver.status_tmp = stat_stop_working;
			ss.mot_revolver.cycle_counter = 0;
			ss.mot_revolver.rampdown_steps = 1;
		}
		
		/* do the ramp down */
		if (ss.mot_revolver.cycle_counter 
				== (REV_STEP_DURATION * ss.mot_revolver.rampdown_steps)) {
			ss.mot_revolver.cycle_counter = 0;
			ss.mot_revolver.rampdown_steps++;
			if (ss.mot_revolver.rampdown_steps == REV_RAMP_DURATION) {
				ss.mot_revolver.status_tmp = stat_stop_working;
				ss.mot_revolver.status = stat_finished;
			}
			REV_MOVE_STEP;
		}
	}
#endif
	
#if !TESTING_RAMPS
	/* stop working */
	if (ss.mot_revolver.status == stat_stop_working) {
		ss.mot_revolver.status_tmp = stat_stop_working;
		ss.mot_revolver.status = stat_finished;
	}
#endif
	
	/* stop */
	if (ss.mot_revolver.status == stat_finished) {
		if (ss.mot_revolver.status_tmp == stat_stop_working) {
			ss.mot_revolver.status_tmp = stat_finished;
			ss.mot_revolver.cycle_counter = 0;
		}
		/* go on rotating until we meet the end position, indicated by the lightbarrier */
		/* but rotate slowly */
		if (ss.mot_revolver.cycle_counter
				== (REV_STEP_DURATION * REV_RAMP_DURATION)) {
			ss.mot_revolver.cycle_counter = 0;
			if (IS_LB_REVOLVER) {
				ss.mot_revolver.status = stat_idle;
				REV_DISABLE;				
			}
		}
	}
}

void rotary_encoder_stuff ()
{
	if (IS_ROTENC_PUSH)
		ss.rotenc.pushtmp = ROTENC_PUSH;

	/* After button released, then count a single button press */
	if (ss.rotenc.pushtmp == ROTENC_PUSH && !IS_ROTENC_PUSH)
	{
		ss.rotenc.push++;
		ss.rotenc.pushtmp = ROTENC_NONE;
	}
	
	if (IS_ROTENC_NONE)
	{
		if (ss.rotenc.rottmp == ROTENC_A)
			ss.rotenc.left++;
		
		if (ss.rotenc.rottmp == ROTENC_B) // store current position
			ss.rotenc.right++;

		if (ss.rotenc.rottmp == ROTENC_BOTH) //choose myself
			ss.rotenc.right++;

		ss.rotenc.rottmp = ROTENC_NONE;	//store current position
	}
	if (IS_ROTENC_AB)
	{
		if (ss.rotenc.rottmp == ROTENC_B)
			ss.rotenc.left++;
		
		if (ss.rotenc.rottmp == ROTENC_A) // store current position
			ss.rotenc.right++;

		if (ss.rotenc.rottmp == ROTENC_NONE) //choose myself
			ss.rotenc.right++;

		ss.rotenc.rottmp = ROTENC_BOTH;
	}
	else if (IS_ROTENC_A)
	{
		if (ss.rotenc.rottmp == ROTENC_BOTH)
			ss.rotenc.left++;
		
		if (ss.rotenc.rottmp == ROTENC_NONE) // store current position
			ss.rotenc.right++;

		if (ss.rotenc.rottmp == ROTENC_B) //choose myself
			ss.rotenc.right++;

		ss.rotenc.rottmp = ROTENC_A;
	}	
	else if (IS_ROTENC_B)
	{
		if (ss.rotenc.rottmp == ROTENC_NONE)
			ss.rotenc.left++;
		
		if (ss.rotenc.rottmp == ROTENC_BOTH) // store current position
			ss.rotenc.right++;

		if (ss.rotenc.rottmp == ROTENC_A) //choose myself
			ss.rotenc.right++;

		ss.rotenc.rottmp = ROTENC_B;
	}	
}

void lightbarrier_stuff ()
{
	if (IS_LB_CATCHER) {
		/* recognize status changes from free to blocked */
		if (ss.lb_catcher.status_tmp == lb_free) {
			ss.lb_catcher.status_tmp = lb_blocked;
			ss.lb_catcher.status = lb_blocked;
		}
	}
	else {
		/* if status has changed fromm blocked to free, count a pass */
		if (ss.lb_catcher.status_tmp == lb_blocked) {
			ss.lb_catcher.status_tmp = lb_free;
			ss.lb_catcher.status = lb_free;
			ss.lb_catcher.passes++;
		}
	}

	if (IS_LB_REVOLVER) {
		/* recognize status changes from free to blocked */
		if (ss.lb_revolver.status_tmp == lb_free) {
			ss.lb_revolver.status_tmp = lb_blocked;
			ss.lb_revolver.status = lb_blocked;
		}
	}
	else {
		/* if status has changed fromm blocked to free, count a pass */
		if (ss.lb_revolver.status_tmp == lb_blocked) {
			ss.lb_revolver.status_tmp = lb_free;
			ss.lb_revolver.status = lb_free;
			ss.lb_revolver.passes++;
		}
	}
}

void sensor_stuff ()
{
	
}


void shaker_stuff ()
{
	if (ss.shkr.statustmp == stat_idle)
		if (ss.shkr.status == stat_working)
		{
			ss.shkr.statustmp = stat_working;
			ss.shkr.duration = 500; 		//500 ms
		}
	if (ss.shkr.status == stat_working)
	{
		ss.shkr.duration--;
		if (ss.shkr.duration == 0)
		{
			ss.shkr.status = stat_finished;
			ss.shkr.statustmp = stat_working;
		}
	}
}
void display_stuff ()
{
	
}
void disp_set_mode ()
{
	
}

void disp_set_title ()
{
	
}

void disp_set_color ()
{
	
}
