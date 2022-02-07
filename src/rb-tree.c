#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "rb-tree.h"

#define SWAP(x,y) do { \
  typeof(x) tmp = x;   \
  x = y;               \
  y = tmp;             \
} while (0)

static void _rb_print(rb_tree *root, int d) {
  printf("%*s", d*2, "");
  if (!root) {
    printf("nil B\n");
    return;
  }
  printf("%d %c\n",root->val, root->color == rb_BLACK ? 'B' : 'R');
  if (root->left || root->right) {
    _rb_print(root->left, d+1);
    _rb_print(root->right, d+1);
  }
}

static void rb_rotate_left(rb_tree **root, rb_tree *v) {
  rb_tree *u = v->right;
  v->right = u->left;
  if (u->left)
    u->left->parent = v;
  u->parent = v->parent;
  if (v == *root) {
    *root = u;
  } else {
    *(v == v->parent->right ? &v->parent->right : &v->parent->left) = u;
  }
  u->left = v;
  v->parent = u;
}

static void rb_rotate_right(rb_tree **root, rb_tree *v) {
  rb_tree *u = v->left;
  v->left = u->right;
  if (u->right)
    u->right->parent = v;
  u->parent = v->parent;
  if (v == *root) {
    *root = u;
  } else {
    *(v == v->parent->right ? &v->parent->right : &v->parent->left) = u;
  }
  u->right = v;
  v->parent = u;
}

static void rb_insert_fix(rb_tree **root, rb_tree *node) {
  while (node != *root && node->parent->color == rb_RED) {
    rb_tree *grandparent = node->parent->parent;
    rb_tree *uncle = grandparent->left == node->parent ? grandparent->right : grandparent->left;
    if (uncle && uncle->color == rb_RED) {
      uncle->color = node->parent->color = rb_BLACK;
      grandparent->color = rb_RED;
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

static void rb_fix_double_black(rb_tree **root, rb_tree *v) {
  while (v != *root) {
    rb_tree *sibling = v == v->parent->left ? v->parent->right : v->parent->left;
    if (!sibling) {
      v = v->parent;
      continue;
    }

    if (sibling->color == rb_RED) {
      v->parent->color = rb_RED;
      sibling->color = rb_BLACK;
      if (sibling == sibling->parent->left) {
        rb_rotate_right(root, v->parent);
      } else {
        rb_rotate_left(root, v->parent);
      }
      sibling = v == v->parent->left ? v->parent->right : v->parent->left;
    }

    if ((!sibling->left || sibling->left->color == rb_BLACK) && (!sibling->right || sibling->right->color == rb_BLACK)) {
      sibling->color = rb_RED;
      if (v->parent->color == rb_BLACK) {
        v = v->parent;
        continue;
      }
      v->parent->color = rb_BLACK;
      break;
    }

    if (sibling->left && sibling->left->color == rb_RED) {
      if (sibling == sibling->parent->left) {
        sibling->left->color = sibling->color;
        sibling->color = v->parent->color;
        rb_rotate_right(root, v->parent);
      } else {
        sibling->left->color = v->parent->color;
        rb_rotate_right(root, sibling);
        rb_rotate_left(root, v->parent);
      }
    } else {
      if (sibling == sibling->parent->left) {
        sibling->right->color = v->parent->color;
        rb_rotate_left(root, sibling);
        rb_rotate_right(root, v->parent);
      } else {
        sibling->right->color = sibling->color;
        sibling->color = v->parent->color;
        rb_rotate_left(root, v->parent);
      }
    }
    v->parent->color = rb_BLACK;
    break;
  }
}

void rb_print(rb_tree *root) { _rb_print(root, 0); }

int rb_contains(rb_tree *tree, int val) {
  while (tree && tree->val != val)
    tree = val < tree->val ? tree->left : tree->right;
  return tree != NULL;
}

int rb_insert(rb_tree **root, int val) {
  rb_tree **next = root, *parent = NULL;
  while (*next && (*next)->val != val) {
    parent = *next;
    next = val < (*next)->val ? &(*next)->left : &(*next)->right;
  }
  if (*next)
    return 0;
  *next = malloc(sizeof(rb_tree));
  **next = (rb_tree) { .val = val, .color = rb_RED, .parent = parent };
  rb_insert_fix(root, *next);
  (*root)->color = rb_BLACK;
  return 1;
}

int rb_delete(rb_tree **root, int val) {
  rb_tree *v = *root, *u = NULL;
  while (v && v->val != val)
    v = val < v->val ? v->left : v->right;
  if (v == NULL)
    return 0;

  do {
    u = NULL;
    if (v->left && v->right) {
      for (u = v->right; u->left;)
        u = u->left;
    } else if (v->left || v->right) {
      u = v->left ? v->left : v->right;
    }

    if (!u || !v->left || !v->right)
      break;

    SWAP(v->val, u->val);
    v = u;
  } while (1);

  int uv_black = ((u == NULL || u->color == rb_BLACK) && v->color == rb_BLACK);

  if (v == *root) {
    if (u) {
      v->val = u->val;
      v->left = v->right = NULL;
      v = u;
    } else {
      *root = NULL;
    }
  } else if (u) {
    *(v == v->parent->left ? &v->parent->left : &v->parent->right) = u;
    u->parent = v->parent;
    if (uv_black) {
      rb_fix_double_black(root, u);
    } else {
      u->color = rb_BLACK;
    }
  } else {
    if (uv_black) {
      rb_fix_double_black(root, v);
    } else {
      rb_tree *sibling = v->parent->left == v ? v->parent->right : v->parent->left;
      if (sibling)
        sibling->color = rb_RED;
    }
    *(v == v->parent->left ? &v->parent->left : &v->parent->right) = NULL;
  }
  free(v);
  return 1;
}

void rb_free(rb_tree **root) {
  if (*root == NULL)
    return;
  rb_free(&(*root)->left);
  rb_free(&(*root)->right);
  free(*root);
  *root = NULL;
}

int rb_size(rb_tree *root) {
  if (root == NULL)
    return 0;
  return 1 + rb_size(root->left) + rb_size(root->right);
}
