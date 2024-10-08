#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "user.h"

int user_m() {
  User users[MAX_USERS];
  int userCount = 0;
  float weight;

  loadProfiles(users, &userCount);

  if (userCount > 0) {
    printf("Existing Profiles:\n");
    displayProfiles(users, userCount);
  } else {
    printf("No profiles found.\n");
  }

  weight = selectOrAddProfile(users, userCount);

  return weight;
}

float selectOrAddProfile(User users[], int userCount) {
  int profileNumber;
  float weight;

  printf("Enter the number of your profile or 0 to add a new profile: ");
  scanf("%d", &profileNumber);
  getchar();

  if (profileNumber > 0 && profileNumber <= userCount) {
    int selectedProfileIndex = profileNumber - 1;
    printf("Profile selected!\n");
    printf(
        "Name: %s\nAge: %d\nGender: %s\nHeight: %.2f meters\nWeight: %.2f kg\n",
        users[selectedProfileIndex].name, users[selectedProfileIndex].age,
        users[selectedProfileIndex].gender, users[selectedProfileIndex].height,
        users[selectedProfileIndex].weight);
    weight = users[selectedProfileIndex].weight;
  } else if (profileNumber == 0) {
    weight = getUserDetails();
  } else {
    printf("Invalid selection. Please try again.\n");
    return selectOrAddProfile(users, userCount);
  }

  return weight;
}

float getUserDetails() {
  User newUser;

  printf("Enter your name: ");
  fgets(newUser.name, sizeof(newUser.name), stdin);
  newUser.name[strcspn(newUser.name, "\n")] = '\0';

  printf("Enter your age: ");
  scanf("%d", &newUser.age);
  printf("Enter your gender (M/F): ");
  scanf("%s", newUser.gender);
  printf("Enter your height (in meters): ");
  scanf("%f", &newUser.height);
  printf("Enter your weight (in kg): ");
  scanf("%f", &newUser.weight);

  saveUser(newUser);

  printf("User details have been saved to 'userdatafit.txt'.\n");

  return newUser.weight;
}

int loadProfiles(User users[], int *userCount) {
  FILE *file = fopen(TXT_SAVE_FILE_PATH, "r");
  if (file == NULL) {
    printf("No profiles found. Starting fresh.\n");
    return 0;
  }

  while (fscanf(file,
                "Name: %[^\n]\nAge: %d\nGender: %s\nHeight: %f meters\nWeight: "
                "%f kg\n\n",
                users[*userCount].name, &users[*userCount].age,
                users[*userCount].gender, &users[*userCount].height,
                &users[*userCount].weight) == 5) {
    (*userCount)++;
  }

  fclose(file);
  return *userCount;
}

void displayProfiles(User users[], int userCount) {
  for (int i = 0; i < userCount; i++) {
    printf("%d. %s\n", i + 1, users[i].name);
  }
}

void saveUser(User user) {
  FILE *file = fopen("dataset/userdatafit.txt", "a");
  if (file == NULL) {
    printf("Error opening file!\n");
    exit(1);
  }

  fprintf(file, "Name: %s\n", user.name);
  fprintf(file, "Age: %d\n", user.age);
  fprintf(file, "Gender: %s\n", user.gender);
  fprintf(file, "Height: %.2f meters\n", user.height);
  fprintf(file, "Weight: %.2f kg\n\n", user.weight);

  fclose(file);
}
