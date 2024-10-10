#include "utils.h"
#include "globals.h"
#include <curses.h>
#include <time.h>
#include <unistd.h> // For usleep function

void display_time(WINDOW *win, int height, int width) {
  time_t rawtime;
  struct tm *timeinfo;
  char buffer[9];
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(buffer, sizeof(buffer), "%H:%M", timeinfo);

  // Display the time next to the vertical bar with red color
  wattron(win, COLOR_PAIR(5)); // Red color for the time
  mvwprintw(win, height + 2 + OFFSET_Y, width - 13, "TIME: %s",
            buffer); // Time moved down closer to bottom-right
  wattroff(win, COLOR_PAIR(5));

  wrefresh(win); // Refresh window to show the updated time
}