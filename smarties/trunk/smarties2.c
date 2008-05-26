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
 * Prefixes
 * \todo doku Prefixes
 * 
 */

#include "smarties2.h"
#include "system.h"
#include "inits.h"

//system_mode mode;
//system_step step;
//user_event event;

smartie_sorter ss;

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
	ss.mode = SYS_MODE_INIT;
	menu_entry * current_menu = &entry0; 
	//user_event event;
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
    	switch (ss.mode) {
    	case SYS_MODE_INIT:
    		break;
    	case SYS_MODE_PAUSE:
			break;
		case SYS_MODE_RUNNING:
			if (ss.step.I == SYS_STEP_AWAITED)
				ss.step.I = SYS_STEP_RUNNING;
			
			if (ss.step.I == SYS_STEP_RUNNING) 
			{
				// color sensor stuff
				if (ss.colSensor_ADJD.status == idle)
					ss.colSensor_ADJD.status = working;	//will start SPI detection
				if (ss.colSensor_ADJD.status == working)
					;//TODO: check if SPI reception complete, set col_sens_finished
				
				
				// catcher preparation
				ss.catcher_Engine.targetPos = smartie[POS_SMARTIE_OUT-1].color;
				if (ss.catcher_Engine.status == engineStat_stop)
					if (ss.catcher_Engine.currentPos != ss.catcher_Engine.targetPos)
						ss.catcher_Engine.status = engineStat_rotate;	//will start rotating the catcher
				if (ss.catcher_Engine.status == engineStat_rotate)
				{
					if (ss.catcher_LB.passes > 0)						//FIXME: check multiple passes
					{
						ss.catcher_Engine.currentPos++;
						if (ss.catcher_Engine.currentPos == col_unknown) // reset counter
							ss.catcher_Engine.currentPos = 0;
						ss.catcher_LB.passes = 0;
					}
					if (ss.catcher_Engine.currentPos == ss.catcher_Engine.targetPos)
						ss.catcher_Engine.status = engineStat_stop;
				}
				
				// finishing step I, entering step II
				if ((ss.catcher_Engine.status == engineStat_stop) &&
						(ss.colSensor_ADJD.status == finished))
				{
					ss.colSensor_ADJD.status = idle;
					ss.step.I = SYS_STEP_COMPLETED;
					ss.step.II = SYS_STEP_AWAITED;
				}				
			} // if (ss.step.I == SYS_STEP_RUNNING) 
			
			if (ss.step.II == SYS_STEP_AWAITED) 
				ss.step.II = SYS_STEP_RUNNING;
							
			if (ss.step.II == SYS_STEP_RUNNING) {
				if (ss.shkr.status == idle)
					ss.shkr.status = working;
				if (ss.shkr.duration == 0)
					ss.shkr.status = finished;
					

				revolver_rotate_relative(1);
				RevPos++;
				if (RevPos == REVOLVER_SIZE)
					RevPos = 0;
				
				if (ss.shkr.status == finished)
				{
					ss.shkr.status = idle;
					ss.step.II = SYS_STEP_COMPLETED;
					ss.step.III = SYS_STEP_AWAITED;
				}
			}
			if (ss.step.III == SYS_STEP_AWAITED) {
				ss.step.III = SYS_STEP_RUNNING;
				
				ss.step.III = SYS_STEP_COMPLETED;
				ss.step.I = SYS_STEP_AWAITED;
			}
    		break;
    	default:
			break;
		} // switch (state)
    	
    	//handle user inputs
    	if (ss.mode == SYS_MODE_RUNNING)
    		if (ss.rotenc.push)
    		{
    			ss.mode = SYS_MODE_PAUSE;
    			ss.rotenc.push = 0;
    		}
    	if (ss.mode == SYS_MODE_PAUSE)
    	{
    		//do stuff with menu
    	}
    	
	} // while (1)
	return 0;
}
