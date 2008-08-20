/**
 * @file smarties2.h
 * 
 * Copyright (c) 2008 Simeon Felis
 * 
 * @brief Include file for smarties2 project
 * 
 * @section install Description
 * 
 * This file includes all files necessary for the smarties2 project
 * 
 * 
 */
#ifndef SMARTIES2_H_
#define SMARTIES2_H_


#include <avr/io.h>
#include "lcd_display.h"	// display driver
#include "system.h"
#include "abstraction.h"
#include "menu.h"



// Configuration File for Smarties2

#define FALSE	0			//!< Useful boolean values FALSE
#define TRUE	1			//!< Useful boolean values TRUE


// Small helper functions 
void smartie_lcd_write_color ();

#endif /*SMARTIES2_H_*/
