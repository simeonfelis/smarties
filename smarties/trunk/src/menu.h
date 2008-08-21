#ifndef MENU_H_
#define MENU_H_

/**
 * \file menu.h
 * 
 * Copyright (c) 2008 Simeon Felis
 * 
 * \brief The menu structure and handling
 * 
 * @section intro License
 * 
 * GPL2 Licence
 * 
 * @section arch Architecture
 * 
 * The menu-control is locked to a 2-line alphanumeric display. It will be 
 * entered as soon as the smartie sorter is in \ref SYS_MODE_PAUSE. 
 * Otherwise the lcd displays some status information: 
 * 
 * During \ref SYS_MODE_INIT :
 * \code
 *  +--------------+
 *  | INITIALIZING |
 *  |              |
 *  +--------------+
 * \endcode
 * 
 * During \ref SYS_MODE_RUNNING :
 * \code
 *  +--------------+
 *  | ENTER PAUSE  |
 *  |RUN        COL|
 *  +--------------+
 * \endcode
 *   
 * During \ref SYS_MODE_PAUSE :
 * Greeting menu:
 * \code
 *   right     +--------------+    right     +--------------+   right
 * <---------> |  ENTER MENU  | <----------> |    RESUME    | <--------->
 *   left      |<prev   next >|    left      |<p          n>|   left
 *             +--------------+              +--------------+
 *               |                             |
 *               |enter_submenu()              |sys_resume()
 * \endcode
 * 
 * MAIN menu:
 * \code
 *   right     +--------------+    right     +--------------+   right     +--------------+   right
 * <---------> |    ROTATE    | <----------> |    COLORS    | <---------> |    Go Back   | <------->
 *   left      |<p          n>|    left      |<p          n>|   left      |<p          n>|   left
 *             +--------------+              +--------------+             +--------------+
 *               |                             |                            |
 *               |enter_submenu()              |enter_submenu()             |enter_topmenu()
 * \endcode
 * 
 * ROTATE Submenu
 * \code
 *   right     +--------------+   right      +--------------+   right     +--------------+   right
 * <---------> |   REVOLVER   | <----------> |    CATCHER   | <---------> |    Go Back   | <--------->
 *   left      |<p          n>|   left       |<p          n>|   left      |<p          n>|   left
 *             +--------------+              +--------------+             +--------------+
 *               |                             |                            |
 *               |rotate_revoler()             |rotate_catcher()            |enter_topmenu()
 * \endcode
 * 
 * COLOR submenu
 * \code
 *   right     +--------------+   right      +--------------+   right     +--------------+   right
 * <---------> |  TCS colors  | <----------> | ADJD colors  | <---------> |    Go Back   | <--------->
 *   left      |<p          n>|   left       |<p          n>|   left      |<p          n>|   left
 *             +--------------+              +--------------+             +--------------+
 *               |                             |                            |
 *               |????????????????             |????????????????            |enter_topmenu()
 * \endcode
 * 
 * Each menu layer has its own array:
 * 
 * MAIN menu
 * \code
 * +--------------+  +--------------+ 
 * |      [0]     |  |      [1]     | 
 * |              |  |              | 
 * +--------------+  +--------------+
 * \endcode
 * 
 * ROTATE menu
 * \code
 * +--------------+  +--------------+  +--------------+
 * |      [0]     |  |      [1]     |  |      [2]     |
 * |              |  |              |  |              |
 * +--------------+  +--------------+  +--------------+
 * \endcode
 * 
 * And so on.
 * 
 * The Display has two lines with 24 characters. Here the exact layout during SYS_MODE_RUNNING:
 * \code
 *      1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
 *    + -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - +
 *  1 |                            T  I  T  L  E                               |
 *  2 | S  T  A  T  : [   M  O  D  E      ]       C  O  L  :[ C  O  L  O  R   ]|
 *    + -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - +
 * \endcode
 * 
 * - Line 1: Column 1 to 24 is reserved for the title. If the push button is 
 * pressed, the action described by the title will be executed.
 * - Line 2: 
 *   - Column 6 to 12 is reserved for the current mode. Following modes 
 * can be displayed: 
 *     - PAUSE
 *     - RUNNING
 *   - Column 19 to 24 is reserved for the folling colors:
 *     - yellow: YELLOW
 *     - red: RED
 *     - blue: BLUE
 *     - brown: BROWN
 *     - green: GREEN
 *     - purple: PURPLE
 *     - unknown: UNKNOWN
 * 
 * For some setting possibilties have a look at \ref lcd_display.h
 * 
 * For lcd control functions have a look at \ref lcd_display.c
 * 
 * The init of the menu is done in \ref inits.c
 * 
 */

// menu length (24)				"                        "
#define MEN_TIT_EMPTY			"                        "
#define MEN_TIT_RESUME			"         RESUME         "
#define MEN_TIT_BACK			"        Go back         "
#define MEN_TIT_INITIALIZING	"      INITIALIZING      "

#define MEN_TIT_RUNNING			"       ENTER PAUSE      "
#define MEN_SUBTIT_RUNNING		"COL IN:        O:       "
#define MEN_SUBTIT_PAUSE		"<prev              next>"

#define MEN_TIT_GREETING		"      Enter Menu        "

#define MEN_TIT_MAIN_ROTATE		"         ROTATE         "
#define MEN_TIT_ROT_REV			"    Rotate Revolver     "
#define MEN_TIT_ROT_CATCH		"    Rotate Catcher      "
#define MEN_TIT_SUB_ROT_REV 	"        REVOLVER        "
#define MEN_TIT_SUB_ROT_CATCH	"        CATCHER         "

#define MEN_TIT_MAIN_COLOR		"         COLOR          "
#define MEN_TIT_SUB_TCS			"   TCS colors:          "
#define MEN_TIT_SUB_ADJD		"      ADJD colors       "
#define MEN_SUBTIT_COLOR		"Push:new measur Rot:next"  

#define MEN_MODE_EMPTY			"       "
#define MEN_MODE_RUNNING		"RUNNING"
#define MEN_MODE_PAUSED			"PAUSED "
#define MEN_MODE_INIT			"INIT   "

#define MEN_COL_EMPTY			"      "
#define MEN_COL_YELLOW			"YELLOW"
#define MEN_COL_RED				"   RED"
#define MEN_COL_BLUE			"  BLUE"
#define MEN_COL_ORANGE			"ORANGE"
#define MEN_COL_BROWN			" BROWN"
#define MEN_COL_GREEN			" GREEN"
#define MEN_COL_PURPLE			"PURPLE"
#define MEN_COL_PINK			"  PINK"
#define MEN_COL_UNKNOWN			"UNKNOW"


/**
 * \brief The menu structure  
 * 
 * Each menu entry stores a menu entry which is next (right) or previous (left) from itself. It 
 * also stores menu entries below (submenu) or menu entries above (topmenu) itself. Furthermore, 
 * each menu entry has a specific task wich is stored behind the function pointer. The 'task' 
 * is, for example, changing into the topmenu (go Back), or rotate the catcher. 
 */
typedef struct menu_entry_t {
	void (*function)(void);		//!< If push button pressed, this function will be executed (if available)
	char * text[2];			//!< Text on Display (Max 24 Characters, 2 lines)
	void * topmenu;			//!< The menu item above of current menu item
	void * submenu;			//!< The menu item below of current menu item
	void * prev;			//!< Previous Menu item
	void * next;			//!< Next menu item
} menu_entry;

/**
 * \brief Needed to assign the menu_entry's function pointer to a normally 
 * declerated function. This is necessary to execute this function. Functions
 * are set in \ref init_menu()
 */
void (*menu_action)(void);

/**
 * \brief Will set the current menu to the uppleraying menu (stored in topmenu)
 */
void menu_enter_topmenu();


/**
 * \brief Will set the current menu to the underlaying menu (stored in submenu)
 */
void menu_enter_submenu();

void menu_enter_nextmenu ();
void menu_enter_prevmenu ();

#endif /*MENU_H_*/
