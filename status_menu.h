#ifndef STATUS_MENU_H
#define STATUS_MENU_H
#include "git_status.h"

int ch;
int rows,cols;
int menu_index;
int previous_index;

void display_branch();
void show_git_diff();
void show_help();
void move_menu(int);
void highlight();
//void print_center(char*);
int left_center_col();
int right_center_col();
int center_col();
void print_files_menu();
void check_row();
int menu_length();

#endif
