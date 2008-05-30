//#include "system.h"
#include "menu.h"

extern menu_entry *menu_current;

void menu_enter_submenu ()
{
	menu_current = menu_current->submenu;
}

void menu_enter_topmenu()
{
	menu_current = menu_current->topmenu;
}

void menu_enter_nextmenu ()
{
	menu_current = menu_current->next;
}

void menu_enter_prevmenu ()
{
	menu_current = menu_current->prev;
}
