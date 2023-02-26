#ifndef _hshell_H
#define _hshell_H

void hshell_loop(void);
char *hshell_read_line(void);
char **hshell_split_line(char *line);
int hshell_launch(char **args);
int hshell_exec(char **args);

// Builtin programs
int hshell_cd(char **args);
int hshell_help(char **args);
int hshell_exit(char **args);
int hshell_num_builtins();

#endif