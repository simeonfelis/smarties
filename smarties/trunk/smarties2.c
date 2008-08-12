/**
 * \file smarties2.c
 * 
 * Copyright (c) 2008 Simeon Felis
 * 
 * \brief Entry file for Smarties project
 * 
 * \version 0.1
 * 
 * \mainpage Smarties2 software
 * 
 * \section intro License
 * 
 * GPL2 Licence
 * 
 * \section install Description
 * 
 * This is the Source code's API and flow documentation  for Smarties2
 * 
 * \section arch Architecture
 * 
 * Goal of this application is a state machine controlled by status flags.
 * The main() function controls the program flow by reading and setting status
 * flags. These status flags are polled each millisecond in an timer interrupt 
 * routine. This timer interrupt routine reads and sets the IO ports and sets 
 * corresponding status flags.
 * 
 * Following image clarifies the structure of the software:
 * 
 * \image html api-layer.png "Layers of the software stack"
 * \image latex api-layer.png "Layers of the software stacke" width=15cm
 * 
 * The task of the differen layers, high level, abstraction layer and low level can be described
 * like in following image:
 * 
 * \image html state-machine.png "Executing the different layers"
 * \image latex state-machine.png "Executing the different layers" width=15cm
 * 
 * The state machine is devided into two sections, the mode and steps. Modes
 * are represented as an enum \ref system_mode_t and the steps as an struct \ref system_step_t
 * 
 * The modes are changed depending on the user inputs or after powering on/reseting. The 
 * next picture clarifies the modes of the state machine.
 * 
 * \image html state-machine-modes.png "State diagram of the smartie sorter" 
 * \image latex state-machine-modes.png "State diagram of the smartie sorter" width=10cm
 * 
 * The mode \ref SYS_MODE_RUNNING equals the automatic mode, where everything is 
 * controlled in several steps. The last step, step III, is a transition step to
 * begin from the start again. See next picture for the overview of the steps.
 * 
 * \image html state-machine-steps.png "Executing steps of the mode SYS_MODE_RUNNING"
 * \image latex state-machine-steps.png "Executing steps of the mode SYS_MODE_RUNNING" width=10cm
 * 
 * Each steps starts several tasks and waits until they are finished. Then the next
 * step will be entered. 
 * 
 * To see what is happening exactly in the different steps, please have a look at the 
 * sourcecode.
 * 
 * The modes, steps and all input/output related parts of the Smartie sorter are administrated
 * within struct. The structs are organized like in the following picture.
 * 
 * \image html objects-overview.png "Brief overview of several objects (elements) of the smartie sorter"
 * \image latex objects-overview.png "Brief overview of several objects (elements) of the smartie sorter" width=15cm
 * 
 * Note that this picture doesn't show all objects and all elements. It's just a draft overview.
 * For the detailed overview please refere to the code and documentation of \ref smartie_sorter_t. 
 * 
 * The application entry point is located is the \ref smarties2.c file.
 * - The main function first performs the initialization of inputs and outputs
 * - It handles the modes of the smartie sorter
 * - It handles the state machine
 * 
 * The LCD controlling is done with the \ref lcd_display.h
 * 
 * The Menu structure is described in \ref menu.h
 * 
 * The system related IO actions are all defined in \ref system.h There are controlled
 * - moving the revolver
 * - moving the catcher
 * - user input controlls
 * 
 * Minor configurations are made in \ref smarties2.h
 */

#include "smarties2.h"
#include "system.h"
#include "inits.h"
#include <avr/interrupt.h>

smartie_sorter ss;

menu_entry *menu_current;
menu_entry men_initializing;
menu_entry men_running;
menu_entry men_pausing;
menu_entry entries[3][3];


/**
  * \brief  Entry functin for smarties2
  *
  * This function controls the whole smarties machine by 
  * checking the current mode and performing the single 
  * steps required for the mode.
  * 
  * This function also handles the user inputs.
  * 
  * The subfunctions for driving the periphals are collected in \ref system.h. The 
  * LCD menu is controlled by functions collected in \ref lcd_display.h. 
  * 
  */
int main(void)
{	
	
	ss.state.mode = SYS_MODE_INIT;
	ss.state.modetmp = SYS_MODE_INIT;
	//menu_entry * current_menu = &entry0; 
	//user_event event;
	uint8_t RevPos = 0;
	Smartie smartie[REVOLVER_SIZE];

	init_all();
	sei();
	
	/* initializing done, set state */
	ss.state.modetmp = ss.state.mode;
	ss.state.mode = SYS_MODE_RUNNING;

	/* update menu, give notice to user */
	ss.menu = men_running;
	menu_current = &men_running;
	lcd_clrscr();
	lcd_puts(ss.menu.text);
		
	while (1) /* Testing loop */
	{
		/* handle user inputs, do corresponding menu function */
		if (ss.rotenc.push) {
			menu_action = ss.menu.function;
			menu_action();
			ss.rotenc.push = 0;
		}
    	if (ss.state.mode == SYS_MODE_PAUSE) {
			if (ss.state.modetmp != SYS_MODE_PAUSE) {
				lcd_clrscr();
				lcd_puts(MEN_TITLE_MAIN_MENU);
			}
			ss.state.modetmp = SYS_MODE_PAUSE;
		}
    	if (ss.lb_revolver.passes > 0) {
    		lcd_clrscr();
    		lcd_puts ("Revolver One pass");
    		ss.lb_revolver.passes = 0;
    	}
    	if (ss.lb_catcher.passes > 0) {
    		lcd_clrscr();
    		lcd_puts ("Catcher One pass");
    		ss.lb_catcher.passes = 0;
    	}
    	switch (ss.mot_revolver.status) {
    	case idle:
    		lcd_clrscr();
    		lcd_puts("Revolver idle");
    		break;
    	case start_working:
    		lcd_clrscr();
    		lcd_puts("Revolver starts");
    		break;
    	case working:
    		lcd_clrscr();
    		lcd_puts("Revolver works");
    		break;
    	case stop_working:
    		lcd_clrscr();
    		lcd_puts("Revolver stops");
    		break;
    	case finished:
    		lcd_clrscr();
    		lcd_puts("Revolver finished");
    		break;
    	default: 
    		break;
    	}
	} /* Testing loop end */
	
	
	init_all();
 
    while(1)
    {
    	switch (ss.state.mode) {
    	case SYS_MODE_INIT:
    		break;
    	case SYS_MODE_PAUSE:
    		ss.menu = men_pausing;
    		lcd_puts(ss.menu.text);
			break;
		case SYS_MODE_RUNNING:
			if (ss.state.step.I == SYS_STEP_AWAITED)
				ss.state.step.I = SYS_STEP_RUNNING;
			
			if (ss.state.step.I == SYS_STEP_RUNNING) 
			{
				// color sensor stuff
				if (ss.colSensor_ADJD.status == idle)
					ss.colSensor_ADJD.status = working;	//will start SPI detection
				if (ss.colSensor_ADJD.status == working)
					;//TODO: check if SPI reception complete, set col_sens_finished
				
				
				// catcher preparation
				ss.mot_catcher.targetPos = smartie[POS_SMARTIE_OUT-1].color;
				if (ss.mot_catcher.status == idle)
					if (ss.mot_catcher.currentPos != ss.mot_catcher.targetPos)
						ss.mot_catcher.status = start_working;	//will start rotating the catcher
				if (ss.mot_catcher.status == working) // status working will be entered automatically
				{
					if (ss.lb_catcher.passes > 0)						//FIXME: check multiple passes
					{
						ss.mot_catcher.currentPos++;
						if (ss.mot_catcher.currentPos == col_unknown) // reset counter
							ss.mot_catcher.currentPos = 0;
						ss.lb_catcher.passes = 0;
					}
					if (ss.mot_catcher.currentPos == ss.mot_catcher.targetPos)
					{
						ss.mot_catcher.status_tmp = working;
						ss.mot_catcher.status = stop_working;
					}
				}
				
				// finishing step I, entering step II
				if ((ss.mot_catcher.status == finished) &&
						(ss.colSensor_ADJD.status == finished))
				{
					ss.colSensor_ADJD.status = idle;
					ss.mot_catcher.status = idle;
					ss.mot_catcher.status_tmp = finished;
					ss.state.step.I = SYS_STEP_COMPLETED;
					ss.state.step.II = SYS_STEP_AWAITED;
				}				
			} // if (ss.step.I == SYS_STEP_RUNNING) 
			
			if (ss.state.step.II == SYS_STEP_AWAITED) 
				ss.state.step.II = SYS_STEP_RUNNING;
							
			if (ss.state.step.II == SYS_STEP_RUNNING) 
			{
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
					ss.shkr.statustmp = idle;
					ss.state.step.II = SYS_STEP_COMPLETED;
					ss.state.step.III = SYS_STEP_AWAITED;
				}
			}
			if (ss.state.step.III == SYS_STEP_AWAITED) 
			{
				ss.state.step.III = SYS_STEP_RUNNING;
				ss.state.step.III = SYS_STEP_COMPLETED;
				ss.state.step.I = SYS_STEP_AWAITED;
			}
    		break;
    	default:
			break;
		} // switch (state)
    	
    	//handle user inputs
    	if (ss.rotenc.push)
    	{
    		menu_action = (*menu_current).function;
    		menu_action();
    		ss.rotenc.push = 0;
    	}
	} // while (1)
	return 0;
}
