#include "smarties2.h"
#include "system.h"

extern system_mode mode;
extern uint8_t rot_pos;

user_event get_user_action()
{
	user_event ret = EV_NO_EVENT;
	
	uint8_t rot_pos_new = get_rotencoder_position();
	
	if (rot_pos_new != rot_pos) {
		if (rot_pos == RE_NONE) {
			if (rot_pos_new == RE_A)
				ret = EV_ROTATE_RIGHT;
			if (rot_pos_new == RE_B)
				ret = EV_ROTATE_LEFT;
		}
		if (rot_pos == RE_A) {
			if (rot_pos_new == RE_BOTH)
				ret = EV_ROTATE_RIGHT;
			if (rot_pos_new == RE_NONE)
				ret = EV_ROTATE_LEFT;
		}
		if (rot_pos == RE_BOTH) {
			if (rot_pos_new == RE_B)
				ret = EV_ROTATE_RIGHT;
			if (rot_pos_new)
				ret = EV_ROTATE_LEFT;
		}
		if (rot_pos == RE_B) {
			if (rot_pos_new == RE_NONE)
				ret = EV_ROTATE_RIGHT;
			if (rot_pos_new == RE_BOTH)
				ret = EV_ROTATE_LEFT;
		}
	}
	
	rot_pos = rot_pos_new;
	
	return ret;
}

rotencoder get_rotencoder_position()
{
	rotencoder ret = RE_NONE;
	
	if (IS_ROTENC_A)
		ret = RE_A;
	if (IS_ROTENC_B)
		ret = RE_B;
	if (IS_ROTENC_A && IS_ROTENC_B)
		ret = RE_BOTH;
	
	return ret;
}

void sys_pause()
{
	if (mode == SYS_MODE_PAUSE)
		return;
	mode = SYS_MODE_PAUSE;
}

void sys_resume()
{
	if (mode == SYS_MODE_RUNNING)
		return;
	
}

void sys_rotate_revolver()
{
	
}

void sys_rotate_catcher()
{
	
}

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

void catcher_rotate_relative(float f_steps)
{
	uint16_t i;
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
