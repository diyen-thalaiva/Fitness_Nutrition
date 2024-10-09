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
