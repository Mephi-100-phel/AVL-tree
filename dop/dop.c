#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../errors.h"
#include "../tree.h"
#include "dop.h"

#define DELIM "–—,.;:\"!?()… "

Tree* parseFile(char *file_name) {
  FILE *file = fopen(file_name, "r");
  if (file == NULL) {
    return NULL;
  }
  Tree *tree = createTree();
  char *str = f_readline(file); 
  size_t line_num = 1;
  short errcode = 0;
  while (str != NULL) {
    char *word = strtok(str, DELIM);
    while (word != NULL) {
      word = strdup(word);
      if (tree->root == NULL) {
        tree->root = createNode(word, line_num, NULL);
      } else {
        errcode = insert(tree->root, word, &line_num);
        if (errcode == -1) {
          free(word);
        }
        if (tree->root->parent != NULL) {
          tree->root = tree->root->parent;
        }
      }
      word = strtok(NULL, DELIM);
    }
    free(str);
    str = f_readline(file);
    line_num++;
  }
  fclose(file);
  return tree;
}

static char* f_readline(FILE* file) {
  const int block_size = 80;
  int block_count = 1, helper = 0;
  char* str = (char*)calloc(block_size + 1, sizeof(char)), buffer[81] = {0};
  helper = fscanf(file, "%80[^\n]", buffer);
  while (helper != 0 && helper != EOF) {
    strcat(str, buffer);
    block_count++;
    str = realloc(str, (block_count * block_size + 1) * sizeof(char));
    helper = fscanf(file, "%80[^\n]", buffer);
  }
  if (helper == EOF) {
    free(str);
    return NULL;
  }
  str = realloc(str, (strlen(str) + 1) * sizeof(char));
  fscanf(file, "%*c");
  return str;
}
