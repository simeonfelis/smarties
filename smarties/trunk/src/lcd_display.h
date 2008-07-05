/**
 * \file lcd_display.h
 * 
 * \brief Settings and commands for LCD display
 * 
 * Mostly from http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial
 * 
 * Portbit settings can be found below. Due to a hardware bug (portbits 
 * missmatched) the function \ref swap_byte() is needed.
 * 
 * Further documentation can be found in \ref lcd_display.c
 */
#ifndef LCD_DISPLAY_H_
#define LCD_DISPLAY_H_

#include "smarties2.h"

/////////////// P R O T O T Y P E S /////////////


void lcd_data(unsigned char temp1);
void lcd_string(char *data);
void lcd_command(unsigned char temp1);
void lcd_enable(void);
void lcd_init(void);
void lcd_home(void);
void lcd_clear(void);
void set_cursor(uint8_t x, uint8_t y);

uint8_t swap_byte (uint8_t data); 

////////////// M A C R O S //////////////////////


////////////// D E F I N E S ///////////////////

// LCD COMMANDS 
#define LC_CLEAR_DISPLAY 	0x01		//!< Clear display command
#define LC_CURSOR_HOME		0x02		//!< Set cursor to home position (1x1)
#define LC_4_BIT_MODE		0x20		//!< Command for 4 Bit mode
 
// Pin configuration for LCD 
#define LCD_PORT      PORTC
#define LCD_DDR       DDRC

#define LCD_RS        PC7
#define LCD_EN        PC6

// Data bit connection
#define LCD_DB4		  PC2
#define LCD_DB5		  PC3
#define LCD_DB6		  PC4
#define LCD_DB7		  PC5
 

#define LCD_CLEAR_DATABUS	(LCD_PORT &= ~((1<<LCD_DB4) | (1<<LCD_DB5) | (1<<LCD_DB6) | (1<<LCD_DB7)))
#define LCD_COMMAND			(LCD_PORT &= ~(1<<LCD_RS))

#endif /*LCD_DISPLAY_H_*/
