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

  diff_window = newwin(rows - 4, cols - diff_col - 1, 2, diff_col + 1);
  diff(filename(menu_index), diff_window);
  delwin(diff_window);
}

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

  if(maxi <= 0){
    endwin();
    printf("On branch %s\nnothing to commit, working directory clean\n", get_branch_name());
    git_repository_free(repo);
    exit(EXIT_SUCCESS);
  }

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

  //Adding selected files on the index
  for (i = 0; i < maxi; ++i) {
    if(repofile_list[i]->check){
      git_index_add_bypath(my_repo_index, repofile_list[i]->filename);
      // printf("file: [%s]", repofile_list[i]->filename);
    }
  }

  // //writing index
  // git_index_write(my_repo_index);
  // git_index_free(my_repo_index);

  //Create commit signature
  const char *email, *name;
  git_config_get_string(&email, cfg, "user.email");
  git_config_get_string(&name, cfg, "user.name");
  git_signature_now(&signature, name, email);

  //tree_lookup
  // git_revparse_single(&git_obj, repo, "HEAD^{tree}");
  // git_tree *tree = (git_tree *)git_obj;


  //get last commit => parent
  git_revparse_single(&git_obj, repo, "HEAD");
  git_oid *parent_oid = (git_oid *)git_obj;

  int error = git_commit_lookup(&parent, repo, parent_oid);
  if (error != 0){
    git_repository_free(repo);
    endwin();
    printf("problem loading parent \n");
    exit(1);
  }

  //writing index
  git_oid tree_oid;
  git_index_write_tree(&tree_oid, my_repo_index);
  // git_index_free(my_repo_index);
  git_tree *tree;
  git_tree_lookup(&tree, repo, &tree_oid);

  //Create the commit
  // git_commit_create_v(
  //   &new_commit_id,
  //   repo,
  //   "HEAD",
  //   signature, //author
  //   signature, //commiter
  //   NULL, /* use default message encoding */
  //   msg,
  //   tree,
  //   1, // qty of parents
  //   parent //parent commit
  // );

  // //writing index
  // git_index_write(my_repo_index);
  // git_index_free(my_repo_index);

  git_repository_free(repo);
  endwin();
  exit(EXIT_SUCCESS);
}


// End curses window, free git repo + exit
void clean_exit(){
  endwin();
  git_repository_free(repo);
  exit(EXIT_SUCCESS);
}

//function tries to add the selected files on index running
//system functione instead of writing the index and creating a commit
//with libgit2
void open_system_index_add_window(){
  mvprintw(rows - 3, 1, "Commit message: ");
  move(rows - 2, 1);
  echo();
  getstr(msg);
  noecho();
  endwin(); //Ending curses window
  int i=0;
  int check_total = 0;
  char cmd[255];

  system("git reset . > /dev/null");
  for (i = 0; i < maxi; ++i) {
    if(repofile_list[i]->check){
      check_total++;
      sprintf(cmd, "git add --all %s > /dev/null", repofile_list[i]->filename);
      //printf("%s\n", cmd);
      system(cmd);
    } else {
      sprintf(cmd, "git reset HEAD %s > /dev/null", repofile_list[i]->filename);
      //printf("%s\n", cmd);
      system(cmd);
    }
  }

  if(check_total > 0){
    sprintf(cmd, "git commit -m \"%s\"", msg);
    //printf("\n [ %s ] \n", cmd);
    system(cmd);
  }

  git_repository_free(repo);
  exit(EXIT_SUCCESS);
}
