#ifndef STATUS_MENU_H
#define STATUS_MENU_H

int ch;
int rows,cols;
int menu_index;
int previous_index;

void display_branch();
void show_help();
void move_menu(int);
void highlight();
//void print_center(char*);
int left_center_col();
int right_center_col();
void print_files_menu();

#endif
