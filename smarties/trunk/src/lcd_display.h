/**
 * @file lcd_display.h
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
 * defined here
 * 
 * The Display has two lines with each 21 characters. The layout for different
 * modes are draftly described in file \ref menu.h
 * 
 * Here the exact layout:
 * \code
 *     1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21
 *    +-  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -+
 *  1 |[                    T  I  T  L  E                          ]|
 *  2 |[  M  O  D  E     ]                                   C  O  L|
 *    +-  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -+
 * \endcode
 * 
 * - Line 1: Column 1 to 21 is reserved for the title. If the push button is 
 * pressed, the action described by the title will be executed.
 * - Line 2: 
 *   - Column 1 to 6 is reserved for the current mode. Following modes 
 * can be displayed: 
 *     - PAUSE
 *     - RUNNING
 *   - Column 19 to 21 is reserved for the folling color abreviations:
 *     - yellow: yel
 *     - red: red
 *     - blue: blu
 *     - brown: bro
 *     - green: gre
 *     - purple: pur
 *     - unknown: unk
 */
#ifndef LCD_DISPLAY_H_
#define LCD_DISPLAY_H_

#include "smarties2.h"


// Ansteuerung eines HD44780 kompatiblen LCD im 4-Bit-Interfacemodus
// http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial
//

/////////////// P R O T O T Y P E S /////////////

/**
  * \brief  Prints temp1 on Display
  *
  * \param      temp1		Character to print
  * \return                 void
  *
  */
void lcd_data(unsigned char temp1);
// TODO: Documentation
void lcd_string(char *data);
// TODO: Documentation
void lcd_command(unsigned char temp1);
// TODO: Documentation
void lcd_enable(void);
// TODO: Documentation
void lcd_init(void);
// TODO: Documentation
void lcd_home(void);
// TODO: Documentation
void lcd_clear(void);
// TODO: Documentation
void set_cursor(uint8_t x, uint8_t y);


////////////// M A C R O S //////////////////////


////////////// D E F I N E S ///////////////////

// LCD COMMANDS 
#define CLEAR_DISPLAY 0x01
#define CURSOR_HOME   0x02
 
// Pin configuration for LCD 
#define LCD_PORT      PORTC
#define LCD_DDR       DDRC
#define LCD_RS        PC5
#define LCD_EN        PC4

// Data bit connection
#define LCD_DB4		  PC0
#define LCD_DB5		  PC1
#define LCD_DB6		  PC2
#define LCD_DB7		  PC3
 



#endif /*LCD_DISPLAY_H_*/
