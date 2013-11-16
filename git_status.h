
#ifndef GIT_STATUS_H
#define GIT_STATUS_H

#define MAX_PATHSPEC 8

typedef struct repofile {
  char filename[255];
  bool check;
  char status;
} repofile;


int i, npaths, zterm;
struct git_repository *repo;
struct git_status_list *status;
char *repodir, *pathspec[MAX_PATHSPEC];
repofile **repofile_list;
size_t maxi;


void initial_check();

void check(int,const char*, const char *);

void fail(const char *);

char* get_branch_name();

void get_files_list();

char* formatted_filename(int);

#endif
