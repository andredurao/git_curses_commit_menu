#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include "status_menu.h"
#include "git_status.h"

void display_branch(){
  mvprintw(4, left_center_col(), get_branch_name());
}

void show_help(){
  clear();
  mvprintw(0, 0, "Help");
  getch();
  clear();
  print_files_menu();
}

void move_menu(int dir){
  previous_index = menu_index;
  menu_index= (menu_index+6+dir) % 6;  
  mvchgat(5+previous_index, left_center_col(), 6, A_NORMAL, 0, NULL);
  highlight();
}

void highlight(){
  mvchgat(5+menu_index, left_center_col(), 6, A_REVERSE, 0, NULL);
}

/*void print_center(char* mesg){
  mvprintw(rows/2,(cols-strlen(mesg))/2,"%s",mesg);
}*/

int left_center_col(){
  return cols/4;
}

int right_center_col(){
  return cols*3/4;
}

void print_files_menu(){
  int i=0;

  display_branch();

	for (i = 0; i < maxi; ++i) {
    //mvprintw(5+i, left_center_col(), repofile_list[i]->filename);
    mvprintw(5+i, left_center_col(), formatted_filename(i));
  }
  highlight();
}

void check_row(){
  repofile_list[menu_index]->check = !repofile_list[menu_index]->check;
  print_files_menu();
}
