#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024
#define MAX_FOOD_ITEMS 100

// Define a structure to hold food information
typedef struct FoodItem {
    char food[100];
    char serving[100];
    char calories[100];
} FoodItem;

void extractServingData(FoodItem* item) {
    int amount = 0;
    // Extract the first number (if any)
    sscanf(item->serving, "%d", &amount);

     


    char *unit;

    // Extract the first word (string) after the number
    char *token = strtok(item->serving, " ");
    if (token != NULL) {
        token = strtok(NULL, " ");  // Skip the number and get the first string
        if (token != NULL) {
            strcpy(unit, token);
        }
    }

    printf("amount %d serving %s\n", amount, unit);
}

// Function to prompt user for a food and display the corresponding calories
void promptForFood(FoodItem foodItems[], int itemCount) {
    char input[100];
    printf("Enter the food you have consumed: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; // Remove trailing newline

    // Search for the food in the list
    int found = 0;
    for (int i = 0; i < itemCount; i++) {
        if (strcasecmp(input, foodItems[i].food) == 0) { // Case-insensitive comparison
            printf("You consumed %s, Serving: %s, Calories: %s\n", 
                    foodItems[i].food, foodItems[i].serving, foodItems[i].calories);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Food not found in the list.\n");
    }
}

int main() {
    FILE *file = fopen("FoodandCalories.csv", "r");
    if (file == NULL) {
        printf("Error: Could not open file.\n");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    FoodItem foodItems[MAX_FOOD_ITEMS];
    int line_count = 0;

    // Skip the header line
    fgets(line, MAX_LINE_LENGTH, file);

    int i = 0;

   

    // Read the file and store food items in an array
    while (fgets(line, MAX_LINE_LENGTH, file) && line_count < MAX_FOOD_ITEMS) {
        sscanf(line, "%[^,],%[^,],%[^\n]", foodItems[line_count].food, 
               foodItems[line_count].serving, foodItems[line_count].calories);
        line_count++;

        if (i++ >= 5)
        {
           break;
        }
        
    }

    fclose(file);
    extractServingData(&foodItems[2]);
    // Call the function to prompt for food and display calories
    // promptForFood(foodItems, line_count);

    return 0;
}