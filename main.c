#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "getint/insert.h"
#include "readline/readline.h"
#include "errors.h"
#include "dialog.h"
#include "tree.h"

int main(void) {
  Tree *tree = createTree();
  short end_flag = 0, errcode = 0;
  do {
    time_t time = 0;
    short (*dialogFunc)(Tree*, time_t*) = getDialogFunc();
    if (dialogFunc == NULL) {
      break;
    }
    errcode = dialogFunc(tree, &time);
    switch (errcode) {
      case EOF:
        end_flag = 1;
        break;
      case INVALID_KEY:
        printf("Неверный ключ\n");
        break;
      case INVALID_FILE:
        printf("Неверное имя файла/файла не существует\n");
        break;
      case DUPLICATE_KEY:
        printf("Дублирующийся ключ\n");
        break;
      case KEY_NOT_FOUND:
        printf("Ключ не найден\n");
        break;
      case TREE_EMPTY:
        printf("Дерево пустое\n");
        break;
      case INVALID_PTR:
        printf("Неверный указатель\n");
        end_flag = 1;
        break;
      case EXIT:
        end_flag = 1;
        break;
    }
    printf("Время выполнения (в тиках): %d\n", time);
  } while (end_flag != 1);
  freeTree(tree->root);
  free(tree);
  return NO_ERR;
}

