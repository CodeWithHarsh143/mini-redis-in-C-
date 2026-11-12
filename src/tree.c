#include "tree.h"
Tree root = {.node = {.tag = (TagRoot | TagNode),
                      .north = (Node *)&root,
                      .west = 0,
                      .east = 0,
                      .path = "/"}};
void zero(int8 *str, int16 size)
{
  int8 *p;
  int16 n;
  for (p = str, n = 0; n < size; n++, p++)
    *p = 0;
}
Node *create_node(Node *parent, int8 *path)
{
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
  n->path[255] = 0;
  n->east = NULL;
  return n;
}

Leaf *find_last(Node *parent)
{
  Leaf *l;
  assert(parent);
  if (!parent->east)
  {
    return NULL;
  }
  for (l = parent->east; l; l->east = l->east)
    ; // linklist like traversal
  assert(l); // checking if l is valid
  return l;
}
Leaf *create_leaf(Node *parent, int8 *key, int8 *value, int16 count)
{
  Leaf *l, *newLeaf;
  int16 size;
  assert(parent);
  l = find_last(parent);
  size = sizeof(struct TreeLeaf);
  newLeaf = malloc(size);
  assert(newLeaf);
  *newLeaf = (Leaf){0};
  newLeaf->tag = TagLeaf;
  if (!l)
  {
    parent->east = newLeaf;
    newLeaf->west = (union u_tree *)parent;
  }
  else
  {
    l->east = newLeaf;
    newLeaf->west = (union u_tree *)l;
  }
  newLeaf->east = NULL;
  strncpy((char *)newLeaf->key, (char *)key, 128);
  newLeaf->value = malloc(count+1);
  assert(newLeaf->value);
  zero(newLeaf->value, count+1);
  strncpy((char *)newLeaf->value, (char *)value, count);
  newLeaf->size = count;
  return newLeaf;
}
int main()
{
  Node *n;
  Leaf *l;
  n = create_node((Node *)&root, "/Users");
  l = create_leaf(n, "Harsh", "Baghel", sizeof("Baghel"));
  printf("Path of Node: %s\n", (char *)n->path);
  printf("Address of leaf %p\n", l);
  printf("Key: %s\n", (char *)l->key);
  printf("Value: %s\n", (char *)l->value);
  return 0;
}
