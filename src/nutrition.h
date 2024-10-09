#ifndef NUTRITION_H
#define NUTRITION_H

#define MAX_LINE_LENGTH 1024
#define MAX_FOOD_ITEMS 1000
#define MAX_MATCHES 50
#define CSV_NUTRITION_PATH "dataset/FoodandCalories.csv"

typedef struct FoodItem {
  char food[100];
  char serving[100];
  float calories;
} FoodItem;

static FoodItem foodItems[MAX_FOOD_ITEMS];
static int line_count = 0;

char *my_strcasestr2(const char *haystack, const char *needle);
int promptForFood();
int initCSV();

#endif
