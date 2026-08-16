#ifndef PARSER_H
#define PARSER_H

#define SHELL_TOK_BUFSIZE 64
#define SHELL_TOK_DELIM " \t\r\n\a"

char **shell_split_line(char *line);

#endif
