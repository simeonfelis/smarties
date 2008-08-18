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
 * For color measurement have a look at \ref col_tab_blu in \ref system.c
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
	uint8_t temp = 1;
	
	int8_t index_temp;
	smartie_color col_temp;

	//smartie smart[REVOLVER_SIZE];
	
	char s[7];

	ss.state.mode = SYS_MODE_INIT;
	ss.state.mode_last = SYS_MODE_INIT;

	init_all();
	sei	();

	/* Wait for some init functions to be finished */
	while (temp) {
#if 1 /* catcher motor status output */
    				switch (ss.mot_catcher.status) {
    				case stat_idle:
    					lcd_gotoxy(12,1); lcd_puts("idle");
    					break;
    				case stat_start_working:
    					lcd_gotoxy(12,1); lcd_puts("start");
    					break;
    				case stat_working:
    					lcd_gotoxy(12,1); lcd_puts("worki");
    					break;
    				case stat_stop_working:
    					lcd_gotoxy(12,1); lcd_puts("stop");
    					break;
    				case stat_finished:
    					lcd_gotoxy(12,1); lcd_puts("finish");
    					break;
    				default:
    					break;
    				}
    				switch (ss.mot_catcher.status_last) {
    				case stat_idle:
    					lcd_gotoxy(18,1); lcd_puts("idle");
    					break;
    				case stat_start_working:
    					lcd_gotoxy(18,1); lcd_puts("start");
    					break;
    				case stat_working:
    					lcd_gotoxy(18,1); lcd_puts("worki");
    					break;
    				case stat_stop_working:
    					lcd_gotoxy(18,1); lcd_puts("stop");
    					break;
    				case stat_finished:
    					lcd_gotoxy(18,1); lcd_puts("finish");
    					break;    				
    				default:
    					break;
    				}
#endif
#if 1 /* revolver motor status output */
    				switch (ss.mot_revolver.status) {
    				case stat_idle:
    					lcd_gotoxy(0,1); lcd_puts("idle");
    					break;
    				case stat_start_working:
    					lcd_gotoxy(0,1); lcd_puts("start");
    					break;
    				case stat_working:
    					lcd_gotoxy(0,1); lcd_puts("worki");
    					break;
    				case stat_stop_working:
    					lcd_gotoxy(0,1); lcd_puts("stop");
    					break;
    				case stat_finished:
    					lcd_gotoxy(0,1); lcd_puts("finish");
    					break;
    				default:
    					break;
    				}
    				switch (ss.mot_revolver.status_last) {
    				case stat_idle:
    					lcd_gotoxy(6,1); lcd_puts("idle");
    					break;
    				case stat_start_working:
    					lcd_gotoxy(6,1); lcd_puts("start");
    					break;
    				case stat_working:
    					lcd_gotoxy(6,1); lcd_puts("worki");
    					break;
    				case stat_stop_working:
    					lcd_gotoxy(6,1); lcd_puts("stop");
    					break;
    				case stat_finished:
    					lcd_gotoxy(6,1); lcd_puts("finish");
    					break;    				
    				default:
    					break;
    				}
#endif
		if ( (ss.mot_revolver.status == stat_idle) 
				&& (ss.mot_catcher.status == stat_idle) ) {
			if ( (ss.mot_revolver.status_last != stat_idle) 
					&& (ss.mot_catcher.status_last != stat_idle) ) {
				ss.mot_catcher.status_last = stat_idle;
				ss.mot_revolver.status_last = stat_idle;
				lcd_puts("Init done");
				temp = 0;
			}
		}
	}

	/* initializing done, set state */
	ss.state.mode_last = ss.state.mode;
	ss.state.mode = SYS_MODE_PAUSE;

	/* update menu, give notice to user */
	menu_current = &men_lay_greeting[1];
	lcd_clrscr();
	lcd_gotoxy(0,0); lcd_puts(menu_current->text[0]);
	lcd_gotoxy(0,1); lcd_puts(menu_current->text[1]);
		
	while (1) /* Main loop */
	{
    	switch (ss.state.mode) {
    	case SYS_MODE_INIT:
    		break;
    	case SYS_MODE_PAUSE:
    		if (ss.state.mode_last != SYS_MODE_PAUSE) { /* if we just entered that mode */
    			menu_current = &men_lay_greeting[0];
    			/* if we come from a reset */
    			if (ss.state.mode_last == SYS_MODE_INIT)
        			menu_current = &men_lay_greeting[1];    				
    			lcd_gotoxy(0,0); lcd_puts(menu_current->text[0]);
    			lcd_gotoxy(0,1); lcd_puts(menu_current->text[1]);
    			ss.state.mode_last = SYS_MODE_PAUSE;
    			ss.sens_tcs.status_last = stat_idle;
    		}
    		
    		/* color measure program tcs */
    		if (ss.sens_tcs.status == stat_idle) {
    			if (ss.sens_tcs.status_last == stat_finished) {
    				ss.sens_tcs.status_last = stat_idle;
    				//lcd_gotoxy(0,1); lcd_puts(MEN_TIT_EMPTY);
    				lcd_gotoxy(0,1);
    				lcd_puts("Blue:");
    				if (ss.sens_tcs.filter_freq_blue<10) lcd_puts(" ");
    				lcd_puts(itoa(ss.sens_tcs.filter_freq_blue, s, 10));
    				lcd_puts(" Green:");
    				if (ss.sens_tcs.filter_freq_green<10) lcd_puts(" ");
    				lcd_puts(itoa(ss.sens_tcs.filter_freq_green, s, 10));
    				lcd_puts(" Red:");
    				if (ss.sens_tcs.filter_freq_red<10) lcd_puts("  ");
    				lcd_puts(itoa(ss.sens_tcs.filter_freq_red, s, 10));
    			}
    		}
    		
    		/* rotate program revolver */
    		if (ss.mot_revolver.status == stat_idle) {
    			if (ss.mot_revolver.status_last == stat_finished) {
    				ss.mot_revolver.status_last = stat_idle;
    				lcd_gotoxy(9,1); lcd_puts("Pos:");
    				if (ss.mot_revolver.current_pos<10) lcd_puts(" ");
    				lcd_puts(itoa(ss.mot_revolver.current_pos, s, 10));
    			}
    		}
    		
    		/* rotate program catcher */
    		if (ss.mot_catcher.status == stat_idle) {
    			if (ss.mot_catcher.status_last == stat_finished) {
    				ss.mot_revolver.status_last = stat_idle;
    				lcd_gotoxy(9,1); lcd_puts("Pos: ");
    				lcd_puts(itoa(ss.mot_catcher.current_pos, s, 10));
    			}
    		}
    		if (ss.mot_catcher.status != stat_idle) {
    			lcd_gotoxy(6,1);
    			if (ss.lb_catcher.passes > 9) lcd_puts(" ");
    			lcd_puts(itoa(ss.lb_catcher.passes,s, 10));
    		}
    		
    		break; /* SYS_MODE_PAUSE */
		case SYS_MODE_RUNNING:
			if (ss.state.mode_last != SYS_MODE_RUNNING) { /* if we just started to run */
				ss.state.mode_last = SYS_MODE_RUNNING;
				menu_current = &men_running;
				lcd_gotoxy(0,0); lcd_puts(menu_current->text[0]);
				lcd_gotoxy(0,1); lcd_puts(menu_current->text[1]);
				lcd_gotoxy(5,1); lcd_puts("RUN/  ");
				ss.state.step.I = stat_start_working;
				ss.state.step.II = stat_idle;
				ss.state.step.III = stat_idle;
			}
			if (ss.state.step.I == stat_start_working) {
				lcd_gotoxy(9,1);
				lcd_puts("I  ");
				ss.state.step.I = stat_working;
				/* Initiate all work to be done */
				sensor_tcs_get_color();
				/* Move the catcher to the right position. Therefore, */
				/* we need the color from the smartie which will be above the hole */
				/* Therefore, we need the correct index: */
				if ( (ss.rev.position + REV_POS_SMARTIE_OUT) > REV_MAX_SIZE )
					index_temp =  ss.rev.position - REV_MAX_SIZE + REV_POS_SMARTIE_OUT; 
				else
					index_temp = ss.rev.position + REV_POS_SMARTIE_OUT;
				col_temp = ss.rev.smart[index_temp].color;
				catcher_rotate_absolute(col_temp);
			}
			if (ss.state.step.I == stat_working) {
				if ( (ss.sens_tcs.status == stat_idle)
						&& (ss.mot_catcher.status == stat_idle)) {
					if ( (ss.sens_tcs.status_last == stat_finished)
							&& (ss.mot_catcher.status_last == stat_finished)) {
						ss.sens_tcs.status_last = stat_idle;
						ss.mot_catcher.status_last = stat_idle;
						ss.state.step.I = stat_stop_working;
						lcd_gotoxy(18,1); lcd_puts(MEN_COL_UNKNOWN);
					}
				}
			}
			if (ss.state.step.I == stat_stop_working) {
				ss.state.step.I = stat_idle;
				ss.state.step.II = stat_start_working;
			}
			break;
		default:
			break;
    	}
    		
		if (ss.rotenc.push) {
			ss.rotenc.push = 0;
			menu_action = (*menu_current).function;
    		menu_action();
			lcd_gotoxy(0,0); lcd_puts(menu_current->text[0]);
			lcd_gotoxy(0,1); lcd_puts(menu_current->text[1]);    			
		}
		if (ss.rotenc.left) {
			ss.rotenc.left = 0;
			menu_current = menu_current->prev;
			lcd_gotoxy(0,0); lcd_puts(menu_current->text[0]);
			lcd_gotoxy(0,1); lcd_puts(menu_current->text[1]);    			
		}
		if (ss.rotenc.right) {
			ss.rotenc.right = 0;
			menu_current = menu_current->next;
			lcd_gotoxy(0,0); lcd_puts(menu_current->text[0]);
			lcd_gotoxy(0,1); lcd_puts(menu_current->text[1]);    			
		}
	} /* Main loop end */
	
    while(1) /* deprecated */
    {
    	switch (ss.state.mode) {
		case SYS_MODE_RUNNING:
			
//			if (ss.state.step.I == SYS_STEP_RUNNING) 
			{
				// catcher preparation
				//ss.mot_catcher.targetPos = smartie[POS_SMARTIE_OUT-1].color;
				if (ss.mot_catcher.status == stat_idle)
					if (ss.mot_catcher.current_pos != ss.mot_catcher.target_pos)
						ss.mot_catcher.status = stat_start_working;	//will start rotating the catcher
				if (ss.mot_catcher.status == stat_working) // status working will be entered automatically
				{
					if (ss.lb_catcher.passes > 0)						//FIXME: check multiple passes
					{
						ss.mot_catcher.current_pos++;
						if (ss.mot_catcher.current_pos == col_unknown) // reset counter
							ss.mot_catcher.current_pos = 0;
						ss.lb_catcher.passes = 0;
					}
					if (ss.mot_catcher.current_pos == ss.mot_catcher.target_pos)
					{
						ss.mot_catcher.status_last = stat_working;
						ss.mot_catcher.status = stat_stop_working;
					}
				}
				
				// finishing step I, entering step II
//				if ((ss.mot_catcher.status == stat_finished) &&
//						(ss.col_sens_ADJD.status == stat_finished))
//				{
//					ss.col_sens_ADJD.status = stat_idle;
//					ss.mot_catcher.status = stat_idle;
//					ss.mot_catcher.status_last = stat_finished;
//					ss.state.step.I = SYS_STEP_COMPLETED;
//					ss.state.step.II = SYS_STEP_AWAITED;
//				}				
			} // if (ss.step.I == SYS_STEP_RUNNING) 
			
//			if (ss.state.step.II == SYS_STEP_AWAITED) 
//				ss.state.step.II = SYS_STEP_RUNNING;
							
//			if (ss.state.step.II == SYS_STEP_RUNNING) 
			{
				if (ss.shkr.status == stat_idle)
					ss.shkr.status = stat_working;
				if (ss.shkr.duration == 0)
					ss.shkr.status = stat_finished;
					

				revolver_rotate_relative(1);
//				RevPos++;
//				if (RevPos == REVOLVER_SIZE)
//					RevPos = 0;
				
				if (ss.shkr.status == stat_finished)
				{
					ss.shkr.status = stat_idle;
					ss.shkr.status_last = stat_idle;
//					ss.state.step.II = SYS_STEP_COMPLETED;
//					ss.state.step.III = SYS_STEP_AWAITED;
				}
			}
//			if (ss.state.step.III == SYS_STEP_AWAITED) 
			{
//				ss.state.step.III = SYS_STEP_RUNNING;
//				ss.state.step.III = SYS_STEP_COMPLETED;
//				ss.state.step.I = SYS_STEP_AWAITED;
			}
    		break;
    	default:
			break;
		} // switch (state)
	} // while (1)
	return 0;
}
