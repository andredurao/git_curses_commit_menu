#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <git2.h>
#include "git_status.h"

char *colors[] = {
	"\033[m", /* reset */
	"\033[1m", /* bold */
	"\033[31m", /* red */
	"\033[32m", /* green */
	"\033[36m" /* cyan */
};

int printer(
	const git_diff_delta *delta,
	const git_diff_range *range,
	char usage,
	const char *line,
	size_t line_len,
	void *data)
{
	int color = 0;
  char formatted_line[255];

	(void)delta; (void)range; (void)line_len;

		switch (usage) {
		case GIT_DIFF_LINE_ADDITION: attron(COLOR_PAIR(2)); break; //3 green
		case GIT_DIFF_LINE_DELETION: attron(COLOR_PAIR(1)); break; //2 red
		case GIT_DIFF_LINE_ADD_EOFNL: attron(COLOR_PAIR(2)); break; //3 green
		case GIT_DIFF_LINE_DEL_EOFNL: attron(COLOR_PAIR(1)); break; //2 red
		case GIT_DIFF_LINE_FILE_HDR: attron(COLOR_PAIR(1)); break; //1 bold
		case GIT_DIFF_LINE_HUNK_HDR: attron(COLOR_PAIR(3)); break; //4 cyan
		default: color = 0;
		}

  //TODO: Clear \t characters which are trashing the output or replace them with \s
  strlcpy(formatted_line, line, diff_col_width);
  mvprintw(diff_start_row, diff_start_col, formatted_line);
  diff_start_row += 1;
	return 0;
}


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


void get_files_list(){
  
	size_t i;
  maxi = git_status_list_entrycount(status);
  const git_status_entry *s;

  max_file_length = -1;

  repofile_list = (repofile**) malloc(maxi * sizeof(repofile));
  status_index = 0;
	for (i = 0; i < maxi; i++) {
		s = git_status_byindex(status, i);
    repofile_list[i] = (repofile*) malloc(sizeof(repofile));
    strcpy(repofile_list[i]->filename,s->index_to_workdir->old_file.path); 
    repofile_list[i]->status = s->status;
    repofile_list[i]->check = FALSE;
    printf("--%s [%d] \n", s->index_to_workdir->old_file.path, s->status);
    if ((int)strlen(s->index_to_workdir->old_file.path) > max_file_length)
      max_file_length = strlen(s->index_to_workdir->old_file.path);
	}
  max_file_length+=4;
  /* statuses constants
 GIT_STATUS_WT_NEW
 GIT_STATUS_INDEX_MODIFIED
 GIT_STATUS_INDEX_DELETED
 GIT_STATUS_INDEX_RENAMED
 GIT_STATUS_INDEX_TYPECHANGE*/
}

char* formatted_filename(int i){
  char* filename = (char*) malloc(sizeof(char)*255);
  if(repofile_list[i]->check){
    strcpy(filename, "[x] ");
  }
  else{
    strcpy(filename, "[ ] ");
  }
  strcat(filename, repofile_list[i]->filename);
  return filename; 
}

char* filename(int i){
  return repofile_list[i]->filename;
}

void diff(char* filename, int row, int col){
  git_diff_list *diff = NULL;
  int color = 0;
	git_diff_options opts = GIT_DIFF_OPTIONS_INIT;
  opts.pathspec.strings = &filename;
  opts.pathspec.count   = 1;
  git_diff_index_to_workdir(&diff, repo, NULL, &opts);
  diff_start_row = row;
  diff_start_col = col;
	git_diff_print_patch(diff, printer, &color);
  attron(COLOR_PAIR(0));
}
