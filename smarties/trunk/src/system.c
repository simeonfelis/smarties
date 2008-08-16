/**
 * \file system.c
 * 
 * \brief Short routines for controlling the whole system and modules
 */

#include "smarties2.h"
#include "system.h"

extern smartie_sorter ss;

/**
 * \brief Will Enter the pause mode 
 * 
 * This function will enter the pause mode after the current function is finished. This function 
 * is usually called from the lcd menu. This function is only available in the 
 * mode \ref SYS_MODE_RUNNING
 */
void sys_pause()
{
	if (ss.state.mode == SYS_MODE_PAUSE)
		return;
	ss.state.mode_last = ss.state.mode;
	ss.state.mode = SYS_MODE_PAUSE;
}

/**
 * \brief Leave the pause mode
 * 
 * This function will leave the pause mode. This function 
 * is usually called from the lcd menu. This function is only available in the 
 * mode \ref SYS_MODE_PAUSE
 */
void sys_resume()
{
	if (ss.state.mode == SYS_MODE_RUNNING)
		return;
	ss.state.mode_last = ss.state.mode;
	ss.state.mode = SYS_MODE_RUNNING;
}

/**
 * \brief Initiate a color measurement with the tcs color sensor.
 */
void sys_measure_tcs () {
	ss.sens_tcs.status = stat_start_working;
}

/**
 * \brief Initiate a color measurement with the adjd color sensor.
 */
void sys_measure_adjd () {
	
}

/**
 * \brief Rotates the revolver for one position
 * 
 * This function is intened for manual usage and should not be used 
 * during normal running mode \ref SYS_MODE_RUNNING, only during 
 * \ref SYS_MODE_PAUSE. 
 */
void sys_rotate_revolver() {
	revolver_rotate_relative(1);
}

/**
 * \brief Rotates the catcher for one position
 * 
 * This function is intened for manual usage and should not be used 
 * during normal running mode \ref SYS_MODE_RUNNING, only during 
 * \ref SYS_MODE_PAUSE. 
 */
void sys_rotate_catcher() {
	catcher_rotate_relative(1);
}

//TODO: deprecated
/**
 * \brief Waits a time in microseconds
 * 
 * It executes a couple of nop's so that at any CPU speed it waits always waits 
 * about time in microseconds until it returns
 * 
 * \param time Time in microseconds
 */
void sys_wait(uint16_t time) {
	float f_cycles;
	uint16_t u_cycles;

	f_cycles = 0.000001 / F_CPU;

	f_cycles += 0.5;
	u_cycles = f_cycles; // round and convert

	do {
		do {
			asm volatile ("nop");
		}while (u_cycles--);
	} while (time--);
}

//TODO: docs
void vibrator_start() {
}

/**
 * \brief Initiates the color detection by the ADJD color sensor
 */
void sensor_adjd_get_color() {
	ss.sens_adjd.status = stat_start_working;
}

/**
 * \brief Initiates the color detection by the TCS color sensor
 */
void sensor_tcs_get_color() {
	ss.sens_tcs.status = stat_start_working;
}

/**
 * \brief Rotates the catcher to a certain position
 * 
 * This function will rotate the catcher to position specified 
 * by the a color \ref smartie_color_t.
 */
void catcher_rotate_absolute(smartie_color move_to) {
	int8_t move_rel = 0;
	
	if (move_to == ss.catch.position) {
		/* just indicate we have done something */
		ss.mot_catcher.status_last = stat_finished;
		return;
	}
	if (move_to > ss.catch.position)
		move_rel = move_to - ss.catch.position;
	if (move_to < ss.catch.position)
		move_rel = CATCH_MAX_SIZE - ss.catch.position - move_to;
	
	catcher_rotate_relative(move_rel);
}

/**
 * \brief Rotates the catcher from the current position to a relative next position
 * regarding 'hole above hole'
 * 
 * Most of the work is being done in \ref motor_stuff.
 * 
 * Warning: This function does not check if the catcher is already working. Before 
 * calling this function, check if the motor's status and last status 
 * equals \ref stat_idle. It also doesn't check the parameter, if the value is reasonable.
 * 
 * \param rel_pos The position where to move to. The value of rel_pos will be not 
 *                   checked, so the value must be lower than \ref REV_MAX_SIZE or 
 *                   \ref CATCH_MAX_SIZE. 
 */
void catcher_rotate_relative(int8_t rel_pos)
{
	if (rel_pos == 0) {
		/* Pretend we have done something */
		ss.mot_catcher.status_last = stat_finished;
		return;
	}
	
	if ( (ss.mot_catcher.current_pos + rel_pos) > CATCH_MAX_SIZE ) 
		ss.mot_catcher.target_pos = CATCH_MAX_SIZE - rel_pos;
	else 
		ss.mot_catcher.target_pos = ss.mot_catcher.current_pos + rel_pos;
	
	ss.mot_catcher.status = stat_start_working;
}

/**
 * \brief Rotates the revolver from the current position to a relative next position
 * regarding 'hole above hole'
 * 
 * Most of the work is being done in \ref motor_stuff.
 * 
 * Warning: This function does not check if the revolver is already working. Before 
 * calling this function, check if the motor's status and last status 
 * equals \ref stat_idle. It also doesn't check the parameter, if the value is reasonable.
 * 
 * \param rel_pos The position where to move to. The value of rel_pos will be not 
 *                checked, so the value must be lower than \ref REV_MAX_SIZE or 
 *                \ref CATCH_MAX_SIZE. 
 */
void revolver_rotate_relative(int8_t rel_pos) {
	if (rel_pos == 0) {
		/* Pretend we have done something */
		ss.mot_revolver.status_last = stat_finished;
		return;
	}
	
	if ( (ss.mot_revolver.current_pos + rel_pos) > REV_MAX_SIZE ) 
		ss.mot_revolver.target_pos = ss.mot_revolver.current_pos - REV_MAX_SIZE + rel_pos;
	else 
		ss.mot_revolver.target_pos = ss.mot_revolver.current_pos + rel_pos;
	
	ss.mot_revolver.status = stat_start_working;
}

//TODO: docs
smartie_color make_color_merge(smartie_color color1, smartie_color color2) {
	return 0; 
}

/**
 * \brief Color table reference blue for TCS measures.
 * 
 * These tables contain the ranges of the measurements with the 
 * TCS color sensor. These ranges are determined once by trying 
 * several light conditions. They represent the frequency of the 
 * TCS OUT pin each with the corresponding filter activated. This 
 * is a graph of the determined colors:
 * 
 * \image html color-reference-tcs.png "Color reference graph for TCS sensor"
 * \image latex color-reference-tcs.pdf "Color reference graph for TCS sensor" width=15cm
 * 
 * Smarties detected as \ref col_unknown, which is out of the
 * index of these color tables, will be treeted as empty placeholders
 */
uint8_t col_tab_blu [col_unknown][2] = {
		/* {blue_lo_limit, blue_hi_limit} */
		{0,0}, //!< Index \ref col_yellow
		{0,0}, //!< Index \ref col_red
		{0,0}, //!< Index \ref col_blue
		{0,0}, //!< Index \ref col_brown
		{0,0}, //!< Index \ref col_green
		{0,0} //!< Index \ref col_purple
};


/**
 * \brief See \ref col_tab_blu
 */
uint8_t col_tab_gre [col_unknown][2] = {
		/* {green_lo_limit, green_hi_limit} */
		{0,0}, /* COL_INDEX_YELLOW */
		{0,0}, /* COL_INDEX_RED */
		{0,0}, /* COL_INDEX_BLUE */
		{0,0}, /* COL_INDEX_BROWN */
		{0,0}, /* COL_INDEX_GREEN */
		{0,0} /* COL_INDEX_PURPLE */
};

/**
 * \brief See \ref col_tab_blu
 */
uint8_t col_tab_red [col_unknown][2] = {
		// {red_lo_limit, red_hi_limit}
		{0,0}, /* COL_INDEX_YELLOW */
		{0,0}, /* COL_INDEX_RED */
		{0,0}, /* COL_INDEX_BLUE */
		{0,0}, /* COL_INDEX_BROWN */
		{0,0}, /* COL_INDEX_GREEN */
		{0,0} /* COL_INDEX_PURPLE */
};
