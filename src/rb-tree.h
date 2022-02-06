#ifndef RB_TREE_H
#define RB_TREE_H

/** @brief Enum of rb-tree colors */
enum rb_color { rb_BLACK, rb_RED };

/** @brief A red-black tree struct */
typedef struct rb_tree {
  int val;
  enum rb_color color;
  struct rb_tree *left, *right, *parent;
} rb_tree;

/** @brief Search the tree for the value 'val'.
 *  @param root The root node of the tree.
 *  @param val The value to look up.
 *  @return 1 if the tree contains 'val', 0 otherwise.
 */
int rb_contains(rb_tree **root, int val);

/** @brief Insert a value into the tree.
 *  @param root The root node of the tree.
 *  @param val The value to insert.
 *  @return 0 if the tree already contained the value, 1 otherwise.
 */
int rb_insert(rb_tree **root, int val);

/** @brief Delete a value from the tree.
 *  @param root The root node of the tree.
 *  @param val The value to delete.
 *  @return 0 if the tree did not contain the value, 1 otherwise.
 */
int rb_delete(rb_tree **root, int val);

/** @brief Free the tree, i.e free the memory.
 *  @param root The root node of the tree.
 */
void rb_free(rb_tree **root);

/** @brief Print the tree to stdout, mainly for debugging purposes.
 *  @param root The root node of the tree.
 */
void rb_print(rb_tree **root);

/** @brief Returns the size of the tree.
 *  @param root The root node of the tree.
 *  @return The size of the tree.
 */
int rb_size(rb_tree **root);

#endif // RB_TREE_H
