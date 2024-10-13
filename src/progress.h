#ifndef PROGRESS_H
#define PROGRESS_H
#include <curses.h>


void display_calorie_balance(WINDOW *win, float cal_consumed, float cal_spent, int height);
void drawWindowprogress(WINDOW *win, float cal_consumed, float cal_spent);
int display_progress_menu(float consumed , float spent);



#endif