/**
 * \file system.c
 * 
 * \brief Short routines for controlling the whole system and modules
 */


#include <avr/eeprom.h>
#include "system.h"
#include "menu.h"

extern smartie_sorter ss;
extern menu_entry *menu_current;


/**
 * \brief Will set the current menu to the upper menu layer
 * 
 * This function is prepared especially for functions started from the 
 * menu. This function works only if the menu is correct initialized.
 */
void sys_enter_topmenu () {
	menu_current = menu_current->topmenu;
	ss.prog = prog_none; /* exit any program if we were executing one */
}

/**
 * \brief Will set the current menu to to lower menu layer
 * 
 * This function is prepared especially for functions started from the 
 * menu. This function works only if the menu is correct initialized.
 */
void sys_enter_submenu () {
	menu_current = menu_current->submenu;
	ss.prog = prog_none; /* exit any program if we were executing one */
}

/**
 * \brief Diables (power off) the catcher stepper motor
 * 
 * This function is prepared especially for functions started from the 
 * menu. This function works only if the menu is correct initialized.
 */
void sys_catcher_disable () {
	CATCH_DISABLE;
}

/**
 * \brief Enables (power on) the catcher stepper motor
 * 
 * This function is prepared especially for functions started from the 
 * menu. This function works only if the menu is correct initialized.
 */
void sys_catcher_enable () {
	CATCH_ENABLE;
}

/**
 * \brief Moves the catcher stepper motor for on step
 */
void sys_catcher_move_step () {
	CATCH_MOVE_STEP;
}

/**
 * \brief Rotates the catcher for one position
 * 
 * This function is intened for manual usage and should not be used 
 * during normal running mode \ref SYS_MODE_RUNNING, only during 
 * \ref SYS_MODE_PAUSE. 
 */
void sys_catcher_rotate() {
	if ( (ss.mot_catcher.status != stat_idle) && (ss.mot_catcher.status_last != stat_idle) )
		return;
	
	ss.prog = prog_rotate_catcher;
	catcher_rotate_relative(1);
}

/**
 * \brief Gets the correct out index of the next to drop smartie
 * 
 * \return The index of smartie in the revolver which will be dropped next
 */
uint8_t sys_get_out_pos () {
	uint8_t ret = ss.mot_revolver.current_pos + REV_POS_SMARTIE_OUT;
	if (ret >= 12)
		ret -= REV_MAX_SIZE;
	return ret;	
}

/**
 * \brief Returns status for the catcher lightbarrier
 *   
 * This function is prepared especially for functions started from the 
 * menu. This function works only if the menu is correct initialized.
 */
int8_t sys_catcher_is_lb_blocked () {
	return IS_LB_CATCHER;
}


void sys_revolver_disable () {
	REV_DISABLE;
}

void sys_revolver_enable () {
	REV_ENABLE;
}

/**
 * \brief Rotates the revolver for one position
 * 
 * This function is intened for manual usage and should not be used 
 * during normal running mode \ref SYS_MODE_RUNNING, only during 
 * \ref SYS_MODE_PAUSE. 
 */
void sys_revolver_rotate() {
	if ( (ss.mot_revolver.status != stat_idle) && (ss.mot_revolver.status_last != stat_idle) )
		return;
	ss.prog = prog_rotate_revolver;
	revolver_rotate_relative(1);
}

/**
 * \brief Moves the revolver for one step
 */
void sys_revolver_move_step () {
	REV_MOVE_STEP;
}

/**
 * \brief Returns the status for the revolver lightbarrier
 * 
 * This function is prepared especially for functions started from the 
 * menu. This function works only if the menu is correct initialized.
 */ 
int8_t sys_revolver_is_lb_blocked () {
	return IS_LB_REVOLVER;
}

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
 * \brief Initiates color callibration for corresponding smartie color 
 * 
 * This function is prepared especially for functions started from the 
 * menu. This function works only if the menu is correct initialized.
 */ 
void sys_reference_measure_blue () {
	ss.prog = prog_set_colors_blue;
}

/**
 * \brief See \ref sys_reference_measure_blue ()
 */
void sys_reference_measure_green() {
	ss.prog = prog_set_colors_green;
}

/**
 * \brief See \ref sys_reference_measure_blue ()
 */
void sys_reference_measure_red() {
	ss.prog = prog_set_colors_red;
}

/**
 * \brief See \ref sys_reference_measure_blue ()
 */
void sys_reference_measure_yellow() {
	ss.prog = prog_set_colors_yellow;	
}

/**
 * \brief See \ref sys_reference_measure_blue ()
 */
void sys_reference_measure_orange() {
	ss.prog = prog_set_colors_orange;
}

/**
 * \brief See \ref sys_reference_measure_blue ()
 */
void sys_reference_measure_brown() {
	ss.prog = prog_set_colors_brown;
}

/**
 * \brief See \ref sys_reference_measure_blue ()
 */
void sys_reference_measure_purple() {
	ss.prog = prog_set_colors_purple;
}

/**
 * \brief See \ref sys_reference_measure_blue ()
 */
void sys_reference_measure_pink() {
	ss.prog = prog_set_colors_pink;
}

/**
 * \brief Initiates to restore the color reference values
 * 
 * This function is prepared especially for functions started from the 
 * menu. This function works only if the menu is correct initialized.
 */ 
void sys_reference_measure_restore() {
	ss.prog = prog_set_colors_restore;
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
 * \brief Set the operating speed of the sorter to the current demand value 
 */ 
void sys_set_speed () {
	ss.mot_catcher.pause_duration = ss.speed;
	ss.mot_revolver.pause_duration = ss.speed;
	ss.vibr.duration = ss.speed + (ss.speed >> 1); /* Set to 1.5 of ss.speed (binary division by two) */
}

/**
 * \brief Set the operating speed of the sorter up by decreasing pause times 
 * 
 * This function is prepared especially for functions started from the 
 * menu. This function works only if the menu is correct initialized.
 */ 
void sys_speed_up () {
	ss.prog = prog_set_speed;
	if (ss.speed < 5000)
		ss.speed += 100;
	sys_set_speed ();
}

/**
 * \brief Set the operating speed of the sorter down by increasing pause times 
 * 
 * This function is prepared especially for functions started from the 
 * menu. This function works only if the menu is correct initialized.
 */ 
void sys_speed_down () {
	ss.prog = prog_set_speed;
	if (ss.speed > 100)
		ss.speed -= 100;
	sys_set_speed();
}

/**
 * \brief Initiate a color measurement with the tcs color sensor.
 */
void sys_measure_tcs () {
	ss.prog = prog_color_tcs;
	ss.sens_tcs.status = stat_start_working;
}

/**
 * \brief Initiate a color measurement with the adjd color sensor.
 */
void sys_measure_adjd () {
}

#if 0

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
#endif


/**
 * \brief Initiates the vibrator to start
 */
void vibrator_start() {
	ss.vibr.status = stat_start_working;
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
	
	if (move_to == ss.mot_catcher.current_pos) {
		/* just indicate we have done something */
		ss.mot_catcher.status_last = stat_finished;
		return;
	}
#if 0	/* now catcher which can handle unrecognized colors */
	if (move_to == col_unknown) { /* Treat as empty */
		/* Pretend we have done something */
		ss.mot_catcher.status_last = stat_finished;
		return;
	}
#endif
	
	if (move_to > ss.mot_catcher.current_pos)
		move_rel = move_to - ss.mot_catcher.current_pos;
	if (move_to <= ss.mot_catcher.current_pos)
		move_rel = CATCH_MAX_SIZE - ss.mot_catcher.current_pos + move_to;
	
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
void catcher_rotate_relative(int8_t rel_pos) {
	if (rel_pos == 0) {
		/* Pretend we have done something */
		ss.mot_catcher.status_last = stat_finished;
		return;
	}
	
	if ( (ss.mot_catcher.current_pos + rel_pos) >= CATCH_MAX_SIZE ) 
		ss.mot_catcher.target_pos = ss.mot_catcher.current_pos - CATCH_MAX_SIZE + rel_pos;
	else 
		ss.mot_catcher.target_pos = ss.mot_catcher.current_pos + rel_pos;
	
	//ss.mot_catcher.lb->passes = 0;
	
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
	
	if ( (ss.mot_revolver.current_pos + rel_pos) >= REV_MAX_SIZE ) 
		ss.mot_revolver.target_pos = ss.mot_revolver.current_pos - REV_MAX_SIZE + rel_pos;
	else 
		ss.mot_revolver.target_pos = ss.mot_revolver.current_pos + rel_pos;
	
	ss.mot_revolver.status = stat_start_working;
}

#if 0 /* deprecated ? */
//TODO: docs
smartie_color make_color_merge(smartie_color color1, smartie_color color2) {
	return 0; 
}
#endif

#if TABLE_REFERENCE_DETECTION

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
 * \image latex color-reference-tcs.png "Color reference graph for TCS sensor" width=15cm
 * 
 * The col_tab_* arrays can be accesed line by line, where each line has 
 * two collumns, for a low and high value. The line's index represents
 * the color when using the \ref smartie_color_t as variable type.
 * 
 * \code
uint8_t col_tab_blu [col_unknown][2] = {
		{LO,HI}, // Index col_blue
		{ 6, 9}, // Index col_green
		{ 6, 8}, // Index col_red
		{ 5, 5}, // Index ol_yellow
		{ 5, 7}, // Index col_orange
		{ 2, 4}, // Index col_brown
		{ 7, 9}, // Index col_purple
		{10,12} // Index col_pink
};
 * \endcode
 * 
 * Smarties detected as \ref col_unknown, which is out of the
 * index of these color tables, will be treeted as empty carriers.
 */
uint8_t col_tab_blu [col_unknown][2] = {
		/* {blue_lo_limit, blue_hi_limit} */
		{29,33}, //!< Index \ref col_blue
		{ 6, 9}, //!< Index \ref col_green
		{ 5, 7}, //!< Index \ref col_red
		{ 9,11}, //!< Index \ref col_yellow
		{ 5, 8}, //!< Index \ref col_orange
		{ 4, 8}, //!< Index \ref col_brown
		{ 7,12}, //!< Index \ref col_purple
		{12,15} //!< Index \ref col_pink
};

/**
 * \brief See \ref col_tab_blu
 */
uint8_t col_tab_gre [col_unknown][2] = {
		/* {green_lo_limit, green_hi_limit} */
		{24,28}, // Index \ref col_blue
		{20,23}, // Index \ref col_green
		{ 9,11}, // Index \ref col_red
		{32,35}, // Index \ref col_yellow
		{12,17}, // Index \ref col_orange
		{ 6,10}, // Index \ref col_brown
		{ 8,15}, // Index \ref col_purple
		{12,17} // Index \ref col_pink
};

/**
 * \brief See \ref col_tab_blu
 */
uint8_t col_tab_red [col_unknown][2] = {
		// {red_lo_limit, red_hi_limit}
		{30,34}, // Index \ref col_blue
		{19,23}, // Index \ref col_green
		{43,50}, // Index \ref col_red
		{37,45}, // Index \ref col_yellow
		{41,49}, // Index \ref col_orange
		{17,22}, // Index \ref col_brown
		{19,31}, // Index \ref col_purple
		{41,47} // Index \ref col_pink
};

uint8_t col_tab_non [col_unknown][2] = {
		{10,96}, // Index \ref col_blue
		{54,55}, // Index \ref col_green
		{56,67}, // Index \ref col_red
		{10,98}, // Index \ref col_yellow
		{64,80}, // Index \ref col_orange
		{31,44}, // Index \ref col_brown
		{46,51}, // Index \ref col_purple
		{73,81} // Index \ref col_pink		
};
#endif 

#if DISTANCE_DETECTION | DISTANCE_NORM_DETECTION

color_avarage col_ava_blu;
color_avarage col_ava_gre;
color_avarage col_ava_red;

#endif