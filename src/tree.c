#include "tree.h"
Tree root = {.node = {.tag = (TagRoot | TagNode),
                      .north = (Node *)&root,
                      .west = 0,
                      .east = 0,
                      .path = "/"}};
Node *create_node(Node *parent, int8 *path) {
  Node *n;
  int16 size;
  assert(parent);
  size = sizeof(struct TreeNode);
  n = malloc(size);
  assert(n);
  *n = (Node){};
  parent->west = n;
  n->north = parent;
  n->tag = TagNode;
  strncpy((char *)n->path, (char *)path, 255);
  return n;
}

Leaf *find_last(Node *parent) {
  Leaf *l;
  assert(parent);
  if (!parent->east) {
    perror("Not exist last leaf");
    return NULL;
  }
  for (l = parent->east; l; l->east = l->east)
    ; // linklist like traversal
  assert(l);
  return l;
}
Leaf *create_leaf(Node *parent, int8 *key, int8 *value, int16 count) {
  Leaf *l, *newLeaf;
  Node *n;
  int16 size;
  assert(parent);
  l = find_last(parent);
  size = sizeof(struct TreeLeaf);
  newLeaf = malloc(size);
  assert(newLeaf);
  *newLeaf = (Leaf){0};
  newLeaf->tag = TagLeaf;
  if (!l) {
    parent->east = newLeaf;
    newLeaf->west = (Node *)parent;
  } else {
    l->east = newLeaf;
    newLeaf->west = (Leaf *)l;
  }
  newLeaf->east = NULL;
  strncpy((char *)newLeaf->key, (char *)key, 128);
  newLeaf->value = malloc(count);
  assert(newLeaf->value);
  newLeaf->value = (int8)0;
  strncpy((char *)newLeaf->value, (char *)value, count);
  newLeaf->size = count;
  return newLeaf;
}
int main() {}
