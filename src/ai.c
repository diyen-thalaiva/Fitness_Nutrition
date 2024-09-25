#include <ctype.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ai.h"

static const char *fitness_keywords[] = {
    "fitness",   "nutrition", "exercise", "workout",  "diet", "calories",
    "protein",   "muscle",    "cardio",   "strength", "yoga", "hydration",
    "endurance", "training",  "wellness", "recovery", "fat",  "health",
    "gym",       "weight",    "lift"};

static int num_keywords =
    sizeof(fitness_keywords) / sizeof(fitness_keywords[0]);

static int is_fitness_related(char *input) {
  char lower_input[500];
  for (int i = 0; input[i]; i++) {
    lower_input[i] = tolower(input[i]);
  }
  lower_input[strlen(input)] = '\0';

  for (int i = 0; i < num_keywords; i++) {
    if (strstr(lower_input, fitness_keywords[i]) != NULL) {
      return 1;
    }
  }
  return 0;
}

static void extract_response(char *json_response, char *output) {
  char *start = strstr(json_response, "\"response\":");
  if (start != NULL) {
    start += strlen("\"response\":");

    while (*start != '\"')
      start++;
    start++;

    char *end = start;
    while (*end != '\"' || (*(end - 1) == '\\' && *(end - 2) != '\\')) {
      end++;
    }

    strncpy(output, start, end - start);
    output[end - start] = '\0';

    decode_escaped_characters(output);
  } else {
    strcpy(output, "No response found");
  }
}
