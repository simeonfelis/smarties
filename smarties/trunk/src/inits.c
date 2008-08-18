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
#include "i2cmaster.h"

extern smartie_sorter ss;
extern menu_entry *menu_current;

void init_adc()
{	
}

void init_all() {
	uint8_t x;
	init_io();
	lcd_init(LCD_DISP_ON);
	init_menu();
		lcd_clrscr();
		lcd_puts(menu_current->text[0]);
	init_timer();
	init_interrupts();
	//i2c_init();
	//	ss.col_sens_ADJD.ret = i2c_start(COL_SENS_ADJD_DEVICE_ADDRESS + I2C_WRITE);
	init_sensor_tcs();
	init_motors();
	
	/* init memory */
	for (x=0; x<REV_MAX_SIZE; x++)
		ss.rev.smart[x].color = col_unknown;
	ss.mot_catcher.current_pos = 0;
	ss.mot_revolver.current_pos = col_yellow; /* col_yellow is in the smart_color enum 0 */
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
	
	/* interrupt on falling edge */
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
void init_timer()
{
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

/**
 * \brief Will throw out remaining smarties and put revolver and catcher to 
 * defined positions
 */
void init_motors()
{
	/***************** C A T C H E R **********************/

	ss.mot_catcher.status = stat_idle;
	ss.mot_catcher.status_last = stat_idle;
	
	CATCH_SET_CW;
	
	/* find the next defined position */
	if (!IS_LB_CATCHER)
		catcher_rotate_relative(1);
	
	
	/*************** R E V O L V E R *********************/
	
	ss.mot_revolver.status = stat_idle;
	ss.mot_revolver.status_last = stat_idle;
	
	REV_SET_CCW;
		
	/* find the next defined position */
	if (!IS_LB_REVOLVER)
		revolver_rotate_relative(1); 
		/* this will put the revolver to a positon 'hole above hole' */
	
	/* this will throw out eventually remaining smarties */ 
	//revolver_rotate_relative(12);
	
	
	/* last, rotate the catcher to a position where it is possible 
	 * to remove the possibly remaining smarties */
	//catcher_rotate_relative(4);
}

/**
 * \brief Creates the menu stucture and connections
 * 
 * The menu structure and functionality is explained in \ref menu.h in 
 * detailed.
 */
void init_menu()
{
	extern menu_entry *menu_current;
	extern menu_entry men_initializing;
	extern menu_entry men_running;
	extern menu_entry men_lay_greeting[2];
	extern menu_entry men_lay_main[3];
	extern menu_entry men_lay_sub_rotate[3];
	extern menu_entry men_lay_sub_color[3];
	
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
	men_lay_greeting[0].function = menu_enter_submenu;
	
//	men_lay_greeting[1].text[0] = MEN_TIT_BACK;
	men_lay_greeting[1].text[0] = MEN_TIT_RESUME;
	men_lay_greeting[1].text[1] = MEN_SUBTIT_PAUSE;
	men_lay_greeting[1].next = &men_lay_greeting[0];
	men_lay_greeting[1].prev = &men_lay_greeting[0];
	men_lay_greeting[1].function = sys_resume;
	
	men_lay_main[0].text[0] = MEN_TIT_MAIN_ROTATE;
	men_lay_main[0].text[1] = MEN_SUBTIT_PAUSE;
	men_lay_main[0].next = &men_lay_main[1];
	men_lay_main[0].prev = &men_lay_main[2];
	men_lay_main[0].submenu = &men_lay_sub_rotate[0];
	men_lay_main[0].function = menu_enter_submenu;
	
	men_lay_main[1].text[0] = MEN_TIT_MAIN_COLOR;
	men_lay_main[1].text[1] = MEN_SUBTIT_PAUSE;
	men_lay_main[1].next = &men_lay_main[2];
	men_lay_main[1].prev = &men_lay_main[0];
	men_lay_main[1].submenu = &men_lay_sub_color[0];
	men_lay_main[1].function = menu_enter_submenu;
	
	men_lay_main[2].text[0] = MEN_TIT_BACK;
	men_lay_main[2].text[1] = MEN_SUBTIT_PAUSE;
	men_lay_main[2].next = &men_lay_main[0];
	men_lay_main[2].prev = &men_lay_main[1];
	men_lay_main[2].topmenu = &men_lay_greeting[0];
	men_lay_main[2].function = menu_enter_topmenu;
	
	men_lay_sub_rotate[0].text[0] = MEN_TIT_SUB_ROT_REV;
	men_lay_sub_rotate[0].text[1] = MEN_SUBTIT_PAUSE;
	men_lay_sub_rotate[0].next = &men_lay_sub_rotate[1];
	men_lay_sub_rotate[0].prev = &men_lay_sub_rotate[2];
	men_lay_sub_rotate[0].function = sys_rotate_revolver;
	
	men_lay_sub_rotate[1].text[0] = MEN_TIT_SUB_ROT_CATCH;
	men_lay_sub_rotate[1].text[1] = MEN_SUBTIT_PAUSE;
	men_lay_sub_rotate[1].next = &men_lay_sub_rotate[2];
	men_lay_sub_rotate[1].prev = &men_lay_sub_rotate[0];
	men_lay_sub_rotate[1].function = sys_rotate_catcher;
	
	men_lay_sub_rotate[2].text[0] = MEN_TIT_BACK;
	men_lay_sub_rotate[2].text[1] = MEN_SUBTIT_PAUSE;
	men_lay_sub_rotate[2].next = &men_lay_sub_rotate[0];
	men_lay_sub_rotate[2].prev = &men_lay_sub_rotate[1];
	men_lay_sub_rotate[2].topmenu = &men_lay_main[0];
	men_lay_sub_rotate[2].function = menu_enter_topmenu;
	
	men_lay_sub_color[0].text[0] = MEN_TIT_SUB_TCS;
	men_lay_sub_color[0].text[1] = MEN_SUBTIT_COLOR;
	men_lay_sub_color[0].next = &men_lay_sub_color[1];
	men_lay_sub_color[0].prev = &men_lay_sub_color[2];
	men_lay_sub_color[0].function = sys_measure_tcs;
	
	men_lay_sub_color[1].text[0] = MEN_TIT_SUB_ADJD;
	men_lay_sub_color[1].text[1] = MEN_SUBTIT_COLOR;
	men_lay_sub_color[1].next = &men_lay_sub_color[2];
	men_lay_sub_color[1].prev = &men_lay_sub_color[0];
	men_lay_sub_color[1].function = sys_measure_adjd;
	
	men_lay_sub_color[2].text[0] = MEN_TIT_BACK;
	men_lay_sub_color[2].text[1] = MEN_SUBTIT_PAUSE;
	men_lay_sub_color[2].next = &men_lay_sub_color[0];
	men_lay_sub_color[2].prev = &men_lay_sub_color[1];
	men_lay_sub_color[2].topmenu = &men_lay_main[0];
	men_lay_sub_color[2].function = menu_enter_topmenu;
		
	menu_current = &men_initializing;
}











