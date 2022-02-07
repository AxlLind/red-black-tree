#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rb-tree.h"

int verify_tree(rb_tree *node) {
  if (node == NULL)
    return 1;

  if (node->color == rb_RED) {
    if (node->left && node->left->color == rb_RED)
      return 0;
    if (node->right && node->right->color == rb_RED)
      return 0;
  }

  int l_black_height = verify_tree(node->left);
  if (l_black_height == 0)
    return l_black_height;
  int r_black_height = verify_tree(node->right);
  if (r_black_height == 0)
    return r_black_height;

  if (l_black_height != r_black_height)
    return 0;
  return l_black_height + (node->color == rb_BLACK);
}

int valid_rb_tree(rb_tree *root) {
  if (root && root->color != rb_BLACK)
    return 0;
  return verify_tree(root) != 0;
}

int main(int argc, char *argv[]) {
  srand(time(NULL));
  int ops = 10000, insertions = 0, deletions = 0;
  if (argc == 2)
    sscanf(argv[1], "%d", &ops);
  printf("Running fuzz test with %d operations.\n", ops);
  rb_tree *tree = NULL;
  for (int i = 0; i < ops; i++) {
    int n = rand() >> 16;
    if (n & 1) {
      rb_insert(&tree, n >> 1);
      ++insertions;
    } else {
      rb_delete(&tree, n >> 1);
      ++deletions;
    }
    if (!valid_rb_tree(tree)) {
      printf("[failed] Invalid tree!");
      rb_print(tree);
      return 1;
    }
  }
  printf("[passed] RB-tree valid after %d operations!\n", ops);
  printf("         size=%d, insertions=%d, deletions=%d\n", rb_size(tree), insertions, deletions);
  rb_free(&tree);
}
