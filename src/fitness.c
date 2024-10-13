#include <ctype.h>
#include <curses.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> // for usleep()

#include "calc.h"
#include "globals.h"
#include "menu.h"
#include "utils.h"
#include "users.h"
#include "fitness.h"
#include "nutrition.h"

// Assume finalCaloriesConsumed is accessible globally from the other module
float finalCaloriesConsumed; // Declare external variable

float cal_consumed = 0;
float cal_spent = 0;

ActivityLog activityLog[MAX_ACTIVITY_LOG];
int logcount = 0;


// Structure to store exercise information

Exercise exercises[MAX_EXERCISES]; // Array of exercises loaded from CSV
int exercise_count = 0;            // Number of exercises loaded

// Function to initialize exercises from CSV file
void initExercises() {
  FILE *file = fopen(
      "C:/Users/diyen/OneDrive/Desktop/Cock/Final/dataset/exercise_dataset.csv",
      "r");
  if (file == NULL) {
    printf("Error opening file\n");
    return;
  }

  char line[1024];
  int count = 0;

  // Skip the header line
  fgets(line, 1024, file);

  // Read each line of the CSV
  while (fgets(line, 1024, file)) {
    char *token = strtok(line, ",");
    if (token != NULL && count < MAX_EXERCISES) {
      // Extract Activity (name)
      strcpy(exercises[count].name, token);

      // Move to next token for Calories column
      token = strtok(NULL, ",");
      if (token != NULL) {
        char *endptr;
        float calories = strtof(token, &endptr);

        // Check if the conversion was successful
        if (*endptr != '\0' && *endptr != '\n') {
          printf("Error converting calories for exercise: %s\n",
                 exercises[count].name);
        } else {
          exercises[count].calories_per_kg = calories;
          printf("Loaded exercise: %s with calories_per_kg: %.2f\n",
                 exercises[count].name,
                 exercises[count].calories_per_kg); // Debug print
          count++;
        }
      } else {
        printf("Error: Missing calories value for exercise: %s\n",
               exercises[count].name);
      }
    }
  }

  fclose(file);
  exercise_count = count; // Set the number of exercises loaded
}

// Initialize the ncurses window and draw the layout (border, lines, and time)
void drawFitnessWindow(WINDOW *win) {
  int width = 105, height = 20;

  // Draw the border with dark blue color
  wattron(win, COLOR_PAIR(6)); // Use dark blue for border color
  wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
          ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wattroff(win, COLOR_PAIR(6));

  // Add the header "Fitness Tracker" in cyan
  wattron(win, COLOR_PAIR(2)); // Cyan color for header
  mvwprintw(win, 1, (width / 2) - 7, "Fitness Tracker"); // Centered header text
  wattroff(win, COLOR_PAIR(2));

  // Horizontal line under header
  wattron(win, COLOR_PAIR(6)); // Dark blue color
  mvwhline(win, 2, 1, ACS_HLINE, width + 3);
  mvwaddch(win, 2, 0, ACS_LTEE);         // Connect to left border
  mvwaddch(win, 2, width + 4, ACS_RTEE); // Connect to right border
  wattroff(win, COLOR_PAIR(6));

  // Horizontal line before Submit and Time (magenta)
  wattron(win, COLOR_PAIR(4)); // Magenta color for the line before submit/time
  mvwhline(win, height + 1 + OFFSET_Y, 1, ACS_HLINE,
           width + 3); // Horizontal line at the bottom
  wattron(win, COLOR_PAIR(6));
  mvwaddch(win, height + 1 + OFFSET_Y, 0, ACS_LTEE); // Connect to left border
  mvwaddch(win, height + 1 + OFFSET_Y, width + 4,
           ACS_RTEE); // Connect to right border
  wattroff(win, COLOR_PAIR(6));

  wattron(win, COLOR_PAIR(3)); // Submit button uses magenta
  mvwprintw(win, height + 2 + OFFSET_Y, 5,
            "ACTIV X"); // Submit button moved down closer to bottom-left
  wattroff(win, COLOR_PAIR(3));
  wattroff(win, A_REVERSE);

  // Vertical bar before Submit (white)
  wattron(win, COLOR_PAIR(7)); // White color for the vertical bar
  for (int i = height + 2 + OFFSET_Y; i < height + 3 + OFFSET_Y; i++) {
    mvwaddch(win, i, 16, ACS_VLINE); // Position the vertical bar
  }
  wattroff(win, COLOR_PAIR(7));

  // Display the vertical bar before the time with white color
  wattron(win, COLOR_PAIR(7)); // Assuming color pair 7 is white
  mvwprintw(win, height + 2 + OFFSET_Y, width - 19,
            "|"); // White vertical bar before time
  wattroff(win, COLOR_PAIR(7));

  // Display the current time in the window (on the right side)
  display_time(win, height, width);

  // Redraw the border in blue after each key press
  wattron(win, COLOR_PAIR(6)); // Turn on blue color
  wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
          ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wattroff(win, COLOR_PAIR(6)); // Turn off blue color
  wrefresh(win);                // Refresh the window after each key press
}

// Custom string case-insensitive search function
char *my_strcasestr(const char *haystack, const char *needle) {
  if (!*needle) {
    return (char *)haystack;
  }

  for (const char *p = haystack; *p != '\0'; p++) {
    const char *h = p, *n = needle;
    while (*h && *n &&
           tolower((unsigned char)*h) == tolower((unsigned char)*n)) {
      h++;
      n++;
    }
    if (!*n) {
      return (char *)p;
    }
  }

  return NULL;
}

// Function to find exercises matching user input
int find_exercise(const char *input, Exercise exercises[], int count,
                  int matchIndexes[]) {
  int matchCount = 0;
  for (int i = 0; i < count; i++) {
    if (my_strcasestr(exercises[i].name, input) != NULL) {
      matchIndexes[matchCount] = i;
      matchCount++;
      if (matchCount >= MAX_MATCHES_FIT) {
        break;
      }
    }
  }
  return matchCount;
}

// New Function to display exercise options with arrow navigation
int display_exercise_options_with_arrows(WINDOW *win, Exercise exercises[],
                                         int matchIndexes[], int matchCount) {
  int highlight = 0; // The index to highlight
  int choice = -1;
  int key;

  wattron(win, COLOR_PAIR(4)); // Green color for "Select an exercise" text
  mvwprintw(win, 9, 2, "Select an exercise:");
  wattroff(win, COLOR_PAIR(4));

  // Redraw the border in blue after each key press
  wattron(win, COLOR_PAIR(6)); // Turn on blue color
  wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
          ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wattroff(win, COLOR_PAIR(6)); // Turn off blue color
  wrefresh(win);                // Refresh the window after each key press

  while (1) {
    for (int i = 0; i < matchCount; i++) {
      if (i == highlight) {
        wattron(win, A_REVERSE); // Highlight the selected exercise
      }
      wattron(win, COLOR_PAIR(2)); // Green color for the exercise options
      mvwprintw(win, 11 + i, 2, "%d. %s", i + 1,
                exercises[matchIndexes[i]].name);
      wattroff(win, COLOR_PAIR(2));
      wattroff(win, A_REVERSE);
    }
    key = wgetch(win);

    switch (key) {
    case KEY_UP:
      highlight = (highlight == 0) ? matchCount - 1 : highlight - 1;
      break;
    case KEY_DOWN:
      highlight = (highlight == matchCount - 1) ? 0 : highlight + 1;
      break;
    case 10: // Enter key
      choice = highlight;
      return matchIndexes[choice];
    }

    // Redraw the border in blue after each key press
    wattron(win, COLOR_PAIR(6)); // Turn on blue color
    wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
            ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
    wattroff(win, COLOR_PAIR(6)); // Turn off blue color
    wrefresh(win);                // Refresh the window after each key press
  }
}

// Function to clear the screen except for the "Total calories burned" line
void clear_screen_except_total(WINDOW *win, float total_calories_burned) {
  // Clear everything on the screen
  werase(win);

  // Redraw the window structure
  drawFitnessWindow(win);

  // Re-display the total calories burned
  wattron(win, COLOR_PAIR(5)); // Yellow color for "Total calories burned"
  mvwprintw(win, 20 + OFFSET_Y, 2, "Total calories burned: %.2f cal",
            total_calories_burned);
  wattroff(win, COLOR_PAIR(5));

  // Refresh window to show cleared screen with total calories
  // Redraw the border in blue after each key press
  wattron(win, COLOR_PAIR(6)); // Turn on blue color
  wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
          ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wattroff(win, COLOR_PAIR(6)); // Turn off blue color
  wrefresh(win);                // Refresh the window after each key press
}

// Function to display the user's activity log after they type "exit"
void display_activity_log(WINDOW *win, ActivityLog activityLog[], int logCount,
                          float total_calories_burned,
                          float finalCaloriesConsumed) {
  clear_screen_except_total(win, total_calories_burned);

  wattron(win, COLOR_PAIR(3)); // Magenta color for "Activities performed" text
  mvwprintw(win, 4, 2, "Activities performed:");
  wattroff(win, COLOR_PAIR(3));

  for (int i = 0; i < logCount; i++) {
    wattron(win, COLOR_PAIR(1)); // Yellow color for activity log items
    mvwprintw(win, 5 + i, 2, "%d. %s", i + 1,
              activityLog[i].name); // Display only the exercise name
    wattroff(win, COLOR_PAIR(1));
  }

  // Redraw the border in blue after each key press
  wattron(win, COLOR_PAIR(6)); // Turn on blue color
  wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
          ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wattroff(win, COLOR_PAIR(6)); // Turn off blue color
  wrefresh(win);                // Refresh the window after each key press
  // Redraw the border in blue after each key press
  wattron(win, COLOR_PAIR(6)); // Turn on blue color
  wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
          ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wattroff(win, COLOR_PAIR(6)); // Turn off blue color
  wrefresh(win);                // Refresh the window after each key press

  int key;
    while (1) {
        key = wgetch(win); // Capture the keypress
        if (key == 27) {   // If 'Escape' key is pressed (ASCII value 27)
            menu_system();   // Go back to the menu system
            break;           // Exit the activity log loop
        }
        // No need for usleep here, the program waits for user input
    }
}

// Function to process activity and duration input, and auto-clear after 5
// seconds
int process_activity(WINDOW *win, float weight_in_kg,
                     float *total_calories_burned, ActivityLog activityLog[],
                     int *logCount) {
  char activity_input[MAX_LINE_LENGTH_FIT];

  // Prompt to enter activity (in yellow)
  wattron(win, COLOR_PAIR(1)); // Yellow color for activity input prompt
  mvwprintw(win, 4, 2, "Enter activity (type exit to finish): ");
  wattroff(win, COLOR_PAIR(1));

  echo();
  wgetnstr(win, activity_input, MAX_LINE_LENGTH_FIT);
  noecho();

  // Redraw border after activity input
  wattron(win, COLOR_PAIR(6)); // Turn on blue color
  wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
          ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wattroff(win, COLOR_PAIR(6)); // Turn off blue color
  wrefresh(win);                // Refresh the window after activity input

  // Exit condition check
  if (strcmp(activity_input, "exit") == 0) {
    return 1; // Signal exit to display the activity log
  }

  int matchIndexes[MAX_MATCHES_FIT];
  int matchCount =
      find_exercise(activity_input, exercises, exercise_count, matchIndexes);

  if (matchCount == 0) {
    wattron(win, COLOR_PAIR(5)); // Red color for "No matching exercise found"
    mvwprintw(win, 5, 2, "No matching exercise found.");
    wattroff(win, COLOR_PAIR(5));
  } else {
    int idx = -1;
    if (matchCount == 1) {
      idx = matchIndexes[0];
    } else {
      // Use arrow key navigation for selecting exercise
      idx = display_exercise_options_with_arrows(win, exercises, matchIndexes,
                                                 matchCount);
    }

    float time_in_hours = 0;

    wattron(win, COLOR_PAIR(3)); // Green color for duration prompt
    mvwprintw(win, 5, 2, "Enter duration (hours): ");
    wattroff(win, COLOR_PAIR(3));

    echo();
    wscanw(win, "%f", &time_in_hours);
    noecho();

    float calories_burned =
        weight_in_kg * exercises[idx].calories_per_kg * time_in_hours;

    wattron(win, COLOR_PAIR(3)); // Green color for calories burned output
    mvwprintw(win, 6, 2, "Calories burned for %s: %.2f cal",
              exercises[idx].name, calories_burned);
    wattroff(win, COLOR_PAIR(3));

    // Log the activity
    strcpy(activityLog[*logCount].name, exercises[idx].name);
    activityLog[*logCount].calories_burned = calories_burned;
    activityLog[*logCount].time_spent = time_in_hours;
    (*logCount)++;

    *total_calories_burned += calories_burned;
    cal_spent = *total_calories_burned;
  }
  // Redraw the border in blue after each key press
  wattron(win, COLOR_PAIR(6)); // Turn on blue color
  wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
          ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wattroff(win, COLOR_PAIR(6)); // Turn off blue color
  wrefresh(win);                // Refresh the window after each key press

  // Wait for 5 seconds before clearing
  sleep(3);

  // Clear activity area and everything except the "Total calories burned"
  clear_screen_except_total(win, *total_calories_burned);

  return 0; // Continue the loop
}


int display_fitness_menu() {
  float weight = user_weight;
  float total_calories_burned = 0.0;
 

  // Initialize ncurses
  initscr();
  start_color(); // Enable color functionality
  cbreak();
  noecho();
  keypad(stdscr, TRUE); // Ensure keypad input is recognized
  curs_set(0);          // Hide cursor

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
  int width = 110, height = 30; // Window size
  WINDOW *win = newwin(height, width, (LINES - height) / 2, (COLS - width) / 2);
  keypad(win, TRUE); // Ensure arrow key input works

  // Set the window to non-blocking mode for input
  nodelay(win, TRUE); // Do not block on wgetch()

  // Load exercises from the CSV file
  initExercises();

  // Main loop for activity entry and calorie calculation
  int exitSignal = 0;
  while (!exitSignal) {
    drawFitnessWindow(win);

    wattron(win, COLOR_PAIR(5)); // Yellow color for "Total calories burned"
    mvwprintw(win, 20 + OFFSET_Y, 2, "Total calories burned: %.2f cal",
              total_calories_burned);
    wattroff(win, COLOR_PAIR(5));

    exitSignal = process_activity(win, weight, &total_calories_burned,
                                  activityLog, &logcount);

    if (exitSignal) {
      display_activity_log(win, activityLog, logcount, total_calories_burned,
                           finalCaloriesConsumed);
      // Keep showing the activity log after "exit"
      while (1) {
        usleep(1000000); // Just update the clock after "exit"
      }
    }

    usleep(1000000); // Sleep for 1 second (for clock update)
  }

  // End ncurses mode
  delwin(win);
  endwin();

  return 0;
}
