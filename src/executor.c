#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "executor.h"

char *builtin_str[] = {
  "cd",
  "help",
  "exit",
  "pwd",
  "echo"
};

int (*builtin_func[]) (char **) = { // массив указателей на функции
  &shell_cd,
  &shell_help,
  &shell_exit,
  &shell_pwd,
  &shell_echo
};

int shell_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *); // общий размер всего массива в байтах / размер одного указателя = точное кол-во комманд 
}

int shell_cd(char **args) {
  if (args[1] == NULL) {
    // если путь не передан
    char *home = getenv("HOME"); // считывает путь к домашней папке из переменных окружения
    if (home == NULL || chdir(home) != 0) { // posix системный вызов ядра, меняющий текущую рабочую среду текущего родительского процесса
      perror("Equilibrium: cd");
    }
  } else {
    if (chdir(args[1]) != 0) {
      perror("Equilibrium: cd");
    }
  }
  return 1;
}

int shell_help(char **args) {
  (void)args;
  printf("--- Equilibrium Built-In Commands ---\n");
  printf("Built-In commands:\n");
  for (int i = 0; i < shell_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }
  return 1;
}

int shell_exit(char **args) {
  (void)args;
  return 0;
}

int shell_pwd(char **args) {
  (void)args;
  char cwd[PATH_MAX];

  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("%s\n", cwd);
  } else {
    perror("Equilibrium: pwd");
  }
  return 1;
}

int shell_echo(char **args) {
  for (int i = 1; args[i] != NULL; i++) { // начинаем с 1 т.к. args[0] это echo
    printf("%s", args[i]);
    if (args[i + 1] != NULL) {
      printf(" "); // пробел между аргументами
    }
  }
  printf("\n");
  return 1;
}

int shell_launch(char **args) {
  pid_t pid;
  pid_t wpid;
  int status;

  pid = fork(); // системный вызов разделяющий поток выполнения на 2 процесса (ниже)
  // pid == 0 - код сейчас выполняется внутри дочернего процесса
  // pid > 0 - код сейчас выполняется внутри родителя (Equilibrium), а значение pid = индетификатору созданного потомка
  if (pid == 0) { 
    if (execvp(args[0], args) == -1) { // ищет бинарник по системным путям $PATH и передает ему массив параметров args
      perror("Equilibrium"); // если не нашёл
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) { // ядро не смогло выделить процесс (кончилась память или pid)
    perror("Equilibrium: fork error");
  } else {
    // род. процесс, ждем завершения дочернего
    do {
      wpid = waitpid(pid, &status, WUNTRACED); // заставляет родительский процесс приостановить выполнение, пока дочерний процесс
      // с заданым pid не изменит свое состояние
    } while (!WIFEXITED(status) && !WIFSIGNALED(status)); // макросы проверки:
    // WIFEXITED - завершился ли процесс штатно (через exit() или возврат в main)
    // WIFSIGNALED - был ли процесс принудительно убит сигналом (ctrl + c, sigkill)
    (void)wpid; // штука, подавляющая предупреждение компилятора о неиспользованной переменной при включенном флаге -Wextra
  }

  return 1;
}

int shell_execute(char **args) {
  if (args[0] == NULL) {
    return 1; // пустая командная строка
  }

  for (int i = 0; i < shell_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return shell_launch(args);
}
