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

#include "menu.h"

// CPU Frequency
#define F_CPU 16000000

// COLOR SENSOR ADJD-S371 is a digital color sensor with own light source
#define COL_SENS_ADJD_LED_PORT	PORTA	//!< The ADJD color sensor Port for the LED 
#define COL_SENS_ADJD_LED_PIN	PINA
#define COL_SENS_ADJD_LED_DDR	DDRA
#define COL_SENS_ADJD_LED_BIT	PA0		//!< The ADJD color sensor Portbit for the LED

#define COL_SENS_ADJD_LED_ON		\
	(COL_SENS_ADJD_LED_PORT |= (1<<COL_SENS_ADJD_LED_BIT))	//!< Switches the LED of the ADJD color sensor on
#define COL_SENS_ADJD_LED_OFF		\
	(COL_SENS_ADJD_LED_PORT &= ~(1<<COL_SENS_ADJD_LED_BIT))	//!< Switches the LED of the ADJD color sensor off

#define COL_SENS_ADJD_DEVICE_ADDRESS	0x23

//COLOR SENSOR ADJD i2c commands, registers, bits
#define ADJD_REG_CTRL				0
#define ADJD_REG_CONFIG				1
#define ADJD_REG_CAP_RED			6
#define ADJD_REG_CAP_GREEN			7
#define ADJD_REG_CAP_BLUE			8
#define ADJD_REG_CAP_CLEAR			9
#define ADJD_REG_INT_RED_LO			10
#define ADJD_REG_INT_RED_HI			11
#define ADJD_REG_INT_GREEN_LO		12
#define ADJD_REG_INT_GREEN_HI		13
#define ADJD_REG_INT_BLUE_LO		14
#define ADJD_REG_INT_BLUE_HI		15
#define ADJD_REG_INT_CLEAR_LO		16
#define ADJD_REG_INT_CLEAR_HI		17


// COLOR SENSOR TCSS230 works with an clock output and color filters
#define COL_SENS_TCS_IN_PORT		PORTD		//!< Input port for TCS color sensor
#define COL_SENS_TCS_IN_DDR			DDRD
#define COL_SENS_TCS_IN_ICP			PD2			//!< ICP pin for TCS color sensor (clock signal)

#define COL_SENS_TCS_OUT_PORT		PORTA		//!< Output port for TCS color sensor 
#define COL_SENS_TCS_OUT_DDR		DDRA
#define COL_SENS_TCS_S2_BIT			PA7			//!< S2 settings pin for TCS color sensor
#define COL_SENS_TCS_S0_BIT			PA6			//!< S0 settings pin for TCS color sensor
#define COL_SENS_TCS_S3_BIT			PA5			//!< S3 settings pin for TCS color sensor
#define COL_SENS_TCS_S1_BIT			PA4			//!< S1 settings pin for TCS color sensor
#define COL_SENS_TCS_OE_BIT			PA3			//!< Output Portbit enable for TCS color sensor

#define COL_SENS_TCS_SAMPLE_TIME	30			//!< Time to measure TCS OUT frequency in milliseconds

#define COL_SENS_TCS_ENABLE			\
	(COL_SENS_TCS_OUT_PORT &= ~(1<<COL_SENS_TCS_OE_BIT))	//!< Enables the TCS color sensor output clk
#define COL_SENS_TCS_DISABLE		\
	(COL_SENS_TCS_OUT_PORT |= (1<<COL_SENS_TCS_OE_BIT))	 	//!< Disables the TCS color sensor output clk
#define COL_SENS_TCS_FREQ_MESURE_EN (GICR |= (1<<INT0))		//!< Enables interrupt for counting slopes (falling) from the TCS OUT pin
#define COL_SENS_TCS_FREQ_MESURE_DI (GICR &= ~(1<<INT0))	//!< Enables interrupt for counting slopes (falling) from the TCS OUT pin

/**
 * \brief Sets the frequency scaler for the TCS color sensor
 * 
 * \param percentage \b 2 2% \n
 *                   \b 20 20% \n
 *                   \b 100 100%
 */
#define COL_SENS_TCS_SET_FREQ_SCALE(percentage)					\
	do {														\
		COL_SENS_TCS_OUT_PORT &= ~(1<<COL_SENS_TCS_S1_BIT); 	\
		COL_SENS_TCS_OUT_PORT &= ~(1<<COL_SENS_TCS_S0_BIT);	 	\
		if (percentage == 2) {									\
			COL_SENS_TCS_OUT_PORT |= (1<<COL_SENS_TCS_S1_BIT);	\
		}														\
		else if (percentage == 20) {							\
			COL_SENS_TCS_OUT_PORT |= (1<<COL_SENS_TCS_S0_BIT);	\
		}														\
		else if (percentage == 100) {							\
			COL_SENS_TCS_OUT_PORT |= (1<<COL_SENS_TCS_S0_BIT);	\
			COL_SENS_TCS_OUT_PORT |= (1<<COL_SENS_TCS_S1_BIT);	\
		}														\
	} while (0)

/**
 * \brief Sets the color filter for the TCS color sensor
 *
 * \param color \b col_red Red filter \n
 *              \b col_blue Blue filter \n
 *              \b col_green Green filter on \n
 *              \b col_unknown No filter 
 */
#define COL_SENS_TCS_SET_FILTER(color)							\
	do {														\
		if (color == col_red) {									\
			COL_SENS_TCS_OUT_PORT &= ~(1<<COL_SENS_TCS_S2_BIT);	\
			COL_SENS_TCS_OUT_PORT &= ~(1<<COL_SENS_TCS_S3_BIT);	\
		}														\
		else if (color == col_blue) {							\
			COL_SENS_TCS_OUT_PORT &= ~(1<<COL_SENS_TCS_S2_BIT);	\
			COL_SENS_TCS_OUT_PORT |= (1<<COL_SENS_TCS_S3_BIT);	\
		}														\
		else if (color == col_green) {							\
			COL_SENS_TCS_OUT_PORT |= (1<<COL_SENS_TCS_S2_BIT);	\
			COL_SENS_TCS_OUT_PORT |= (1<<COL_SENS_TCS_S3_BIT);	\
		}														\
		else if (color == col_unknown) {						\
			COL_SENS_TCS_OUT_PORT |= (1<<COL_SENS_TCS_S2_BIT);	\
			COL_SENS_TCS_OUT_PORT &= ~(1<<COL_SENS_TCS_S3_BIT);	\
		}														\
	} while (0)

// STEPPER MOTORS
#define STEPPER_PORT		PORTD		//!< Output port for stepper motors
#define STEPPER_DDR			DDRD
#define STEPPER_PIN			PIND

#define REV_BIT_EN			PD7			//!< Portbit for Enable driver for revolver stepper motor 
#define REV_BIT_CW			PD6			//!< Portbit for Rotate direction for revolver stepper motor (CW/CCW)
#define REV_BIT_CLK			PD5			//!< Portbit for Clock signal for driver for revolver stepper motor

/**
 * \brief Move the revolver stepper motor for one single step. 
 */
#define REV_MOVE_STEP							\
	do {										\
		STEPPER_PORT &= ~(1<<REV_BIT_CLK);		\
		STEPPER_PORT |= (1<<REV_BIT_CLK);		\
} while (0)
#define REV_ENABLE			(STEPPER_PORT |= (1<<REV_BIT_EN))	//!< Enables the driver for the stepper motor 
#define REV_DISABLE			(STEPPER_PORT &= ~(1<<REV_BIT_EN))	//!< Disables the driver for the stepper motor
#define REV_SET_CW			(STEPPER_PORT &= ~(1<<REV_BIT_CW))	//!< Rotating direction clockwise
#define REV_SET_CCW			(STEPPER_PORT |= (1<<REV_BIT_CW))	//!< Rotating directino conter clock wise


#define REV_STEP_DURATION	35			//!< Duration of one step in milliseconds. This value controlles the rotating speed
#define REV_RAMP_DURATION	1			//!< Duration of the ramp up or ramp down in steps
#define REV_MAX_SIZE		12			//!< The amount of smarties (holes) which fit into the revolver
#define REV_STEPS_ESTIMATED	25			//!< Amount of steps for each positions 'hole abouve hole'
#define REV_PAUSE_DURATION	1000

#define REV_POS_SENS1			1
#define REV_POS_SENS2			1
#define REV_POS_SMARTIE_OUT		11
#define REV_POS_SMARTIE_IN		0

// Stepper motor for catcher
#define CATCH_BIT_EN		PD4			//!< Portbit for Enable driver for catcher stepper motor
#define CATCH_BIT_CW		PD1			//!< Portbit for Rotate direction for catcher stepper motor (CW/CC
#define CATCH_BIT_CLK		PD0			//!< Portbit for Clock signal for driver for catcher stepper motorW)

/**
 * \brief Move the catcher stepper motor for one single step. 
 */
#define CATCH_MOVE_STEP							\
	do {										\
		STEPPER_PORT &= ~(1<<CATCH_BIT_CLK);	\
		STEPPER_PORT |= (1<<CATCH_BIT_CLK);		\
} while (0)
#define CATCH_ENABLE		(STEPPER_PORT |= (1<<CATCH_BIT_EN))		//!< Enables the driver for the steper motor
#define CATCH_DISABLE		(STEPPER_PORT &= ~(1<<CATCH_BIT_EN))	//!< Disables the driver for the steper motor
#define CATCH_SET_CW		(STEPPER_PORT &= ~(1<<CATCH_BIT_CW))	//!< Rotating direction clockwise
#define CATCH_SET_CCW		(STEPPER_PORT |= (1<<CATCH_BIT_CW))	//!< Rotating directino conter clock wise

#define CATCH_STEP_DURATION 	11			//!< Duration of one step in one Millisecond. This value controlles the rotating speed
#define CATCH_RAMP_DURATION	 	2			//!< Duration of the ramp up or ramp down in steps
#define CATCH_MAX_SIZE			8			//!< The amount of catcher tubes for sorting the smarties
#define CATCH_STEPS_ESTIMATED	34			//!< Amount of steps for each positions 'hole abouve hole'
#define CATCH_PAUSE_DURATION	1000

// Miscaleous input output
// Shaker 
#define VIBR_PORT		PORTB		//!< The Vibrator Port
#define VIBR_DDR		DDRB
#define VIBR_PIN		PINB
#define VIBR_BIT		PB3 		//!< The Vibrator Portbit
#define VIBR_ON			(VIBR_PORT |= (1<<VIBR_BIT))	//!< Switches the Vibrator on	
#define VIBR_OFF		(VIBR_PORT &=~(1<<VIBR_BIT))	//!< Switches the Bibrator off
#define VIBR_TOGGLE		(VIBR_PORT ^= (1<<VIBR_BIT))

#define VIBR_DURATION		500			//!< Default duration for shaker (vibrator)

// Input rotary encoder
#define ROTENC_PORT		PORTB		//!< Rotary encoder port (output) for AB signals
#define ROTENC_PIN		PINB
#define ROTENC_DDR		DDRB 

#define ROTENC_A_BIT		PB0			//!< Rotary encoder signal A (Pin 5 of connector)
#define ROTENC_B_BIT		PB1			//!< Rotary encoder signal B (Pin 4 of connector)
#define ROTENC_PUSH_BIT		PB2			//!< Rotary encoder pushbutton signal (Pin 2 of connector)

/**
 * \brief Initializes input periphals for Rotary encoder
 */
#define ROTENC_INIT()		(ROTENC_DDR   &= ~((1<<ROTENC_A_BIT) | (1<<ROTENC_B_BIT) | (1<<ROTENC_PUSH_BIT)))

#define IS_ROTENC_A			((ROTENC_PIN & (1<<ROTENC_A_BIT)))		//!< Output status of rotary encoder
#define IS_ROTENC_B			((ROTENC_PIN & (1<<ROTENC_B_BIT)))		//!< Output status of rotary encoder
#define IS_ROTENC_AB		(IS_ROTENC_A && IS_ROTENC_B)			//!< Output status of rotary encoder
#define IS_ROTENC_NONE		(!IS_ROTENC_A && !IS_ROTENC_B)			//!< Output status of rotary encoder
#define IS_ROTENC_PUSH		(ROTENC_PIN & (1<<ROTENC_PUSH_BIT))		//!< Output status of rotary encoder

// The Light barrieres (LB)
#define LB_PORT				PORTA		//!< Lightbarriere Port
#define LB_DDR				DDRA
#define LB_PIN				PINA
#define LB_BIT_CATCH		PA2			//!< Lightbarriere Catcher Positinoer Portbit
#define LB_BIT_REV			PA1			//!< Lightbarriere Revolver Positioner Portbit

#define IS_LB_CATCHER		(!(LB_PIN & (1<<LB_BIT_CATCH)))		//!< Returns \ref TRUE if lightbarrier is blocked
#define IS_LB_REVOLVER		(!(LB_PIN & (1<<LB_BIT_REV)))		//!< Returns \ref TRUE if lightbarrier is blocked

/**
 * \brief The mode of the machine
 */
typedef enum system_mode_t {
	SYS_MODE_INIT = 0, 		//!< After reset or power on
	SYS_MODE_PAUSE,			//!< Pausing the smartie sorter and operated manually
	SYS_MODE_RUNNING,		//!< Smartie sorter is running automatically
} system_mode;

/**
 * \brief The status a device can have
 * 
 * The different modules of the smartie sorter can have different operating 
 * status. You can recognize if a module has just finished by checking the
 * last status.  
 */
typedef enum common_stat_t {
	stat_idle = 0,			//!< The module/device is doing nothing and is ready for a new job
	stat_start_working,		//!< Setting a module's state to this will start the device to work 
	stat_working,			//!< Indicates the device is busy. The device will automatically change to this state
	stat_stop_working,		//!< Setting a device to this state will stop the device 
	stat_finished			//!< Indicates the device is finished. The device will automatically change to this state
} common_stat;

/**
 *  \brief The single steps during running mode \ref SYS_MODE_RUNNING
 */
typedef struct system_step_t {
	common_stat I;			///< detecting colors and position catcher
	common_stat II;			///< positioning revolver
	common_stat III;		///< begin new mode cycle
} system_step;

/**
 * \brief Grouping of mode and steps
 */
typedef struct system_state_t {
	system_mode mode;				//!< Stores the current mode
	system_mode mode_last;			//!< Stores the last mode for transition steps
	system_step step;				//!< Stores the current step of the mode
} system_state;

//TODO docs
typedef enum program_t {
	prog_none = 0,
	prog_rotate_catcher,
	prog_rotate_revolver,
	prog_color_tcs,
	prog_color_adjd
} program;

/**
 * \brief The rotary encoder's elements can have following status
 */
typedef enum rotary_encoder_status_t {
	ROTENC_NONE = 0,		//!< Applicable for Push button, A-Pin and B-Pin
	ROTENC_PUSH,			//!< The Button is pushed
	ROTENC_A,				//!< Currently only the A-Pin of the rotary encoder is set 
	ROTENC_B,				//!< Currently only the B-Pin of the rotary encoder is set
	ROTENC_BOTH				//!< Currently both, the A-Pin and B-Pin of the rotary encoder are set
} rotary_encoder_status;

/**
 * \brief The rotary encoder (user input device) structure 
 */
typedef struct rotary_encoder_t {
	uint8_t push;			//!< The amount of detected pushes
	uint8_t right;			//!< The amount of detected right turns
	uint8_t left;			//!< The amount of detected left turns
	uint8_t pushtmp;		//!< Stores temporarely status of pushbutton
	uint8_t rottmp;			//!< Stores temporarely status of rotation
} rotary_encoder;

/**
 *  \brief All supported colors.
 * 
 * This enum is used for indexing the color tables (\ref col_tab_blu etc.), the
 * positioning of the catcher and may be more. \ref col_unknown is often used as 
 * end conditions for loops. In the color tables, this enums elements represent
 * the index for color rows.
 * 
 * If a smartie is detected to have \ref col_unknown, it will be handled as not present. 
 */
typedef enum smartie_color_t {
	col_yellow = 0,		//!< Yellow
	col_red,			//!< Red
	col_blue,			//!< Blue
	col_orange, 		//!< Orange
	col_brown,			//!< Brown
	col_green,			//!< Green
	col_purple,			//!< Purple
	col_pink,			//!< Pink
	col_unknown			//!< Indexed as last color (highest counter). Insert colors above this one! 
} smartie_color;

/**
 * \brief Describes the properties a smartie, which is transported by the \ref revolver_t, can have.
 */
typedef struct smartie_t {
	smartie_color color;	//!< Merged color
	smartie_color color1;	//!< From TCS color sensor
	smartie_color color2;	//!< From ADJD color sensor
} smartie;


/**
 * \brief The status a lightbarrier can have
 */
typedef enum lightbarrier_status_t {
	lb_free = 0,						//!< Nothing inbetween the lightbarrier
	lb_blocked							//!< The lightbarrier is blocked
} lightbarrier_status;

/**
 * \brief Describes the module lightbarrier
 */ 
typedef struct lightbarrier_t {
	lightbarrier_status status;			//!< The actual status of the lightbarrier
	lightbarrier_status status_last;	//!< For recognising a pass
	uint8_t passes;						//!< The amount of recognized passes through the lightbarrier
	int8_t (*is_blocked)();				//TODO
} lightbarrier;

/**
 * \brief The stepper motor
 */
typedef struct stepper_motor_t {
	common_stat status;			//!< Status
	common_stat status_last; 	//!< The last status
	int8_t current_pos;			//!< Current position (\ref smartie_color_t can also be used)
	int8_t target_pos;			//!< Target position (\ref smartie_color_t can also be used)
	uint16_t cycle_counter;		//!< One cycle takes 1 millisecond. The duration of \ref REV_STEP_DURATION or \ref CATCH_STEP_DURATION cycles lasts one step
	uint8_t steps;				//!< Count every step to estimate when to ramp down
	uint8_t ramp_steps;			//!< One step takes \ref CATCH_STEP_DURATION or \ref REV_STEP_DURATION steps
	uint8_t ramp_duration;
	uint8_t steps_estimated;
	int8_t max_size;
	int8_t step_duration;
	int16_t pause;
	int16_t pause_duration;
	lightbarrier *lb;			//TODO: docs
	void (*enable)();			//TODO: docs
	void (*disable) ();
	void (*move_step) ();		//TODO: docs
} stepper_motor;

/**
 * \brief Describes the ADJD-S371 color sensor
 */
typedef struct color_sensor_adjd_t {
	common_stat status;			//!< The current status of the color sensor
	common_stat status_last;	//!< The last status of the color sensor
	smartie_color color;		//!< The value from the last color detection
} color_sensor_adjd;

/**
 * \brief Describes the TCS230 color sensor
 */
typedef struct color_sensor_tcs_t {
	common_stat status;			//!< The current status of the color sensor
	common_stat status_last;	//!< The status before current status
	smartie_color color;		//!< The value from the last color detection
	int16_t time;				//!< Especially for the TCS frequency mesurement. In milliseconds
	int16_t filter_freq_blue;	//!< The clock frequency in kHz measured with blue filter on
	int16_t filter_freq_green;	//!< The clock frequency in kHz measured with green filter on
	int16_t filter_freq_red;	//!< The clock frequency in kHz measured with red filter on
	int16_t slopes;				//!< The amount of slopes recognised during \ref COL_SENS_TCS_SAMPLE_TIME
} color_sensor_tcs;

/**
 *  \brief Describes the virbrator (shaker) module
 */ 
typedef struct vibrator_t {
	common_stat status;					//!< Status
	common_stat status_last;			//!< The last status
	uint16_t duration;					//!< How long to vibrate the shaker in ms ( \ref SHAKER_DURATION )
} vibrator;

#if 0
/**
 * \brief Describes the catcher modul (the disc with tubes)
 */
typedef struct catcher_t {
	smartie_color position;		//!< How far the catcher is rotated. Maximum is \ref CATCH_MAX_SIZE.
} catcher;
#endif

/**
 * \brief Describes the revolver module (the disc)
 */
typedef struct revolver_t {
	int8_t position;				//!< How far the revolver is rotated. Maximum is \ref REV_MAX_SIZE.
	smartie smart[REV_MAX_SIZE];	//!< A list of all smarties the revolver contains.
} revolver;


/**
 *  \brief All devices from the smartie sorter collected to one bundle
 */
typedef struct smartie_sorter_t {
	system_state state;					//!< Stores the current state
	program prog;						//TODO docs
	color_sensor_adjd sens_adjd;		//!< Digital color sensor
	color_sensor_tcs sens_tcs;			//!< Analog color sensor
	stepper_motor mot_catcher;			//!< Stepper motor for the catcher area
	stepper_motor mot_revolver;			//!< Stepper motor for the revolver
	lightbarrier lb_catcher;			//!< Lightbarrier for the catcher
	lightbarrier lb_revolver;			//!< Lightbarrier for the revolver
	vibrator vibr;						//!< Shaker (or vibrator)
	rotary_encoder rotenc;				//!< The rotary encoder (user input)
//	catcher catch;						//!< The catcher disc with tubes
	revolver rev;						//!< The revolver disc with smarties
} smartie_sorter;


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////// P R O T O T Y P E S /////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

void 	sys_enter_topmenu ();
void 	sys_enter_submenu ();

void 	sys_catcher_enable ();
void	sys_catcher_disable ();
void 	sys_catcher_move_step ();
void 	sys_catcher_rotate ();
int8_t 	sys_catcher_is_lb_blocked ();

void 	sys_revolver_enable ();
void 	sys_revolver_disable ();
void 	sys_revolver_move_step ();
void	sys_revolver_rotate ();
int8_t 	sys_revolver_is_lb_blocked ();

void 	sys_pause();
void 	sys_resume();
//void 	sys_rotate_revolver();
//void 	sys_rotate_catcher();
void 	sys_measure_tcs();
void 	sys_measure_adjd();
void 	sys_wait(uint16_t time);

void 	sensor_adjd_get_color();
void 	sensor_tcs_get_color();
void 	catcher_rotate_absolute(smartie_color move_to);
void 	catcher_rotate_relative(int8_t);
void 	revolver_rotate_relative(int8_t rel_pos);
void 	vibrator_start();

smartie_color make_color_merge(smartie_color color1, smartie_color color2);

#endif /*SYSTEM_H_*/
