#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
//include "utils.h"
int ch;
int rows,cols;
int menu_index = 0;
int previous_index = -1;
bool quit;

WINDOW *commit_window;
WINDOW *help_window;

void print_center(char* mesg);
int left_center_col();
int right_center_col();
void print_files_menu();
void move_menu(int dir);
void highlight_menu();
void show_help();
void highlight();

//TODO: try to only use the help as a window, the main window is the stdscr
int main(int argc, char** argv){
  ESCDELAY = 1;
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

void print_center(char* mesg){
  mvprintw(rows/2,(cols-strlen(mesg))/2,"%s",mesg);
}

int left_center_col(){
  return cols/4;
}

int right_center_col(){
  return cols*3/4;
}

void print_files_menu(){
  char files[6][10]= {"file1","file2","file3","file4","file5","file6"};
  int i=0;
  while (i<6){
    mvprintw(5+i, left_center_col(), files[i]);
    i++;
  }
  highlight();
}

