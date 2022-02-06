# red-black-tree
This repo contains an implementation of the classic datastructure `red-black tree` implemented in C. An rb-tree is a self-balancing binary search tree, providing `O(log n)` time complexity of the primary operations `lookup, insert, delete`.

Note that this implementation is a set, e.g it does not allow duplicate values.

## Usage
```bash
make build/rb-tree.o # compile the lib into an object file
make fuzz            # compile and run the fuzz test
./build/fuzz 1000    # run the fuzz test with 1000 operations
```

## API
See the header file [rb-tree.h](./src/rb-tree.h) for the full API.

To include in a project, simply compile the [rb-tree.c](./src/rb-tree.c) c-file and include the header file [rb-tree.h](./src/rb-tree.h).

Use and initialize the tree like so:
```c
#include "rb-tree.h"

int main(void) {
  // define an empty tree
  rb_tree *tree = NULL;

  // insert values into the tree
  rb_insert(&tree, 50);
  rb_insert(&tree, 30);
  rb_insert(&tree, 20);
  rb_insert(&tree, 40);
  rb_insert(&tree, 40); // returns 0 since the tree already contains the value

  // check if the tree contains a specific value
  if (rb_contains(&tree, 30)) {
    // do something
  }

  // delete values from the tree
  rb_delete(&tree, 50);
  rb_delete(&tree, 20);
  rb_delete(&tree, 10); // returns 0 since the tree did not contain the value

  // free all memory associated with the tree
  rb_free(&tree);

  // rb_free sets the root pointer to NULL
  assert(tree == NULL);
}
```
