#ifndef AI_H
#define AI_H

#define MAX_OUTPUT_LENGTH 2000 // Increased for larger output
#define MAX_QUERY_LENGTH 500
#define OFFSET_Y 6

// Fitness keywords for validation
static const char *fitness_keywords[] = {
    "fitness",  "nutrition", "exercise",  "workout",  "diet",
    "calories", "protein",   "muscle",    "cardio",   "strength",
    "yoga",     "hydration", "endurance", "training", "wellness",
    "recovery", "fat",       "health",    "gym",      "weight"};
static int num_keywords =
    sizeof(fitness_keywords) / sizeof(fitness_keywords[0]);


int ai_draw_prompt();

#endif // !AI_H
