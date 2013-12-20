#ifndef STATUS_MENU_H
#define STATUS_MENU_H
#include "git_status.h"

int ch;
int rows,cols;
int menu_index;
int previous_index;
WINDOW *diff_window; 
char msg[360];

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
void check_all();
int menu_length();

void open_commit_window();

#endif
