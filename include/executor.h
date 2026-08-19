#ifndef EXECUTOR_H
#define EXECUTOR_H

int shell_cd(char **args);
int shell_exit(char **args);
int shell_help(char **args);
int shell_pwd(char **args);
int shell_echo(char **args);

int shell_launch(char **args); // отвечает за создание процесса fork + execvp
int shell_execute(char **args); // функция маршрутизатор, определяет встроенная ли это команда или внешний бинарник

#endif
