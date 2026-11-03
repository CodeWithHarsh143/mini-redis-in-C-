#ifndef TREE
#define TREE
// includes
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#define TagRoot 1;
#define TagNode 2;
#define TagLeaf 4;

typedef unsigned int int32;
typedef short int int16;
typedef unsigned char int8;
typedef unsigned char Tag;
// struct
struct TreeLeaf
{
  Tag tag;
  union u_tree *west;
  struct TreeLeaf *east;
  int8 key[128];
  int8 *value;
  int16 size;
};
struct TreeNode
{
  Tag tag;
  struct TreeNode *north;
  struct TreeNode *west;
  struct TreeLeft *east;
  int8 path[256];
};
typedef struct TreeNode Node;
typedef struct TreeLeaf Leaf;
union u_tree
{
  Leaf leaf;
  Node node;
};
typedef union u_tree Tree;
#endif
