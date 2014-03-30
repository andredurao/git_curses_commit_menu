#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <ncurses.h>
#include <string.h>
#include "status_menu.h"
#include "git_status.h"

//TODO: If a directory has none changed or new files the app should quit
//TODO: If a repo has already modified, new or deleted files on the index they should
//      start checked, before commit it should reset the unchecked files

int main(int argc, char** argv){
  setlocale(LC_ALL, "en_US.utf8");  
  initial_check();
  get_files_list();

  ESCDELAY = 1;
  menu_index = 0;
  previous_index = -1;
  
  initscr();
  noecho();
  keypad(stdscr, TRUE);
  getmaxyx(stdscr,rows,cols);
  diff_col_width = cols/2-1;
  start_color();
  use_default_colors();
  init_pair(1, COLOR_RED,     -1);
  init_pair(2, COLOR_GREEN,   -1);
  init_pair(3, COLOR_CYAN,    -1);
  refresh();

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
      case ' ':
        check_row();
        break;
      case 'a':
        check_all();
        break;
      case 'c':
        open_system_index_add_window();
        break;
    }
    ch = getch();  
  }
  
  endwin();
  git_threads_shutdown();
  return(EXIT_SUCCESS);
}
