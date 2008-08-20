//#include "system.h"
/**
 * \file menu.c
 * 
 * \brief Contains small help funcitions for menu administration.
 * 
 * These functions are connected to the \ref (*function) element
 * of \ref menu_entry objects when necessary.
 */
#include "menu.h"

extern menu_entry *menu_current;

/**
 * \brief Enter the submenu
 *
void menu_enter_submenu() {
	menu_current = menu_current->submenu;
}
*/

/**
 * \brief Enter the menu above
 *
void menu_enter_topmenu() {
	menu_current = menu_current->topmenu;
}
*/

/**
 * Switches to the next menu entry on the right
 */
void menu_enter_nextmenu() {
	menu_current = menu_current->next;
}

/**
 * Switches to the next menu entry on the left
 */
void menu_enter_prevmenu() {
	menu_current = menu_current->prev;
}
