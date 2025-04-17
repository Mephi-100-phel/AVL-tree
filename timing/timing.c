#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../tree.h"

static char* __getRandStr(void);

int main(void) {
  srand(clock());
  printf("Стартовое количество элементов: ");
  int elem_num = 0;
  short errcode = scanf("%d", &elem_num);
  if (errcode == EOF) {
    return EOF;
  }
  FILE *file = fopen("time_res.txt", "w");
  Tree *tree = createTree();
  for (int i = 0; i < elem_num; i++) {
    char *key = __getRandStr();
    size_t info = rand() % 10000;
    if (i == 0) {
      tree->root = createNode(key, info, NULL);
    } else {
      errcode = insert(tree->root, key, &info);
    }
  }
  for (int a = 0; a < 5; a++) {
    char *key_arr[1000] = {0};
    clock_t start = 0, stop = 0, sum = 0;
    for (int i = 0; i < 1000; i++) {
      char *key = __getRandStr();
      key_arr[i] = key;
      size_t info = rand() % 10000;
      start = clock();
      errcode = insert(tree->root, key, &info);
      stop = clock();
      if (tree->root->parent != NULL) {
        tree->root = tree->root->parent;
      }
      sum += (stop - start);
    }
    fprintf(file, "Вставка: %ld\n", sum);
    sum = 0;
    for (int i = 0; i < 1000; i++) {
      char *key = key_arr[i];
      start = clock();
      delete(tree, key);
      stop = clock();
      if (tree->root->parent != NULL) {
        tree->root = tree->root->parent;
      }
      sum += (stop - start);
    }
    fprintf(file, "Удаление: %ld\n", sum);
    sum = 0;
    for (int i = 0; i < 1000; i++) {
      size_t len = 1;
      start = clock();
      traverse(tree->root, "1", &len);
      stop = clock();
      sum += (stop - start);
    }
    fprintf(file, "Обход: %ld\n", sum);
  }
  freeTree(tree->root);
  free(tree);
  return 0;
}

static char* __getRandStr(void) {
  srand(clock());
  int len = rand() % 15 + 1;
  char *str = (char*)calloc(len + 1, sizeof(char));
  for (int i = 0; i < len; i++) {
    char letter = rand() % 127;
    if (letter < 33) {
      letter = 33;
    }
    str[i] = letter;
  }
  return str;
}
