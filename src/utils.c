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
  