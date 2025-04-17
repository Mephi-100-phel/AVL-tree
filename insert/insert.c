#include <stdio.h>
#include <stdlib.h>
#include "insert.h"

int getInt(int* x, char* prompt, int (*check)(int)) {
  int helper = 0;
  if (prompt != NULL) {
    printf("%s", prompt);
  }
  do {
    helper = scanf("%d", x);
    if (helper == EOF) {
      return EOF;
    }
    if (helper != 1 || check(*x) == 0) {
      printf("Некорректный ввод\n");
      printf("Повторите ввод: ");
      scanf("%*[^\n]");
    }
  } while (helper != 1 || check(*x) == 0);
  scanf("%*[^\n]");
  scanf("%*c");
  return 0;
}

int any(int x) {
  return 1;
}

int pos(int x) {
  return (x > 0) ? 1 : 0;
}

int pos_or_zero(int x) {
  return (x >= 0) ? 1 : 0;
}

int neg_or_zero(int x) {
  return (x <= 0) ? 1 : 0;
}

int neg(int x) {
  return (x < 0) ? 1 : 0;
}
