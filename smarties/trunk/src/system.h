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
#define STEPPER_PORT		PORTD		//!< Output port for stepper motors
#define STEPPER_DDR			DDRD		//!< Port direction for stepper motors
#define STEPPER_PIN			PIND		//!< For reading the state of stepper motors
// Stepper motor for revolver
#define REV_EN_BIT			PD7			//!< Enable driver for revolver stepper motor 
#define REV_CW_BIT			PD6			//!< Rotate direction for revolver stepper motor (CW/CCW)
#define REV_CLK_BIT			PD5			//!< Clock signal for driver for revolver stepper motor

#define REV_EN()			(STEPPER_PORT |= (1<<REV_EN_BIT))
#define REV_DIS()			(STEPPER_PORT &= ~(1<<REV_EN_BIT))
#define REV_MOVE_STEP()		(STEPPER_PORT ^= (1<<REV_CLK_BIT))	//!< Move the revolver stepper motor for one single step

#define REV_STEP_DURATION	32			//!< Duration of one step in milliseconds	
#define REV_RAMP_DURATION	4			//!< Duration of the ramp up or ramp down in steps

// Stepper motor for catcher
#define CATCH_EN_BIT		PD4			//!< Enable driver for catcher stepper motor
#define CATCH_CW_BIT		PD3			//!< Rotate direction for catcher stepper motor (CW/CC
#define CATCH_CLK_BIT		PD2			//!< Clock signal for driver for catcher stepper motorW)

#define CATCH_EN()			(STEPPER_PORT |= (1<<CATCH_EN_BIT))
#define CATCH_DIS()			(STEPPER_PORT &= ~(1<<CATCH_EN_BIT))
#define CATCH_MOVE_STEP()	(STEPPER_PORT ^= (1<<CATCH_CLK_BIT))	//!< Move the catcher steper motor for one single step

#define CATCH_STEP_DURATION 32			//!< Duration of one step in one Millisecond 
#define CATCH_RAMP_DURATION 5			//!< Duration of the ramp up or ramp down in steps


// Miscaleous input output
// Shaker 
#define VIBR_PORT			PORTB
#define VIBR_DDR			DDRB
#define VIBR_PIN			PINB		///!< The the Vibrator state pin
#define VIBRATOR			PB1 		///!< The Vibrator Portbit

#define SHAKER_DURATION		500			//!< Default duration for shaker (vibrator)

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


/**
 * \brief Initializes input periphals for Rotary encoder
 */
#define ROTENC_INIT()		\
	do {													\
		ROTENC_AB_DDR   &= ~((1<<ROTENC_A_BIT) | (1<<ROTENC_B_BIT)); 	\
		ROTENC_PUSH_DDR &= ~(1<<ROTENC_PUSH_BIT); 						\
}while(0)

#define IS_ROTENC_A			(ROTENC_AB_PIN & (1<<ROTENC_A_BIT))
#define IS_ROTENC_B			(ROTENC_AB_PIN & (1<<ROTENC_B_BIT))
#define IS_ROTENC_BOTH		(IS_ROTENC_A && IS_ROTENC_B)
#define IS_ROTENC_NONE		(!IS_ROTENC_A && !IS_ROTENC_B)
#define IS_ROTENC_PUSH		(ROTENC_PUSH_PIN &(1<<ROTENC_PUSH_BIT))

// The Light barrieres (LB)
#define LB_PORT				PORTC
#define LB_DDR				DDRC
#define LB_PIN				PINC
#define LB_CATCH_BIT		PC7			//!< Lightbarriere Catcher Positinoer Portbit
#define LB_REV_BIT			PC6			//!< Lightbarriere Revolver Positioner Portbit

#define IS_LB_CATCHER		(LB_PIN & (1<<LB_CATCH_BIT))
#define IS_LB_REVOLVER		(LB_PIN & (1<<LB_REV_BIT))

/**
 * \brief The mode of the machine
 */
typedef enum system_mode_t {
	SYS_MODE_INIT = 0, 		//!< After reset or power on
	SYS_MODE_PAUSE,			//!< Pausing the smartie sorter and operated manually
	SYS_MODE_RUNNING,		//!< Smartie sorter is running automatically
	SYS_MODE_USER_EVENT		//!< TODO
} system_mode;

/**
 * \brief The step description during running mode SYS_MODE_RUNNING
 */ 
typedef enum system_step_description_t {
	SYS_STEP_COMPLETED = 0,		//!< If current step is completed the make the next step
	SYS_STEP_RUNNING,			//!< This step is currently running
	SYS_STEP_AWAITED			//!< This step is awaited now to be done next
} system_step_description;

/**
 *  \brief The single steps during running mode SYS_MODE_RUNNING
 */
typedef struct system_step_t {
	system_step_description I;			///< detecting colors
	system_step_description II;			///< positioning catcher
	system_step_description III;		///< positioning revolver
	system_step_description IV;			///< begin new mode cycle
} system_step;


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
 *  \brief All supported colors
 */
typedef enum smartie_color_t {
	col_yellow = 0,			//!< Yellow
	col_red,				//!< Red
	col_blue,				//!< Blue
	col_brown,				//!< Brown
	col_green,				//!< Green
	col_purple,				//!< Purple
	col_unknown				//!< Indexed as last color (highest counter). Insert new colors above this one!
} smartie_color;

/**
 *  \brief Properties a smartie can have
 */ 
typedef struct Smartie_t {
	uint8_t filled;			//!< If this is actually a smartie or not or empty transporter
	smartie_color color;	//!< Merged color
	smartie_color color1;	//!< From analogue color sensor
	smartie_color color2;	//!< From digital color sensor
} Smartie;

/**
 * \brief The status a device can have
 * 
 * The different modules of the smartie sorter can have different operating 
 * status. But mainly they all have common status like working or finished.  
 */
typedef enum common_stat_t {
	idle = 0,			//!< The module/device is doing nothing and is ready for a new job
	start_working,		//!< This will initiate the any work the device can have 
	working,			//!< Indicates the device is busy. The device will automatically change to this state
	stop_working,		//!< Setting a device to this state will stop the device 
	finished			//!< Indicates the device is finished. The device will automatically change to this state
} common_stat;

/**
 * \brief The stepper motor
 */
typedef struct engine_t {
	common_stat status;			//!< Status
	common_stat status_tmp; 	//!< The last status (before the current one)
	uint16_t currentPos;		//!< Current position (\ref smartie_color_t can also be used)
	uint16_t targetPos;			//!< Target position (\ref smartie_color_t can also be used)
	uint16_t cycle_counter;		//!< One cycle takes 1 millisecond
	uint8_t rampup_steps;		//!< One step takes \ref CATCH_STEP_DURATION or \ref REV_STEP_DURATION steps
	uint8_t rampdown_steps;		//!< Steps used for ramping down 
} engine;

/**
 * \brief Describes a color sensor
 */
typedef struct color_sensor_t {
	common_stat status;			//!< The current status of the color senso
	smartie_color value;		//!< The value from the last color detection
} color_sensor;

/**
 * \brief The status a lightbarrier can have
 */
typedef enum lightbarrier_status_t {
	lb_free = 0,						//!< Nothing inbetween the lightbarrier
	lb_blocked							//!< The lightbarrier is blocked
} lightbarrier_status;

/**
 * \brief Describes the component 'lightbarrier'
 */ 
typedef struct lightbarrier_t {
	lightbarrier_status status;			//!< The actual status of the lightbarrier
	lightbarrier_status status_tmp;		//!< For recognising a pass
	uint8_t passes;						//!< The amount of recognized passes through the lightbarrier
} lightbarrier;

//! \brief Properties the shaker (virbrator) can have 
typedef struct shaker_t {
	common_stat status;					//!< Status
	common_stat statustmp;				//!< The last status (before current)
	uint16_t duration;					//!< How long to vibrate the shaker in ms ( \ref SHAKER_DURATION )
} shaker;


/**
 *  \brief All devices from the smartie sorter collected to one bundle
 */
typedef struct smartie_sorter_t {
	system_mode mode;					//!< Stores the current mode
	system_step step;					//!< Stores the current step of the mode
	color_sensor colSensor_ADJD;		//!< Digital color sensor
	color_sensor colSensor_TMS;			//!< Analog color sensor
	engine catcher_Engine;				//!< Stepper motor for the catcher area
	engine revolver_Engine;				//!< Stepper motor for the revolver
	lightbarrier catcher_LB;			//!< Lightbarrier for the catcher
	lightbarrier revolver_LB;			//!< Lightbarrier for the revolver
	shaker shkr;						//!< Shaker (or vibrator)
	rotary_encoder rotenc;				//!< The rotary encoder (user input)
} smartie_sorter;


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////// P R O T O T Y P E S /////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

void sys_pause();
void sys_resume();
void sys_rotate_revolver();
void sys_rotate_catcher();
void sys_wait(uint16_t time);
void start_shaker();
void start_get_color_1();
void start_get_color_2();
void catcher_rotate_absolute(smartie_color color_now);
void catcher_rotate_relative(uint16_t);
void revolver_rotate_absolute(uint8_t abs_pos);
void revolver_rotate_relative(uint8_t rel_pos);

smartie_color make_color_merge(smartie_color color1, smartie_color color2);






#endif /*SYSTEM_H_*/
