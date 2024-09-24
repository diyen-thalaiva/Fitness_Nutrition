#include "ai.h"


static const char *fitness_keywords[] = {
    "fitness",   "nutrition", "exercise", "workout",  "diet", "calories",
    "protein",   "muscle",    "cardio",   "strength", "yoga", "hydration",
    "endurance", "training",  "wellness", "recovery", "fat",  "health",
    "gym",       "weight",    "lift"};

static int num_keywords =
    sizeof(fitness_keywords) / sizeof(fitness_keywords[0]);
