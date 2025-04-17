#ifndef DIALOG_H
#define DIALOG_H

#include "tree.h"
#include <time.h>

short D_insert(Tree*, time_t*);
short D_delete(Tree*, time_t*);
short D_specialSearch(Tree*, time_t*);
short D_search(Tree*, time_t*);
short D_traverse(Tree*, time_t*);
short D_exit(Tree*, time_t*);
short D_import(Tree*, time_t*);
short D_import(Tree*, time_t*);
short D_programmPrint(Tree*, time_t*);
short D_countNums(Tree*, time_t*);
short (*getDialogFunc(void))(Tree*, time_t*);
int __comp(int);

#endif
