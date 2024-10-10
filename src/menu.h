#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include <curses.h>

void draw_menu_window(WINDOW *win, char *menu_title, char *menu_options[],
                      int selected_option, int menu_option_count);


int menu_system();



#endif // MENU_SYSTEM_H
