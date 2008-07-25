/**
 * @file lcd_display.c
 * 
 * Copyright (c) 2008 Simeon Felis
 * 
 * @brief Control functions for Alphanumeric LCD display
 * 
 * The Display has two lines with 24 characters. The layout for different
 * modes are draftly described in file \ref menu.h 
 * 
 * Here the exact layout:
 * \code
 *      1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
 *    + -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - +
 *  1 |                            T  I  T  L  E                               |
 *  2 |[   M  O  D  E      ]                                [ C  O  L  O  R   ]|
 *    + -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - +
 * \endcode
 * 
 * - Line 1: Column 1 to 24 is reserved for the title. If the push button is 
 * pressed, the action described by the title will be executed.
 * - Line 2: 
 *   - Column 1 to 6 is reserved for the current mode. Following modes 
 * can be displayed: 
 *     - PAUSE
 *     - RUNNING
 *   - Column 19 to 21 is reserved for the folling colors:
 *     - yellow: YELLOW
 *     - red: RED
 *     - blue: BLUE
 *     - brown: BROWN
 *     - green: GREEN
 *     - purple: PURPLE
 *     - unknown: UNKNOWN
 * 
 * For some setting possibilties have a look at \ref lcd_display.h
 */
 
#include "lcd_display.h"

void lcd_command (uint8_t command) {

	/* create fitst nibble to send (high) */
	if (command & 0x10) 
		LCD_DATA_PORT |= (1<<LCD_DATA0_PIN);
	if (command & 0x20)
		LCD_DATA_PORT |= (1<<LCD_DATA1_PIN);
	if (command & 0x40)
		LCD_DATA_PORT |= (1<<LCD_DATA2_PIN);
	if (command & 0x80)
		LCD_DATA_PORT |= (1<<LCD_DATA3_PIN);
	lcd_enable();	/* send first nibble */
	
	_delay_us(1);
	
	/* create second nibble to send (low) */
	if (command & 0x01) 
		LCD_DATA_PORT |= (1<<LCD_DATA0_PIN);
	if (command & 0x02)
		LCD_DATA_PORT |= (1<<LCD_DATA1_PIN);
	if (command & 0x04)
		LCD_DATA_PORT |= (1<<LCD_DATA2_PIN);
	if (command & 0x08)
		LCD_DATA_PORT |= (1<<LCD_DATA3_PIN);
	lcd_enable();	/* send second nibble */
	
	_delay_us(1);
	
	LCD_DATA_PORT &= ~(LCD_DATA_ALL_PINS);
}

void lcd_init () {
	
	/* port directions */
	LCD_RS_PORT &= ~(1<<LCD_RS_PIN);
	LCD_RS_DDR |= (1<<LCD_RS_PIN);

	LCD_E_PORT &= ~(1<<LCD_E_PIN);
	LCD_E_DDR |= (1<<LCD_E_PIN);
	
	LCD_RW_PORT &= ~(1<<LCD_RW_PIN);
	LCD_RW_DDR |= (1<<LCD_RW_PIN);

	LCD_DATA_PORT &= ~LCD_DATA_ALL_PINS;
	LCD_DATA_DDR |= LCD_DATA_ALL_PINS;
	
	/* wait 16 ms after powering up */
	_delay_ms (16);
	
	/* set 4 Bit datamode */
	LCD_DATA_PORT |= (1<<LCD_DATA1_PIN) | (1<<LCD_DATA0_PIN);
	lcd_enable();
	
	/* repeat command several times */
	_delay_ms (5);
	lcd_enable();
	_delay_us (100);
	lcd_enable();
	
	LCD_DATA_PORT &= ~(LCD_DATA_ALL_PINS);
	
	LCD_DATA_PORT |= (1<<LCD_DATA1_PIN);
	_delay_us (100);
	lcd_enable();
	
	/* set 4 Bit, 2 rows, 5x8 digits */
	lcd_command (0x38);
	lcd_enable();
	_delay_us (100);
	
}
