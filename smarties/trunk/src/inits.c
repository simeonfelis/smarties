/**
 * \file inits.c
 * 
 * Copyright (c) 2008 Simeon Felis
 * 
 * \brief Some init functions.
 * 
 * 
 */
#include "smarties2.h"
#include "lcd_display.h"
#include "inits.h"
#include "system.h"
#include "menu.h"
#include "ee.h"

#if DISTANCE_DETECTION | DISTANCE_NORM_DETECTION

extern float col_ava_blu [col_unknown];
extern float col_ava_gre [col_unknown];
extern float col_ava_red [col_unknown];

#endif

extern smartie_sorter ss;
extern menu_entry *menu_current;



void init_all() {
	
	init_memory ();
	
	init_io();
	lcd_init(LCD_DISP_ON);
	init_menu();
		lcd_clrscr();
		lcd_puts(menu_current->text[0]);
	init_timer();
//	init_interrupts();
	//i2c_init();
	//	ss.col_sens_ADJD.ret = i2c_start(COL_SENS_ADJD_DEVICE_ADDRESS + I2C_WRITE);
	init_sensor_tcs();
	init_motors();	
}

void init_io()
{
	
    /*
     *  sorry to say, the lcd is on the jtag pins
     */
	MCUCSR |= (1<<JTD);	
	MCUCSR |= (1<<JTD);	/* disable the JTAG interface twice! */

	STEPPER_PORT &= ~((1 << REV_BIT_CLK) | (1<<REV_BIT_CW) | (1<<REV_BIT_EN) | (1<<CATCH_BIT_CLK) | (1<<CATCH_BIT_CW) | (1<<CATCH_BIT_EN));
	STEPPER_DDR |= (1 << REV_BIT_CLK) | (1<<REV_BIT_CW) | (1<<REV_BIT_EN) | (1<<CATCH_BIT_CLK) | (1<<CATCH_BIT_CW) | (1<<CATCH_BIT_EN);
	// STEPPER_PORT |= (1<<REV_BIT_CLK) | (1<<CATCH_BIT_CLK); /* The clock signal should be high by default */

	/* ADJD color sensor */
	COL_SENS_ADJD_LED_OFF;  /* LED by default of */
	COL_SENS_ADJD_LED_DDR |= (1<<COL_SENS_ADJD_LED_BIT);
	/* some of the inits is done in the i2cmaster library */
	
	/* the TCS color sensor */
	COL_SENS_TCS_OUT_PORT &= ~(
			(1<<COL_SENS_TCS_S0_BIT) | (1<<COL_SENS_TCS_S1_BIT) | 
			(1<<COL_SENS_TCS_S2_BIT) | (1<<COL_SENS_TCS_S3_BIT));
	COL_SENS_TCS_OUT_PORT |= (1<<COL_SENS_TCS_OE_BIT); /* active low */
	COL_SENS_TCS_OUT_DDR |= 
		(1<<COL_SENS_TCS_S0_BIT) | (1<<COL_SENS_TCS_S1_BIT) | 
		(1<<COL_SENS_TCS_S2_BIT) | (1<<COL_SENS_TCS_S3_BIT) |
		(1<<COL_SENS_TCS_OE_BIT);

	COL_SENS_TCS_IN_DDR &= ~(1<<COL_SENS_TCS_IN_ICP);
	
	/* interrupt on falling edge for TCS frequency measurement */
	MCUCR &= ~((1<<ISC00) | (1<<ISC01));
	MCUCR |= (1<<ISC01);
	
	/* rotatry encoder */
	ROTENC_INIT();
	
	/* get current rotary encoder position */
	if (IS_ROTENC_A)
		ss.rotenc.rottmp = ROTENC_A;
	if (IS_ROTENC_B)
		ss.rotenc.rottmp = ROTENC_B;
	if (IS_ROTENC_AB)
		ss.rotenc.rottmp = ROTENC_BOTH;
	if (IS_ROTENC_NONE)
		ss.rotenc.rottmp = ROTENC_NONE;
	
	/* lightbarrier */
	LB_DDR &= ~((1<<LB_BIT_CATCH) | (1<<LB_BIT_REV));
	
	/* Vibrator */
	VIBR_DDR |= (1<<VIBR_BIT);
}

void init_interrupts() {
}

void init_sensor_tcs() {
	COL_SENS_TCS_DISABLE;
	COL_SENS_TCS_SET_FREQ_SCALE(100);
}

/**
 * \brief This will generate an Interrupt every millisecond by Timer 0 on 
 * compare match
 * 
 * The interrupt settings are related like following:
 * \f[
 * T_{Compare Match} = (F_{CPU})^{-1} \cdot Prescaler \cdot (Register_{OutputCompare}) 
 * \f]
 */
void init_timer() {
	/* Output compare register */
	OCR0 = 250;
	
	/* Prescaler 64 */
	TCCR0 &= ~((1<<CS00) | (1<<CS01) | (1<<CS02));
	TCCR0 |= (1<<CS00) | (1<<CS01);

	/* CTC mode */
	TCCR0 &= ~((1<<WGM00) | (1<<WGM00));
	TCCR0 |= (1<<WGM01);

	/* enable compare match interrupt */
	TIMSK |= (1<<OCIE0);
	
}

void init_memory () {
	uint8_t x;
	
	/* init revolver */
	for (x=0; x<REV_MAX_SIZE; x++)
		ss.rev.smart[x].color = col_unknown;
	
	ss.speed = 750; /* default speed */

#if DISTANCE_DETECTION | DISTANCE_NORM_DETECTION
	eeprom_read_block (col_ava_blu, ee_mem.usr_blu, sizeof(color_avarage));
	eeprom_read_block (col_ava_gre, ee_mem.usr_gre, sizeof(color_avarage));
	eeprom_read_block (col_ava_red, ee_mem.usr_red, sizeof(color_avarage));
#endif
	
	
}

/**
 * \brief Will throw out remaining smarties and put revolver and catcher to 
 * defined positions
 */
void init_motors() {
	/***************** C A T C H E R **********************/
	
	ss.mot_catcher.status = stat_idle;
	ss.mot_catcher.status_last = stat_idle;

	ss.mot_catcher.enable = sys_catcher_enable;
	ss.mot_catcher.disable = sys_catcher_disable;
	ss.mot_catcher.move_step = sys_catcher_move_step;
	ss.mot_catcher.lb = &ss.lb_catcher;
	ss.mot_catcher.lb->is_blocked = sys_catcher_is_lb_blocked;
	
	ss.mot_catcher.max_size = CATCH_MAX_SIZE;
	ss.mot_catcher.step_duration = CATCH_STEP_DURATION;
	ss.mot_catcher.steps_estimated = CATCH_STEPS_ESTIMATED;
	ss.mot_catcher.steps_estim_def = CATCH_STEPS_ESTIMATED;
	ss.mot_catcher.ramp_duration = CATCH_RAMP_DURATION;
//	ss.mot_catcher.pause_duration = CATCH_PAUSE_DURATION;
	ss.mot_catcher.pause_duration = ss.speed;

	
	CATCH_SET_CW;
	CATCH_ENABLE;
	
	/* find the next defined position */
	if (!IS_LB_CATCHER)
		catcher_rotate_relative(1);

	ss.mot_catcher.current_pos = 0;
	
	
	/*************** R E V O L V E R *********************/

	ss.mot_revolver.status = stat_idle;
	ss.mot_revolver.status_last = stat_idle;
	
	ss.mot_revolver.enable = sys_revolver_enable;
	ss.mot_revolver.disable = sys_revolver_disable;
	ss.mot_revolver.move_step = sys_revolver_move_step;
	ss.mot_revolver.lb = &ss.lb_revolver;
	ss.mot_revolver.lb->is_blocked = sys_revolver_is_lb_blocked;
	
	ss.mot_revolver.max_size = REV_MAX_SIZE;
	ss.mot_revolver.step_duration = REV_STEP_DURATION;
	ss.mot_revolver.steps_estimated = REV_STEPS_ESTIMATED;
	ss.mot_revolver.steps_estim_def = REV_STEPS_ESTIMATED;
	ss.mot_revolver.ramp_duration = REV_RAMP_DURATION;
//	ss.mot_revolver.pause_duration = REV_PAUSE_DURATION;
	ss.mot_revolver.pause_duration = ss.speed;
		
	REV_SET_CCW;
	REV_ENABLE;
	ss.mot_revolver.current_pos = 0;
	ss.lb_revolver.passes = 0;
	/* find the next defined position */
	if (!IS_LB_REVOLVER)
		revolver_rotate_relative(1); 

}

/**
 * \brief Creates the menu stucture and connections
 * 
 * The menu structure and functionality is explained in \ref menu.h in 
 * detailed.
 */
void init_menu() {
	extern menu_entry *menu_current;
	extern menu_entry men_initializing;
	extern menu_entry men_running;
	extern menu_entry men_lay_greeting[2];
	extern menu_entry men_lay_main[MEN_LAY_MAIN_SIZE];
	extern menu_entry men_lay_speed;
	extern menu_entry men_lay_reference [col_unknown + 1];
	
	men_initializing.text[0] = MEN_TIT_INITIALIZING;
	
	men_running.text[0] = MEN_TIT_RUNNING;
	men_running.text[1] = MEN_SUBTIT_RUNNING;
	men_running.next = &men_running;
	men_running.prev = &men_running;
	men_running.function = sys_pause;
	
	men_lay_greeting[0].text[0] = MEN_TIT_GREETING;
	men_lay_greeting[0].text[1] = MEN_SUBTIT_PAUSE;
	men_lay_greeting[0].next = &men_lay_greeting[1];
	men_lay_greeting[0].prev = &men_lay_greeting[1];
	men_lay_greeting[0].submenu = &men_lay_main[0];
	men_lay_greeting[0].function = sys_enter_submenu;
	
	men_lay_greeting[1].text[0] = MEN_TIT_RESUME;
	men_lay_greeting[1].text[1] = MEN_SUBTIT_PAUSE;
	men_lay_greeting[1].next = &men_lay_greeting[0];
	men_lay_greeting[1].prev = &men_lay_greeting[0];
	men_lay_greeting[1].function = sys_resume;
	
	men_lay_main[0].text[0] = MEN_TIT_ROT_REV;
	men_lay_main[0].text[1] = MEN_SUBTIT_PAUSE;
	men_lay_main[0].next = &men_lay_main[1];
	men_lay_main[0].prev = &men_lay_main[3];
	men_lay_main[0].function = sys_revolver_rotate;
	
	men_lay_main[1].text[0] = MEN_TIT_ROT_CATCH;
	men_lay_main[1].text[1] = MEN_SUBTIT_PAUSE;
	men_lay_main[1].next = &men_lay_main[2];
	men_lay_main[1].prev = &men_lay_main[0];
	men_lay_main[1].function = sys_catcher_rotate;
	
	men_lay_main[2].text[0] = MEN_TIT_SUB_TCS;
	men_lay_main[2].text[1] = MEN_SUBTIT_PAUSE;
	men_lay_main[2].next = &men_lay_main[3];
	men_lay_main[2].prev = &men_lay_main[1];
	men_lay_main[2].function = sys_measure_tcs;
	
	men_lay_main[3].text[0] = MEN_TIT_SPEED;
	men_lay_main[3].text[1] = MEN_SUBTIT_PAUSE;
	men_lay_main[3].next = &men_lay_main[4];
	men_lay_main[3].prev = &men_lay_main[2];
	men_lay_main[3].submenu = &men_lay_speed;
	men_lay_main[3].function = sys_enter_submenu;
	
	men_lay_main[4].text[0] = MEN_TIT_REFERENCE;
	men_lay_main[4].text[1] = MEN_SUBTIT_PAUSE;
	men_lay_main[4].next = &men_lay_main[5];
	men_lay_main[4].prev = &men_lay_main[3];
	men_lay_main[4].submenu = &men_lay_reference[0];
	men_lay_main[4].function = sys_enter_submenu;
	
	men_lay_main[5].text[0] = MEN_TIT_BACK;
	men_lay_main[5].text[1] = MEN_SUBTIT_PAUSE;
	men_lay_main[5].next = &men_lay_main[0];
	men_lay_main[5].prev = &men_lay_main[4];
	men_lay_main[5].topmenu = &men_lay_greeting[0];
	men_lay_main[5].function = sys_enter_topmenu;
	
	men_lay_speed.text[0] = MEN_TIT_SPEED;
	men_lay_speed.text[1] = MEN_TIT_EMPTY;
	men_lay_speed.r_action = sys_speed_up;
	men_lay_speed.l_action = sys_speed_down;
	men_lay_speed.topmenu = &men_lay_main[3];
	men_lay_speed.function = sys_enter_topmenu;
	
	men_lay_reference[0].text[0] = MEN_TIT_SUB_R_BLUE;
	men_lay_reference[0].text[1] = MEN_SUBTIT_REFERENCE;
	men_lay_reference[0].next = &men_lay_reference[1];
	men_lay_reference[0].prev = &men_lay_reference[col_unknown];
	men_lay_reference[0].function = sys_reference_measure_blue;
	
	men_lay_reference[1].text[0] = MEN_TIT_SUB_R_GREEN;
	men_lay_reference[1].text[1] = MEN_SUBTIT_REFERENCE;
	men_lay_reference[1].next = &men_lay_reference[2];
	men_lay_reference[1].prev = &men_lay_reference[0];
	men_lay_reference[1].function = sys_reference_measure_green;

	men_lay_reference[2].text[0] = MEN_TIT_SUB_R_RED;
	men_lay_reference[2].text[1] = MEN_SUBTIT_REFERENCE;
	men_lay_reference[2].next = &men_lay_reference[3];
	men_lay_reference[2].prev = &men_lay_reference[1];
	men_lay_reference[2].function = sys_reference_measure_red;

	men_lay_reference[3].text[0] = MEN_TIT_SUB_R_YELLOW;
	men_lay_reference[3].text[1] = MEN_SUBTIT_REFERENCE;
	men_lay_reference[3].next = &men_lay_reference[4];
	men_lay_reference[3].prev = &men_lay_reference[2];
	men_lay_reference[3].function = sys_reference_measure_yellow;

	men_lay_reference[4].text[0] = MEN_TIT_SUB_R_ORANGE;
	men_lay_reference[4].text[1] = MEN_SUBTIT_REFERENCE;
	men_lay_reference[4].next = &men_lay_reference[5];
	men_lay_reference[4].prev = &men_lay_reference[3];
	men_lay_reference[4].function = sys_reference_measure_orange;

	men_lay_reference[5].text[0] = MEN_TIT_SUB_R_BROWN;
	men_lay_reference[5].text[1] = MEN_SUBTIT_REFERENCE;
	men_lay_reference[5].next = &men_lay_reference[6];
	men_lay_reference[5].prev = &men_lay_reference[4];
	men_lay_reference[5].function = sys_reference_measure_brown;

	men_lay_reference[6].text[0] = MEN_TIT_SUB_R_PURPLE;
	men_lay_reference[6].text[1] = MEN_SUBTIT_REFERENCE;
	men_lay_reference[6].next = &men_lay_reference[7];
	men_lay_reference[6].prev = &men_lay_reference[5];
	men_lay_reference[6].function = sys_reference_measure_purple;
	
	men_lay_reference[7].text[0] = MEN_TIT_SUB_R_PINK;
	men_lay_reference[7].text[1] = MEN_SUBTIT_REFERENCE;
	men_lay_reference[7].next = &men_lay_reference[8];
	men_lay_reference[7].prev = &men_lay_reference[6];
	men_lay_reference[7].function = sys_reference_measure_pink;
	
	men_lay_reference[8].text[0] = MEN_TIT_BACK;
	men_lay_reference[8].text[1] = MEN_SUBTIT_PAUSE;
	men_lay_reference[8].next = &men_lay_reference[0];
	men_lay_reference[8].prev = &men_lay_reference[7];
	men_lay_reference[8].topmenu = &men_lay_main[0];
	men_lay_reference[8].function = sys_enter_topmenu;



	menu_current = &men_initializing;
}











