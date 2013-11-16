
#ifndef GIT_STATUS_H
#define GIT_STATUS_H

#define MAX_PATHSPEC 8

int i, npaths, zterm;
struct git_repository *repo;
struct git_status_list *status;
char *repodir, *pathspec[MAX_PATHSPEC];

void initial_check();

void check(int,const char*, const char *);

void fail(const char *);

#endif
