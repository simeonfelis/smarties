/**
 * @file smarties2.c
 * 
 * Copyright (c) 2008 Simeon Felis
 * 
 * @brief Entry file for Smarties project
 * 
 * @version 0.1
 * 
 * @mainpage Smarties2 software
 * 
 * @section intro License
 * 
 * GPL2 Licence
 * 
 * @section install Description
 * 
 * This is the Source code's API and flow documentation  for Smarties2
 * 
 * @section arch Architecture
 * 
 * The application entry point is located is the smarties2.c file.
 * - The main function first performs the initialization of inputs and outputs
 * - It handles the modes of the smartie sorter
 * - It handles the single steps of each mode
 * 
 * The LCD controlling is done with the lcd_display.h
 * 
 * The system related IO actions are all defined in system.h There are controlled
 * - moving the revolver
 * - moving the catcher
 * - user input controlls
 * 
 * Minor configurations are made in smarties2.h
 * 
 * The format conventions are like this:
 * - functions are descriptive names with several words divided by underscores written in small letters
 *   e.g. revolver_rotate_abolute()
 * - local variables TODO
 * - global variables TODO
 * - values which are set in defines are written in upper case letters e.g. REVOLVER_SIZE 
 * - makros are written with capitals for each word, e.g. CatcherEnable()
 * 
 */

#include "smarties2.h"
#include "system.h"
#include "inits.h"

system_mode mode;
system_step step;
//user_event event;
rotencoder rot_pos;

menu_entry entry0;
menu_entry entry1;
menu_entry entry2;
menu_entry entry3;
menu_entry entry30;
menu_entry entry31;
menu_entry entry32;

/**
  * \brief  Entry functin for smarties2
  *
  * This function controls the whole smarties machine by 
  * checking the current mode and performing the single 
  * steps required for the mode.
  * 
  * This function also handles the user inputs.
  * 
  * The subfunctions for driving the periphals are collected in system.h. The 
  * LCD menu is controlled by functions collected in lcd_display.h. 
  * 
  */
int main(void)
{	
	mode = SYS_MODE_INIT;
	menu_entry * current_menu = &entry0; 
	user_event event;
	uint8_t RevPos = 0;
	Smartie smartie[REVOLVER_SIZE];
	
	init_all();
    lcd_data('T');
    lcd_data('e');
    lcd_data('s');
    lcd_data('t');
 
    set_cursor(0,2);
 
    lcd_string("Hello World!");
 
    while(1)
    {
    	switch (mode) {
    	case SYS_MODE_INIT:
    		break;
    	case SYS_MODE_PAUSE:
			break;
		case SYS_MODE_RUNNING:
			if (step.I == SYS_STEP_AWAITED) {
				step.I = SYS_STEP_RUNNING;
				start_get_color_1();
				start_get_color_2();
				catcher_rotate_absolute(smartie[POS_SMARTIE_OUT-1].color);
			}
			if (step.I == SYS_STEP_RUNNING) {
//TODO:			if (get_color_1_complete && get_color_2_complete && catcher_rotate_complete) {
					make_color_merge (
						smartie[POS_SMARTIE_OUT-1].color1, 
						smartie[POS_SMARTIE_OUT-1].color2);			
					step.I = SYS_STEP_COMPLETED;
					step.II = SYS_STEP_AWAITED;				
//				}
			}
			if (step.II == SYS_STEP_AWAITED) {
				step.II = SYS_STEP_RUNNING;
				
				start_shaker();
				revolver_rotate_relative(1);
				RevPos++;
				if (RevPos == REVOLVER_SIZE)
					RevPos = 0;
				
				step.II = SYS_STEP_COMPLETED;
				step.III = SYS_STEP_AWAITED;
			}
			if (step.II == SYS_STEP_RUNNING) {
//TODO:			if (shaker_finished && revolver_rotate_complete) {
					step.II = SYS_STEP_COMPLETED;
					step.III = SYS_STEP_AWAITED;
//				}
			}
			if (step.III == SYS_STEP_AWAITED) {
				step.III = SYS_STEP_RUNNING;
				
				step.III = SYS_STEP_COMPLETED;
				step.I = SYS_STEP_AWAITED;
			}
    		break;
    	default:
			break;
		} // switch (state)
    	
    	event = get_user_action();
		switch (event) {
		case EV_PUSH_BUTTON:
			if (current_menu->function) {
				menu_action = current_menu->function;
				menu_action();
			}
			else if (current_menu->submenu) {
				current_menu = current_menu->submenu;
			}
			else if (current_menu->topmenu) {
				current_menu = current_menu->topmenu;
			}
			break;
		case EV_ROTATE_LEFT:
/*	Not implemented yet
 * 			if (current_menu->leftaction) {
				menu_action = current_menu->leftaction;
				menu_action();
			}
*/
			current_menu = current_menu->prev;
			break;
		case EV_ROTATE_RIGHT:
/*	Not implemented yet
 * 			if (current_menu->rightaction) {
				menu_action = current_menu->rightaction;
				menu_action();
			}
*/
			current_menu = current_menu->next;
			break;
		default:
			break;
		} // switch (event)
	} // while (1)
	return 0;
}
