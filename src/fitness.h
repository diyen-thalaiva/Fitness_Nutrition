#ifndef FITNESS_H
#define FITNESS_H

#define MAX_LINE_LENGTH_FIT 1024
#define MAX_ACTIVITIES_FIT 1000
#define MAX_MATCHES_FIT 50
#define CSV_FITNESS_PATH "dataset/exercise_dataset.csv"

typedef struct {
  char name[MAX_LINE_LENGTH_FIT];
  float calories[4];
  float calories_per_kg;
} Exercise;

static Exercise exercises[MAX_ACTIVITIES_FIT];

char *my_strcasestr(const char *haystack, const char *needle);
int find_exercise(const char *input, Exercise exercises[], int count,
                  int matchIndexes[]);
void display_intensity_options(Exercise exercises[], int matchIndexes[],
                               int matchCount);
int load_exercises(const char *filename, Exercise exercises[]);
float initCSV2(float weight);

#endif // FITNESS_H
