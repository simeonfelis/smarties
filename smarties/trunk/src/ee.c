/**
 * \file ee.c
 * 
 * \brief Values for system defaults, stored in EEProm
 * 
 * When compiling this file, a smarties2.eep file int intel hex
 * format will be created, which can be uploaded to the EEprom 
 * of the ATMega32. The ATMega32 will not override the EEProm with
 * this this values on reset.
 */

#include "system.h"
#include "ee.h"

#if DISTANCE_DETECTION | DISTANCE_NORM_DETECTION

extern float col_ava_blu [col_unknown];
extern float col_ava_gre [col_unknown];
extern float col_ava_red [col_unknown];

/**
 * \brief Values for structure in EEProm
 * 
 * Important! For keeping backward compatibility add new variables only at the end!
 */ 
const ee_memory ee_mem EEMEM = {
		.dummy = 0x00,
		.def_blu = {
				30.00, // Index \ref col_blue
				 9.00, // Index \ref col_green
				 6.17, // Index \ref col_red
				 9.83, // Index \ref col_yellow
				 6.50, // Index \ref col_orange
				 6.33, // Index \ref col_brown
				11.50, // Index \ref col_purple
				14.25 // Index \ref col_pink
		},
		.def_gre = {
				25.67, // Index \ref col_blue
				19.83, // Index \ref col_green
				 9.83, // Index \ref col_red
				32.83, // Index \ref col_yellow
				14.67, // Index \ref col_orange
				 8.33, // Index \ref col_brown
				10.63, // Index \ref col_purple
				14.25 // Index \ref col_pink
		},
		.def_red = {
				31.00, // Index \ref col_blue
				19.17, // Index \ref col_green
				39.67, // Index \ref col_red
				44.33, // Index \ref col_yellow
				44.17, // Index \ref col_orange
				18.67, // Index \ref col_brown
				23.25, // Index \ref col_purple
				43.75 // Index \ref col_pink
		},
		.usr_blu = {
				30.00, // Index \ref col_blue
				 9.00, // Index \ref col_green
				 6.17, // Index \ref col_red
				 9.83, // Index \ref col_yellow
				 6.50, // Index \ref col_orange
				 6.33, // Index \ref col_brown
				11.50, // Index \ref col_purple
				14.25 // Index \ref col_pink
				
		},
		.usr_gre = {
				25.67, // Index \ref col_blue
				19.83, // Index \ref col_green
				 9.83, // Index \ref col_red
				32.83, // Index \ref col_yellow
				14.67, // Index \ref col_orange
				 8.33, // Index \ref col_brown
				10.63, // Index \ref col_purple
				14.25 // Index \ref col_pink	
		},
		.usr_red = {
				31.00, // Index \ref col_blue
				19.17, // Index \ref col_green
				39.67, // Index \ref col_red
				44.33, // Index \ref col_yellow
				44.17, // Index \ref col_orange
				18.67, // Index \ref col_brown
				23.25, // Index \ref col_purple
				43.75 // Index \ref col_pink
		},
		.speed = 100
};

#endif
