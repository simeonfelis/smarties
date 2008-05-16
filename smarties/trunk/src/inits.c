#include "smarties2.h"
#include "lcd_display.h"
#include "inits.h"
#include "system.h"

void init_all()
{
	init_io();
	init_menu();
	lcd_init();
	init_timer();
	init_interrupts();
	init_positions();
}

void init_io()
{
	extern rotencoder rot_pos;
	
	STEPPER_PORT &= ~((1 << REV_CLK_BIT) | (1<<REV_CW_BIT) | (1<<REV_EN_BIT) | (1<<CATCH_CLK_BIT) | (1<<CATCH_CW_BIT) | (1<<CATCH_EN_BIT));
	STEPPER_DDR |= (1 << REV_CLK_BIT) | (1<<REV_CW_BIT) | (1<<REV_EN_BIT) | (1<<CATCH_CLK_BIT) | (1<<CATCH_CW_BIT) | (1<<CATCH_EN_BIT);
	
	
	TCS_OUT_PORT &= ~((1<<TCS_S0_BIT) | (1<<TCS_S1_BIT) | (1<<TCS_S2_BIT) | (1<<TCS_S3_BIT));
	TCS_OUT_DDR |= (1<<TCS_S0_BIT) | (1<<TCS_S1_BIT) | (1<<TCS_S2_BIT) | (1<<TCS_S3_BIT);
	
	TCS_IN_DDR &= ~(1<<TCS_IN_ICP);
	
	rot_pos = get_rotencoder_position();
}
void init_timer()
{
	
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
	extern menu_entry entry0;
	extern menu_entry entry1;
	extern menu_entry entry2;
	extern menu_entry entry3;
	extern menu_entry entry30;
	extern menu_entry entry31;
	extern menu_entry entry32;
	
	entry0.text = "Main Menu";
	entry1.text = "Pause";
	entry2.text = "Resume";
	entry3.text = "Rotate";
	entry30.text = "Revolver";
	entry31.text = "Catcher";
	entry32.text = "Back";

	entry1.function = sys_pause;
	entry2.function = sys_resume;
	entry3.function = 0;
	entry30.function = sys_rotate_revolver;
	entry31.function = sys_rotate_catcher;
	entry32.function = 0;
	
	entry1.submenu = 0;
	entry2.submenu = 0;
	entry3.submenu = (void *) &entry30;
	entry30.submenu = 0;
	entry31.submenu = 0;
	entry32.submenu = 0;
	
	entry0.topmenu = 0;
	entry1.topmenu = 0;
	entry3.topmenu = 0;
	entry30.topmenu = (void *) &entry3;
	entry31.topmenu = 0;
	entry32.topmenu = 0;
	
	entry0.next = (void *) &entry1;
	entry1.next = (void *) &entry2;
	entry2.next = (void *) &entry3;
	entry3.next = (void *) &entry0;
	
	entry30.next = (void *) &entry31;
	entry31.next = (void *) &entry32;
	entry32.next = (void *) &entry30;
	
	
	entry0.prev = (void *) &entry3;
	entry1.prev = (void *) &entry0;
	entry2.prev = (void *) &entry1;
	entry3.prev = (void *) &entry2;	
	
	entry30.prev = (void *) &entry32;
	entry31.prev = (void *) &entry30;
	entry32.prev = (void *) &entry30;
}
