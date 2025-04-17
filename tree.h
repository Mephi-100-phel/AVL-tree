#ifndef TREE_H
#define TREE_H

typedef struct Info {
  size_t info;
} Info;

typedef struct Node {
  char *key;
  Info *info;
  struct Node *left;
  struct Node *right;
  struct Node *parent;
  unsigned short left_h;
  unsigned short right_h;
} Node;

typedef struct Tree {
  Node *root;
} Tree;

// Аллокация 
Info* createInfo(const size_t);
Node* createNode(char* const, const size_t, Node* const);
Tree* createTree(void);

// Поиск
Node* search(Tree*, const char*);
Node* specialSearch(Tree*, const char*);

// Вставка
short insert(Node*, char* const, size_t*);

// Восстановление свойств дерева
void fixup(Node*);
static void __resolveHight(Node*);
static void __rightFixup(Node*);
static void __leftFixup(Node*);

// Удаление
short delete(Tree*, char* const);
static void __delRootNode(Tree*);
static void __delEmptyNode(Node*);
static void __delNonEmptyNode(Node*);
static void __delFullNode(Node*);
static Node* __findVictim(Node*);
static void __resolveDelHight(Node*);

// Повороты
void rotateLeft(Node*);
void rotateRight(Node*);
static void __resolveParent(Node*, Node*);

// Вывод
void prettyPrint(Node*, Node*, char*);
void printNode(Node*);

// Освобождение памяти
void freeTree(Node*);
void freeNode(Node*);

// Обход
void traverse(Node*, const char*, const size_t*);

// Общие функции
static short max(const short, const short);

#endif
