#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include "status_menu.h"
#include "git_status.h"


int main(int argc, char** argv){
  
  initial_check();

  ESCDELAY = 1;
  menu_index = 0;
  previous_index =- 1;
  
  initscr();
  noecho();
  keypad(stdscr, TRUE);
  getmaxyx(stdscr,rows,cols);
  refresh();

  display_branch();
  print_files_menu();

  //move(5,right_center_col());
  //printw("diff");
  ch = getch();  
  while(ch != 27 && ch != 'q'){
    switch(ch){
      case KEY_UP: 
        move_menu(-1);
        break;
      case KEY_DOWN: 
        move_menu(1);
        break;
      case 'h':
        show_help();
        break;
      case '?':
        show_help();
        break;
    }
    ch = getch();  
  }
  
  endwin();

  return(EXIT_SUCCESS);
}


