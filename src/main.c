#include <stdio.h>

#include "ai.h"
#include "fitness.h"
#include "nutrition.h"


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

