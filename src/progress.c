#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ai.h"
#include "calc.h"
#include "fitness.h"

#include "menu.h"
#include "nutrition.h"
#include "users.h"
#include "utils.h"
#include "calc.h"
#include "progress.h"
#include "globals.h"

#define OFFSET_Y 6



// Function to display the calorie deficit or surplus
void display_calorie_balance(WINDOW *win, float cal_consumed, float cal_spent, int height) {
    float balance = cal_consumed - cal_spent;
    

    // Determine the color based on balance
    if (balance > 0) {
        // Calorie Surplus -> Red (Color Pair 5)
        wattron(win, COLOR_PAIR(5)); 
        mvwprintw(win, height - 10  + OFFSET_Y, 5, "Calorie Surplus by: %.2f cal", balance);
    } else if (balance < 0) {
        // Calorie Deficit -> Green (Color Pair 3)
        wattron(win, COLOR_PAIR(3)); 
        mvwprintw(win, height - 10 + OFFSET_Y, 5, "Calorie Deficit by: %.2f cal", -balance);
    } else {
        // Perfectly Balanced -> White (Color Pair 7)
        wattron(win, COLOR_PAIR(7)); 
        mvwprintw(win, height - 10 + OFFSET_Y, 5, "Calorie Perfectly Balanced: %.2f cal", balance);
    }

    // Turn off the current color pair
    wattroff(win, COLOR_PAIR(5)); // Red off
    wattroff(win, COLOR_PAIR(3)); // Green off
    wattroff(win, COLOR_PAIR(7)); // White off

    // Refresh the window to display changes
    wrefresh(win);
}


// Initialize the ncurses window and draw the layout (border, lines, and time)
void drawWindowprogress(WINDOW *win, float cal_consumed, float cal_spent) {
    int width = 105, height = 20;

    // Draw the border with dark blue color
    wattron(win, COLOR_PAIR(6));  // Use dark blue for border color
    wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
    wattroff(win, COLOR_PAIR(6));


    // Header and separator
    wattron(win, COLOR_PAIR(2));
    mvwprintw(win, 1, 50, "  Progress  ");               
    wattroff(win, COLOR_PAIR(4));

    // Horizontal line under header
    wattron(win, COLOR_PAIR(6));  // Dark blue color
    mvwhline(win, 2, 1, ACS_HLINE, width + 3);
    mvwaddch(win, 2, 0, ACS_LTEE);  // Connect to left border
    mvwaddch(win, 2, width + 4, ACS_RTEE);  // Connect to right border
    wattroff(win, COLOR_PAIR(6));

    // Horizontal line before Submit and Time (magenta)
    wattron(win, COLOR_PAIR(4));  // Magenta color for the line before submit/time
    mvwhline(win, height + 1 + OFFSET_Y, 1, ACS_HLINE, width + 3);  // Horizontal line at the bottom
    wattron(win, COLOR_PAIR(6));
    mvwaddch(win, height + 1 + OFFSET_Y, 0, ACS_LTEE);  // Connect to left border
    mvwaddch(win, height + 1 + OFFSET_Y, width + 4, ACS_RTEE);  // Connect to right border
    wattroff(win, COLOR_PAIR(6));


   wattron(win, COLOR_PAIR(1)); // Use a color pair for the display
mvwprintw(win, height - 4 + OFFSET_Y, 5, 
          "Total calories consumed: %.2f cal", cal_consumed); // Display total calories
wattroff(win, COLOR_PAIR(1));

// Display the list of consumed foods
wattron(win, COLOR_PAIR(2)); 
mvwprintw(win, height - 20 + OFFSET_Y, 5, "You have consumed:"); // Title for consumed items
wattroff(win, COLOR_PAIR(2));

if (consumedCount > 0) {
    for (int i = 0; i < consumedCount; i++) {
        mvwprintw(win, height - 19 + OFFSET_Y + i, 5, "%s", consumedFoods[i]); // Print each food
    }
} else {
    mvwprintw(win, height - 555 + OFFSET_Y, 5, "You have not consumed any food items."); // Message for no items consumed
}

    // Display total calories burned
    wattron(win, COLOR_PAIR(4)); // Use a color pair for the display
    mvwprintw(win, height - (2 + consumedCount) + OFFSET_Y, 5, "Total calories burned: %.2f cal", cal_spent);
    wattroff(win, COLOR_PAIR(4));

    wattron(win, COLOR_PAIR(3)); 
    mvwprintw(win, height - (8 + consumedCount) + OFFSET_Y, 5, "You have performed the following activities:");
    wattroff(win, COLOR_PAIR(3));
    // Display the list of performed activities
    if (logcount > 0) {
         // Title for performed activities

        for (int i = 0; i < logcount; i++) {
            mvwprintw(win, height - (8 + consumedCount) + OFFSET_Y + i + 1, 5, "%s", activityLog[i].name); // Print each activity
        }
    } else {
        mvwprintw(win, height - (555 + consumedCount) + OFFSET_Y, 5, "You have not performed any activities."); // Message for no activities performed
    }


    wattron(win, COLOR_PAIR(3)); // Submit button uses magenta
        mvwprintw(win, height + 2 + OFFSET_Y, 5,
            "ACTIV X"); // Submit button moved down closer to bottom-left
        wattroff(win, COLOR_PAIR(3));
        wattroff(win, A_REVERSE);


    // Vertical bar before Submit (white)
    wattron(win, COLOR_PAIR(7));  // White color for the vertical bar
    for (int i = height + 2 + OFFSET_Y; i < height + 3 + OFFSET_Y; i++) {
        mvwaddch(win, i, 16, ACS_VLINE);  // Position the vertical bar
    }
    wattroff(win, COLOR_PAIR(7));

    // Vertical bar after Submit (white)
    wattron(win, COLOR_PAIR(7));  // White color for the vertical bar
    for (int i = height + 2 + OFFSET_Y; i < height + 3 + OFFSET_Y; i++) {
        mvwaddch(win, i, 85, ACS_VLINE);  // Position the vertical bar
    }
    wattroff(win, COLOR_PAIR(7));

    // Display the current time in the window (on the right side)
    display_time(win, height, width);

   

    wrefresh(win);  // Refresh window to reflect changes
}

int display_progress_menu(float consumed , float spent) {
    // Sample data for calorie calculations
    

    // Initialize ncurses
    initscr();
    start_color();  // Enable color functionality
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);  // Hide cursor

    // Initialize colors
  init_pair(1, COLOR_YELLOW, COLOR_BLACK); // Color for query
  init_pair(2, COLOR_CYAN, COLOR_BLACK);   // Header color
  init_pair(3, COLOR_GREEN, COLOR_BLACK);  // Color for response text
  init_pair(4, COLOR_MAGENTA,
            COLOR_BLACK); // Color for submit button and magenta line
  init_pair(5, COLOR_RED, COLOR_BLACK); // Color for time
  init_pair(6, COLOR_BLUE,
            COLOR_BLACK); // Dark blue for horizontal lines and border
  init_pair(7, COLOR_WHITE, COLOR_BLACK); // White for vertical lines

    // Create the window and draw the layout
    int width = 110, height = 30;  // Window size
    WINDOW *win = newwin(height, width, (LINES - height) / 2, (COLS - width) / 2);

    // Set the window to non-blocking mode for input
    nodelay(win, TRUE);  // Do not block on wgetch()



    // Loop to continuously update the time every second
    while (1) {
    // Clear the window before drawing to prevent visual artifacts
    werase(win);

    // Redraw the window
    drawWindowprogress(win, cal_consumed, cal_spent);
    
    // Display calorie balance
    display_calorie_balance(win, consumed, spent, height);

    // Wait for 1 second
    usleep(1000000);  // Sleep for 1 second

    // Check for user input
    int ch = wgetch(win);  // Non-blocking input

    // Exit on 'ESC' key press (ASCII 27)
    if (ch == 27) {
        // Call the menu system and exit this loop
        menu_system();
        break;
    }

    // Exit on 'q' key press
    if (ch == 'q') {
        break;
    }
}


     display_time(win, height, width);
    // End ncurses mode
    delwin(win);
    endwin();

    return 0;
}