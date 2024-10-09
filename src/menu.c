#include <stdio.h>

#include "ai.h"
#include "fitness.h"
#include "nutrition.h"
#include "user.h"

void fitness_tracker() {
  float weight_fit = user_m();
  initCSV();
  float cal_consumed = promptForFood();

  float cal_burned = initCSV2(weight_fit);
  printf("\n");
  printf("Today's calorie information:\n");
  printf("Calories consumed: %.2f Kcal\n", cal_consumed);
  printf("Calories burned: %.2f Kcal\n", cal_burned);
  printf("\n");

  if (cal_consumed > cal_burned) {
    float surplus = cal_consumed - cal_burned;
    printf("Calorie surplus: %.2f Kcal\n\n", surplus);
    printf("You consumed more calories than you burned today. Consider "
           "adjusting your intake or increasing physical activity.\n");
  } else if (cal_consumed < cal_burned) {
    float deficit = cal_burned - cal_consumed;
    printf("Calorie deficit: %.2f Kcal\n\n", deficit);
    printf("You burned more calories than you consumed today. Great job! "
           "Ensure you are eating enough to meet your nutritional needs.\n");
  } else {
    printf("Calorie balance: %.2f Kcal\n", cal_consumed - cal_burned);
    printf("You consumed and burned an equal amount of calories today. Well "
           "balanced!\n");
  }
}

int main(void) {
  int option = -1;

  while (1) {
    printf("Select option:\n");
    printf("1 Ask LomeshAI\n");
    printf("2 Fitness & Nutrition Tracker\n");
    printf("3 Quit\n");
    printf("Enter input: ");
    scanf("%d", &option);

    switch (option) {
    case 1:
      while (1) {
        if (prompt_user_ai() == -1)
          break;
      }
      break;
    case 2:
      fitness_tracker();
      break;
    case 3:
      goto exit;
    default:
      printf("Invalid input. ");
      break;
    }

    option = -1;
  }

exit:

  return 0;
}
