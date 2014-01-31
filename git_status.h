#ifndef GIT_STATUS_H
#define GIT_STATUS_H

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <git2.h>
#include "git2/diff.h"
#include "git2/repository.h"

#define MAX_PATHSPEC 8

typedef struct repofile {
  char filename[255];
  bool check;
  char status;
} repofile;


int i, npaths, zterm, diff_start_row, diff_start_col, diff_col_width, diff_col_height, max_file_length;
git_repository *repo;
git_index *my_repo_index;
git_status_list *status;
git_config *cfg;
git_signature *author, *cmtter;
char *message;
git_signature *signature;

git_oid tree_id, parent_id, commit_id;
git_tree *tree;
git_commit *parent;

unsigned int parents, p;

char *repodir, *pathspec[MAX_PATHSPEC];
repofile **repofile_list;
size_t maxi, status_index;
WINDOW *diff_window; 

int printer(const git_diff_delta*, const git_diff_hunk*, const git_diff_line*, void*);

void initial_check();

void check(int,const char*, const char *);

void fail(const char *);

char* get_branch_name();

void get_files_list();

void get_files_by_status(int, char);

char* formatted_filename(int);

char* filename(int);

void diff(char*, WINDOW*);

#endif
