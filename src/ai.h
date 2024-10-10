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
"recovery", "fat",       "health",    "gym",      "weight",
"aerobics", "agility",   "antioxidants", "appetite", "balance",
"barbell",  "bench press", "biomechanics", "BMI",    "body fat",
"body mass", "bootcamp", "breathing", "burpees", "caffeine",
"calisthenics", "carbohydrates", "cardiorespiratory", "cholesterol", "circuit",
"clean eating", "conditioning", "crossfit", "cut", "deadlift",
"deficit", "dehydration", "detox", "dumbbell", "dynamic stretches",
"eating habits", "electrolytes", "endorphins", "energy", "fatigue",
"fiber", "flexibility", "food diary", "food pyramid", "free weights",
"glucose", "glycogen", "green tea", "growth", "gut health",
"HIIT", "hormones", "hypertrophy", "inflammation", "injury prevention",
"intermittent fasting", "isometric", "jump rope", "kettlebell", "keto",
"lean body", "lifestyle", "macronutrients", "metabolism", "micronutrients",
"mobility", "motivation", "multivitamins", "natural foods", "nutrient-dense",
"nutritional value", "organic", "overtraining", "oxygen", "paleo",
"pilates", "plank", "portion control", "posture", "powerlifting",
"pre-workout", "recovery meal", "repetitions", "resistance", "rest day",
"running", "salad", "saturated fats", "self-discipline", "sleep",
"slow carbs", "sodium", "sports nutrition", "squats", "stability",
"stamina", "steady state", "stress management", "supplements", "toning",
"trans fats", "treadmill", "vegan", "vegetables", "vitamins"};
static int num_keywords =
    sizeof(fitness_keywords) / sizeof(fitness_keywords[0]);


int ai_draw_prompt();

#endif // !AI_H
