#include "tree.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

extern Tree root;

/* Build a big chain tree similar to example_tree:
 * /a, /a/b, /a/b/c, ... (depth n)
 */
static void build_big_chain(Node *start, int depth) {
  Node *cur = start;
  int8 path[256];
  zero(path, 256);
  for (int i = 0; i < depth && i < 26; i++) {
    int len = (int)strlen((char *)path);
    path[len] = '/';
    path[len + 1] = (int8)('a' + i);
    path[len + 2] = '\0';
    cur = create_node(cur, path);
    assert(cur != NULL);
  }
}

int main(void) {
  /* 1) Big tree like example_tree */
  Tree t = example_tree();
  printf("=== Big tree (example_tree) ===\n");
  print_tree(STDOUT_FILENO, t);

  /* basic checks on structure/paths */
  assert(find_node_linear((int8 *)"/a") != NULL);
  assert(find_node_linear((int8 *)"/a/b") != NULL);
  assert(find_node_linear((int8 *)example_path('e')) != NULL);
  assert(find_node_linear((int8 *)example_path('z')) != NULL);

  /* 2) Add leaves to nodes in the big tree */
  Node *n1 = find_node_linear((int8 *)"/a");
  Node *n2 = find_node_linear((int8 *)"/a/b/c");
  Node *n3 = find_node_linear((int8 *)example_path('m'));
  Node *n4 = find_node_linear((int8 *)example_path('z'));
  assert(n1 && n2 && n3 && n4);

  Leaf *l1 = create_leaf(n1, (int8 *)"k1", (int8 *)"value1", 6);
  Leaf *l2 = create_leaf(n2, (int8 *)"k2", (int8 *)"value2", 6);
  Leaf *l3 = create_leaf(n2, (int8 *)"k3", (int8 *)"value3", 6);
  Leaf *l4 = create_leaf(n3, (int8 *)"middle", (int8 *)"mval", 4);
  Leaf *l5 = create_leaf(n4, (int8 *)"end", (int8 *)"zval", 4);
  assert(l1 && l2 && l3 && l4 && l5);

  printf("\n=== After adding leaves ===\n");
  print_tree(STDOUT_FILENO, t);

  /* 3) Check lookup function */
  Leaf *r;

  r = lookup_linear((int8 *)"/a", (int8 *)"k1");
  assert(r != NULL);
  assert(strcmp((char *)r->key, "k1") == 0);
  assert(strcmp((char *)r->value, "value1") == 0);

  r = lookup_linear((int8 *)"/a/b/c", (int8 *)"k2");
  assert(r != NULL);
  assert(strcmp((char *)r->value, "value2") == 0);

  r = lookup_linear((int8 *)"/a/b/c", (int8 *)"k3");
  assert(r != NULL);
  assert(strcmp((char *)r->value, "value3") == 0);

  r = lookup_linear((int8 *)example_path('m'), (int8 *)"middle");
  assert(r != NULL);
  assert(strcmp((char *)r->value, "mval") == 0);

  r = lookup_linear((int8 *)example_path('z'), (int8 *)"end");
  assert(r != NULL);
  assert(strcmp((char *)r->value, "zval") == 0);

  /* negative lookups */
  assert(lookup_linear((int8 *)"/a", (int8 *)"missing") == NULL);
  assert(lookup_linear((int8 *)"/a/b", (int8 *)"k1") == NULL);
  assert(lookup_linear((int8 *)"/no/such/path", (int8 *)"k1") == NULL);

  printf("\nAll lookup checks executed.\n");
  return 0;
}
