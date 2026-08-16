#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"

char **shell_split_line(char *line) { // выделяем в куче память не под символы, а под массив указателей на строки
  size_t bufsize = SHELL_TOK_BUFSIZE; // изачальная вместимость массива (64 аргумента)
  size_t position = 0; // индек, в какую ячейку мы запишем следующий найденный аргумент
  char **tokens = malloc(bufsize * sizeof(char *)); // tokens - указатель на первый эелемент в списке указателей
  // мы выделяем память (malloc) умножая вместимость массива на размер одного указателя
  char *token;
  char *saveptr; // Служебный указатель для strtok_r. Функция сохраняет в него адрес места,
  // где она остановилась внутри строки, чтобы при следующем вызове знать, откуда продолжить поиск

  if (!tokens) { // если у системы закончилась оперативка, чтобы не было 
    perror("Equilibrium: allocation error");
    exit(EXIT_FAILURE);  
  }

  token = strtok_r(line, SHELL_TOK_DELIM, &saveptr); // line - сама строка, строка с разделителями (" \t\r\n\a"), и адрес служебного указателя
  // Функция находит первое слово, заменяет разделитель после него на \0 и кладёт адрес первого символа в переменную token.
  // Если строка была пустой (пользователь просто нажал Enter), token сразу станет NULL
  while (token != NULL) { // Пока находим слова, сохраняем адрес найденного слова token в tokens[position], после чего увеличиваем position на 1
    tokens[position++] = token;
    
    if (position >= bufsize) { // проверяем не заполнился ли массив 
      bufsize += SHELL_TOK_BUFSIZE; // увеличиваем размер массива (станет 128)
      char **tokens_backup = tokens; // защита от утечки памяти
      tokens = realloc(tokens_backup, bufsize * sizeof(char *));
      if (!tokens) {
        free(tokens_backup);
        perror("Equilibrium: reallocation error");
        exit(EXIT_FAILURE);
        /* если realloc не сможет найти непрерывнй кусок памяти, он вернет null.
        имея копию токена мы можем безопасно вызвать free(tokens_backup) перед паническим выходом */
      }
    }

    token = strtok_r(NULL, SHELL_TOK_DELIM, &saveptr); // когда мы в strtok_r передаем NULL функция понимает, что нужно продолжить парсинг
    // той же самой строки используя адрес
  }
  
  tokens[position] = NULL; // в последнюю ячейку обязательно передаем NULL, потому что функция идет по массиву пока не наткнется на NULL
  return tokens;
}
