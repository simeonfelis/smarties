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
	uint8_t dummy;
	color_avarage def_blu;
	color_avarage def_gre;
	color_avarage def_red;
	color_avarage usr_blu;
	color_avarage usr_gre;
	color_avarage usr_red;
	uint16_t speed;
	uint8_t data_from_default;
	uint8_t data_from_user;
} ee_memory ;

extern const ee_memory ee_mem;

#endif /*EE_H_*/
