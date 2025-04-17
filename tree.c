#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "errors.h"

// Аллокация структур

Info* createInfo(const size_t info) {
  Info *strct = (Info*)calloc(1, sizeof(Info));
  strct->info = info;
  return strct;
}

Node* createNode(char* const key, const size_t info, Node* const par) {
  Node *node = (Node*)calloc(1, sizeof(Node));
  node->key = key;
  node->info = createInfo(info);
  node->parent = par;
  return node;
}

Tree* createTree(void) {
  return (Tree*)calloc(1, sizeof(Tree));
}

// Поиск

Node* search(Tree *tree, const char *key) {
  if (tree->root == NULL) {
    return NULL;
  }
  Node *found_node = tree->root;
  while (found_node != NULL) {
    int diff = strcmp(key, found_node->key);
    if (diff == 0) {
      return found_node;
    }
    found_node = (diff < 0) ? found_node->left : found_node->right;
  }
  return NULL;
}

Node* specialSearch(Tree *tree, const char *key) {
  Node *node = tree->root;
  int diff = strcmp(key, node->key);
  if (diff >= 0) {
    while (node->left != NULL) {
      node = node->left;
    }
  } else {
    while (node->right != NULL) {
      node = node->right;
    }
  }
  return node;
}

// Повороты

void rotateLeft(Node *cur) {
  Node *par = cur->parent, *right = cur->right;
  if (par != NULL) {
    __resolveParent(cur, right);
  }
  right->parent = par;
  cur->right = right->left;
  if (right->left != NULL) {
    right->left->parent = cur;
  }
  right->left = cur;
  cur->parent = right;
}

void rotateRight(Node *cur) {
  Node *par = cur->parent, *left = cur->left;
  if (par != NULL) {
    __resolveParent(cur, left);
  }
  left->parent = par;
  cur->left = left->right;
  if (left->right != NULL) {
    left->right->parent = cur;
  }
  left->right = cur;
  cur->parent = left;
}

static void __resolveParent(Node *cur_child, Node *new_child) {
  Node *par = cur_child->parent;
  if (par->left == cur_child) {
    par->left = new_child;
  } else {
    par->right = new_child;
  }
}

// Освобождение памяти

void freeTree(Node *node) {
  if (node == NULL) {
    return;
  } 
  if (node->left != NULL) {
    freeTree(node->left);
  }
  if (node->right != NULL) {
    freeTree(node->right);
  }
  freeNode(node);
}

void freeNode(Node *node) {
  free(node->key);
  free(node->info);
  free(node);
}

// Вывод

#define SPACE "     "
#define KEY "└──"
#define BLOCK "│    "

void prettyPrint(Node *node, Node *prev, char *prompt) {
  printf("%s", prompt);
  printf(KEY);
  printf(" %s R: %d L: %d\n", node->key, node->right_h, node->left_h);
  if (prev->left != NULL && prev->right != NULL && node != prev && prev->left != node) {
    strcat(prompt, BLOCK);
  } else {
    strcat(prompt, SPACE);
  }
  if (node->right != NULL) {
    char *new_prompt = (char*)calloc(120, sizeof(char));
    strcpy(new_prompt, prompt);
    prettyPrint(node->right, node, new_prompt);
    free(new_prompt);
  } 
  if (node->left != NULL) {
    char *new_prompt = (char*)calloc(120, sizeof(char));
    strcpy(new_prompt, prompt);
    prettyPrint(node->left, node, new_prompt);
    free(new_prompt);
  }
}

void printNode(Node *node) {
  printf("Ключ: %s, Инфо: %ld, Lh: %d, Rh: %d\n", node->key, node->info->info, node->left_h, node->right_h);
}

// Вставка

short insert(Node *node, char* const key, size_t *info) {
  int diff = strcmp(key, node->key);
  short hight = 0;
  if (diff == 0) {
    return -1;
  }
  if (diff < 0) {
    if (node->left == NULL) {
      node->left = createNode(key, *info, node);
      node->left_h = 1;
    } else {
      hight = insert(node->left, key, info);
      if (hight == -1) {
        return -1;
      }
      node->left_h = hight + 1;
    }
  } else {
    if (node->right == NULL) {
      node->right = createNode(key, *info, node);
      node->right_h = 1;
    } else {
      hight = insert(node->right, key, info);
      if (hight == -1) {
        return -1;
      }
      node->right_h = hight + 1;
    }
  }
  if (abs(node->left_h - node->right_h) > 1) {
    fixup(node);
    if (node->parent != NULL) {
      node = node->parent;
    }
  }
  return max(node->right_h, node->left_h);
}

// Восстановление свойств дерева (вставка)

void fixup(Node *node) {
  if (node->right_h > node->left_h) {
    __rightFixup(node);
  } else {
    __leftFixup(node);
  }
}

static void __resolveHight(Node *node) {
  if (node == NULL) {
    return;
  }
  if (node->left != NULL) {
    node->left_h = max(node->left->left_h, node->left->right_h) + 1;
  } else {
    node->left_h = 0;
  }
  if (node->right != NULL) {
    node->right_h = max(node->right->left_h, node->right->right_h) + 1;
  } else {
    node->right_h = 0;
  }
}

static void __rightFixup(Node *node) {
  Node *temp = node->right;
  if (temp->right_h > temp->left_h) {
    rotateLeft(node);
    __resolveHight(node);
    __resolveHight(temp);
  } else {
    rotateRight(temp);
    __resolveHight(temp);
    __resolveHight(temp->parent);
    rotateLeft(node);
    __resolveHight(node);
    __resolveHight(node->parent);
  }
}

static void __leftFixup(Node *node) {
  Node *temp = node->left;
  if (temp->left_h > temp->right_h) {
    rotateRight(node);
    __resolveHight(node);
    __resolveHight(temp);
  } else {
    rotateLeft(temp);
    __resolveHight(temp);
    __resolveHight(temp->parent);
    rotateRight(node);
    __resolveHight(node);
    __resolveHight(node->parent);
  }
}

// Удаление

short delete(Tree *tree, char* const key) {
  Node *del = search(tree, key);
  if (del == NULL) {
    return KEY_NOT_FOUND;
  }
  Node *par = del->parent;
  if (del == tree->root) {
    __delRootNode(tree);
  } else if (del->right == NULL && del->left == NULL) {
    __delEmptyNode(del);
  } else if (del->right != NULL && del->left != NULL) {
    __delFullNode(del);
  } else {
    __delNonEmptyNode(del);
  }
  if (par != NULL && abs(par->left_h - par->right_h) > 1) {
    fixup(par);
  }
  return NO_ERR;
}

static void __delRootNode(Tree *tree) {
  Node *del = tree->root;
  if (del->left == NULL && del->right == NULL) {
    freeNode(del);
    tree->root = NULL;
  } else if (del->left != NULL && del->right != NULL) {
    __delFullNode(del);
  } else {
    Node *child = (del->left != NULL) ? del->left : del->right;
    tree->root = child;
    child->parent = NULL;
    freeNode(del);
  }
  del = tree->root;
  if (del != NULL && abs(del->left_h - del->right_h) > 1) {
    fixup(del);
  }
}

static void __delEmptyNode(Node *node) {
  Node *par = node->parent;
  if (par->left == node) {
    par->left = NULL;
    par->left_h--;
  } else {
    par->right = NULL;
    par->right_h--;
  }
  freeNode(node);
  if (par != NULL && abs(par->left_h - par->right_h) > 1) {
    fixup(par);
  }
  __resolveDelHight(par);
}

static void __delNonEmptyNode(Node *node) {
  Node *child = (node->left != NULL) ? node->left : node->right, *par = node->parent;
  if (par->right == node) {
    par->right = child;
    child->parent = par;
    par->right_h--;
  } else {
    par->left = child;
    child->parent = par;
    par->left_h--;
  }
  freeNode(node);
  __resolveDelHight(par);
}

static void __delFullNode(Node *node) {
  Node *victim = __findVictim(node);
  free(node->key);
  node->key = victim->key;
  victim->key = NULL;
  if (victim->left == NULL) {
    __delEmptyNode(victim);
  } else {
    __delNonEmptyNode(victim);
  }
}

static Node* __findVictim(Node *node) {
  node = node->left;
  while (node->right != NULL) {
    node = node->right;
  }
  return node;
}

static void __resolveDelHight(Node *node) {
  if (node == NULL || node->parent == NULL) {
    return;
  }
  if (node->left != NULL) {
    node->left_h = max(node->left->left_h, node->left->right_h) + 1;
  }
  if (node->right != NULL) {
    node->right_h = max(node->right->left_h, node->right->right_h) + 1;
  }
  short new_hight = 0;
  Node *par = node->parent;
  new_hight = max(node->left_h, node->right_h);
  while (new_hight < (par->right == node) ? par->right_h : par->left_h) {
    if (par->right == node) {
      par->right_h = new_hight + 1;
    } else {
      par->left_h = new_hight + 1;
    }
    node = par;
    par = par->parent;
    if (par == NULL) {
      return;
    }
    new_hight = max(node->left_h, node->right_h);
  }
}

// Обход

void traverse(Node *node, const char *str, const size_t *len) {
  if (node == NULL) {
    return;
  }
  if (node->right != NULL) {
    traverse(node->right, str, len);
  }
  if (node->left != NULL) {
    traverse(node->left, str, len);
  }
  if (strncmp(node->key, str, *len) == 0) {
    printNode(node);
  }
}

// Общие функции

static short max(const short x, const short y) {
  return (x > y) ? x : y;
}
