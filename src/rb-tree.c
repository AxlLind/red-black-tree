#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define BLACK 0
#define RED   1

#define SWAP(x,y) do { \
  typeof(x) tmp = x;   \
  x = y;               \
  y = tmp;             \
} while (0)

typedef struct rb_tree {
  int val;
  char color;
  struct rb_tree *left, *right, *parent;
} rb_tree;

void print_tree(rb_tree **root, int d) {
  printf("%*s", d*2, "");
  if (*root == NULL) {
    printf("nil B\n");
    return;
  }
  printf("%d %c\n",(*root)->val, (*root)->color == BLACK ? 'B' : 'R');
  if ((*root)->left || (*root)->right) {
    print_tree(&(*root)->left, d+1);
    print_tree(&(*root)->right, d+1);
  }
}

void rb_rotate_left(rb_tree **root, rb_tree *x) {
  rb_tree *y = x->right;
  x->right = y->left;
  if (y->left)
    y->left->parent = x;
  y->parent = x->parent;
  if (x == *root) {
    *root = y;
  } else if (x == x->parent->right) {
    x->parent->right = y;
  } else {
    x->parent->left = y;
  }
  y->left = x;
  x->parent = y;
}

void rb_rotate_right(rb_tree **root, rb_tree *x) {
  rb_tree *y = x->left;
  x->left = y->right;
  if (y->right)
    y->right->parent = x;
  y->parent = x->parent;
  if (x == *root) {
    *root = y;
  } else if (x == x->parent->right) {
    x->parent->right = y;
  } else {
    x->parent->left = y;
  }
  y->right = x;
  x->parent = y;
}

void rb_insert_fix(rb_tree **root, rb_tree *node) {
  while (node != *root && node->parent->color == RED) {
    rb_tree *grandparent = node->parent->parent;
    rb_tree *uncle = grandparent->left == node->parent ? grandparent->right : grandparent->left;
    if (uncle && uncle->color == RED) {
      uncle->color = BLACK;
      node->parent->color = BLACK;
      grandparent->color = RED;
      node = grandparent;
      continue;
    }
    if (node->parent == grandparent->left) {
      if (node == node->parent->right) {
        rb_rotate_left(root, node->parent);
        node = node->parent;
      }
      rb_rotate_right(root, grandparent);
    } else {
      if (node == node->parent->left) {
        rb_rotate_right(root, node->parent);
        node = node->parent;
      }
      rb_rotate_left(root, grandparent);
    }
    SWAP(grandparent->color, node->parent->color);
  }
}

int rb_contains(rb_tree **root, int val) {
  rb_tree **next = root;
  while (*next != NULL && (*next)->val != val)
    next = val < (*next)->val ? &(*next)->left : &(*next)->right;
  return *next != NULL;
}

void rb_insert(rb_tree **root, int val) {
  rb_tree **next = root, *parent = NULL;
  while (*next != NULL && (*next)->val != val) {
    parent = *next;
    next = val < (*next)->val ? &(*next)->left : &(*next)->right;
  }
  if (*next != NULL)
    return;
  *next = malloc(sizeof(rb_tree));
  **next = (rb_tree) { .val = val, .color = RED, .parent = parent };
  rb_insert_fix(root, *next);
  (*root)->color = BLACK;
}

void rb_delete(rb_tree **root, int val) {
  rb_tree **next = root;
  while (*next != NULL && (*next)->val != val)
    next = val < (*next)->val ? &(*next)->left : &(*next)->right;
  if (*next == NULL)
    return;

  // no children
  if ((*next)->left == NULL && (*next)->right == NULL) {
    if (*next == *root) {
      *root = NULL;
    } else if (*next == (*next)->parent->left) {
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

  if (smallest->parent == *next) {
    smallest->parent->right = smallest->right;
  } else {
    smallest->parent->left = smallest->right;
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

int main() {
  rb_tree *tree = NULL;
  rb_insert(&tree, 8);
  rb_insert(&tree, 18);
  rb_insert(&tree, 5);
  rb_insert(&tree, 15);
  rb_insert(&tree, 17);
  rb_insert(&tree, 25);
  rb_insert(&tree, 40);
  rb_insert(&tree, 80);
  print_tree(&tree, 0);
}
