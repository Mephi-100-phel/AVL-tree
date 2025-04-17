#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>
#include <graphviz/cdt.h>
#include <stdlib.h>
#include "graph.h"
#include "tree.h"

void graph(Tree *root) {
  GVC_t *gvc = gvContext();
  Agraph_t *graph = createGraph(root);
  gvLayout(gvc, graph, "dot");
  FILE *file = fopen("graph.txt", "w");
  gvRender(gvc, graph, "dot", file);
  fclose(file);
  gvFreeLayout(gvc, graph);
  agclose(graph);
  gvFreeContext(gvc);
}

Agraph_t* createGraph(Tree *root) {
  Agraph_t *graph = agopen("graph", Agstrictundirected, 0);
  agattr(graph, AGNODE, "color", "black");
  agattr(graph, AGEDGE, "color", "black");
  agattr(graph, AGNODE, "fontcolor", "black");
  agattr(graph, AGEDGE, "label", "");
  agattr(graph, AGNODE, "root", "false");
  Agnode_t *root_n = graphSubTree(root->root, graph);
  agset(root_n, "root", "true");
  return graph;
}

Agnode_t* graphSubTree(Node *node, Agraph_t *tree) {
  Agnode_t *root = agnode(tree, node->key, 1);
  Agedge_t *edge = NULL;
  if (node->left != NULL) {
    Agnode_t *left_node = graphSubTree(node->left, tree);
    char *left_h = __intToStr(node->left_h);
    edge = agedge(tree, root, left_node, "", 1);
    agset(edge, "label", left_h);
    free(left_h);
  } else {
    char *func_str = __getRandStr(node->info->info);
    Agnode_t *node = agnode(tree, func_str, 1);
    free(func_str);
    agset(node, "color", "white");
    agset(node, "fontcolor", "white");
    edge = agedge(tree, root, node, "", 1);
    agset(edge, "color", "white");
  }
  if (node->right != NULL) {
    Agnode_t *right_node = graphSubTree(node->right, tree);
    char *right_h = __intToStr(node->right_h);
    edge = agedge(tree, root, right_node, "", 1);
    agset(edge, "label", right_h);
    free(right_h);
  } else {
    char *func_str = __getRandStr(node->info->info);
    Agnode_t *node = agnode(tree, func_str, 1);
    free(func_str);
    agset(node, "color", "white");
    agset(node, "fontcolor", "white");
    edge = agedge(tree, root, node, "", 1);
    agset(edge, "color", "white");
  }
  return root;
}

static char* __intToStr(size_t num) {
  int len = __numLen(num);
  char *num_str = (char*)calloc(len + 1, sizeof(char));
  for (int i = len - 1; i >= 0; i--) {
    num_str[i] = '0' + (num % 10);
    num /= 10;
  }
  return num_str;
}

static int __numLen(size_t num) {
  int len = 0;
  while (num > 0) {
    len++;
    num /= 10;
  }
  return len;
}

static char* __getRandStr(const size_t num) {
  srand(num);
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
