#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rb-tree.h"

int verify_tree(rb_tree *node) {
  if (node == NULL)
    return 1;

  if (node->color == RED) {
    if (node->left && node->left->color == RED)
      return 0;
    if (node->right && node->right->color == RED)
      return 0;
  }

  int left_black_height = verify_tree(node->left);
  if (left_black_height == 0)
    return left_black_height;

  int right_black_height = verify_tree(node->right);
  if (right_black_height == 0)
    return right_black_height;

  if (left_black_height != right_black_height)
    return 0;
  return left_black_height + (node->color == BLACK);
}

int valid_rb_tree(rb_tree **root) {
  if (*root && (*root)->color != BLACK)
    return 0;
  return verify_tree(*root) != 0;
}

int main(int argc, char *argv[]) {
  int iters = 100000;
  if (argc == 2)
    sscanf(argv[1], "%d", &iters);
  printf("Running fuzz test with %d operations.\n", iters);
  srand(time(NULL));
  rb_tree *tree = NULL;
  int insertions = 0, deletions = 0;
  for (int i = 0; i < iters; i++) {
    int n = rand() >> 16;
    int type = n & 1;
    int val = n >> 1;
    if (type) {
      rb_insert(&tree, val);
      ++insertions;
    } else {
      rb_delete(&tree, val);
      ++deletions;
    }
    if (!valid_rb_tree(&tree)) {
      printf("[failed] Invalid tree!");
      rb_print(&tree);
      return 1;
    }
  }
  printf("[passed] RB-tree valid after %d iterations.\n", iters);
  printf("         tree size %d, %d insertions, %d deletions\n", rb_size(&tree), insertions, deletions);
}
