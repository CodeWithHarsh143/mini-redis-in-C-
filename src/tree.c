#include "tree.h"
Tree root = {.node = {.tag = (TagRoot | TagNode),
                      .north = (Node *)&root,
                      .south = 0,
                      .east = 0,
                      .path = "/"}};
void print_tree(int fd, Tree TreeRoot) {
  int8 indendation;
  Node *n;
  Leaf *l;
  indendation = 0;
  for (n = &TreeRoot.node; n; n = n->south) {
    indent(fd, indendation++);
    WRITE_NODE(fd, n->path);
    if (n->east) {
      write(fd, "/$Data= ", strlen("/$Data= "));
      for (l = n->east; l; l = l->east) {
        WRITE_LEAF(fd, l);
      }
    }
    write(fd, "\n", 1);
  }
}
void zero(int8 *str, int16 size) {
  int8 *p;
  int16 n;
  for (p = str, n = 0; n < size; n++, p++)
    *p = 0;
}
Node *create_node(Node *parent, int8 *path) {
  Node *n;
  int16 size;
  assert(parent);
  size = sizeof(struct TreeNode);
  n = malloc(size);
  assert(n);
  *n = (Node){};
  parent->south = n;
  n->north = parent;
  n->tag = TagNode;
  strncpy((char *)n->path, (char *)path, 255);
  n->path[255] = 0;
  n->east = NULL;
  return n;
}

Leaf *find_last(Node *parent) {
  Leaf *l;
  assert(parent);
  if (!parent->east) {
    return NULL;
  }
  for (l = parent->east; l->east; l = l->east)
    ;        // linklist like traversal
  assert(l); // checking if l is valid
  return l;
}
Leaf *create_leaf(Node *parent, int8 *key, int8 *value, int16 count) {
  Leaf *l, *newLeaf;
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
    newLeaf->south = (union u_tree *)parent;
  } else {
    l->east = newLeaf;
    newLeaf->south = (union u_tree *)l;
  }
  newLeaf->east = NULL;
  strncpy((char *)newLeaf->key, (char *)key, 128);
  newLeaf->value = malloc(count + 1);
  assert(newLeaf->value);
  zero(newLeaf->value, count + 1);
  strncpy((char *)newLeaf->value, (char *)value, count);
  newLeaf->size = count;
  return newLeaf;
}
void indent(int fd, int16 steps) {
  int step;
  for (step = 0; step < steps; step++)
    write(fd, "  ", strlen("  "));
}
int main() {
  // example
  Node *n1, *n2, *n3;
  Leaf *l;
  n1 = create_node((Node *)&root, "/Users");
  n2 = create_node(n1, "/Users/Document");
  n3 = create_node(n2, "/Users/Document/Redis");
  l = create_leaf(n3, "Harsh", "Baghel", sizeof("Baghel"));
  l = create_leaf(n3, "Shivam", "Baghel", sizeof("Baghel"));
  print_tree(STDOUT_FILENO, root);
  return 0;
}
