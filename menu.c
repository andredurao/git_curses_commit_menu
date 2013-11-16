#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include "status_menu.h"


//TODO: try to only use the help as a window, the main window is the stdscr
int main(int argc, char** argv){
  
  ESCDELAY = 1;
  menu_index = 0;
  previous_index =- 1;
  
  initscr();
  noecho();
  keypad(stdscr, TRUE);
  getmaxyx(stdscr,rows,cols);
  refresh();

  print_files_menu();

  //move(5,right_center_col());
  //printw("diff");
  ch = getch();  
  while(ch != 27){
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


