#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "user.h"

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

