#include "tree.h"
#include <linux/limits.h>
#include <unistd.h>
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

Node *find_node_linear(int8 *path) {
  int16 size = strlen((char *)path);
  if (size > 255)
    return NULL;
  int8 *ch, *pp;
  Leaf *l;
  int depth;
  Node *p;
  depth = 0;
  ch = path;
  while (*ch) {
    if (*ch == '/') {
      ch++;
      if (*ch)
        depth++;
    } else
      ch++;
  }
  for (p = &root.node; p && depth; p = p->south, depth--)
    ;

  if (!p)
    return NULL;

  for (ch = path, pp = p->path; *ch && *pp && (*ch == *pp); ch++, pp++)
    ;
  if (*ch != *pp)
    return NULL;
  return p;
}
Leaf *lookup_linear(int8 *path, int8 *key) {
  Node *p;
  Leaf *l;
  int16 size;
  size = strlen((char *)key);
  if (size > 127)
    return NULL;
  if (!find_node_linear(path))
    return NULL;
  if (!p->east)
    return NULL;

  for (l = p->east; (l && strcmp((char *)l->key, key)); l = l->east)
    ;

  if (!l)
    return NULL;

  return l;
}

Tree example_tree(void) {
  Node *n, *p;
  int8 path[256]; // allocating space for path
  int8 ch;
  int32 len;
  zero(path, 256);
  for (ch = 'a', n = &root.node; ch <= 'z'; ch++) {
    len = (int32)strlen((char *)path);
    *(path + len++) = '/';
    *(path + len) = ch;
    // p = n; // parent
    n = create_node(n, path);
  }

  return root;
}
int main() {
  // example
  Tree t;
  t = example_tree();
  print_tree(STDOUT_FILENO, t);
  return 0;
}
