/**
 * \file lcd_display.h
 * 
 * \brief Settings and commands for LCD display
 */
#ifndef LCD_H
#define LCD_H


//#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>
//#include <avr/pgmspace.h>


#define LCD_RS_PORT			PORTC
#define LCD_RS_DDR			DDRC
#define LCD_RW_PORT			PORTD
#define LCD_RW_DDR			DDRD
#define LCD_E_PORT			PORTC
#define LCD_E_DDR			DDRC
#define LCD_RS_PIN			7
#define LCD_RW_PIN			3
#define LCD_E_PIN			6

#define LCD_DATA_PORT		PORTC
#define LCD_DATA_DDR		DDRC
#define LCD_DATA0_PIN		2
#define LCD_DATA1_PIN		3
#define LCD_DATA2_PIN		4
#define LCD_DATA3_PIN		5

#define LCD_DATA_ALL_PINS		((1<<LCD_DATA0_PIN) | (1<<LCD_DATA1_PIN) | (1<<LCD_DATA2_PIN) | (1<<LCD_DATA3_PIN))

#define lcd_enable() do {	\
	LCD_E_PORT &= (1<<LCD_E_PIN);		\
	_delay_us (1);						\
	LCD_E_PORT |= (1<<LCD_E_PIN);		\
} while (0)

void lcd_init();
void lcd_command(uint8_t);


#endif //LCD_H
