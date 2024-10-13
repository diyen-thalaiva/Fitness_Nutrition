#include "menu.h"
#include "users.h"
#include "utils.h"
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // For usleep function

#define OFFSET_Y 6
#define BLINK_DELAY 500000 // 500 milliseconds (0.5 second delay for blinking)

// Function to display the ACTIV X logo
// Function to display the new ACTIV X logo
// Updated ACTIV X logo
void display_logo(WINDOW *win, int width) {
  // New ASCII art for the logo with added space between ACTIV and X
  const char *logo[] = {
      "        ***      ******   **********  **********  **             **",
      "      **   **   **    **      **          **       **           **",
      "     **     **  **            **          **        **         **",
      "    **       ** **            **          **         **       **",
      "    **       ** **            **          **          **     **",
      "    *********** **            **          **           **   **",
      "    **       ** **            **          **            ** **",
      "    **       **  **    **     **          **             ***",
      "    **       **   ******      **      **********          *",
      "",
      "",

      "                            **       **      ",
      "                             **     **       ",
      "                              **   **        ",
      "                               ** **         ",
      "                                ***          ",
      "                               ** **         ",
      "                              **   **        ",
      "                             **     **       ",
      "                            **       **      "};

  // Print the logo to the screen
  wattron(win, COLOR_PAIR(1)); // Cyan color for the logo
  for (int i = 0; i < 20; ++i) {
    mvwprintw(win, 4 + i, (width / 2) - 30, "%s",
              logo[i]); // Centered logo (adjusted for wider logo)
  }
  wattroff(win, COLOR_PAIR(2));

  // Print the motivational quote below the logo, with some space after "X"
  wattron(win, COLOR_PAIR(3)); // Green color for the quote
  mvwprintw(win, 26, (width / 2) - 18,
            " Your success is found in your daily routine!");
  wattroff(win, COLOR_PAIR(3));

  wrefresh(win); // Refresh to show changes
}

// Initialize the ncurses window and draw the layout (border, lines, and time)
void drawWindow(WINDOW *win) {
  int width = 105, height = 20;

  // Draw the border with dark blue color
  wattron(win, COLOR_PAIR(6)); // Use dark blue for border color
  wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
          ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wattroff(win, COLOR_PAIR(6));

  // **WELCOME text centered at the top**
  wattron(win, COLOR_PAIR(2));                   // Use red color for "WELCOME"
  mvwprintw(win, 1, (width / 2) - 3, "WELCOME"); // Position the text centrally
  wattroff(win, COLOR_PAIR(5));

  // Horizontal line under header
  wattron(win, COLOR_PAIR(6)); // Dark blue color
  mvwhline(win, 2, 1, ACS_HLINE, width + 3);
  mvwaddch(win, 2, 0, ACS_LTEE);         // Connect to left border
  mvwaddch(win, 2, width + 4, ACS_RTEE); // Connect to right border
  wattroff(win, COLOR_PAIR(6));

  // Display the ACTIV X logo just under the horizontal line
  display_logo(win, width);

  // Horizontal line before Submit and Time (magenta)
  wattron(win, COLOR_PAIR(4)); // Magenta color for the line before submit/time
  mvwhline(win, height + 1 + OFFSET_Y, 1, ACS_HLINE,
           width + 3); // Horizontal line at the bottom
  wattron(win, COLOR_PAIR(6));
  mvwaddch(win, height + 1 + OFFSET_Y, 0, ACS_LTEE); // Connect to left border
  mvwaddch(win, height + 1 + OFFSET_Y, width + 4,
           ACS_RTEE); // Connect to right border
  wattroff(win, COLOR_PAIR(6));

  // Display the current time in the window (on the right side)
  display_time(win, height, width);

  // Display the vertical bar before the time with white color
  wattron(win, COLOR_PAIR(7)); // Assuming color pair 7 is white
  mvwprintw(win, height + 2 + OFFSET_Y, width - 19,
            "|"); // White vertical bar before time
  wattroff(win, COLOR_PAIR(7));

  wrefresh(win); // Refresh window to reflect changes
}

void blink_message(WINDOW *win, int height, int width) {
  int visible = 1;    // Start with the message visible
  nodelay(win, TRUE); // Non-blocking input (getch won't block)
  time_t last_time_update = time(NULL); // Store the last time update

  while (1) {
    // Get the current time
    time_t current_time = time(NULL);

    // Update the blinking message
    if (visible) {
      // Display the blinking message with the full dots
      mvwprintw(win, height - 2, 2, "Press ENTER to continue");
    } else {
      // Clear the entire message area
      mvwprintw(win, height - 2, 2, "                        ");
    }

    // Refresh the window to show changes
    wrefresh(win);
    usleep(BLINK_DELAY); // Wait for the blink delay
    visible = !visible;  // Toggle the visibility

    // Check if 1 second has passed since the last time update
    if (difftime(current_time, last_time_update) >= 1.0) {
      display_time(win, height, width); // Update the time display
      last_time_update = current_time;  // Reset the last time update
    }

    // Check for user input to stop blinking and continue
    int ch = wgetch(win);
    if (ch == '\n') {
      draw_users_menu();
    }
  }
}

int main() {
  // Initialize ncurses
  initscr();
  start_color(); // Enable color functionality
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0); // Hide cursor

  // Initialize colors
  init_pair(1, COLOR_YELLOW, COLOR_BLACK); // Color for query
  init_pair(2, COLOR_CYAN, COLOR_BLACK);   // Header color
  init_pair(3, COLOR_GREEN, COLOR_BLACK);  // Color for response text
  init_pair(4, COLOR_MAGENTA,
            COLOR_BLACK); // Color for submit button and magenta line
  init_pair(5, COLOR_RED, COLOR_BLACK); // Color for time
  init_pair(6, COLOR_BLUE,
            COLOR_BLACK); // Dark blue for horizontal lines and border
  init_pair(7, COLOR_WHITE,
            COLOR_BLACK); // White text on black background for the vertical bar

  // Create the window and draw the layout
  int width = 110, height = 30; // Window size
  WINDOW *win = newwin(height, width, (LINES - height) / 2, (COLS - width) / 2);

  // Draw the initial layout (border, lines, and time)
  drawWindow(win);

  // Implement manual blinking for "Press ENTER to continue..."
  blink_message(win, height, width);

  // End ncurses mode
  delwin(win);
  endwin();

  return 0;
}
