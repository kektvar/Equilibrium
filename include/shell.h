#define _POSIX_C_SOURCE 200809L

#ifndef SHELL_H
#define SHELL_H //Include guard (предотвращает дублирование объявлений)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//прототипы функций
char *shells_readline(void);
void shells_loop(void);

#endif
