#include "shell.h"

char *shells_readline(void) {
  char *line = NULL; 
  size_t bufsize =0; // если передать getline() указатель на NULL и размер 0
  // то функция сама выделит нужный объем памяти
  ssize_t char_read; // тип со знаком signed, потому что при ошибке или конце файла
  // функция возвращает -1

  char_read = getline(&line, &bufsize, stdin); // блокирует выполнение и ждёт ввода
  // от пользователя

  if (char_read == -1) { // срабатывает если нажали ctrl + d 
    if (line != NULL) {
      free(line);
    }             //важное правило работы с памятью: если произошла ошибка, 
    return NULL; //getline всё равно могла успеть выделить буфер, поэтому мы его явно очищаем перед возвратом NULL
  }

  return line;
}
