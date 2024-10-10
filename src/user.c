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

void displayUserDetails(WINDOW *win, User user) {
  wclear(win); // Clear the window before drawing profile details

  // Redraw the window after clearing it
  drawUsersWindow(win, 20, 110);

  wattron(win, COLOR_PAIR(1)); // Set color for user details
  mvwprintw(win, 5, 2, "Name: %s",
            user.name); // Adjusted all x-coordinates to 2 for left alignment
  mvwprintw(win, 6, 2, "Age: %d", user.age);
  mvwprintw(win, 7, 2, "Gender: %s", user.gender);
  mvwprintw(win, 8, 2, "Height: %.2f meters", user.height);
  mvwprintw(win, 9, 2, "Weight: %.2f kg", user.weight);
  wattroff(win, COLOR_PAIR(1)); // Turn off color for user details

  // Redraw border after activity input
  wattron(win, COLOR_PAIR(6)); // Turn on blue color
  wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
          ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wattroff(win, COLOR_PAIR(6)); // Turn off blue color
  wrefresh(win);                // Refresh the window after activity input
}

int navigateAndSelectProfile(WINDOW *win, User users[], int *userCount) {
  int highlight = 0; // Track which profile is highlighted
  int choice;
  int selected = -1;

  keypad(win, TRUE); // Enable arrow key input

  while (1) {
    // Only update the highlighted part, avoid clearing the window
    displayProfiles(win, users, *userCount,
                    highlight); // Display profiles with highlighting

    choice = wgetch(win); // Capture user input

    switch (choice) {
    case KEY_UP:
      if (highlight > 0)
        highlight--;
      break;
    case KEY_DOWN:
      if (highlight < *userCount && highlight < MAX_USERS)
        highlight++;
      break;
    case 10: // Enter key
      if (highlight == *userCount && *userCount < MAX_USERS) {
        // Add new profile when highlighted option is "Add New Profile"
        wattron(win, COLOR_PAIR(5)); // Set color for adding new profile text
        mvwprintw(win, 25, 2, "Adding a new profile!\n");
        wattron(win, COLOR_PAIR(6)); // Turn on blue color
        wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
                ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
        wattroff(win, COLOR_PAIR(6)); // Turn off blue color
        wrefresh(win);                // Refresh the window after activity input
        wattroff(win,
                 COLOR_PAIR(5)); // Turn off color for adding new profile text
        getUserDetails(win, &users[*userCount]); // Add new profile
        saveProfileToFile(users[*userCount]);
        (*userCount)++;                          // Increment the user count
        wclear(win); // Clear the window after adding the profile
        drawUsersWindow(win, 20, 110); // Redraw the window
        highlight = *userCount - 1;    // Set highlight to new profile
      } else {
        selected = highlight;
        return selected; // Return the selected profile index
      }
      break;
    default:
      break;
    }
  }
}

float getUserDetails(WINDOW *win, User *newUser) {
  echo();

  // Set a safe maximum input width for each field to avoid breaking the right
  // border
  int inputMaxWidth =
      50; // Maximum width for input (adjust to window width as necessary)

  wattron(win, COLOR_PAIR(2)); // Set color for input text prompts
  mvwprintw(win, 10, 2, "Enter your name: ");
  // Redraw border after activity input
  wattron(win, COLOR_PAIR(6)); // Turn on blue color
  wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
          ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wattroff(win, COLOR_PAIR(6)); // Turn off blue color
  wrefresh(win);                // Refresh the window after activity input
  wclrtoeol(
      win); // Clear the line before input to ensure old input doesn't persist
  // Redraw border after activity input
  wattron(win, COLOR_PAIR(6)); // Turn on blue color
  wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
          ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wattroff(win, COLOR_PAIR(6)); // Turn off blue color
  wrefresh(win);                // Refresh the window after activity input
  wgetnstr(win, newUser->name,
           inputMaxWidth - 1); // Limit input length to fit within the window

  wattron(win, COLOR_PAIR(2)); // Set color for input text prompts
  mvwprintw(win, 11, 2, "Enter your age: ");
  // Redraw border after activity input
  wattron(win, COLOR_PAIR(6)); // Turn on blue color
  wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
          ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wattroff(win, COLOR_PAIR(6)); // Turn off blue color
  wrefresh(win);                // Refresh the window after activity input
  wclrtoeol(win);               // Clear the line before input
  // Redraw border after activity input
  wattron(win, COLOR_PAIR(6)); // Turn on blue color
  wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
          ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wattroff(win, COLOR_PAIR(6)); // Turn off blue color
  wrefresh(win);                // Refresh the window after activity input
  wscanw(win, "%d", &newUser->age);

  wattron(win, COLOR_PAIR(2)); // Set color for input text prompts
  mvwprintw(win, 12, 2, "Enter your gender (M/F): ");
  // Redraw border after activity input
  wattron(win, COLOR_PAIR(6)); // Turn on blue color
  wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
          ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wattroff(win, COLOR_PAIR(6)); // Turn off blue color
  wrefresh(win);                // Refresh the window after activity input
  wclrtoeol(win);               // Clear the line before input
  // Redraw border after activity input
  wattron(win, COLOR_PAIR(6)); // Turn on blue color
  wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
          ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wattroff(win, COLOR_PAIR(6)); // Turn off blue color
  wrefresh(win);                // Refresh the window after activity input
  wscanw(win, "%s", newUser->gender);

  wattron(win, COLOR_PAIR(2)); // Set color for input text prompts
  mvwprintw(win, 13, 2, "Enter your height (in meters): ");
  // Redraw border after activity input
  wattron(win, COLOR_PAIR(6)); // Turn on blue color
  wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
          ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wattroff(win, COLOR_PAIR(6)); // Turn off blue color
  wrefresh(win);                // Refresh the window after activity input
  wclrtoeol(win);               // Clear the line before input
  // Redraw border after activity input
  wattron(win, COLOR_PAIR(6)); // Turn on blue color
  wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
          ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wattroff(win, COLOR_PAIR(6)); // Turn off blue color
  wrefresh(win);                // Refresh the window after activity input
  wscanw(win, "%f", &newUser->height);

  wattron(win, COLOR_PAIR(2)); // Set color for input text prompts
  mvwprintw(win, 14, 2, "Enter your weight (in kg): ");
  // Redraw border after activity input
  wattron(win, COLOR_PAIR(6)); // Turn on blue color
  wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
          ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wattroff(win, COLOR_PAIR(6)); // Turn off blue color
  wrefresh(win);                // Refresh the window after activity input
  wclrtoeol(win);               // Clear the line before input
  // Redraw border after activity input
  wattron(win, COLOR_PAIR(6)); // Turn on blue color
  wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
          ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wattroff(win, COLOR_PAIR(6)); // Turn off blue color
  wrefresh(win);                // Refresh the window after activity input
  wscanw(win, "%f", &newUser->weight);

  wattroff(win, COLOR_PAIR(3)); // Turn off color for input text prompts

  // Redraw border after activity input
  wattron(win, COLOR_PAIR(6)); // Turn on blue color
  wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
          ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wattroff(win, COLOR_PAIR(6)); // Turn off blue color
  wrefresh(win);                // Refresh the window after activity input

  wattron(win, COLOR_PAIR(5)); // Set color for success message
  mvwprintw(win, 25, 2, "New profile added successfully!\n");
  wattroff(win, COLOR_PAIR(5)); // Turn off color for success message

  // Redraw border after activity input
  wattron(win, COLOR_PAIR(6)); // Turn on blue color
  wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
          ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wattroff(win, COLOR_PAIR(6)); // Turn off blue color
  wrefresh(win);                // Refresh the window after activity input
  usleep(1000000);              // Delay for 1 second

  noecho();

  return newUser->weight;
}
