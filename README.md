# red-black-tree
This repo contains an implementation of the classic datastructure `red-black tree` implemented in C. An rb-tree is a self-balancing binary search tree, providing `O(log n)` time complexity of the primary operations search, insert, and delete.

Note that this implementation is a set, e.g it does not allow duplicate values.

## Usage
```bash
make build/rb-tree.o  # compile the lib into an object file
make fuzz             # compile and run the fuzz test
make fuzz-valgrind    # run memory-safety check
./build/fuzz 1000     # run the fuzz test with 1000 operations
```
## API
See the header file [rb-tree.h](./src/rb-tree.h) for the full API. Initialize and use the tree like so:
```c
#include "rb-tree.h"

rb_tree *tree = NULL; // define an empty tree

rb_insert(&tree, 50);
rb_insert(&tree, 30);
rb_insert(&tree, 20);
rb_insert(&tree, 40);
rb_insert(&tree, 40); // returns 0 since the tree already contains the value

if (rb_contains(&tree, 30)) {
  // do something
}

rb_delete(&tree, 50);
rb_delete(&tree, 20);
rb_delete(&tree, 10); // returns 0 since the tree did not contain the value

assert(rb_size(&tree) == 2); // only contains 30, 40

rb_free(&tree);

// rb_free sets the root pointer to NULL
assert(tree == NULL);
```

## Testing strategy
For this project, I wanted to test the feasability of fuzz-testing instead of writing manual test cases. The fuzz-test works as follows:
- Perform a random operation of insert or delete with a random value.
- Verify that all red-black properties still hold.
- Repeat.

This is implemented in [fuzz.c](./src/fuzz.c). The test has ran successfully for **1 billion** operations, which took over an hour on my machine. Given the amount of operations, you can be reasonably certain that every possible scenario of insertion and deletion has been tested. This gives you an exhaustive test.

The downside of this approach is for one the test time, however this only has to be done once. Second, it relies on a correct implementation of step 2: verifying all red-black properties of the tree. However, implementing this is orders of magnitude easier than the red-black tree itself. Verifying that this function is correct is not difficult.

Regarding memory-safety, valgrind reports no errors when running the fuzz test.
