#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "file_read.h"
#include "graph.h"
#include "readline/readline.h"
#include "getint/insert.h"
#include "dialog.h"
#include "errors.h"
#include "tree.h"
#include "dop/dop.h"

#define IsErr(errcode, err) {if (errcode == err) return err;}
#define TreeIsValid(tree) {if (tree == NULL) return INVALID_PTR;}
#define TreeNotEmpty(tree) {if (tree->root == NULL) return TREE_EMPTY;}
#define KeyIsValid(key) {if (key == NULL) return INVALID_KEY;}

short D_insert(Tree *tree, time_t *time) {
  TreeIsValid(tree);
  size_t info = 0;
  char *key = readline("Ключ: ");
  KeyIsValid(key);
  short errcode = getInt(&info, "Инфо: ", pos_or_zero);
  if (errcode == EOF) {
    free(key);
    return EOF;
  }
  if (tree->root == NULL) {
    time_t start = clock();
    tree->root = createNode(key, info, NULL);
    *time = clock() - start;
    return NO_ERR;
  }
  time_t start = clock();
  errcode = insert(tree->root, key, &info);
  *time = clock() - start;
  if (tree->root->parent != NULL) {
    tree->root = tree->root->parent;
  }
  if (errcode == -1) {
    free(key);
    return DUPLICATE_KEY;
  }
  return NO_ERR;
}

short D_delete(Tree *tree, time_t *time) {
  TreeIsValid(tree);
  TreeNotEmpty(tree);
  char *key = readline("Ключ: ");
  KeyIsValid(key);
  time_t start = clock();
  short errcode = delete(tree, key);
  *time = clock() - start;
  free(key);
  if (tree->root != NULL && tree->root->parent != NULL) {
    tree->root = tree->root->parent;
  }
  IsErr(errcode, KEY_NOT_FOUND);
  return NO_ERR;
}

short D_specialSearch(Tree *tree, time_t *time) {
  TreeIsValid(tree);
  TreeNotEmpty(tree);
  char *key = readline("Ключ: ");
  time_t start = clock();
  Node *node = specialSearch(tree, key);
  *time = clock() - start;
  free(key);
  printNode(node);
  return NO_ERR;
}

short D_search(Tree *tree, time_t *time) {
  TreeIsValid(tree);
  char *key = readline("Ключ: ");
  KeyIsValid(key);
  time_t start = clock();
  Node *node = search(tree, key);
  *time = clock() - start;
  free(key);
  if (node == NULL) {
    return KEY_NOT_FOUND;
  }
  printNode(node);
  return NO_ERR;
}

short D_traverse(Tree *tree, time_t *time) {
  TreeIsValid(tree);
  TreeNotEmpty(tree);
  char *substr = readline("Введите подстроку: ");
  if (substr == NULL) {
    return EOF;
  }
  size_t len = strlen(substr);
  time_t start = clock();
  traverse(tree->root, substr, &len);
  *time = clock() - start;
  free(substr);
  return NO_ERR;
}

short D_import(Tree* tree, time_t *time) {
  TreeIsValid(tree);
  char *file_name = readline("Имя файла: ");
  if (file_name == NULL) {
    return EOF;
  }
  time_t start = clock();
  short errcode = import(tree, file_name);
  *time = clock() - start;
  free(file_name);
  IsErr(errcode, INVALID_FILE);
  return NO_ERR;
}

short D_print(Tree *tree, time_t *time) {
  TreeIsValid(tree);
  TreeNotEmpty(tree);
  time_t start = clock();
  char prompt[120] = {'\0'};
  prettyPrint(tree->root, tree->root, prompt);
  *time = clock() - start;
  return NO_ERR;
}

short D_programmPrint(Tree *tree, time_t *time) {
  TreeIsValid(tree);
  TreeNotEmpty(tree);
  time_t start = clock();
  graph(tree);
  *time = clock() - start;
  return NO_ERR;
}

short D_exit(Tree *tree, time_t *time) {
  return EXIT;
}

short D_findWord(Tree *tree, time_t *time) {
  char *file_name = readline("Название файла: ");
  if (file_name == NULL) {
    return EOF;
  }
  clock_t start = clock();
  Tree *file_tree = parseFile(file_name);
  *time = (clock() - start);
  free(file_name);
  if (file_tree == NULL) {
    return INVALID_FILE;
  }
  char prompt[120] = {0};
  prettyPrint(file_tree->root, file_tree->root, prompt);
  char *word = readline("Слово для поиска: ");
  if (word == NULL) {
    freeTree(file_tree->root);
    free(file_tree);
    return EOF;
  }
  Node *node = search(file_tree, word);
  free(word);
  if (node == NULL) {
    freeTree(file_tree->root);
    free(file_tree);
    return KEY_NOT_FOUND;
  }
  printNode(node);
  freeTree(file_tree->root);
  free(file_tree);
  return NO_ERR;
}

short (*getDialogFunc(void))(Tree*, time_t*) {
  short (*dialog_func_arr[])(Tree*, time_t*) = {D_insert, D_delete, D_specialSearch, D_search, D_print, D_traverse, D_import, D_programmPrint, D_findWord, D_exit};
  short option = 0, errcode = 0;
  const char *options[] = {"0: Вставка", "1: Удаление", "2: Особый поиск", "3: Поиск", "4: Вывод", "5: Обход", "6: Импорт", "7: Вывод с помощью Graphviz", "8: Поиск слова в файле", "9: Выход"};
  for (int i = 0; i < 10; i++){
    printf("%s\n", options[i]);
  }
  errcode = getInt(&option, NULL, __comp);
  if (errcode == EOF) {
    return NULL;
  }
  return dialog_func_arr[option];
}

int __comp(int x) {
  return (x < 0 || x > 9) ? 0 : 1;
}
