#include "smarties2.h"
#include "system.h"

extern smartie_sorter ss;

/**
 * \brief Will Enter the pause mode 
 * 
 * This function will enter the pause mode after the current function is finished. This function 
 * is usually called from the lcd menu. This function is only available in the 
 * mode SYS_MODE_RUNNING
 */
void sys_pause()
{
	if (ss.state.mode == SYS_MODE_PAUSE)
		return;
	ss.state.modetmp = ss.state.mode;
	ss.state.mode = SYS_MODE_PAUSE;
}

/**
 * \brief Leave the pause mode
 * 
 * This function will leave the pause mode. This function 
 * is usually called from the lcd menu. This function is only available in the 
 * mode \ref SYS_MODE_PAUSE
 * 
 */
void sys_resume()
{
	if (ss.state.mode == SYS_MODE_RUNNING)
		return;
	
}

/**
 * \brief Rotates the revolver in Pause Mode
 * 
 * This function will rotate the revolver for one position, so that the 
 * next smartie is loaded and/or the next smartie will be underlying the
 * color sensor.
 * 
 * It will check the light barrier for this position rather than counting 
 * or calculating degrees and steps
 * 
 * This function is intened for manual usage and therefor is not available
 * during normal running mode SYS_MODE_RUNNING, only during SYS_MODE_PAUSE. 
 * 
 */
void sys_rotate_revolver()
{
	
}

void sys_rotate_catcher()
{
	
}

/**
 * \brief Waits a time in microseconds
 * 
 * It executes a couple of nop's so that at any CPU speed it waits always waits 
 * about time in microseconds until it returns
 * 
 * \param time
 * Time in microseconds
 * 
 */
void sys_wait(uint16_t time)
{
	float f_cycles;
	uint16_t u_cycles;
	
	f_cycles = 0.000001 / F_CPU;
	
	f_cycles += 0.5; u_cycles = f_cycles; // round and convert

	do {
		do {
			asm volatile ("nop");
		} while (u_cycles--);
	}while (time--);
}


void start_shaker()
{
	
}

void start_get_color_1()
{
	
}

void start_get_color_2()
{
	
}

smartie_color get_catcher_position()
{
	return 0;
}

/**
 * \brief Rotates the catcher to a certain position
 * 
 * This function will rotate the catcher to position specified by the a color
 * 
 */
void catcher_rotate_absolute(smartie_color color_new)
{
	smartie_color color_tmp = 0;
	uint8_t color_count = 0;
	float steps = 0;
	uint16_t angle = 0;
	
	color_tmp = get_catcher_position();
	
	///! Calculate the difference to move the motor
	while (color_tmp != color_new) {
		color_tmp++;
		if (color_tmp == col_unknown) {
			color_tmp = 0;
		}
		color_count++;
	}
	
	///! calculate steps needed for rotation
	///! assuming full steps
	angle = color_count * CATCHER_ANGLE_PER_COLOR;
	steps = angle / CATCHER_ANGLE_PER_STEP;
	
	catcher_rotate_relative(steps);
}

/**
 * \brief rotates the catcher by some steps
 * 
 * \param catcher
 * The amount of catcher which must pass the light barrier
 */
void catcher_rotate_relative(uint16_t catcher)
{
/*	uint16_t i;
	uint16_t u_steps;
	///! round the value steps
	f_steps += 0.5;
	u_steps = f_steps;
	
	CATCH_EN();
	for (i=u_steps; i>0; i-- ) {
		CATCH_MOVE_STEP();
		sys_wait(255);
		if (i==3) {	// check for position towards the end
			if (IS_LB_CATCHER) { // as soon as a positoner touches the lightbarrier
				i = 0;
			}			
		}
	} // for ()
	CATCH_DIS();
*/
}

uint8_t catcher_rotate_steps (uint16_t f_steps)
{
/*	extern engine eng_catcher;
	
	if (eng_catcher.eng_stat != eng_stop)
		return FALSE;
	
	//TODO: ramp for 
	
	eng_catcher.eng_stat = eng_rotate;
	eng_catcher.tar_pos = eng_catcher.cur_pos + f_steps;
	
	CATCH_START_ROTATING();
*/
	return 0;
}

void revolver_rotate_absolute(uint8_t abs_pos)
{
	
}

void revolver_rotate_relative(uint8_t rel_pos)
{
	
}


smartie_color make_color_merge(smartie_color color1, smartie_color color2)
{
	return 0; 
}
