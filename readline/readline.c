#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readline.h"

char* readline(const char* prompt) {
  char *string = NULL, buffer[81] = {0};
  if (prompt != NULL) {
    printf("%s", prompt);
  }
  int block = 1, helper = 0, string_len = 0;
  helper = scanf("%80[^\n]", buffer);
  string = (char*)calloc((80 * block) + 1, sizeof(char));
  if (helper == EOF) {
    scanf("%*[^\n] %*c");
    free(string);
    return NULL;
  }
  while (helper == 1) {
    string = strcat(string, buffer);
    block++;
    string = realloc(string, ((80 * block + 1) * sizeof(char)));
    helper = scanf("%80[^\n]", &buffer);
  }
  string_len = strlen(string);
  string = realloc(string, (string_len + 1) * sizeof(char));
  if (helper == 0) {
    scanf("%*c");
  }
  return string;
}

