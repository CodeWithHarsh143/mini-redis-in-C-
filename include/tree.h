#ifndef TREE_H
#define TREE_H
// includes
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TagRoot 1
#define TagNode 2
#define TagLeaf 4
#define WRITE_NODE(fd, path) write(fd, (char *)path, strlen((char *)path))
#define WRITE_LEAF(fd, l)                                                      \
  do {                                                                         \
    write(fd, l->key, strlen(l->key));                                         \
    write(fd, "->", 2);                                                        \
    write(fd, l->value, strlen(l->value));                                     \
    if (l->east)                                                               \
      write(fd, ",", 1);                                                       \
  } while (0)
typedef unsigned int int32;
typedef short int int16;
typedef unsigned char int8;
typedef unsigned char Tag;

// struct
struct TreeLeaf {
  Tag tag;
  union u_tree *west;
  struct TreeLeaf *east;
  int8 key[128];
  int8 *value;
  int16 size;
};
struct TreeNode {
  Tag tag;
  struct TreeNode *north;
  struct TreeNode *west;
  struct TreeLeaf *east;
  int8 path[256];
};
typedef struct TreeNode Node;
typedef struct TreeLeaf Leaf;
union u_tree {
  Node node;
  Leaf leaf;
};
typedef union u_tree Tree;
void print_tree(int, Tree);
void indent(int, int16);
void zero(int8 *, int16);
Leaf *find_last(Node *);
Leaf *create_leaf(Node *, int8 *, int8 *, int16);
Node *create_node(Node *, int8 *);
int main();
#endif
