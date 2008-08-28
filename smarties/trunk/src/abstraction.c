/**
 * @file abstraction.c
 * 
 * All these functions are expected to be called every millisecond by the function 
 * ISR (TIMER0_OVF_vect) in the file my_interrupt.c
 * 
 */


#include "abstraction.h"
#include "system.h"


extern smartie_sorter ss;

#if DISTANCE_DETECTION || DISTANCE_NORM_DETECTION
#include <math.h>
#endif

#if TABLE_REFERENCE_DETECTION
extern uint8_t col_tab_blu[col_unknown][2];
extern uint8_t col_tab_gre[col_unknown][2];
extern uint8_t col_tab_red[col_unknown][2];
#endif

#if DISTANCE_DETECTION
extern float col_ava_blu [col_unknown];
extern float col_ava_gre [col_unknown];
extern float col_ava_red [col_unknown];
extern float col_ava_bri [col_unknown];
#endif

#if DISTANCE_NORM_DETECTION
extern float col_ava_blu [col_unknown];
extern float col_ava_gre [col_unknown];
extern float col_ava_red [col_unknown];
extern float col_ava_bri [col_unknown];
#endif

void motor_universal_stuff (stepper_motor *this);


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
void motor_stuff () {
	//motor_catcher_stuff();
	//motor_revolver_stuff();
	motor_universal_stuff(&ss.mot_catcher);
	motor_universal_stuff(&ss.mot_revolver);
}

void motor_universal_stuff (stepper_motor *this) {
	if (this->status != stat_idle) {
		if (this->pause)    /* Count cycles/milliseconds for pause */
			this->pause--;
		else				/* If we are not in pause, we can count the milliseconds */
			this->cycle_counter++;
	}

	/* start and ramp up */
	if (this->status == stat_start_working) {
		/* if we just started to rotate, prepare ramp up */
		if (this->status_last != stat_start_working) {
			this->status_last = stat_start_working;
			this->ramp_steps = this->ramp_duration; /* will be decreased during ramp up */
			this->pause = this->pause_duration;
			this->steps_estimated = this->steps_estim_def;
			this->cycle_counter = 0;
			this->steps = 0;
		}
		
		/* do the ramp up */
		if ( (this->cycle_counter == (this->step_duration * this->ramp_steps) )
				&& !(this->pause) ) {
//			this->enable();
			this->cycle_counter = 0;
			this->ramp_steps--;
			if (this->ramp_steps == 0) {
				this->status = stat_working;
			}
			this->move_step();
			this->steps++;
		}
	}
	/* just rotate */
	if (this->status == stat_working) {
		if (this->status_last != stat_working) {
			this->status_last = stat_working;
			this->cycle_counter = 0;
		}
		if (this->cycle_counter == this->step_duration) {
			this->move_step();
			this->steps++;
			this->cycle_counter = 0;

			/* count the passes on the lightbarrier. One pass, one position */
			if (this->lb->passes > 0) {
				this->lb->passes--;
				this->steps = 0;
				this->current_pos++;
				if (this->current_pos == this->max_size)
					this->current_pos = 0;
			}
		}
				
		/* take care of if we are too far */
		if (this->current_pos == this->target_pos) {
			if (this->lb->is_blocked()) {
				/* Do some more steps if we reached a difined position */
				if (!this->steps_estimated)
					this->status = stat_stop_working;
				this->steps_estimated--;
			}
		}
	}

	/* pause down */
	if (this->status == stat_stop_working) {
		if (this->status_last == stat_working) {
			this->status_last = stat_stop_working;
			this->pause = this->pause_duration;
		}
		
		/* stop */
		if (!this->pause) { /* If pause is over */
			this->status = stat_finished;
		}
	}
	
	/* stop */
	if (this->status == stat_finished) {
		if (this->status_last == stat_stop_working) {
			this->status_last = stat_finished;
			this->cycle_counter = 0;
		}
		this->status = stat_idle;
//		this->disable();
	}
}

/** \brief Takes controll over the rotary encoder (user input) device
 * 
 * This function is expected to be called regulary. It polls the 
 * input pins for the rotary encoder and sets the corresponding 
 * flags of the rotary_encoder struct. 
 */
void rotary_encoder_stuff() {
	if (IS_ROTENC_PUSH)
		ss.rotenc.pushtmp = ROTENC_PUSH;

	/* After button released, then count a single button press */
	if (ss.rotenc.pushtmp == ROTENC_PUSH && !IS_ROTENC_PUSH) {
		ss.rotenc.push++;
		ss.rotenc.pushtmp = ROTENC_NONE;
	}

	if (IS_ROTENC_NONE) {
		if (ss.rotenc.rottmp == ROTENC_A)
			ss.rotenc.left++;

		if (ss.rotenc.rottmp == ROTENC_B) // store current position
			ss.rotenc.right++;

		if (ss.rotenc.rottmp == ROTENC_BOTH) //choose myself
			ss.rotenc.right++;

		ss.rotenc.rottmp = ROTENC_NONE; //store current position
	}
	if (IS_ROTENC_AB) {
		if (ss.rotenc.rottmp == ROTENC_B)
			ss.rotenc.left++;

		if (ss.rotenc.rottmp == ROTENC_A) // store current position
			ss.rotenc.right++;

		if (ss.rotenc.rottmp == ROTENC_NONE) //choose myself
			ss.rotenc.right++;

		ss.rotenc.rottmp = ROTENC_BOTH;
	} else if (IS_ROTENC_A) {
		if (ss.rotenc.rottmp == ROTENC_BOTH)
			ss.rotenc.left++;

		if (ss.rotenc.rottmp == ROTENC_NONE) // store current position
			ss.rotenc.right++;

		if (ss.rotenc.rottmp == ROTENC_B) //choose myself
			ss.rotenc.right++;

		ss.rotenc.rottmp = ROTENC_A;
	} else if (IS_ROTENC_B) {
		if (ss.rotenc.rottmp == ROTENC_NONE)
			ss.rotenc.left++;

		if (ss.rotenc.rottmp == ROTENC_BOTH) // store current position
			ss.rotenc.right++;

		if (ss.rotenc.rottmp == ROTENC_A) //choose myself
			ss.rotenc.right++;

		ss.rotenc.rottmp = ROTENC_B;
	}
}

/**
 *  \brief Take control over the lightbarriers
 * 
 * This function is expected to be called regulary. It polls the
 * input pins of the lightbarrier and sets the corresponding flags 
 * of lightbarrier struct. This function assumes that lightbarriers
 * are always enabled.
 */
void lightbarrier_stuff ()
{
	if (IS_LB_CATCHER) {
		/* recognize status changes from free to blocked */
		if (ss.lb_catcher.status_last == lb_free) {
			ss.lb_catcher.status_last = lb_blocked;
			ss.lb_catcher.status = lb_blocked;
		}
	}
	else {
		/* if status has changed fromm blocked to free, count a pass */
		if (ss.lb_catcher.status_last == lb_blocked) {
			ss.lb_catcher.status_last = lb_free;
			ss.lb_catcher.status = lb_free;
			ss.lb_catcher.passes++;
		}
	}

	if (IS_LB_REVOLVER) {
		/* recognize status changes from free to blocked */
		if (ss.lb_revolver.status_last == lb_free) {
			ss.lb_revolver.status_last = lb_blocked;
			ss.lb_revolver.status = lb_blocked;
		}
	}
	else {
		/* if status has changed fromm blocked to free, count a pass */
		if (ss.lb_revolver.status_last == lb_blocked) {
			ss.lb_revolver.status_last = lb_free;
			ss.lb_revolver.status = lb_free;
			ss.lb_revolver.passes++;
		}
	}
}

//not implemented
void sensor_adjd_stuff() {
}

/**
 * \brief Takes control over the TCS color sensor
 * 
 * This function is expected to be called exectly every millisecond to
 * work properly. This function will read the current color of the smartie.
 * 
 * About the way of color detection please refere to the Main page
 */
void sensor_tcs_stuff() {
	uint8_t y;
	static uint16_t f_blu, f_gre, f_red;
	
#if DISTANCE_DETECTION
	double col_dis_blu [col_unknown] = { 1000., 1000., 1000., 1000., 1000., 1000., 1000., 1000. };
	double col_dis_gre [col_unknown] = { 1000., 1000., 1000., 1000., 1000., 1000., 1000., 1000. };
	double col_dis_red [col_unknown] = { 1000., 1000., 1000., 1000., 1000., 1000., 1000., 1000. };
	double 	col_dis_all [col_unknown] = { 1000., 1000., 1000., 1000., 1000., 1000., 1000., 1000. };
	double 	minimum = 1000.;
	smartie_color probably = col_unknown;
#endif
#if DISTANCE_NORM_DETECTION
	double col_dis_blu [col_unknown] = { 1000., 1000., 1000., 1000., 1000., 1000., 1000., 1000. };
	double col_dis_gre [col_unknown] = { 1000., 1000., 1000., 1000., 1000., 1000., 1000., 1000. };
	double col_dis_red [col_unknown] = { 1000., 1000., 1000., 1000., 1000., 1000., 1000., 1000. };
	double col_dis_all [col_unknown] = { 1000., 1000., 1000., 1000., 1000., 1000., 1000., 1000. };
	double minimum = 1000.;
	smartie_color probably = col_unknown;
#endif
	
	if (ss.sens_tcs.status == stat_start_working) {
		if (ss.sens_tcs.status_last == stat_idle) {
			ss.sens_tcs.status = stat_working;
			ss.sens_tcs.status_last = stat_start_working;
			COL_SENS_TCS_ENABLE;
			COL_SENS_TCS_SET_FILTER(col_unknown);
			COL_SENS_TCS_FREQ_MESURE_EN;
			ss.sens_tcs.time = 0;
			ss.sens_tcs.slopes = 0;
			ss.sens_tcs.filter_freq_blue = 0;
			ss.sens_tcs.filter_freq_green = 0;
			ss.sens_tcs.filter_freq_red = 0;
			ss.sens_tcs.filter_freq_none = 0;
		}
	}

	if (ss.sens_tcs.status == stat_working) {
		ss.sens_tcs.time++;
		if (ss.sens_tcs.status_last == stat_start_working) {
			ss.sens_tcs.status_last = stat_working;
			COL_SENS_TCS_SET_FILTER(col_blue);
		}

		if (ss.sens_tcs.time == COL_SENS_TCS_SAMPLE_TIME) {
			if (ss.sens_tcs.filter_freq_blue == 0) {
				ss.sens_tcs.filter_freq_blue = ss.sens_tcs.slopes / COL_SENS_TCS_SAMPLE_TIME ;
				COL_SENS_TCS_SET_FILTER(col_green);
				ss.sens_tcs.time = 0;
				ss.sens_tcs.slopes = 0;
			}
			else if (ss.sens_tcs.filter_freq_green == 0) {
				ss.sens_tcs.filter_freq_green = ss.sens_tcs.slopes / COL_SENS_TCS_SAMPLE_TIME;
				COL_SENS_TCS_SET_FILTER(col_red);
				ss.sens_tcs.time = 0;
				ss.sens_tcs.slopes = 0;
			}
			else if (ss.sens_tcs.filter_freq_red == 0) {
				ss.sens_tcs.filter_freq_red = ss.sens_tcs.slopes / COL_SENS_TCS_SAMPLE_TIME;
				COL_SENS_TCS_SET_FILTER(col_unknown);
				ss.sens_tcs.time = 0;
				ss.sens_tcs.slopes = 0;
			}
			else if (ss.sens_tcs.filter_freq_none == 0) {
				ss.sens_tcs.filter_freq_none = ss.sens_tcs.slopes / COL_SENS_TCS_SAMPLE_TIME;
				ss.sens_tcs.status = stat_stop_working;
			}
		}
	}
	
	if (ss.sens_tcs.status == stat_stop_working) {
		if (ss.sens_tcs.status_last == stat_working) {
			ss.sens_tcs.status_last = stat_stop_working;
			COL_SENS_TCS_FREQ_MESURE_DI;
			COL_SENS_TCS_DISABLE;
			f_blu = ss.sens_tcs.filter_freq_blue;
			f_gre = ss.sens_tcs.filter_freq_green;
			f_red = ss.sens_tcs.filter_freq_red;
		}
		/* Now detect colors */
#if DISTANCE_NORM_DETECTION
		for (y=0; y<=col_unknown; y++) {
			col_dis_blu[y] = (col_ava_blu[y] / f_blu );
			col_dis_gre[y] = (col_ava_gre[y] / f_gre);
			col_dis_red[y] = (col_ava_red[y] / f_red);
			col_dis_all[y] = sqrt ( (col_dis_blu[y])*(col_dis_blu[y]) + (col_dis_gre[y])*(col_dis_gre[y]) + (col_dis_red[y])*(col_dis_red[y]));
			if (minimum >= col_dis_all[y]) {
				minimum = col_dis_all[y];
				probably = y;
				ss.sens_tcs.distance = (int16_t)minimum;
			}
		}

		if (minimum < 6.)
			ss.sens_tcs.color = probably;
		else
			ss.sens_tcs.color = col_unknown;

#endif
		
#if DISTANCE_DETECTION
		for (y=0; y<=col_unknown; y++) {
			col_dis_blu[y] = fabs (col_ava_blu[y] - f_blu);
			col_dis_gre[y] = fabs (col_ava_gre[y] - f_gre);
			col_dis_red[y] = fabs (col_ava_red[y] - f_red);
			col_dis_all[y] = sqrt ( (col_dis_blu[y])*(col_dis_blu[y]) + (col_dis_gre[y])*(col_dis_gre[y]) + (col_dis_red[y])*(col_dis_red[y]));
			if (minimum >= col_dis_all[y]) {
				minimum = col_dis_all[y];
				probably = y;
				ss.sens_tcs.distance = (int16_t)minimum;
			}
		}

		if (minimum <= 6)
			ss.sens_tcs.color = probably;
		else
			ss.sens_tcs.color = col_unknown;

#endif
#if TABLE_REFERENCE_DETECTION
		for (y=0; y<=col_unknown; y++) {
			if ( ((f_blu >= col_tab_blu[y][0]) && (f_blu <= col_tab_blu[y][1])) && 
					((f_gre >= col_tab_gre[y][0]) && (f_gre <= col_tab_gre[y][1])) &&
					((f_red >= col_tab_red[y][0]) && (f_red <= col_tab_red[y][1])) ) {
				break;
			}
		}
		ss.sens_tcs.color = y;
#endif
		ss.sens_tcs.status = stat_finished;
	}
	
	if (ss.sens_tcs.status == stat_finished) {
		ss.sens_tcs.status = stat_idle;
		ss.sens_tcs.status_last = stat_finished;
	}
}

/**
 *  \brief Takes control over the shaker (vibrator) device
 * 
 * This function is expected to be called regulary. It polls the
 * input pins of the shaker and sets the corresponding flags 
 * of the shaker struct. This function assumes that 
 */
void vibrator_stuff () {
	if (ss.vibr.status == stat_start_working) {
		if (ss.vibr.status_last != stat_start_working) {
			ss.vibr.status_last = stat_start_working; 
			ss.vibr.status = stat_working;
			ss.vibr.duration = VIBR_DURATION;
			VIBR_ON;
		}
	}
	
	if (ss.vibr.status == stat_working) {
		if (ss.vibr.status_last != stat_working)
			ss.vibr.status_last = stat_working;
		
		ss.vibr.duration--;
		if (ss.vibr.duration == 0) {
			ss.vibr.status = stat_stop_working;
		}
	}
	
	if (ss.vibr.status == stat_stop_working) {
		if (ss.vibr.status_last != stat_stop_working)
			ss.vibr.status_last = stat_stop_working;
		VIBR_OFF;
		ss.vibr.status = stat_finished;
	}
	
	if (ss.vibr.status == stat_finished) {
		if (ss.vibr.status_last != stat_finished)
			ss.vibr.status_last = stat_finished;
		ss.vibr.status = stat_idle;
	}
}
