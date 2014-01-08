#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <git2.h>
#include "git2/diff.h"
#include "git2/repository.h"

static const char *colors[] = {	"\033[m","\033[1m", "\033[31m",	"\033[32m","\033[36m"}; /* reset bold red green cyan */
struct opts {git_diff_options diffopts;	git_diff_find_options findopts; int color; git_diff_format_t format; const char *dir; };

static int color_printer(const git_diff_delta*, const git_diff_hunk*, const git_diff_line*, void*);
int diff_output(const git_diff_delta *d, const git_diff_hunk *h, const git_diff_line *l, void *p);

int main(int argc, char *argv[]){
        char* filename=argv[1];
	git_repository *repo = NULL;
	git_diff *diff;
	struct opts o = { GIT_DIFF_OPTIONS_INIT, GIT_DIFF_FIND_OPTIONS_INIT, 0, GIT_DIFF_FORMAT_PATCH, "." };
	git_threads_init();

	git_repository_open_ext(&repo, o.dir, 0, NULL);

        o.diffopts.pathspec.strings = &filename;  o.diffopts.pathspec.count   = 1;

	git_diff_index_to_workdir(&diff, repo, NULL, &o.diffopts);

	fputs(colors[0], stdout);
	git_diff_print(diff, o.format, color_printer, &o.color);
	fputs(colors[0], stdout);

	git_diff_free(diff);
	git_repository_free(repo);

	git_threads_shutdown();

	return 0;
}

static int color_printer(const git_diff_delta *delta, const git_diff_hunk *hunk, const git_diff_line *line, void *data){
	int color = 0;
        char str[255]="\0";

	(void)delta; (void)hunk;

        switch (line->origin) {
          case GIT_DIFF_LINE_ADDITION:  color = 3; break;
          case GIT_DIFF_LINE_DELETION:  color = 2; break;
          case GIT_DIFF_LINE_ADD_EOFNL: color = 3; break;
          case GIT_DIFF_LINE_DEL_EOFNL: color = 2; break;
          case GIT_DIFF_LINE_FILE_HDR:  color = 1; break;
          case GIT_DIFF_LINE_HUNK_HDR:  color = 4; break;
          default: break;
        }

        printf("%s", colors[color]);
        if (line->origin == GIT_DIFF_LINE_CONTEXT || line->origin == GIT_DIFF_LINE_ADDITION || line->origin == GIT_DIFF_LINE_DELETION)
          printf("%c",line->origin);
        strncpy(str, line->content, line->content_len);
        printf("%s", str);

	return 0;
}

