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
repofile **repofile_list;


char *message;
char *repodir, *pathspec[MAX_PATHSPEC];

int i, npaths, zterm, max_file_length;
int diff_start_row, diff_start_col, diff_col_width, diff_col_height;
unsigned int parents, p;

size_t maxi, status_index;

git_repository *repo;
git_index *my_repo_index;
git_status_list *status;
git_config *cfg;
git_signature *author, *cmtter;
git_signature *signature;
git_object *git_obj;
git_oid oid;
git_commit *parent;
git_oid new_commit_id;

WINDOW *diff_window; 

int printer(const git_diff_delta*, const git_diff_hunk*, const git_diff_line*, void*);

void initial_check();
void check(int,const char*, const char *);
void fail(const char *);
void diff(char*, WINDOW*);
void get_files_list();
void get_files_by_status(int, char);

char* get_branch_name();
char* formatted_filename(int);
char* filename(int);

#endif
