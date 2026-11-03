#include "tree.h"
Tree root = {.node = {
                 .tag = (TagRoot | TagNode),
                 .north = (Node *)&root,
                 .west = 0,
                 .east = 0,
                 .path = "/"}};
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
  return n;
}
int main()
{
  Node *n1, *n2;
  n1 = create_node((Node *)&root, (int8 *)"/");
  assert(n1);
  n2 = create_node(n1, (int8 *)"/Users");
  assert(n2);
  printf("%s\n", (char *)n1->path);
  printf("%s\n", (char *)n2->path);
  free(n1);
  free(n2);
}
