#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nutrition.h"

char *my_strcasestr2(const char *haystack, const char *needle) {
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

int promptForFood() {
  float totalCaloriesConsumed = 0.0;
  int continueInput = 1;

  while (continueInput) {
    char input[100];
    printf("Enter the food or drink you have consumed (or type 'exit' to finish): ");
    scanf("%s", input);
    input[strcspn(input, "\n")] = '\0';

    if (strcasecmp(input, "exit") == 0) {
      continueInput = 0;
      break;
    }

    int matchCount = 0;
    int matchIndexes[MAX_MATCHES];

    for (int i = 0; i < line_count; i++) {
      if (my_strcasestr2(foodItems[i].food, input) !=
          NULL) { // Substring case-insensitive match
        matchIndexes[matchCount] = i;
        matchCount++;
        if (matchCount >= MAX_MATCHES) {
          break;
        }
      }
    }

    if (matchCount == 0) {
      printf("No matching food or drink found in the list.\n");
    } else if (matchCount == 1) {
      int idx = matchIndexes[0];
      printf("You consumed %s, Serving: %s, Calories per serving: %.2f\n",
             foodItems[idx].food, foodItems[idx].serving,
             foodItems[idx].calories);

      float servings;
      printf("How many servings did you consume? ");
      scanf("%f", &servings);
      while (getchar() != '\n')
        ; // Clear the input buffer

      float totalCalories = servings * foodItems[idx].calories;
      totalCaloriesConsumed += totalCalories;
      printf("Total calories consumed for this food: %.2f Kcal\n",
             totalCalories);
    } else {
      printf("Multiple matches found. Please select the correct food by number:\n");
      for (int i = 0; i < matchCount; i++) {
        
        printf("%d. %s\n", i+1, foodItems[matchIndexes[i]].food);
      
      }


      int choice;
      printf("Enter the number corresponding to the correct food or drink: ");
      scanf("%d", &choice);
      while (getchar() != '\n')
        ; // Clear the input buffer

      if (choice > 0 && choice <= matchCount) {
        int idx = matchIndexes[choice - 1];
        printf("You consumed %s, Serving: %s, Calories per serving: %.2f\n",
               foodItems[idx].food, foodItems[idx].serving,
               foodItems[idx].calories);

        float servings;
        printf("How many servings did you consume? ");
        scanf("%f", &servings);
        while (getchar() != '\n')
          ; // Clear the input buffer

        float totalCalories = servings * foodItems[idx].calories;
        totalCaloriesConsumed += totalCalories;
        printf("Total calories consumed for this food: %.2f Kcal\n",
               totalCalories);
      } else {
        printf("Invalid selection.\n");
      }
    }
  }

  return totalCaloriesConsumed;
}

int initCSV() {
  FILE *file = fopen(CSV_NUTRITION_PATH, "r");
  if (file == NULL) {
    printf("Error: Could not open file.\n");
    return 1;
  }

  char line[MAX_LINE_LENGTH];

  fgets(line, MAX_LINE_LENGTH, file);

  while (fgets(line, MAX_LINE_LENGTH, file) && line_count < MAX_FOOD_ITEMS) {
    sscanf(line, "%[^,],%[^,],%f", foodItems[line_count].food,
           foodItems[line_count].serving, &foodItems[line_count].calories);
    line_count++;
  }

  fclose(file);

  return 0;
}








































// Clear the area used for food input and results, but exclude the time area
void clearFoodEntry(WINDOW *win, int startRow, int endRow, int width) {
  // Adjust the endRow so it doesn't include the time section
  for (int i = startRow; i <= endRow + 3;
       i++) { // -4 keeps the bottom lines untouched
    mvwprintw(win, i, 2, "%*s", width - 4,
              " "); // Clear row content respecting the window width
  }
  // Redraw border after activity input
  wattron(win, COLOR_PAIR(6)); // Turn on blue color
  wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
          ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wattroff(win, COLOR_PAIR(6)); // Turn off blue color
  wrefresh(win);                // Refresh the window after activity input
}

void promptForFood(WINDOW *win, int width) {
  float totalCaloriesConsumed;
  extern float finalCaloriesConsumed;
  int continueInput = 1;

  char consumedFoods[100][50]; // Array to store the names of the consumed foods
  int consumedCount = 0;       // Count of foods consumed

  echo();           // Enable typed character echoing
  int startRow = 5; // Row where food input starts
  int endRow = 20;  // Row where we will clear till

  // Calculate the max length of input based on window width
  int maxInputLength = width - 8; // 6 = 4 spaces for prompt ">" + 2 extra
                                  // padding, adjusted to 8 for safety

  while (continueInput) {
    // Clear previous entry after processing
    clearFoodEntry(win, startRow, endRow, width);

    char input[100];
    wattron(win, COLOR_PAIR(1)); // Add color to the input prompt
    mvwprintw(win, startRow, 2, "> ");
    // Redraw border after activity input
    wattron(win, COLOR_PAIR(6)); // Turn on blue color
    wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
            ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
    wattroff(win, COLOR_PAIR(6)); // Turn off blue color
    wrefresh(win);                // Refresh the window after activity input
    wattroff(win, COLOR_PAIR(1)); // Turn off color after the prompt

    // Input is limited to maxInputLength to avoid overflowing
    mvwgetnstr(win, startRow, 4, input,
               maxInputLength - 1); // Keep 1 char for the null terminator

    // Handle "exit" command
    if (strcasecmp(input, "exit") == 0) {
      continueInput = 0; // Set the flag to stop the loop
      break;             // Exit the loop immediately
    }

















    int matchCount = 0;
    int matchIndexes[MAX_MATCHES];

    // Search for food matches
    for (int i = 0; i < line_count; i++) {
      if (my_strcasestr2(foodItems[i].food, input) != NULL) {
        matchIndexes[matchCount] = i;
        matchCount++;
        if (matchCount >= MAX_MATCHES) {
          break;
        }
      }
    }

    if (matchCount == 0) {
      // No matching food found, display message and wait for 5 seconds
      wattron(win, COLOR_PAIR(3)); // Adding color to "no match found" message
      mvwprintw(win, startRow + 1, 2, "No matching food found for '%s'.",
                input);
      wattroff(win, COLOR_PAIR(3)); // Turn off the color
      // Redraw border after activity input
      wattron(win, COLOR_PAIR(6)); // Turn on blue color
      wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
              ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
      wattroff(win, COLOR_PAIR(6)); // Turn off blue color
      wrefresh(win);                // Refresh the window after activity input
      sleep(2);                     // Wait for 5 seconds before clearing
    } else if (matchCount == 1) {
      int idx = matchIndexes[0];
      wattron(win, COLOR_PAIR(5)); // Red for the food consumption message
      mvwprintw(win, startRow + 1, 2,
                "Consumed %s, %sCalories per serving: %.2f",
                foodItems[idx].food, foodItems[idx].serving,
                foodItems[idx].calories);
      // Redraw border after activity input
      wattron(win, COLOR_PAIR(6)); // Turn on blue color
      wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
              ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
      wattroff(win, COLOR_PAIR(6)); // Turn off blue color
      wrefresh(win);                // Refresh the window after activity input
      wattroff(win, COLOR_PAIR(5)); // Turn off red color

      float servings;
      wattron(win, COLOR_PAIR(4)); // Magenta for servings prompt
      mvwprintw(win, startRow + 2, 2, "How many servings did you consume? ");
      // Redraw border after activity input
      wattron(win, COLOR_PAIR(6)); // Turn on blue color
      wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
              ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
      wattroff(win, COLOR_PAIR(6)); // Turn off blue color
      wrefresh(win);                // Refresh the window after activity input
      wattroff(win, COLOR_PAIR(4)); // Turn off magenta color
      wscanw(win, "%f", &servings);

      float totalCalories = servings * foodItems[idx].calories;
      totalCaloriesConsumed += totalCalories;
      wattron(win, COLOR_PAIR(6)); // Blue for total calories for this food
      mvwprintw(win, startRow + 3, 2,
                "Total calories for this food/drink: %.2f Kcal", totalCalories);
      wattroff(win, COLOR_PAIR(6)); // Turn off blue color
  
      // Store the consumed food name in the array
      strcpy(consumedFoods[consumedCount], foodItems[idx].food);
      consumedCount++;

      // Wait for 5 seconds before clearing the screen
      // Redraw border after activity input
      wattron(win, COLOR_PAIR(6)); // Turn on blue color
      wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
              ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
      wattroff(win, COLOR_PAIR(6)); // Turn off blue color
      wrefresh(win);                // Refresh the window after activity input
      sleep(5);                     // Wait 5 seconds
    } else {
      int selectedIndex = 0;
      int choice = -1;

      // Enable keypad mode for window to capture arrow keys
      keypad(win, TRUE); // Ensure arrow keys work

      // Disable nodelay mode to wait for keypress
      nodelay(win, FALSE);

      while (1) {
        clearFoodEntry(win, startRow + 1, startRow + 4 + matchCount, width);

        wattron(win, COLOR_PAIR(3)); // Color for multiple match prompt
        mvwprintw(win, startRow + 1, 2,
                  "Multiple matches found. Use UP/DOWN arrows to select:");
        wattroff(win, COLOR_PAIR(3)); // Turn off color

        // Display food options and highlight the selected one
        for (int i = 0; i < matchCount; i++) {
          if (i == selectedIndex) {
            wattron(win, A_REVERSE); // Highlight the selected option
          }
          mvwprintw(win, startRow + 2 + i, 2, "%d. %s", i + 1,
                    foodItems[matchIndexes[i]].food);
          wattroff(win, A_REVERSE); // Turn off highlight
        }

        // Redraw border after activity input
        wattron(win, COLOR_PAIR(6)); // Turn on blue color
        wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
                ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
        wattroff(win, COLOR_PAIR(6)); // Turn off blue color
        wrefresh(win);                // Refresh the window after activity input

        int ch = wgetch(win); // Capture keyboard input
        if (ch == KEY_UP) {
          if (selectedIndex > 0) {
            selectedIndex--;
          }
        } else if (ch == KEY_DOWN) {
          if (selectedIndex < matchCount - 1) {
            selectedIndex++;
          }
        } else if (ch == '\n') { // Enter key
          choice = selectedIndex;
          break;
        } else if (ch == KEY_LEFT) {
          menu_system();
          break;
        }
      }

      if (choice >= 0 && choice < matchCount) {
        int idx = matchIndexes[choice];
        wattron(win, COLOR_PAIR(5)); // Red for the selected food
        mvwprintw(win, startRow + 3 + matchCount, 2,
                  "Consumed %s, %sCalories per serving: %.2f",
                  foodItems[idx].food, foodItems[idx].serving,
                  foodItems[idx].calories);
        // Redraw border after activity input
        wattron(win, COLOR_PAIR(6)); // Turn on blue color
        wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
                ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
        wattroff(win, COLOR_PAIR(6)); // Turn off blue color
        wrefresh(win);                // Refresh the window after activity input

        float servings;
        wattron(win, COLOR_PAIR(4)); // Magenta for servings prompt
        mvwprintw(win, startRow + 4 + matchCount, 2,
                  "How many servings did you consume? ");
        wattroff(win, COLOR_PAIR(4)); // Turn off magenta color
        wscanw(win, "%f", &servings);

        float totalCalories = servings * foodItems[idx].calories;
        totalCaloriesConsumed += totalCalories;
        wattron(win, COLOR_PAIR(6)); // Blue for total calories for this food
        mvwprintw(win, startRow + 5 + matchCount, 2,
                  "Total calories for this food: %.2f Kcal", totalCalories);
        wattroff(win, COLOR_PAIR(6)); // Turn off blue color

        strcpy(consumedFoods[consumedCount], foodItems[idx].food);
        consumedCount++;

        // Redraw border after activity input
        wattron(win, COLOR_PAIR(6)); // Turn on blue color
        wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
                ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
        wattroff(win, COLOR_PAIR(6)); // Turn off blue color
        wrefresh(win);                // Refresh the window after activity input
        sleep(5);                     // Wait 5 seconds
      }
    }

    wattron(win, COLOR_PAIR(3)); // Green for total calories consumed
    mvwprintw(win, 20 + OFFSET_Y, 2,
              "Total calories consumed so far: %.2f Kcal",
              totalCaloriesConsumed);
    wattroff(win, COLOR_PAIR(3)); // Turn off green color
    // Redraw border after activity input
    wattron(win, COLOR_PAIR(6)); // Turn on blue color
    wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
            ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
    wattroff(win, COLOR_PAIR(6)); // Turn off blue color
    wrefresh(win);                // Refresh the window after activity input
  }

  noecho(); // Disable character echoing after input

  finalCaloriesConsumed = totalCaloriesConsumed;

  // Display all the food that the user has consumed when "exit" is typed
  clearFoodEntry(win, startRow, endRow,
                 width);       // Clear the screen for the summary
  wattron(win, COLOR_PAIR(5)); // Color for food list
  mvwprintw(win, startRow, 2, "You have eaten/drank the following items:");

  for (int i = 0; i < consumedCount; i++) {
    mvwprintw(win, startRow + i + 1, 2, "%d. %s", i + 1,
              consumedFoods[i]); // List all consumed food items
  }
  wattroff(win, COLOR_PAIR(1)); // Turn off the color for the summary
  // Redraw border after activity input
  wattron(win, COLOR_PAIR(6)); // Turn on blue color
  wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
          ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wattroff(win, COLOR_PAIR(6)); // Turn off blue color
  wrefresh(win);                // Refresh the window after activity input
  
  sleep(5);
  display_fitness_menu();

  // No exit or auto-close; leave the window open indefinitely for the user to
  // close manually
  while (1) {
    // Infinite loop to keep the list displayed
  }
}











int display_nutrition_menu() {
  // Initialize food items directly within the program
  initFoodItems();

  // Initialize ncurses
  initscr();
  start_color(); // Enable color functionality
  cbreak();
  noecho();
  keypad(stdscr, TRUE); // Enable keypad input to capture arrow keys globally
  curs_set(0);          // Hide cursor

  // Initialize colors
  init_pair(1, COLOR_YELLOW, COLOR_BLACK);  // Color for query
  init_pair(2, COLOR_CYAN, COLOR_BLACK);    // Header color
  init_pair(3, COLOR_GREEN, COLOR_BLACK);   // Green for total calories consumed
  init_pair(4, COLOR_MAGENTA, COLOR_BLACK); // Magenta for servings prompt
  init_pair(5, COLOR_RED, COLOR_BLACK);     // Red for food details
  init_pair(6, COLOR_BLUE,
            COLOR_BLACK); // Blue for total calories for this food
  init_pair(7, COLOR_WHITE, COLOR_BLACK); // White for text
  init_pair(8, COLOR_BLACK, COLOR_BLACK); // White for text

  // Create the window and draw the layout
  int width = 110, height = 30; // Window size

  // Dynamically center the window based on terminal size
  int start_y = (LINES - height) / 2;
  int start_x = (COLS - width) / 2;
  WINDOW *win = newwin(height, width, start_y, start_x);

  // Set the window to blocking mode for input (disable nodelay)
  nodelay(win, FALSE); // Wait for keypress when necessary

  while (1) {
    drawLayout(win, width);
    promptForFood(win, width);

    display_time(win, height, width);

    usleep(1000000);      // Sleep for 1 second
    int ch = wgetch(win); // Non-blocking input
    if (ch == 'q')        // Press 'q' to exit
      break;
  }

  // End ncurses mode
  delwin(win);
  endwin();

  return 0;
}
