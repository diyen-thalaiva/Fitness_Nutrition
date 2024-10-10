
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

// Display the menu options
  wattron(win, COLOR_PAIR(1));
  for (int i = 0; i < menu_option_count; i++) {
    if (i == selected_option) {
      wattron(win, A_REVERSE); // Highlight the selected option
    }
    mvwprintw(win, 4 + i, 3, "%s", menu_options[i]);
    wattroff(win, A_REVERSE);
  }
  wattroff(win, COLOR_PAIR(1));

  // Horizontal line before the Submit button and Time (magenta)
  wattron(win, COLOR_PAIR(4)); // Magenta color for the line before submit/time
  mvwhline(win, height + 1 + OFFSET_Y, 1, ACS_HLINE,
           width + 3); // Horizontal line before Submit button
  wattron(win, COLOR_PAIR(6));
  mvwaddch(win, height + 1 + OFFSET_Y, 0, ACS_LTEE); // Connect to left border
  wattron(win, COLOR_PAIR(6));
  mvwaddch(win, height + 1 + OFFSET_Y, width + 4,
           ACS_RTEE); // Connect to right border
  wattroff(win, COLOR_PAIR(4));

  // Display the vertical bar before the time with white color
  wattron(win, COLOR_PAIR(7)); // Assuming color pair 7 is white
  mvwprintw(win, height + 2 + OFFSET_Y, width - 19,
            "|"); // White vertical bar before time
  wattroff(win, COLOR_PAIR(7));

  // Display the current time in the window (on the right side)
  display_time(win, height, width);

  wrefresh(win); // Refresh window to reflect changes
}
int menu_system() {
  char *menu_title = "MAIN MENU";
  char *menu_options[] = {"1. Ask LomeshAI", "2. Fitness & Nutrition Tracker",
                          "3. Quit"};
  int menu_option_count = sizeof(menu_options) / sizeof(menu_options[0]);
  int selected_option = 0;

  // Initialize ncurses
  initscr();
  start_color(); // Enable color functionality
  cbreak();
  noecho();
  keypad(stdscr, TRUE); // Enable arrow keys for stdscr
  curs_set(0);          // Hide cursor

  // Initialize colors
  init_pair(1, COLOR_YELLOW, COLOR_BLACK); // Color for menu options
  init_pair(2, COLOR_CYAN, COLOR_BLACK);   // Header color
  init_pair(4, COLOR_MAGENTA,
            COLOR_BLACK); // Color for submit button and magenta line
  init_pair(5, COLOR_RED, COLOR_BLACK); // Color for time
  init_pair(6, COLOR_BLUE,
            COLOR_BLACK); // Dark blue for horizontal lines and border

  // Create the window and draw the initial layout
  int width = 110, height = 30; // Increased window size
  WINDOW *win = newwin(height, width, (LINES - height) / 2, (COLS - width) / 2);

  // Enable keypad input for the window (important to catch arrow keys)
  keypad(win, TRUE); // Enable arrow keys for the custom window `win`

  // Non-blocking input
  nodelay(win, TRUE); // Do not block input waiting for a keypress

int ch;
  while (1) {
    draw_menu_window(win, menu_title, menu_options, selected_option,
                     menu_option_count);

    // Fetch input without waiting (non-blocking)
    ch = wgetch(win);
    switch (ch) {
    case KEY_UP:
      selected_option =
          (selected_option - 1 + menu_option_count) % menu_option_count;
      break;
    case KEY_DOWN:
      selected_option = (selected_option + 1) % menu_option_count;
      break;
    case '\n': // User pressed Enter
      switch (selected_option) {
      case 0:
        // Ask LomeshAI (you can implement this separately if needed)
        mvwprintw(win, height - 2, 2, "LomeshAI selected...");
        wrefresh(win);
        sleep(3);
        ai_draw_prompt();
        break;
      case 1:
        // Fitness & Nutrition Tracker
        mvwprintw(win, height - 2, 2, "Fitness Tracker selected...");
        wrefresh(win);
        sleep(3);  // Add sleep(3) here to wait before drawing users menu
        draw_users_menu();
        wrefresh(win);
        sleep(1);  
        break;
      case 2:
        // Quit
        delwin(win);
        endwin();
        exit(0);
        return 0;
      }
      break;
    default:
      break;
    }

    // Update time every 500 milliseconds
    usleep(50); // Sleep for 500 milliseconds (half a second)
    display_time(win, height, width); // Redraw the time
  }

  delwin(win);
  endwin();

  return 0;
}
