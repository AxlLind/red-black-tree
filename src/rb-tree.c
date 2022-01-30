#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define BLACK 0
#define RED   1

typedef struct rb_tree {
  int val;
  char color;
  struct rb_tree *left, *right, *parent;
} rb_tree;

void rb_insert_fix(rb_tree **root, rb_tree *node) {
  // TODO
}

int rb_contains(rb_tree **root, int val) {
  rb_tree **next = root;
  while (*next != NULL && (*next)->val != val)
    next = val < (*next)->val ? &(*next)->left : &(*next)->right;
  return *next != NULL;
}

void rb_insert(rb_tree **root, int val) {
  char color = *root == NULL ? BLACK : RED;
  rb_tree **next = root, *parent = NULL;
  while (*next != NULL && (*next)->val != val) {
    parent = *next;
    next = val < (*next)->val ? &(*next)->left : &(*next)->right;
  }
  if (*next != NULL)
    return;
  *next = malloc(sizeof(rb_tree));
  **next = (rb_tree) { .val = val, .color = color, .parent = parent };
  rb_insert_fix(root, *next);
}

void rb_delete(rb_tree **root, int val) {
  rb_tree **next = root;
  while (*next != NULL && (*next)->val != val)
    next = val < (*next)->val ? &(*next)->left : &(*next)->right;
  if (*next == NULL)
    return;

  // no children
  if ((*next)->left == NULL && (*next)->right == NULL) {
    if ((*next)->parent == NULL) {
      *root = NULL;
    } else if ((*next)->parent->left == *next) {
      (*next)->parent->left = NULL;
    } else {
      (*next)->parent->right = NULL;
    }
    free(*next);
    return;
  }

  // one child
  if ((*next)->left == NULL || (*next)->right == NULL) {
    rb_tree *child = (*next)->left == NULL ? (*next)->right : (*next)->left;
    (*next)->val = child->val;
    (*next)->left = child->left;
    (*next)->right = child->right;
    free(child);
    return;
  }

  // two children
  rb_tree *smallest = (*next)->right;
  while (smallest->left != NULL)
    smallest = smallest->left;

  if (smallest->parent != *next) {
    smallest->parent->left = smallest->right;
  } else {
    smallest->parent->right = smallest->right;
  }

  (*next)->val = smallest->val;
  free(smallest);
}

void rb_free(rb_tree **root) {
  if (*root == NULL)
    return;
  rb_free(&(*root)->left);
  rb_free(&(*root)->right);
  free(*root);
  *root = NULL;
}

void print_tree(rb_tree **root, int d) {
  if (*root == NULL)
    return;
  for (int i = 0; i < d; ++i)
    printf("  ");
  printf("%d\n",(*root)->val);
  print_tree(&(*root)->left, d+1);
  print_tree(&(*root)->right, d+1);
}

int main() {
  rb_tree *tree = NULL;
  rb_insert(&tree, 50);
  rb_insert(&tree, 30);
  rb_insert(&tree, 20);
  rb_insert(&tree, 40);
  rb_insert(&tree, 70);
  rb_insert(&tree, 60);
  rb_insert(&tree, 80);
  print_tree(&tree, 0);
  rb_free(&tree);

  tree = NULL;
  for (int i = 0; i < 10; i++)
    rb_insert(&tree, i);
  print_tree(&tree, 0);
}
