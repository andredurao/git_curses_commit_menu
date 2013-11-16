#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <git2.h>
#include "git_status.h"

void initial_check(){
  
  repo = NULL;
  npaths = 0;
  zterm = 0;
  git_status_options opt = GIT_STATUS_OPTIONS_INIT;
	opt.show  = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
	opt.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED | GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX | GIT_STATUS_OPT_SORT_CASE_SENSITIVELY;
  repodir = ".";
	
  check(git_repository_open_ext(&repo, repodir, 0, NULL),
		  "Could not open repository", repodir);

	if (git_repository_is_bare(repo))
		fail("Cannot report status on bare repository");

	check(git_status_list_new(&status, repo, &opt),
		  "Could not get status", NULL);
}

void check(int error, const char *message, const char *extra)
{
	const git_error *lg2err;
	const char *lg2msg = "", *lg2spacer = "";

	if (!error)
		return;

	if ((lg2err = giterr_last()) != NULL && lg2err->message != NULL) {
		lg2msg = lg2err->message;
		lg2spacer = " - ";
	}

	if (extra)
		fprintf(stderr, "%s '%s' [%d]%s%s\n",
			message, extra, error, lg2spacer, lg2msg);
	else
		fprintf(stderr, "%s [%d]%s%s\n",
			message, error, lg2spacer, lg2msg);

	exit(1);
}

void fail(const char *message)
{
	check(-1, message, NULL);
}


char* get_branch_name()
{
	int error = 0;
	const char *branch_name = NULL;
	git_reference *head = NULL;

	error = git_repository_head(&head, repo);

	if (error == GIT_EORPHANEDHEAD || error == GIT_ENOTFOUND)
		branch_name = NULL;
	else if (!error) {
		branch_name = git_reference_name(head);
		if (!strncmp(branch_name, "refs/heads/", strlen("refs/heads/")))
			branch_name += strlen("refs/heads/");
	} else
		check(error, "failed to get current branch", NULL);

//  printf("# On branch %s\n", branch ? branch : "Not currently on any branch.");

	git_reference_free(head);
  //force const char* to char*
  return (strstr(branch_name, branch_name));
}
