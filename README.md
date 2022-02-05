# red-black-tree
This repo contains an implementation of the classic datastructure `red-black tree` implemented in C. An rb-tree is a self-balancing binary search tree, providing `O(log n)` time complexity of the primary operations `lookup, insert, delete`.

## Usage
See the header file [rb-tree.h](./src/rb-tree.h) for the full API.

To include in a project, simply compile the [rb-tree.c](./src/rb-tree.c) c-file and include the header file [rb-tree.h](./src/rb-tree.h).

Use and initialize the tree like this:
```c
#include "rb-tree.h"
...
rb_tree *root = NULL;
rb_insert(&tree, 50);
rb_insert(&tree, 30);
rb_insert(&tree, 20);
rb_insert(&tree, 40);
...
if (rb_contains(&tree, 30)) {
  // do something
}
...
rb_delete(&tree, 50);
...
rb_free(&tree);
```
