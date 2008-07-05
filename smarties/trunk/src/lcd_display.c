/**
 * @file lcd_display.c
 * 
 * Copyright (c) 2008 Simeon Felis
 * 
 * @brief Control functions for Alphanumeric LCD display
 * 
 * @section intro License
 * 
 * Copied from http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial
 * 
 * @section install Description
 * 
 * This is the API for the alphanumeric LCD display. The connected portbits are
 * defined in \ref lcd_display.h. This software should be compatible to HD44780 
 * LCDs in 4-Bit-Interfacemode. 
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
 
#include <smarties2.h>
#include "lcd_display.h"
#include <util/delay.h>
 
/**
 * \brief Sends a databyte to display on the LCD
 * 
 * \param temp1 The character to display
 */
void lcd_data(unsigned char temp1)
{
   unsigned char temp2;
   //temp1 = swap_byte(temp1);
   temp2 = temp1;
 
   LCD_PORT |= (1<<LCD_RS);        // RS auf 1 setzen
 
   temp1 = temp1 >> 4;
   temp1 = temp1 & 0x0F;
   LCD_PORT &= ((1<<LCD_RS) | (1<<LCD_EN));
   LCD_PORT |= temp1;               // setzen
   lcd_enable();
 
   temp2 = temp2 & 0x0F;
   LCD_PORT &= ((1<<LCD_RS) | (1<<LCD_EN));
   LCD_PORT |= temp2;               // setzen
   lcd_enable();
   
   _delay_us(42);
}
 
/**
 * \brief Send a command to the LCD.
 * 
 * \param temp1 Command to send. Prepared commands are
 * - \ref LS_CLEAR_DISPLAY
 * - \ref LS_CURSOR_HOME
 */
void lcd_command(unsigned char temp1)
{
	unsigned char temp2;
	//temp1 = swap_byte (temp1);
	temp2 = temp1;

	LCD_CLEAR_DATABUS;
	LCD_COMMAND;
 
	temp1 = temp1 >> 2;              // oberes Nibble holen
	temp1 = temp1 & 0x3B;            // maskieren
	LCD_PORT |= temp1;               // setzen
	lcd_enable();
   
	temp2 = temp2 << 2;				// unteres Nibble holen 
	temp2 = temp2 & 0x3B;            // maskieren
	LCD_PORT |= temp2;               // setzen
	lcd_enable();
   
   _delay_us(42);
   
}
 
/**
 * \brief Creates Enable - slope
 */
void lcd_enable(void)
{
   // Bei Problemen ggf. Pause gemäß Datenblatt des LCD Controllers einfügen
   // http://www.mikrocontroller.net/topic/81974#685882
   LCD_PORT |= (1<<LCD_EN);
    _delay_us(1);                   // kurze Pause
   // Bei Problemen ggf. Pause gemäß Datenblatt des LCD Controllers verlängern
   // http://www.mikrocontroller.net/topic/80900
   LCD_PORT &= ~(1<<LCD_EN);
}
 
/**
 * \brief Init function for LCD display.
 * 
 * Must be called once after Power up or reset
 */
void lcd_init(void)
{	
	// sorry to say, the lcd is on the jtag pins
	MCUCSR |= (1<<JTD);	//Disable the JTAG interface
	MCUCSR |= (1<<JTD);	//Disable the JTAG interface twice!

	_delay_ms(15); 
	   
	// Port auf Ausgang schalten
	LCD_DDR |= 
			(1<<LCD_DB4) | (1<<LCD_DB5) | (1<<LCD_DB6) | (1<<LCD_DB7) | 
			(1<<LCD_EN) | (1<<LCD_RS);
	
	_delay_ms(15);

	// 4 Bit Modus aktivieren 
	// muss 3mal hintereinander gesendet werden zur Initialisierung

	LCD_CLEAR_DATABUS;
	LCD_PORT |= (1<<LCD_DB5);	//4 Bit;
	LCD_COMMAND;
	lcd_enable();
	_delay_ms(1);

	lcd_enable();
	_delay_ms(1);
	
	lcd_enable();
	_delay_ms(1);
	
	lcd_command(0x26); // 4 Bit; 2 Lines; 5x7 Dots
	_delay_ms(5);
	
	lcd_enable();
	_delay_ms(1);
	
	lcd_enable();
	_delay_ms(1);
	
	
	LCD_CLEAR_DATABUS;
	lcd_command(0x0f); // display on; cursor on; cursor blink on;
	_delay_ms(1);
	lcd_enable();

	_delay_ms(1);
 
   // 4Bit / 2 Zeilen / 5x7
//   lcd_command(0x28);
    
   // Display ein / Cursor aus / kein Blinken
//   lcd_command(0x0C); 
 
   // inkrement / kein Scrollen
//   lcd_command(0x06);
 
   lcd_clear();
}
 
/**
 * \brief Will clear the display
 */ 
void lcd_clear(void)
{
   lcd_command(LC_CLEAR_DISPLAY);
   _delay_ms(5);
}
 
/**
 * \brief Will set the cursor to Home position
 */
void lcd_home(void)
{
   lcd_command(LC_CURSOR_HOME);
   _delay_ms(5);
}
 
/**
 * \brief Set the cursor to a defined position
 * 
 * \param x The column
 * \param y The line
 */
void set_cursor(uint8_t x, uint8_t y)
{
  uint8_t tmp;
 
  switch (y) {
    case 1: tmp=0x80+0x00+x; break;    // 1. Zeile
    case 2: tmp=0x80+0x40+x; break;    // 2. Zeile
    case 3: tmp=0x80+0x10+x; break;    // 3. Zeile
    case 4: tmp=0x80+0x50+x; break;    // 4. Zeile
  }
  lcd_command(tmp);
}
 
/**
 * \brief Writes the string to the current cursor position
 * 
 * \param *data String to write on display
 */
void lcd_string(char *data)
{
    while(*data) {
        lcd_data(*data);
        data++;
    }
}

/**
 * \brief Helper function due to hardware bug
 * 
 * The connection is intend to be:
 * \code
 *  MC     DISPLAY
 * Px0  ->   DB4
 * Px1  ->   DB5 
 * Px2  ->   DB6
 * Px3  ->   DB7
 * 
 * Px4  ->   RS
 * Px5  ->   EN
 * \endcode
 * 
 * However it is like this:
 * \code
 *  MC     DISPLAY
 * PC2  ->   DB4
 * PC3  ->   DB5 
 * PC4  ->   DB6
 * PC5  ->   DB7
 * 
 * Px6  ->   EN
 * Px7  ->   RS
 * \endcode
 * 
 * So the databyte will be shifted a bit
 * 
 * \param data Value to swap
 * \return The swapped byte 
 */
/*
uint8_t swap_byte(uint8_t data)
{
	uint8_t ret=0;
	
	// low nibble
	ret |= (data & 0x01) << 4;
	ret |= (data & 0x02) << 2;
	ret |= (data & 0x04) >> 2;
	ret |= (data & 0x08) >> 4;
	
	// high nibble
	ret |= (data & 0x10) << 4;
	ret |= (data & 0x20) << 2;
	ret |= (data & 0x40) >> 2;
	ret |= (data & 0x80) >> 4;
	
	return ret;
}
*/