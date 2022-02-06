#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "rb-tree.h"

// The example code referenced in the README.

int main(void) {
  rb_tree *tree = NULL; // define an empty tree

  rb_insert(&tree, 50);
  rb_insert(&tree, 30);
  rb_insert(&tree, 20);
  rb_insert(&tree, 40);
  rb_insert(&tree, 40); // returns 0 since the tree already contains the value

  assert(rb_contains(&tree, 30));

  rb_delete(&tree, 50);
  rb_delete(&tree, 20);
  rb_delete(&tree, 10); // returns 0 since the tree did not contain the value

  assert(rb_size(&tree) == 2); // only contains 30, 40

  rb_free(&tree);

  assert(tree == NULL); // rb_free sets the root pointer to NULL
}
