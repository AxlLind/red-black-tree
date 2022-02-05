#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "rb-tree.h"

#define SWAP(x,y) do { \
  typeof(x) tmp = x;   \
  x = y;               \
  y = tmp;             \
} while (0)

static void _rb_print(rb_tree **root, int d) {
  printf("%*s", d*2, "");
  if (*root == NULL) {
    printf("nil B\n");
    return;
  }
  printf("%d %c\n",(*root)->val, (*root)->color == BLACK ? 'B' : 'R');
  if ((*root)->left || (*root)->right) {
    _rb_print(&(*root)->left, d+1);
    _rb_print(&(*root)->right, d+1);
  }
}

static void rb_rotate_left(rb_tree **root, rb_tree *x) {
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

static void rb_rotate_right(rb_tree **root, rb_tree *x) {
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

static void rb_insert_fix(rb_tree **root, rb_tree *node) {
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

static void rb_fix_double_black(rb_tree **root, rb_tree *x) {
  while (x != *root) {
    rb_tree *sibling = x == x->parent->left ? x->parent->right : x->parent->left;
    rb_tree *parent = x->parent;
    if (sibling == NULL) {
      x = parent;
      continue;
    }

    if (sibling->color == RED) {
      parent->color = RED;
      sibling->color = BLACK;
      if (sibling == sibling->parent->left) {
        rb_rotate_right(root, parent);
      } else {
        rb_rotate_left(root, parent);
      }
      continue;
    }

    if ((sibling->left == NULL || sibling->left->color == BLACK) && (sibling->right == NULL || sibling->right->color == BLACK)) {
      sibling->color = RED;
      if (parent->color == BLACK) {
        x = parent;
        continue;
      }
      parent->color = BLACK;
      break;
    }

    if (sibling->left && sibling->left->color == RED) {
      if (sibling == sibling->parent->left) {
        sibling->left->color = sibling->color;
        sibling->color = parent->color;
        rb_rotate_right(root, parent);
      } else {
        sibling->left->color = parent->color;
        rb_rotate_right(root, sibling);
        rb_rotate_left(root, parent);
      }
    } else {
      if (sibling == sibling->parent->left) {
        sibling->right->color = sibling->color;
        rb_rotate_left(root, sibling);
        rb_rotate_right(root, parent);
      } else {
        sibling->right->color = sibling->color;
        sibling->color = parent->color;
        rb_rotate_left(root, parent);
      }
    }
    parent->color = BLACK;
    break;
  }
}

static void rb_delete_node(rb_tree **root, rb_tree *v) {
  rb_tree *u = NULL;
  if (v->left && v->right) {
    u = v->right;
    while (u->left != NULL)
      u = u->left;
  } else if (v->left || v->right) {
    u = v->left ? v->left : v->right;
  }

  if (u && v->left && v->right) {
    SWAP(v->val, u->val);
    rb_delete_node(root, u);
    return;
  }

  int uv_black = ((u == NULL || u->color == BLACK) && v->color == BLACK);
  rb_tree *parent = v->parent;

  if (u == NULL) {
    if (v == *root) {
      *root = NULL;
    } else {
      if (uv_black) {
        rb_fix_double_black(root, v);
      } else {
        rb_tree *sibling = parent->left == v ? parent->right : parent->left;
        if (sibling)
          sibling->color = RED;
      }
      *(v == parent->left ? &parent->left : &parent->right) = NULL;
    }
    free(v);
    return;
  }

  if (v == *root) {
    v->val = u->val;
    v->left = v->right = NULL;
    free(u);
    return;
  }

  *(v == v->parent->left ? &v->parent->left : &v->parent->right) = u;
  u->parent = v->parent;
  if (uv_black) {
    rb_fix_double_black(root, u);
  } else {
    u->color = BLACK;
  }
  free(v);
}

void rb_print(rb_tree **root) { _rb_print(root, 0); }

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
  rb_delete_node(root, *next);
}

void rb_free(rb_tree **root) {
  if (*root == NULL)
    return;
  rb_free(&(*root)->left);
  rb_free(&(*root)->right);
  free(*root);
  *root = NULL;
}

}
