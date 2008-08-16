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

//TODO docs
void sys_measure_tcs () {
	ss.sens_tcs.status = stat_start_working;
}

//TODO docs
void sys_measure_adjd () {
	
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

void sensor_adjd_get_color() {
	ss.col_sens_ADJD.status = stat_start_working;
}

void sensor_tcs_get_color() {
	ss.sens_tcs.status = stat_start_working;
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

//TODO: Doku
void catcher_rotate_relative(uint16_t rel_pos)
{
	if (rel_pos == 0)
		return;
	
	if ( (ss.mot_catcher.currentPos + rel_pos) > CATCH_MAX_SIZE ) 
		ss.mot_catcher.targetPos = CATCH_MAX_SIZE - rel_pos;
	else 
		ss.mot_catcher.targetPos = ss.mot_catcher.currentPos + rel_pos;
	
	ss.mot_catcher.status = stat_start_working;
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

/**
 * \brief Rotates the revolver from the current position to a relative next position
 * regarding 'hole above hole'
 * 
 * Most of the work is being done in \ref motor_stuff.
 * 
 * Warning: This function does not check if the revolver is already working. Before 
 * calling this function, check if the revolver has the status \ref idle. It
 * also doesn't check the parameter, if the value is reasonable.
 * 
 * \param rel_pos  
 * The position where to move to. Only if one whole turn will be fullfilled, 
 * the target position will adjusted. The value of rel_pos will be not checked, so the 
 * value must be lower than \ref REV_MAX_SIZE. 
 */
void revolver_rotate_relative(uint8_t rel_pos)
{
	if (rel_pos == 0)
		return;
	
	if ( (ss.mot_revolver.currentPos + rel_pos) > REV_MAX_SIZE ) 
		ss.mot_revolver.targetPos = REV_MAX_SIZE - rel_pos;
	else 
		ss.mot_revolver.targetPos = ss.mot_revolver.currentPos + rel_pos;
	
	ss.mot_revolver.status = stat_start_working;
}

//TODO: docs
smartie_color make_color_merge(smartie_color color1, smartie_color color2)
{
	return 0; 
}

//TODO: docs
//{blue_lo_limit, blue_hi_limit}
uint8_t col_tab_blu [6][2] = {
		{0,0}, /* COL_INDEX_YELLOW */
		{0,0}, /* COL_INDEX_RED */
		{0,0}, /* COL_INDEX_BLUE */
		{0,0}, /* COL_INDEX_BROWN */
		{0,0}, /* COL_INDEX_GREEN */
		{0,0} /* COL_INDEX_PURPLE */
};

//{green_lo_limit, green_hi_limit}
uint8_t col_tab_gre [6][2] = {
		{0,0}, /* COL_INDEX_YELLOW */
		{0,0}, /* COL_INDEX_RED */
		{0,0}, /* COL_INDEX_BLUE */
		{0,0}, /* COL_INDEX_BROWN */
		{0,0}, /* COL_INDEX_GREEN */
		{0,0} /* COL_INDEX_PURPLE */
};

// {red_lo_limit, red_hi_limit}
uint8_t col_tab_red [6][2] = {
		{0,0}, /* COL_INDEX_YELLOW */
		{0,0}, /* COL_INDEX_RED */
		{0,0}, /* COL_INDEX_BLUE */
		{0,0}, /* COL_INDEX_BROWN */
		{0,0}, /* COL_INDEX_GREEN */
		{0,0} /* COL_INDEX_PURPLE */
};
