/**
 * @file system.h
 * 
 * Copyright (c) 2008 Simeon Felis
 * 
 * @brief Smarites machine API header file 
 * 
 * @section intro License
 * 
 * GPL2 Licence
 * 
 * @section install Description
 * 
 * Here are many IO related functions declarated
 * 
 * @section arch Architecture
 * 
 */
#ifndef SYSTEM_H_
#define SYSTEM_H_

/// CPU Frequency
#define F_CPU 8000000
#define REVOLVER_SIZE		12
#define CATCHER_SIZE		8

/// The 8 color tubes are arranged like that. Angle in degree
#define CATCHER_ANGLE_PER_COLOR \
							45			
/// One step in full step mode (degree)
#define CATCHER_ANGLE_PER_STEP \
							1.6			

#define POS_SENS1			(1)
#define POS_SENS2			(2)
#define POS_SMARTIE_OUT		(11)
#define POS_SMARTIE_IN		(0)



// COLOR SENSORS STUFF
#define ADC_PORT			PORTA		/// TODO: Documentation
#define ADC_DDR				DDRA		/// TODO: Documentation
#define ADC_PIN				PINA		///! TODO: Documentation


// COLOR SENSOR HDJD-S722 works with an analog output
#define ADC_BLUE_PIN		PA2			///! TODO: Documentation
#define ADC_GREEN_PIN		PA1			///! TODO: Documentation
#define ADC_RED_PIN			PA0			///! TODO: Documentation

// COLOR SENSOR TCSS230 works with an clock output and color filters
#define TCS_IN_PORT			PORTD		///! Input port for TCS color sensor
#define TCS_IN_DDR			DDRD		///! Direction port for TCS color sensor
#define TCS_IN_ICP			PD2			///! ICP pin for TCS color sensor (clock signal)

#define TCS_OUT_PORT		PORTA		///! Output port for TCS color sensor 
#define TCS_OUT_DDR			DDRA		///! Direction port for TCS color sensor
#define TCS_S2_BIT			PA7			///! S2 settings pin for TCS color sensor
#define TCS_S0_BIT			PA6			///! S0 settings pin for TCS color sensor
#define TCS_S3_BIT			PA5			///! S3 settings pin for TCS color sensor
#define TCS_S1_BIT			PA4			///! S1 settings pin for TCS color sensor
#define TCS_OE_BIT			PA3			///! Output enable for TCS color sensor

#define TCS_SET_FREQ_SCALE(percentage)			\
	do {										\
		TCS_OUT_PORT ~= (1<<TCS_S1_BIT); 		\
		TCS_OUT_PORT ~= (1<<TCS_S0_BIT);	 	\
		if (percentage == 2) {					\
			TCS_OUT_PORT |= (1<<TCS_S1_BIT);	\
		}										\
		else if (percentage == 20) {			\
			TCS_OUT_PORT |= (1<<TCS_S0_BIT);	\
		}										\
		else if (percentage == 100) {			\
			TCS_OUT_PORT |= (1<<TCS_S0_BIT);	\
			TCS_OUT_PORT |= (1<<TCS_S1_BIT);	\
		}										\
	} while (0)

#define TCS_SET_FILTER(color)					\
	do {										\
		TCS_OUT_PORT ~= (1<<TCS_S2_BIT);		\
		TCS_OUT_PORT ~= (1<<TCS_S3_BIT);		\
		switch (color) {						\
			case COLOR_RED:						\
		}										\
	} while (0)

// STEPPER MOTORS
#define STEPPER_PORT		PORTD		///! Output port for stepper motors
#define STEPPER_DDR			DDRD		///! Port direction for stepper motors
#define STEPPER_PIN			PIND		///! For reading the state of stepper motors
// Stepper motor for revolver
#define REV_EN_BIT			PD7			///! Enable driver for revolver stepper motor 
#define REV_CW_BIT			PD6			///! Rotate direction for revolver stepper motor (CW/CCW)
#define REV_CLK_BIT			PD5			///! Clock signal for driver for revolver stepper motor
// Stepper motor for catcher
#define CATCH_EN_BIT		PD4			///! Enable driver for catcher stepper motor
#define CATCH_CW_BIT		PD3			///! Rotate direction for catcher stepper motor (CW/CC
#define CATCH_CLK_BIT		PD2			///! Clock signal for driver for catcher stepper motorW)

#define REV_EN()			(STEPPER_PORT |= (1<<REV_EN_BIT))

#define CATCH_EN()			(STEPPER_PORT |= (1<<CATCH_EN_BIT))
#define CATCH_DIS()			(STEPPER_PORT &= ~(1<<CATCH_EN_BIT))
#define CATCH_MOVE_STEP()	(STEPPER_PORT ^= (1<<CATCH_CLK_BIT))


// Miscaleous input output
//Vibrator 
#define VIBR_PORT			PORTB
#define VIBR_DDR			DDRB
#define VIBR_PIN			PINB		///!< The the Vibrator state pin
#define VIBRATOR			PB1 		///!< The Vibrator Portbit

// Input rotary encoder
#define ROTENC_AB_PORT		PORTD		//!< Rotary encoder port (output) for AB signals
#define ROTENC_AB_PIN		PIND		//!< Rotary encoder port (input) for AB signals
#define ROTENC_AB_DDR		DDRD		//!< Rotary encoder port direction for AB signals
#define ROTENC_PUSH_PORT	PORTB		//!< Rotary encoder port (output) for pushbutton
#define ROTENC_PUSH_PIN		PINB		//!< Rotary encoder port (input) for pushbutton
#define ROTENC_PUSH_DDR		DDRB		//!< Rotary encoder port direction for pushbutton 

#define ROTENC_A_BIT		PD3			//!< Rotary encoder signal A (Pin TODO of connector)
#define ROTENC_B_BIT		PD2			//!< Rotary encoder signal B (Pin TODO of connector)
#define ROTENC_PUSH_BIT		PB1			//!< Rotary encoder pushbutton signal (Pin TODO of connector)

/* maybe later
#define ROTENC_MASK_NONE	0x00
#define ROTENC_MASK_A		0x08
#define ROTENC_MASK_AB		0x0C
#define ROTENC_MASK_B		0x04
*/
/**
 * \brief Initializes input periphals for Rotary encoder
 * \param None
 */
#define ROTENC_INIT()		\
	do {													\
		ROTENC_AB_DDR   &= ~((1<<ROTENC_A_BIT) | (1<<ROTENC_B_BIT)); 	\
		ROTENC_PUSH_DDR &= ~(1<<ROTENC_PUSH_BIT); 						\
}while(0)

/* deprecated
#define FL_PUSH_BUTTON		0x01
#define FL_ROTATE_LEFT		0x02
#define FL_ROTATE_RIGHT		0x04
#define FL_ROTATE_LEFT_TMP	0x08
#define FL_ROTATE_RIGTH_TMP 0x10
*/
#define IS_ROTENC_A			(ROTENC_AB_PIN & (1<<ROTENC_A_BIT))
#define IS_ROTENC_B			(ROTENC_AB_PIN & (1<<ROTENC_B_BIT))
#define IS_ROTENC_PUSH		(ROTENC_PUSH_PIN &(1<<ROTENC_PUSH_BIT))

// The Light barrieres (LB)
#define LB_PORT				PORTC
#define LB_DDR				DDRC
#define LB_PIN				PINC
#define LB_CATCH_BIT		PC7			///! Lightbarriere Catcher Positinoer
#define LB_REV_BIT			PC6			///! Lightbarriere Revolver Positioner

#define IS_LB_CATCHER		(LB_PIN & (1<<LB_CATCH_BIT))
#define IS_LB_REVOLVER		(LB_PIN & (1<<LB_REV_BIT))

/**
 * \brief The state of the machine
 */
typedef enum system_mode_t {
	SYS_MODE_INIT = 0, 		//!< After reset or power on
	SYS_MODE_PAUSE,			//!< Pausing the smartie sorter and operated manually
	SYS_MODE_RUNNING,		//!< Smartie sorter is running automatically
	SYS_MODE_USER_EVENT		//!< TODO
} system_mode;

/**
 * \brief The single steps during running mode SYS_MODE_RUNNING
 */ 
typedef enum system_step_description_t {
	SYS_STEP_COMPLETED = 0,		//!< If current step is completed the make the next step
	SYS_STEP_RUNNING,			//!< This step is currently running
	SYS_STEP_AWAITED			//!< This step is awaited now to be done next
} system_step_description;

// The current step of the running mode
typedef struct system_step_t {
	uint8_t I;			///< detecting colors
	uint8_t II;			///< positioning catcher
	uint8_t III;		///< positioning revolver
	uint8_t IV;			///< begin new mode cycle
} system_step;

typedef enum user_event_t {
	EV_NO_EVENT = 0,
	EV_PUSH_BUTTON,
	EV_ROTATE_LEFT,
	EV_ROTATE_RIGHT
} user_event;

typedef enum rotencoder_t {
	RE_NONE = 1,
	RE_A,
	RE_B,
	RE_BOTH
} rotencoder;

/**
 * \brief The menu structure
 * 
 * Stucture:
 * \code
 *   menu_entry11 --next--> menu_entry12 --next--> menu_entry13
 *    |
 *    submenu -> menu_entry21 --next--> menu_entry22 --next--> menu_entry23
 *    |
 *    submenu -> menu_entry31 --next--> menu_entry32 
 * \endcode  
 */
typedef struct menu_entry_t {
	void (*function);		//!< If push button pressed, this function will be executed (if available)
	void (*leftaction);		//!< If rotary encoder turned left, this function will be executed (if available)
	void (*rightaction);	//!< If rotary encoder turned right, this function will be executed (if available)
	char * text;			//!< Text on Display (TODO: Max 20 Characters)
	void * topmenu;			//!< The menu item above of current menu item
	void * submenu;			//!< The menu item below of current menu item
	void * prev;			//!< Previous Menu item
	void * next;			//!< Next menu item
} menu_entry;

//! \brief All supported colors
typedef enum smartie_color_t {
	col_yellow = 0,
	col_red,
	col_blue,
	col_brown,
	col_green,
	col_purple,
	col_unknown
} smartie_color;

//! \brief Properties a smartie can have 
typedef struct Smartie_t {
	uint8_t filled;			//!< If this is actually a smartie or not or empty transporter
	smartie_color color;	//!< Merged color
	smartie_color color1;	//!< From analogue color sensor
	smartie_color color2;	//!< From digital color sensor
} Smartie;

/**
 * \brief Needed to assign the menu_entry's function pointer to a normally 
 * declerated function Necessary to execute this function
 */
void (*menu_action)(void);

// Prototypes
/**
 * \brief Will Enter the pause mode 
 * 
 * This function will enter the pause mode after the current function is finished. This function 
 * is usually called from the lcd menu. This function is only available in the 
 * mode SYS_MODE_RUNNING
 */
void sys_pause();

/**
 * \brief Leave the pause mode
 * 
 * This function will leave the pause mode. This function 
 * is usually called from the lcd menu. This function is only available in the 
 * mode SYS_MODE_PAUSE
 * 
 */
void sys_resume();

/**
 * \brief Rotates the revolver in Pause Mode
 * 
 * This function will rotate the revolver for one position, so that the 
 * next smartie is loaded and/or the next smartie will be underlying the
 * color sensor.
 * 
 * It will check the light barrier for this position rather than counting 
 * or calculating degrees and steps
 * 
 * This function is intened for manual usage and therefor is not available
 * during normal running mode SYS_MODE_RUNNING, only during SYS_MODE_PAUSE. 
 * 
 */
void sys_rotate_revolver();

/**
 * \brief Rotates the catcher in Pause Mode
 * 
 * This function will rotate the catcher for one position, so that the 
 * next smartie is loaded and/or the next smartie will be underlying the
 * color sensor.
 * 
 * It will check the light barrier for this position rather than counting 
 * or calculating degrees and steps
 * 
 * This function is intened for manual usage and therefor is not available
 * during normal running mode SYS_MODE_RUNNING, only during SYS_MODE_PAUSE. 
 * 
 */
void sys_rotate_catcher();


/**
 * \brief Waits a time in microseconds
 * 
 * It executes a couple of nop's so that at any CPU speed it waits always waits 
 * about time in microseconds until it returns
 * 
 * \param time
 * Time in microseconds
 * 
 */
void sys_wait(uint16_t time);

void start_shaker();
void start_get_color_1();
void start_get_color_2();
void catcher_rotate_absolute(smartie_color color_now);
void catcher_rotate_relative(float f_steps);
void revolver_rotate_absolute(uint8_t abs_pos);
void revolver_rotate_relative(uint8_t rel_pos);

smartie_color make_color_merge(smartie_color color1, smartie_color color2);

rotencoder get_rotencoder_position();
user_event get_user_action();





#endif /*SYSTEM_H_*/
