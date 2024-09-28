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