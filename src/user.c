#include "users.h"
#include "globals.h"
#include "nutrition.h"
#include "utils.h"
#include <curses.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "users.h"




// Function prototypes
void displayProfiles(WINDOW *win, User users[], int userCount, int highlight);
void displayUserDetails(WINDOW *win, User user);
int navigateAndSelectProfile(WINDOW *win, User users[], int *userCount);
float getUserDetails(WINDOW *win, User *newUser);
void drawUsersWindow(WINDOW *win, int height, int width);

// Function to load profiles from the text file in labeled format
int loadProfiles(User users[], int *userCount) {
  FILE *file = fopen(FILE_PATH, "r");
  if (!file) {
    mvprintw(0, 0, "Error: Could not open file for reading.");
    return 0;
  }

  *userCount = 0;
  char buffer[100];
  while (fgets(buffer, sizeof(buffer), file)) {
    if (strncmp(buffer, "Name:", 5) == 0) {
      // Read the user's name
      sscanf(buffer, "Name: %[^\n]", users[*userCount].name);

      // Read age
      fgets(buffer, sizeof(buffer), file);
      sscanf(buffer, "Age: %d", &users[*userCount].age);

      // Read gender
      fgets(buffer, sizeof(buffer), file);
      sscanf(buffer, "Gender: %s", users[*userCount].gender);

      // Read height
      fgets(buffer, sizeof(buffer), file);
      sscanf(buffer, "Height: %f", &users[*userCount].height);

      // Read weight
      fgets(buffer, sizeof(buffer), file);
      sscanf(buffer, "Weight: %f", &users[*userCount].weight);

      // Skip blank line between profiles
      fgets(buffer, sizeof(buffer), file);

      (*userCount)++;
      if (*userCount >= MAX_USERS) {
        break;
      }
    }
  }

  fclose(file);
  return *userCount;
}

// Function to save a profile to the text file in labeled format
void saveProfileToFile(User user) {
  FILE *file = fopen(FILE_PATH, "a");
  if (!file) {
    mvprintw(0, 0, "Error: Could not open file for writing.");
    return;
  }

  fprintf(file, "Name: %s\n", user.name);
  fprintf(file, "Age: %d\n", user.age);
  fprintf(file, "Gender: %s\n", user.gender);
  fprintf(file, "Height: %.2f meters\n", user.height);
  fprintf(file, "Weight: %.2f kg\n\n", user.weight);

  fclose(file);
}

void displayProfiles(WINDOW *win, User users[], int userCount, int highlight) {
  for (int i = 0; i < userCount; i++) {
    if (i == highlight) {
      wattron(win, A_REVERSE); // Highlight the selected user
    }
    wattron(win, COLOR_PAIR(7)); // Set color for profile names
    mvwprintw(win, 5 + i, 2, "%d. %s", i + 1,
              users[i].name);     // Display user names starting at x = 2
    wattroff(win, COLOR_PAIR(7)); // Turn off color for profile names
    wattroff(win, A_REVERSE);     // Turn off the highlight
  }

  // Show the "Add New Profile" option at the bottom
  if (userCount < MAX_USERS) {
    if (userCount == highlight) {
      wattron(win, A_REVERSE); // Highlight "Add New Profile" option
    }
    wattron(win, COLOR_PAIR(4)); // Set color for "Add New Profile"
    mvwprintw(win, 5 + userCount, 2, "Add New Profile"); // Display the option
    wattroff(win, COLOR_PAIR(4)); // Turn off color for "Add New Profile"
    wattroff(win, A_REVERSE);     // Turn off the highlight
  }

  wattron(win, COLOR_PAIR(2)); // Color for navigation instruction text
  mvwprintw(win, 5 + userCount + 2, 2,
            "Use UP/DOWN to navigate, ENTER to select.");
  wattroff(win,
           COLOR_PAIR(2)); // Turn off color for navigation instruction text

  // Redraw border after activity input
  wattron(win, COLOR_PAIR(6)); // Turn on blue color
  wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
          ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wattroff(win, COLOR_PAIR(6)); // Turn off blue color
  wrefresh(win);                // Refresh the window after activity input
}
