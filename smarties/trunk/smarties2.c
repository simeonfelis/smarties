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
#include <stdlib.h>

smartie_sorter ss;

menu_entry *menu_current;
menu_entry men_initializing;
menu_entry men_running;
menu_entry men_lay_greeting[2];
menu_entry men_lay_main[3];
menu_entry men_lay_sub_rotate[3];
menu_entry men_lay_sub_color[3];


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
int main(void) {
	uint8_t RevPos = 0;
	Smartie smartie[REVOLVER_SIZE];
	
	char s[7];

	ss.state.mode = SYS_MODE_INIT;
	ss.state.modetmp = SYS_MODE_INIT;

	init_all();
	sei();
	
	/* initializing done, set state */
	ss.state.modetmp = ss.state.mode;
	ss.state.mode = SYS_MODE_RUNNING;

	/* update menu, give notice to user */
	menu_current = &men_running;

	while (1) /* Testing loop */
	{
    	switch (ss.state.mode) {
    	case SYS_MODE_INIT:
    		break;
    	case SYS_MODE_PAUSE:
    		if (ss.state.modetmp != SYS_MODE_PAUSE) {
    			ss.state.modetmp = SYS_MODE_PAUSE;
    			menu_current = &men_lay_greeting[0];
    		}
    		/* color measure program */
    		if (ss.sens_tcs.status == stat_idle) {
    			if (ss.sens_tcs.status_last == stat_finished) {
    				ss.sens_tcs.status_last = stat_idle;
    				lcd_gotoxy(0,1);
    				lcd_puts("B:");
    				if (ss.sens_tcs.filter_freq_blue<10) lcd_puts(" ");
    				lcd_puts(itoa(ss.sens_tcs.filter_freq_blue, s, 10));
    				lcd_puts("G:");
    				if (ss.sens_tcs.filter_freq_green<10) lcd_puts(" ");
    				lcd_puts(itoa(ss.sens_tcs.filter_freq_green, s, 10));
    				lcd_puts("R:");
    				if (ss.sens_tcs.filter_freq_red<10) lcd_puts(" ");
    				lcd_puts(itoa(ss.sens_tcs.filter_freq_red, s, 10));
    			}
    		}
    		
			break;
		case SYS_MODE_RUNNING:
			break;
		default:
			break;
    	}
    		
		if (ss.rotenc.push) {
			ss.rotenc.push = 0;
			menu_action = (*menu_current).function;
    		menu_action();
		}
		if (ss.rotenc.left) {
			ss.rotenc.left = 0;
			menu_current = menu_current->prev;
		}
		if (ss.rotenc.right) {
			ss.rotenc.right = 0;
			menu_current = menu_current->next;
		}

#if 0
		switch (ss.mot_catcher.status) {
			case stat_idle:
			lcd_clrscr();
			lcd_puts("idle");
			break;
			case stat_start_working:
			lcd_clrscr();
			lcd_puts("starts");
			break;
			case stat_working:
			lcd_clrscr();
			lcd_puts("works");
			break;
			case stat_stop_working:
			lcd_clrscr();
			lcd_puts("stops");
			break;
			case stat_finished:
			lcd_clrscr();
			lcd_puts("finished");
			break;
			default:
			break;
		}
#endif 
	} /* Testing loop end */

	init_all();
 
    while(1)
    {
    	switch (ss.state.mode) {
    	case SYS_MODE_INIT:
    		break;
    	case SYS_MODE_PAUSE:
    		//TODO ss.menu = men_pausing;
    		lcd_puts(ss.menu->text[0]);
			break;
		case SYS_MODE_RUNNING:
			if (ss.state.step.I == SYS_STEP_AWAITED)
				ss.state.step.I = SYS_STEP_RUNNING;
			
			if (ss.state.step.I == SYS_STEP_RUNNING) 
			{
				// color sensor stuff
				if (ss.col_sens_ADJD.status == stat_idle)
					ss.col_sens_ADJD.status = stat_working;	//will start SPI detection
				if (ss.col_sens_ADJD.status == stat_working)
					;//TODO: check if SPI reception complete, set col_sens_finished
				
				
				// catcher preparation
				ss.mot_catcher.targetPos = smartie[POS_SMARTIE_OUT-1].color;
				if (ss.mot_catcher.status == stat_idle)
					if (ss.mot_catcher.currentPos != ss.mot_catcher.targetPos)
						ss.mot_catcher.status = stat_start_working;	//will start rotating the catcher
				if (ss.mot_catcher.status == stat_working) // status working will be entered automatically
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
						ss.mot_catcher.status_tmp = stat_working;
						ss.mot_catcher.status = stat_stop_working;
					}
				}
				
				// finishing step I, entering step II
				if ((ss.mot_catcher.status == stat_finished) &&
						(ss.col_sens_ADJD.status == stat_finished))
				{
					ss.col_sens_ADJD.status = stat_idle;
					ss.mot_catcher.status = stat_idle;
					ss.mot_catcher.status_tmp = stat_finished;
					ss.state.step.I = SYS_STEP_COMPLETED;
					ss.state.step.II = SYS_STEP_AWAITED;
				}				
			} // if (ss.step.I == SYS_STEP_RUNNING) 
			
			if (ss.state.step.II == SYS_STEP_AWAITED) 
				ss.state.step.II = SYS_STEP_RUNNING;
							
			if (ss.state.step.II == SYS_STEP_RUNNING) 
			{
				if (ss.shkr.status == stat_idle)
					ss.shkr.status = stat_working;
				if (ss.shkr.duration == 0)
					ss.shkr.status = stat_finished;
					

				revolver_rotate_relative(1);
				RevPos++;
				if (RevPos == REVOLVER_SIZE)
					RevPos = 0;
				
				if (ss.shkr.status == stat_finished)
				{
					ss.shkr.status = stat_idle;
					ss.shkr.statustmp = stat_idle;
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
    		menu_action = (*ss.menu->function);
    		menu_action();
    		ss.rotenc.push = 0;
    	}
	} // while (1)
	return 0;
}
