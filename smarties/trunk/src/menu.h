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
 * During \ref SYS_MODE_INIT:
 * \code
 *  +--------------+
 *  | INITIALIZING |
 *  |              |
 *  +--------------+
 * \endcode
 * 
 * During SYS_MODE_RUNNING:
 * \code
 *  +--------------+
 *  | ENTER PAUSE  |
 *  |RUN        COL|
 *  +--------------+
 * \endcode
 *   
 * During \ref SYS_MODE_PAUSE:
 * \code
 *   right     +--------------+    right     +--------------+   right
 * <---------> |   MAIN MENU  | <----------> |    RESUME    | <--------->
 *   left      |<   PAUSE    >|    left      |<   PAUSE    >|   left
 *             +--------------+              +--------------+
 *               |                             |
 *               |enter_submenu()              |sys_resume()
 *               |
 *   right     +--------------+    right     +--------------+   right
 * <---------> |    ROTATE    | <----------> |     BACK     | <--------->
 *   left      |<   PAUSE    >|    left      |<   PAUSE    >|   left
 *             +--------------+              +--------------+
 *               |                             |
 *               |enter_submenu()              |enter_topmenu()
 *               |
 *   right     +--------------+   right      +--------------+   right     +--------------+   right
 * <---------> |   REVOLVER   | <----------> |    CATCHER   | <---------> |     BACK     | <--------->
 *   left      |<   PAUSE    >|   left       |<   PAUSE    >|   left      |<   PAUSE    >|   left
 *             +--------------+              +--------------+             +--------------+
 *               |                             |                            |
 *               |rotate_revoler()             |rotate_catcher()            |enter_topmenu()
 * \endcode
 * 
 * The numbering system is like following
 * \code
 * +--------------+  +--------------+ 
 * |    [0][0]    |  |    [1][0]    | 
 * |              |  |              | 
 * +--------------+  +--------------+
 * 
 * +--------------+  +--------------+
 * |    [0][1]    |  |    [1][1]    |
 * |              |  |              |
 * +--------------+  +--------------+
 * 
 * +--------------+  +--------------+  +--------------+
 * |    [0][2]    |  |    [1][2]    |  |    [2][2]    |
 * |              |  |              |  |              |
 * +--------------+  +--------------+  +--------------+
 * \endcode
 */

// menu length (24)				"                        "
#define MEN_TITLE_MAIN_MENU 	"        MAIN MENU       "
#define MEN_TITLE_RESUME		"         RESUME         "
#define MEN_TITLE_ROTATE		"         ROTATE         "
#define MEN_TITLE_REVOLVER		"        REVOLVER        "
#define MEN_TITLE_CATCHER		"        CATCHER         "
#define MEN_TITLE_BACK			"          BACK          "
#define MEN_TITLE_INITIALIZING  "      INITIALIZING      "
#define MEN_TITLE_ENTER_PAUSE	"       ENTER PAUSE      "


/**
 * \brief The menu structure  
 */
typedef struct menu_entry_t {
	void (*function);		//!< If push button pressed, this function will be executed (if available)
//	void (*leftaction);		//!< If rotary encoder turned left, this function will be executed (if available)
//	void (*rightaction);	//!< If rotary encoder turned right, this function will be executed (if available)
	char * text;			//!< Text on Display (TODO: Max 20 Characters)
	void * topmenu;			//!< The menu item above of current menu item
	void * submenu;			//!< The menu item below of current menu item
	void * prev;			//!< Previous Menu item
	void * next;			//!< Next menu item
} menu_entry;

/**
 * \brief Needed to assign the menu_entry's function pointer to a normally 
 * declerated function Necessary to execute this function
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
