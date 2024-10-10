#ifndef NUTRITION_H 
#define NUTRITION_H
#include <curses.h>

#define MAX_FOOD_ITEMS 1000
#define MAX_MATCHES 13

// Define the structure for food items
typedef struct {
  char food[4000];
  char serving[500];
  float calories;
} FoodItem;

extern float finalCaloriesConsumed;
void initFoodItems();
char *my_strcasestr2(const char *haystack, const char *needle);
void drawLayout(WINDOW *win, int width);
void clearFoodEntry(WINDOW *win, int startRow, int endRow, int width);
void promptForFood(WINDOW *win, int width);


int display_nutrition_menu();

#endif // !NUTRITION_H
