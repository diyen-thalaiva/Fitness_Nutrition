#ifndef USER_H
#define USER_H

#define TXT_SAVE_FILE_PATH "dataset/userdatafit.txt"
#define MAX_USERS 100

typedef struct {
  char name[50];
  int age;
  char gender[10];
  float height;
  float weight;
} User;

int user_m();
float selectOrAddProfile(User users[], int userCount);
float getUserDetails();
int loadProfiles(User users[], int *userCount);
void displayProfiles(User users[], int userCount);
void saveUser(User user);

#endif
