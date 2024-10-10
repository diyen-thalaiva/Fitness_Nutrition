#include <curses.h>
#include <stdlib.h>
#include <unistd.h>

#include "ai.h"
#include "globals.h"
#include "users.h"
#include "utils.h"
#include "menu.h"



// Function to draw the menu window layout
void draw_menu_window(WINDOW *win, char *menu_title, char *menu_options[],
                      int selected_option, int menu_option_count) {
  int width = 105,
      height = 20; // Define the frame width/height for the content area

  // Draw the border with dark blue color
  wattron(win, COLOR_PAIR(6)); // Use dark blue for border color
  wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
          ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wattroff(win, COLOR_PAIR(6)); // Turn off the color

  // Add header with color
  wattron(win, COLOR_PAIR(2));
  mvwprintw(win, 1, (width / 2) - 3, "%s", menu_title);
  wattroff(win, COLOR_PAIR(2));

  // Horizontal line under the menu title (blue)
  wattron(win, COLOR_PAIR(6)); // Dark blue color
  mvwhline(win, 2, 1, ACS_HLINE, width + 3);
  mvwaddch(win, 2, 0, ACS_LTEE);         // Connect to left border
  mvwaddch(win, 2, width + 4, ACS_RTEE); // Connect to right border
  wattroff(win, COLOR_PAIR(6));
