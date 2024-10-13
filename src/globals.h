#ifndef GLOBALS_H 
#define GLOBALS_H
#include <string.h> 
#include "fitness.h"
#define OFFSET_Y 6



#define MAX_CONSUMED 100 // Adjust based on your needs
extern char consumedFoods[MAX_CONSUMED][50]; // Global variable declaration
extern int consumedCount; // To track the number of consumed items


#define MAX_ACTIVITY_LOG 1000
extern ActivityLog activityLog[MAX_ACTIVITY_LOG];
extern int logcount;



#endif // !GLOBALS_H
