
#include "common.h"


static const char *colors[] = {
	"\033[m", /* reset */
	"\033[1m", /* bold */
	"\033[31m", /* red */
	"\033[32m", /* green */
	"\033[36m" /* cyan */
};

/** The 'opts' struct captures all the various parsed command line options. */
struct opts {
	git_diff_options diffopts;
	git_diff_find_options findopts;
	int color;
	int cached;
	git_diff_format_t format;
	const char *treeish1;
	const char *treeish2;
	const char *dir;
};

/** These functions are implemented at the end */
static int color_printer(
	const git_diff_delta*, const git_diff_hunk*, const git_diff_line*, void*);

int main(int argc, char *argv[])
{

        char* filename="menu.c";
	git_repository *repo = NULL;
	git_diff *diff;
	struct opts o = {
		GIT_DIFF_OPTIONS_INIT, GIT_DIFF_FIND_OPTIONS_INIT,
		0, 0, GIT_DIFF_FORMAT_PATCH, NULL, NULL, "."
	};

	git_threads_init();

	git_repository_open_ext(&repo, o.dir, 0, NULL);

        o.diffopts.pathspec.strings = &filename;
        o.diffopts.pathspec.count   = 1;

	git_diff_index_to_workdir(&diff, repo, NULL, &o.diffopts);

	fputs(colors[0], stdout);
	git_diff_print(diff, o.format, color_printer, &o.color);
	fputs(colors[0], stdout);

	/** Cleanup before exiting. */
	git_diff_free(diff);
	git_repository_free(repo);

	git_threads_shutdown();

	return 0;
}

/** This implements very rudimentary colorized output. */
static int color_printer(
	const git_diff_delta *delta,
	const git_diff_hunk *hunk,
	const git_diff_line *line,
	void *data)
{
	int color = 0;

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

        if (color == 1)
          fputs(colors[0], stdout);
        fputs(colors[color], stdout);
	return diff_output(delta, hunk, line, stdout);
}

