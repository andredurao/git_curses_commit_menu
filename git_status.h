#ifndef GIT_STATUS_H
#define GIT_STATUS_H

#include "git2/diff.h"

#define MAX_PATHSPEC 8

typedef struct repofile {
  char filename[255];
  bool check;
  char status;
} repofile;


int i, npaths, zterm, diff_start_row, diff_start_col, diff_col_width, max_file_length;
struct git_repository *repo;
struct git_status_list *status;
char *repodir, *pathspec[MAX_PATHSPEC];
repofile **repofile_list;
size_t maxi, status_index;


int printer(
	const git_diff_delta *delta,
	const git_diff_range *range,
	char usage,
	const char *line,
	size_t line_len,
	void *data);


void initial_check();

void check(int,const char*, const char *);

void fail(const char *);

char* get_branch_name();

void get_files_list();

void get_files_by_status(int, char);

char* formatted_filename(int);

char* filename(int);

void diff(char*,int, int);

#endif
