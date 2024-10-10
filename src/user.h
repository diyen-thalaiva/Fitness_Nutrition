#ifndef USERS_H
#define USERS_H
#include <curses.h>

#define MAX_USERS 10
#define FILE_PATH "./dataset/userdatafit.txt"

// Define the User structure
typedef struct {
  char name[50];
  int age;
  char gender[10];
  float height;
  float weight;
} User;

int loadProfiles(User users[], int *userCount);
void saveProfileToFile(User user);
void displayProfiles(WINDOW *win, User users[], int userCount, int highlight);
void displayUserDetails(WINDOW *win, User user);
int navigateAndSelectProfile(WINDOW *win, User users[], int *userCount);
float getUserDetails(WINDOW *win, User *newUser);
void drawUsersWindow(WINDOW *win, int height, int width);
int draw_users_menu();


int draw_users_menu();

#endif // !USERS_H
