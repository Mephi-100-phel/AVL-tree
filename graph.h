#ifndef GRAPH_H
#define GRAPH_H

#include "tree.h"
#include <graphviz/cgraph.h>

void graph(Tree*);
Agraph_t* createGraph(Tree*);
Agnode_t* graphSubTree(Node*, Agraph_t*);
static char* __intToStr(size_t);
static int __numLen(size_t);
static char* __getRandStr(const size_t);

#endif
