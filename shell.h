#ifndef SHELL_H
#define SHELL_H

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

/*
  Builtin function implementations.
*/

int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);

int lsh_launch(char **args);

int lsh_num_builtins();

int lsh_execute(char **args);

char *lsh_read_line(void);

// Simplified split line, needs to be improved to allow for
// '/' and '"' values in the command line
char **lsh_split_line(char *line);

void lsh_loop(void);

#endif