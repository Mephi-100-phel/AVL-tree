#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_read.h"
#include "tree.h"
#include "errors.h"

short import(Tree *tree, const char *file_name) {
  FILE *file = fopen(file_name, "r");
  if (file == NULL) {
    return INVALID_FILE;
  }
  char *key = NULL;
  short errcode = 0;
  size_t info = 0;
  do {
    key = f_readline(file);
    if (key == NULL) {
      fclose(file);
      return NO_ERR;
    }
    errcode = fscanf(file, "%ld", &info);
    fscanf(file, "%*c");
    if (errcode == 0 || errcode == EOF) {
      free(key);
      fclose(file);
      return NO_ERR;
    }
    if (tree->root == NULL) {
      tree->root = createNode(key, info, NULL);
    } else {
      errcode = insert(tree->root, key, &info);
      if (errcode == -1) {
        free(key);
      }
      if (tree->root->parent != NULL) {
        tree->root = tree->root->parent;
      }
    }
  } while (errcode != EOF || key != NULL);
  free(key);
  fclose(file);
  return NO_ERR;
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

