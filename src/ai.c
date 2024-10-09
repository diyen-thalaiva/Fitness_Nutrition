#include <ctype.h>
#include <curl/curl.h>
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> // for sleep()

#include "menu.h"
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

static size_t write_callback(void *ptr, size_t size, size_t nmemb,
                             void *userdata) {
  size_t total_size = size * nmemb;
  strncat(userdata, (char *)ptr, total_size);
  return total_size;
}

static void decode_escaped_characters(char *str) {
  char *src = str;
  char *dest = str;

  while (*src) {
    if (src[0] == '\\' && src[1] == 'n') {
      *dest++ = '\n';
      src += 2;
    } else if (src[0] == '\\' && src[1] == '\"') {
      *dest++ = '\"';
      src += 2;
    } else if (src[0] == '\\' && src[1] == '\\') {
      *dest++ = '\\';
      src += 2;
    } else {
      *dest++ = *src++;
    }
  }

  *dest = '\0';
}

int prompt_user_ai() {
  char input[500];

  printf("Please enter a query: ");
  fgets(input, sizeof(input), stdin);
  input[strcspn(input, "\n")] = 0;

  if (strcmp(input, "exit") == 0) {
    printf("Exiting the program.\n");
    return -1;
  }

  if (!is_fitness_related(input)) {
    printf("Please ask questions related to fitness, nutrition, or physical "
           "exercise.\n\n");
    return 0;
  }

  CURL *curl;
  CURLcode res;
  char *url = "http://localhost:11434/api/generate";
  char response[10000] = {0};
  char extracted_response[10000] = {0};

  char json_payload[500];
  snprintf(json_payload, sizeof(json_payload),
           "{\"model\": \"qwen2.5:0.5b\", \"prompt\": \"%s\", \"stream\": false}",
           input);

  curl = curl_easy_init();

  if (curl) {
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_payload);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    } else {
      extract_response(response, extracted_response);
      printf("Extracted Response:\n%s\n\n", extracted_response);
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
  }

  return 0;
}










int ai_draw_prompt() {
  char prompt_input[MAX_QUERY_LENGTH] = {0}; // Store the input from the user
  char output[MAX_OUTPUT_LENGTH] = {0};      // Store the output response

  // Initialize ncurses
  initscr();
  start_color(); // Enable color functionality
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0); // Hide cursor

  // Initialize colors
  init_pair(1, COLOR_YELLOW, COLOR_BLACK); // Color for query
  init_pair(2, COLOR_CYAN, COLOR_BLACK);   // Header color
  init_pair(3, COLOR_GREEN, COLOR_BLACK);  // Color for response text
  init_pair(4, COLOR_MAGENTA,
            COLOR_BLACK); // Color for submit button and magenta line
  init_pair(5, COLOR_RED, COLOR_BLACK); // Color for time
  init_pair(6, COLOR_BLUE,
            COLOR_BLACK); // Dark blue for horizontal lines and border

  // Create the window and draw the initial layout
  int width = 110, height = 30; // Increased window size
  WINDOW *win = newwin(height, width, (LINES - height) / 2, (COLS - width) / 2);

  // Call the prompt_user function to handle user input
  prompt_user(win, output, prompt_input);

  // End ncurses mode
  delwin(win);
  endwin();

  return 0;
}