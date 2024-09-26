#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fitness.h"

int load_exercises(const char *filename, Exercise exercises[]) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Error: Could not open file.\n");
    return -1;
  }

  char line[MAX_LINE_LENGTH_FIT];
  int count = 0;

  fgets(line, MAX_LINE_LENGTH_FIT, file);

  while (fgets(line, MAX_LINE_LENGTH_FIT, file) && count < MAX_ACTIVITIES_FIT) {
    if (line[0] == '\n') {
      continue;
    }

    if (sscanf(line, "\"%[^\"]\",%*d,%*d,%*d,%*d,%f", exercises[count].name,
               &exercises[count].calories_per_kg) != 2) {
      continue;
    }

    count++;
  }

  fclose(file);
  return count;
}

float initCSV2(float weight) {
  int exercise_count =
      load_exercises(CSV_FITNESS_PATH, exercises);
  if (exercise_count == -1) {
    return 1;
  }

  float weight_in_kg = weight;
  char activity_input[MAX_LINE_LENGTH_FIT];
  float total_calories_burned = 0.0;

  while (1) {
    printf("\nEnter an activity (or 'exit' to quit): ");
    scanf("%s", activity_input);
    activity_input[strcspn(activity_input, "\n")] = '\0';

    if (strcmp(activity_input, "exit") == 0) {
      break;
    }

    if (strlen(activity_input) == 0) {
      printf("No input provided. Please enter an activity.\n");
      continue;
    }

    int matchIndexes[MAX_MATCHES_FIT];
    int matchCount =
        find_exercise(activity_input, exercises, exercise_count, matchIndexes);

    if (matchCount == 0) {
      printf("No matching exercise found.\n");
    } else if (matchCount == 1) {
      int idx = matchIndexes[0];

      float time_in_hours;
      printf("Enter the duration of the activity in hours: ");
      while (scanf("%f", &time_in_hours) != 1) {
        printf("Invalid input. Please enter a valid number for duration: ");
        while (getchar() != '\n')
          ;
      }
      getchar();

      float calories_burned =
          weight_in_kg * exercises[idx].calories_per_kg * time_in_hours;

      printf("You selected: %s\n", exercises[idx].name);
      printf("Calories burned: %.2f Kcal\n", calories_burned);
      total_calories_burned += calories_burned;

    } else {
      display_intensity_options(exercises, matchIndexes, matchCount);

      int choice;
      printf("Enter the number corresponding to the correct activity: ");
      while (scanf("%d", &choice) != 1 || choice <= 0 || choice > matchCount) {
        printf("Invalid selection. Please enter a number between 1 and %d: ",
               matchCount);
        while (getchar() != '\n')
          ;
      }
      getchar();

      int idx = matchIndexes[choice - 1];
      printf("You selected: %s\n", exercises[idx].name);

      float time_in_hours;
      printf("Enter the duration of the activity in hours: ");
      while (scanf("%f", &time_in_hours) != 1) {
        printf("Invalid input. Please enter a valid number for duration: ");
        while (getchar() != '\n')
          ;
      }
      getchar();

      float calories_burned =
          weight_in_kg * exercises[idx].calories_per_kg * time_in_hours;

      printf("Calories burned: %.2f Kcal\n", calories_burned);
      total_calories_burned += calories_burned;
    }
  }

  return total_calories_burned;
}
