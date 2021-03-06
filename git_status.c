#include "git_status.h"
// TODO: FOLLOW THESE SAMPLES: http://libgit2.github.com/docs/guides/101-samples/

int printer(const git_diff_delta *delta, const git_diff_hunk *hunk, const git_diff_line *line, void *data){ 
  //TODO: get the same colors used in .git/config if there are any
  char linebuffer[1024] = "\0";
  (void)delta; (void)hunk;

  wattrset(diff_window, 0);
  switch (line->origin) { //1 : red 2: green 3: cyan
    case GIT_DIFF_LINE_CONTEXT:   wattrset(diff_window, 0); break;
    case GIT_DIFF_LINE_ADDITION:  wattron(diff_window, COLOR_PAIR(2)); break;
    case GIT_DIFF_LINE_DELETION:  wattron(diff_window, COLOR_PAIR(1)); break;
    case GIT_DIFF_LINE_ADD_EOFNL: wattron(diff_window, COLOR_PAIR(2)); break;
    case GIT_DIFF_LINE_DEL_EOFNL: wattron(diff_window, COLOR_PAIR(1)); break;
    case GIT_DIFF_LINE_FILE_HDR:  wattron(diff_window, A_BOLD);        break;
    case GIT_DIFF_LINE_HUNK_HDR:  wattron(diff_window, COLOR_PAIR(3)); break;
    default: break;
  }
  
  strncpy(linebuffer, line->content, line->content_len);
  if (line->origin == GIT_DIFF_LINE_CONTEXT ||  line->origin == GIT_DIFF_LINE_ADDITION || line->origin == GIT_DIFF_LINE_DELETION){
    mvwprintw(diff_window, diff_start_row, diff_start_col, "%c%s", line->origin, linebuffer);
  } else {
    mvwprintw(diff_window, diff_start_row, diff_start_col, "%s", linebuffer);
  }
  
  diff_start_row += 1;
  wattroff(diff_window, A_BOLD); 
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
	
  git_repository_open_ext(&repo, repodir, 0, NULL);
  if (git_repository_is_bare(repo))
    fail("Cannot report status on bare repository");
  check(git_status_list_new(&status, repo, &opt), "Could not get status", NULL);

  if(git_repository_index(&my_repo_index, repo) < 0 )
    exit(1);//TODO: print error message

  git_config_open_ondisk(&cfg, ".git/config");
}

//TODO: Erase the check method and use the samples from the documentation
void check(int error, const char *message, const char *extra){
  const git_error *lg2err;
  const char *lg2msg = "", *lg2spacer = "";

  if (!error)
    return;

  if ((lg2err = giterr_last()) != NULL && lg2err->message != NULL) {
    lg2msg = lg2err->message;
    lg2spacer = " - ";
  }

  if (extra)
    fprintf(stderr, "%s '%s' [%d]%s%s\n", message, extra, error, lg2spacer, lg2msg);
  else
    fprintf(stderr, "%s [%d]%s%s\n", message, error, lg2spacer, lg2msg);
  exit(1);
}

void fail(const char *message){
  check(-1, message, NULL);
}


char* get_branch_name(){
  int error = 0;
  const char *branch_name = NULL;
  git_reference *head = NULL;

  error = git_repository_head(&head, repo);

  if (error == GIT_ENOTFOUND)
    branch_name = NULL;
  else if (!error) {
    branch_name = git_reference_name(head);
    if (!strncmp(branch_name, "refs/heads/", strlen("refs/heads/")))
      branch_name += strlen("refs/heads/");
  } else
    check(error, "failed to get current branch", NULL);

  git_reference_free(head);
  return (strstr(branch_name, branch_name));
}


void get_files_list(){
  size_t i;
  const git_status_entry *s;
  const char *oldname, *newname, *fname; 

  max_file_length = -1;
  //maxi => git status entries count
  maxi = git_status_list_entrycount(status);

  repofile_list = (repofile**) malloc(maxi * sizeof(repofile));
  status_index = 0;
  for (i = 0; i < maxi; i++) {
    s = git_status_byindex(status, i);

    oldname = s->head_to_index ? s->head_to_index->old_file.path :
      s->index_to_workdir ? s->index_to_workdir->old_file.path : NULL;

    newname = s->index_to_workdir ? s->index_to_workdir->new_file.path :
      s->head_to_index ? s->head_to_index->new_file.path : NULL;

    repofile_list[i] = (repofile*) malloc(sizeof(repofile));

    if(newname)
     fname = newname;
    
    if(oldname)
     fname = oldname;
    
    strcpy(repofile_list[i]->filename, fname);
    //printf("--%s [%d] \n", fname, s->status);
    if ((int)strlen(fname) > max_file_length)
      max_file_length = strlen(fname);
    repofile_list[i]->status = s->status;
    repofile_list[i]->check = (s->status < 128);
  }
  max_file_length+=4;
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

void diff(char* filename, WINDOW* diff_window){
  git_diff *diff = NULL;
  int color = 0;
  git_threads_init();
  git_diff_options opts = GIT_DIFF_OPTIONS_INIT;
  opts.pathspec.strings = &filename;
  opts.pathspec.count   = 1;
  git_diff_index_to_workdir(&diff, repo, NULL, &opts);
  diff_start_row = 1;
  diff_start_col = 0;
  wrefresh(diff_window);
  getmaxyx(diff_window,diff_col_height,diff_col_width);
  git_diff_print(diff, GIT_DIFF_FORMAT_PATCH, printer, &color);
  wrefresh(diff_window);
  attron(COLOR_PAIR(0));
}
