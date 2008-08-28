/**
 * \file ee.h
 * 
 * \brief Some EEProm memory administration
 */
#ifndef EE_H_
#define EE_H_

#include <avr/eeprom.h>

/**
 * \brief Structure in EEProm
 * 
 * Important! For keeping backward compatibility add new variables only at the end!
 */ 
typedef struct ee_memory_t {
	uint8_t dummy;				//!< Dummy! Don't use!
	color_avarage def_blu;		//!< System default color reference values for all smarties, blue channel 
	color_avarage def_gre;		//!< System default color reference values for all smarties, green channel
	color_avarage def_red;		//!< System default color reference values for all smarties, bred channel
	color_avarage usr_blu;		//!< User calibrated color reference values for all smarites, blue channel
	color_avarage usr_gre;		//!< User calibrated color reference values for all smarites, green channel
	color_avarage usr_red;		//!< User calibrated color reference values for all smarites, red channel
	uint16_t speed;				//!< Smartie sorter speed , see \ref smartie_sorter_t
	uint8_t data_from_default;	// not used
	uint8_t data_from_user;		// not used
} ee_memory ;

extern const ee_memory ee_mem;

#endif /*EE_H_*/
