#include "smarties2.h"
#include "lcd_display.h"
#include "inits.h"
#include "system.h"
#include "menu.h"
#include "i2cmaster.h"

extern smartie_sorter ss;

void init_adc()
{	
}

void init_all()
{
	init_io();
	lcd_init(LCD_DISP_ON);
	init_menu();
		lcd_clrscr();
		lcd_puts(ss.menu.text);
	init_timer();
	init_interrupts();
	i2c_init();
		ss.col_sens_ADJD.ret = i2c_start(COL_SENS_ADJD_DEVICE_ADDRESS + I2C_WRITE);
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
	STEPPER_PORT |= (1<<REV_BIT_CLK) | (1<<CATCH_BIT_CLK); /* The clock signal should be high by default */

	/* ADJD color sensor */
	COL_SENS_ADJD_LED_OFF;  /* LED by default of */
	COL_SENS_ADJD_LED_DDR |= (1<<COL_SENS_ADJD_LED_BIT);
	/* some of the inits is done in the i2cmaster library */
	
	/* the TCS color sensor */
	COL_SENS_TCS_OUT_PORT &= ~(
			(1<<COL_SENS_TCS_S0_BIT) | (1<<COL_SENS_TCS_S1_BIT) | 
			(1<<COL_SENS_TCS_S2_BIT) | (1<<COL_SENS_TCS_S3_BIT));
	COL_SENS_TCS_OUT_DDR |= 
		(1<<COL_SENS_TCS_S0_BIT) | (1<<COL_SENS_TCS_S1_BIT) | 
		(1<<COL_SENS_TCS_S2_BIT) | (1<<COL_SENS_TCS_S3_BIT);

	COL_SENS_TCS_IN_DDR &= ~(1<<COL_SENS_TCS_IN_ICP);
	
	MCUCR |= (1<<ISC01); /* behavour of the interrupt sens for frequency measure: on falling edge */
	
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
	COL_SENS_TCS_SET_FREQ_SCALE(20);
}

void init_timer()
{
	/* Output compare register: after 250 * 62.5E-9 = 1ms a compare match */
	//OCR0 = 250;
	
	/* Prescaler 8 */
	TCCR0 |= (1<<CS01);
#if 0
	/* Prescaler 64 */
	TCCR0 |= (1<<CS01) | (1<<CS00);
#endif
	/* CTC mode */
	//TCCR0 |= (1<<WGM01);
	/* enable overflow interrupt */
	TIMSK |= (1<<TOIE0);
	
}

/**
 * \brief Will throw out remaining smarties and put revolver and catcher to 
 * defined positions
 */
void init_motors()
{
	/***************** C A T C H E R **********************/

	ss.mot_catcher.status = stat_idle;
	ss.mot_catcher.status_tmp = stat_idle;
	
	/* find the next defined position */
	if (!IS_LB_CATCHER)
		catcher_rotate_relative(1);
	
	
	/*************** R E V O L V E R *********************/
	
	ss.mot_revolver.status = stat_idle;
	ss.mot_revolver.status_tmp = stat_idle;
		
	/* find the next defined position */
	if (!IS_LB_REVOLVER)
		revolver_rotate_relative(1); 
		/* this will put the revolver to a positon 'hole above hole' */
	
	/* this will throw out eventually remaining smarties */ 
	revolver_rotate_relative(12);
	
	
	/* last, rotate the catcher to a position where it is possible 
	 * to remove the possibly remaining smarties */
	catcher_rotate_relative(4);
}

void init_menu()
{
	extern menu_entry entries[3][3];
	extern menu_entry men_initializing;
	extern menu_entry men_running;
	
	men_initializing.text = MEN_TITLE_INITIALIZING;
	
	men_running.text = MEN_TITLE_ENTER_PAUSE;
	men_running.next = &men_running;
	men_running.prev = &men_running;
	men_running.function = sys_pause;
	
	entries[0][0].text = MEN_TITLE_MAIN_MENU;
	entries[0][0].next = &entries[0][1];
	entries[0][0].prev = &entries[0][1];
	entries[0][0].submenu = &entries[1][0];
	entries[0][0].function = menu_enter_submenu;
	
	entries[0][1].text = MEN_TITLE_RESUME;
	entries[0][1].next = &entries[0][0];
	entries[0][1].prev = &entries[0][0];
	entries[0][1].function = sys_resume;
	
	entries[1][0].text = MEN_TITLE_ROTATE;
	entries[1][0].next = &entries[1][1];
	entries[1][0].prev = &entries[1][1];
	entries[1][0].submenu = &entries[2][0];
	entries[1][0].function = menu_enter_submenu;
	
	entries[1][1].text = MEN_TITLE_BACK;
	entries[1][1].next = &entries[1][0];
	entries[1][1].prev = &entries[1][0];
	entries[1][1].topmenu = &entries[0][0];
	entries[1][1].function = menu_enter_topmenu;
	
	entries[2][0].text = MEN_TITLE_REVOLVER;
	entries[2][0].next = &entries[2][1];
	entries[2][0].prev = &entries[2][2];
	entries[2][0].function = sys_rotate_revolver;
	
	entries[2][1].text = MEN_TITLE_CATCHER;
	entries[2][1].next = &entries[2][2];
	entries[2][1].prev = &entries[2][0];
	entries[2][1].function = sys_rotate_catcher;
	
	entries[2][2].text = MEN_TITLE_BACK;
	entries[2][2].next = &entries[2][0];
	entries[2][2].prev = &entries[2][1];
	entries[2][2].topmenu = &entries[1][0];
	entries[2][2].function = menu_enter_topmenu;
	
	ss.menu = men_initializing;
}
