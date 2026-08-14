#include "shell.h"

void shells_loop() {
  char *line = NULL; // указатель под строку ввода
  
  //fprintf(stderr, "[DEBUG] shell_loop started\n");

  while (1) { // while true 
    printf("?> "); //TODO: сделать логин пользователя вместо ?

    fflush(stdout); // в линукс поток вывода stdout по умолчанию буферизуется строками (Line-buffered)
    // это значит что текст не отправляется на экран терминала, пока в потоке не появится символ \n 
    // т.к. в строке выше переноса строки нет. Без этого промпт мог бы просто не появится до нажатия энтер

    line = shells_readline(); // вызов функции чтения
    if (line == NULL) { // проверка на сигнал завершения ctrl d 
      //fprintf(stderr, "[DEBUG] Got EOF\n");
      printf("\n[Exiting equilibrium...]\n");
      break;
    }

    printf("Input: %s", line);
    free(line); // освобождение памяти после функции getline внутри shells_readline
    // сли забыть эту строку, при каждом нажатии Enter будет утекать по несколько десятков байт оперативной памяти
    
    line = NULL; // обнуляем указатель после очистки
  }
}

int main() {
  shells_loop();
  return EXIT_SUCCESS; // эквивалент return 0; из stdlib.h 
}
