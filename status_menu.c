#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include "status_menu.h"
#include "git_status.h"

void display_branch(){
  mvprintw(0, 0, get_branch_name());
}

void show_git_diff(){
  int y=0;
  int diff_col = max_file_length + 2;
  for(y = 0 ; y < cols ; y++){
    move(y,diff_col);
    clrtoeol();
  }
  refresh();
  mvprintw(0, diff_col, filename(menu_index));
  
  diff_window = newwin(rows - 4, cols - diff_col, 2, diff_col);
  diff(filename(menu_index), diff_window);
  delwin(diff_window);
}

//TODO: Write help explanations, commands and what they do

void show_help(){
  clear();
  attron(COLOR_PAIR(0));
  mvprintw(0, 0, "Help");
  mvprintw(2, 0, "Command       Action");
  mvprintw(3, 0, "UP & DOWN     choose between files");
  mvprintw(4, 0, "Space         select/deselect a file");
  mvprintw(5, 0, "A             select/deselect all files");
  mvprintw(6, 0, "C / Enter     commit selected files");
  mvprintw(7, 0, "Q / Esc       Quit");
  mvprintw(8, 0, "H / ?         Help");
  mvprintw(rows-1, 0, "Press any key to continue...");
  
  getch();
  clear();
  attron(COLOR_PAIR(0));
  print_files_menu();
  show_git_diff();
}

void move_menu(int dir){  
  previous_index = menu_index;
  menu_index= (menu_index+menu_length()+dir) % menu_length();
  show_git_diff();
  mvchgat(5+previous_index, 2, max_file_length, A_NORMAL, 0, NULL);
  highlight();
}

void highlight(){
  mvchgat(5+menu_index, 2, max_file_length, A_REVERSE, 0, NULL);
}

int left_center_col(){
  return cols/4;
}

int right_center_col(){
  return cols*3/4;
}

int center_col(){
  return cols/2+1;
}

void print_files_menu(){
  int i=0;

  display_branch();

	for (i = 0; i < maxi; ++i) {
    mvprintw(5+i, 2, formatted_filename(i));
  }
  highlight();
}

void check_row(){
  repofile_list[menu_index]->check = !repofile_list[menu_index]->check;
  print_files_menu();
}

void check_all(){
	for (i = 0; i < maxi; ++i) {
    repofile_list[i]->check = !repofile_list[i]->check;
  }
  print_files_menu();
}

int menu_length(){
  return maxi;
}


void open_commit_window(){
  mvprintw(rows - 3, 1, "Commit message: ");
  move(rows - 2, 1);
  echo();
  scanw("%s", msg);
  noecho();
  
  int i=0;

	for (i = 0; i < maxi; ++i) {
    if(repofile_list[i]->check){
      printf("file: [%s] result[%d]", repofile_list[i]->filename, git_index_add_bypath(my_repo_index, repofile_list[i]->filename));
    }
  }
  git_index_write(my_repo_index);
  git_index_free(my_repo_index);
  git_repository_free(repo);
  endwin();
  exit(EXIT_SUCCESS);
}
