#include "smarties2.h"
#include "lcd_display.h"
#include "inits.h"
#include "system.h"
#include "menu.h"

void init_all()
{
	init_io();
	init_menu();
	lcd_init(LCD_DISP_ON);
	init_timer();
	init_interrupts();
	init_positions();
}

void init_io()
{
	extern smartie_sorter ss;
	
    /*
     *  sorry to say, the lcd is on the jtag pins
     */
	MCUCSR |= (1<<JTD);	
	MCUCSR |= (1<<JTD);	/* disable the JTAG interface twice! */

	STEPPER_PORT &= ~((1 << REV_CLK_BIT) | (1<<REV_CW_BIT) | (1<<REV_EN_BIT) | (1<<CATCH_CLK_BIT) | (1<<CATCH_CW_BIT) | (1<<CATCH_EN_BIT));
	STEPPER_DDR |= (1 << REV_CLK_BIT) | (1<<REV_CW_BIT) | (1<<REV_EN_BIT) | (1<<CATCH_CLK_BIT) | (1<<CATCH_CW_BIT) | (1<<CATCH_EN_BIT);
	
	
	TCS_OUT_PORT &= ~((1<<TCS_S0_BIT) | (1<<TCS_S1_BIT) | (1<<TCS_S2_BIT) | (1<<TCS_S3_BIT));
	TCS_OUT_DDR |= (1<<TCS_S0_BIT) | (1<<TCS_S1_BIT) | (1<<TCS_S2_BIT) | (1<<TCS_S3_BIT);
	
	TCS_IN_DDR &= ~(1<<TCS_IN_ICP);
	
	ROTENC_INIT();
	
	// get current rotary encoder position
	if (IS_ROTENC_A)
		ss.rotenc.rottmp = ROTENC_A;
	if (IS_ROTENC_B)
		ss.rotenc.rottmp = ROTENC_B;
	if (IS_ROTENC_BOTH)
		ss.rotenc.rottmp = ROTENC_BOTH;
	if (IS_ROTENC_NONE)
		ss.rotenc.rottmp = ROTENC_NONE;
}
void init_timer()
{
	/* Output compare register: after 250 * 62.5E-9 = 1ms a compare match */
	//OCR0 = 250;
	
#if 0
	/* Prescaler 8 */
	TCCR0 |= (1<<CS01);
#endif

	/* Prescaler 64 */
	TCCR0 |= (1<<CS01) | (1<<CS00);
	/* CTC mode */
	//TCCR0 |= (1<<WGM01);
	/* enable overflow interrupt */
	TIMSK |= (1<<TOIE0);
	
}
void init_interrupts()
{
	
}
void init_positions()
{
	
}

void init_adc()
{
	
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
}
