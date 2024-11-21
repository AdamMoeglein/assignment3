/*
 * This file is where you should implement your binary search tree.  It already
 * contains skeletons of the functions you need to implement (along with
 * documentation for each function).  Feel free to implement any additional
 * functions you might need.  Also, don't forget to include your name and
 * @oregonstate.edu email address below.
 *
 * Name: Adam Moeglein
 * Email: moegleia@oregonstate.edu
 */

#include <stdlib.h>
#include <stdio.h>

#include "bst.h"
#include "stack.h"

/*
 * This structure represents a single node in a BST.  In addition to containing
 * pointers to its two child nodes (i.e. `left` and `right`), it contains two
 * fields representing the data stored at this node.  The `key` field is an
 * integer value that should be used as an identifier for the data in this
 * node.  Nodes in the BST should be ordered based on this `key` field.  The
 * `value` field stores data associated with the key.
 *
 * You should not modify this structure.
 */
struct bst_node
{
  int key;
  void *value;
  struct bst_node *left;
  struct bst_node *right;
};

/*
 * This structure represents an entire BST.  It specifically contains a
 * reference to the root node of the tree.
 *
 * You should not modify this structure.
 */
struct bst
{
  struct bst_node *root;
};

/*
 * This function should allocate and initialize a new, empty, BST and return
 * a pointer to it.
 */
struct bst *bst_create() {
  struct bst *bst = malloc(sizeof(struct bst));
  bst->root = NULL;
  
  return bst;
}

struct bst_node* node_create(void* value, int key){
  struct bst_node* node = malloc(sizeof(struct bst_node));
  node->left = node->right = NULL;
  node->value = value;
  node->key = key;
}

/*
 * This function should free the memory associated with a BST.  While this
 * function should up all memory used in the BST itself, it should not free
 * any memory allocated to the pointer values stored in the BST.  This is the
 * responsibility of the caller.
 *
 * Params:
 *   bst - the BST to be destroyed.  May not be NULL.
 */
void bst_free(struct bst *bst) {

  node_free(bst->root);
  free(bst);
  return;
  
}

void node_free(struct bst_node* node){
  if (node->left){
    node_free(node->left);
  }
  if (node->right){
    node_free(node->right);
  }
free(node);
  
}

/*
 * This function should return the total number of elements stored in a given
 * BST.
 *
 * Params:
 *   bst - the BST whose elements are to be counted.  May not be NULL.
 *
 */
int bst_size_helper(struct bst_node* node){
  if (node == NULL)
  {
    return (0);
  }

  else
  {
    return (bst_size_helper(node->left) + 1 + bst_size_helper(node->right));
  }
} 

int bst_size(struct bst *bst)
{
  struct bst_node *node = bst->root;
  return bst_size_helper(node);
  
}



/*
 * This function should insert a new key/value pair into the BST.  The key
 * should be used to order the key/value pair with respect to the other data
 * stored in the BST.  The value should be stored along with the key, once the
 * right location in the tree is found.
 *
 * Params:
 *   bst - the BST into which a new key/value pair is to be inserted.  May not
 *     be NULL.
 *   key - an integer value that should be used to order the key/value pair
 *     being inserted with respect to the other data in the BST.
 *   value - the value being inserted into the BST.  This should be stored in
 *     the BST alongside the key.  Note that this parameter has type void*,
 *     which means that a pointer of any type can be passed.
 *
 */
void bst_insert(struct bst *bst, int key, void *value)
{
  struct bst_node* new_node = node_create(value,key);
  if (bst->root == NULL){
    bst->root = new_node;
    return;
  }
  struct bst_node *curr = NULL;
  struct bst_node *next = bst->root;
  
  while (next != NULL)
  {
    curr = next;
    if (key < next->key)
    {
      next = next->left;
    }
    else
    {
      next = next->right;
    }
  }
  if (key < curr->key){
    curr->left = new_node;
    return;
  }
  else {
    curr->right = new_node;
  }
  
  return;
}

/*
 * This function should remove a key/value pair with a specified key from a
 * given BST.  If multiple values with the same key exist in the tree, this
 * function should remove the first one it encounters (i.e. the one closest to
 * the root of the tree).
 *
 * Params:
 *   bst - the BST from which a key/value pair is to be removed.  May not
 *     be NULL.
 *   key - the key of the key/value pair to be removed from the BST.
 */
void bst_remove(struct bst *bst, int key)
{
  if (bst->root == NULL){
    return;
  }
   // Update the root of the BST after removal
    bst->root = remove_node(bst->root, key);
}

// Helper function to recursively remove a node
struct bst_node* remove_node(struct bst_node* node, int key) {
  if (node == NULL) {
    return NULL; // Key not found
  }

  // Traverse the tree to find the node to delete
  if (key < node->key) {
     node->left = remove_node(node->left, key);
  } 
  else if (key > node->key) {
    node->right = remove_node(node->right, key);
  } 
  else {
    // Node to be deleted is found

    // Case 1: Node has no children or only a right child
    if (node->left == NULL) {
      struct bst_node *temp = node->right;
      free(node);
      return temp;
    }

    // Case 2: Node has only a left child
    if (node->right == NULL) {
      struct bst_node *temp = node->left;
      free(node);
      return temp;
    }

    // Case 3: Node has two children
    // Find the in-order successor (smallest node in the right subtree)
    struct bst_node *succ = get_successor(node);

    // Replace the node's key and value with the successor's
    node->key = succ->key;
    node->value = succ->value;

    // Delete the in-order successor
    node->right = remove_node(node->right, succ->key);
  }

    return node;
}


/*
 * This function should return the value associated with a specified key in a
 * given BST.  If multiple values with the same key exist in the tree, this
 * function should return the first one it encounters (i.e. the one closest to
 * the root of the tree).  If the BST does not contain the specified key, this
 * function should return NULL.
 *
 * Params:
 *   bst - the BST from which a key/value pair is to be removed.  May not
 *     be NULL.
 *   key - the key of the key/value pair whose value is to be returned.
 *
 * Return:
 *   Should return the value associated with the key `key` in `bst` or NULL,
 *   if the key `key` was not found in `bst`.
 */
void *bst_get(struct bst *bst, int key)
{
  struct bst_node *next = bst->root;
  while (next != NULL)
  {
    if (next->key == key)
    {
      return next->value;
    }
    if (key < next->key)
    {
      next = next->left;
    }
    else
    {
      next = next->right;
    }
  }
  return NULL;
}

/*****************************************************************************
 **
 ** BST puzzle functions
 **
 *****************************************************************************/

/*
 * This function should return the height of a given BST, which is the maximum
 * depth of any node in the tree (i.e. the number of edges in the path from
 * the root to that node).  Note that the height of an empty tree is -1 by
 * convention.
 *
 * Params:
 *   bst - the BST whose height is to be computed
 *
 * Return:
 *   Should return the height of bst.
 */
int bst_height(struct bst *bst)
{
  struct bst_node *node = bst->root;
  if (node == NULL){
    return -1;
  }

  return node_height(node);
  
}

/*
 * This function should determine whether a specified value is a valid path
 * sum within a given BST.  In other words, this function should check whether
 * the given BST contains any path from the root to a leaf in which the keys
 * sum to the specified value.
 *
 * Params:
 *   bst - the BST whose paths sums to search
 *   sum - the value to search for among the path sums of `bst`
 *
 * Return:
 *   Should return 1 if `bst` contains any path from the root to a leaf in
 *   which the keys add up to `sum`.  Should return 0 otherwise.
 */
int has_path_sum(struct bst_node *node, int sum) {
    // Base case: if the node is NULL, no path exists
    if (node == NULL) {
        return 0;
    }

    // Check if the node is a leaf
    if (node->left == NULL && node->right == NULL) {
        return (sum == node->key);
    }

    // Recur for left and right subtrees with the reduced sum
    int remaining_sum = sum - node->key;
    return has_path_sum(node->left, remaining_sum) || has_path_sum(node->right, remaining_sum);
}

int bst_path_sum(struct bst *bst, int sum) {
    // Ensure the BST is not NULL
    if (bst == NULL || bst->root == NULL) {
        return 0;
    }

    // Call the helper function starting from the root
    return has_path_sum(bst->root, sum);
}

/*
 * This function should compute a range sum in a given BST.  Specifically, it
 * should compute the sum of all keys in the BST between a given lower bound
 * and a given upper bound.  For full credit, you should not process any subtree
 * whose keys cannot be included in the range sum.
 *
 * Params:
 *   bst - the BST within which to compute a range sum
 *   lower - the lower bound of the range over which to compute a sum; this
 *     should be considered an *inclusive* bound; in other words a key that's
 *     equal to this bound should be included in the sum
 *   upper - the upper bound of the range over which to compute a sum; this
 *     should be considered an *inclusive* bound; in other words a key that's
 *     equal to this bound should be included in the sum
 *
 * Return:
 *   Should return the sum of all keys in `bst` between `lower` and `upper`.
 */
int range_sum_helper(struct bst_node *node, int lower, int upper) {
    // Base case: if the node is NULL, return 0
    if (node == NULL) {
      return 0;
    }

    // If key less than lower, check only right
    if (node->key < lower) {
      return range_sum_helper(node->right, lower, upper);
    }

    // if key greater than upper, check only left
    if (node->key > upper) {
      return range_sum_helper(node->left, lower, upper);
    }

    // If key is within the range, include and recursively call both sides
    return node->key + range_sum_helper(node->left, lower, upper) + range_sum_helper(node->right, lower, upper);
}

int bst_range_sum(struct bst *bst, int lower, int upper) {
    if (bst == NULL || bst->root == NULL) {
        return 0; 
    }

    return range_sum_helper(bst->root, lower, upper);
}

/*****************************************************************************
 **
 ** BST iterator definition (extra credit only)
 **
 *****************************************************************************/

/*
 * Structure used to represent a binary search tree iterator.  It contains
 * only a reference to a stack to be used to implement the iterator.
 *
 * You should not modify this structure.
 */
struct bst_iterator {
  struct stack *stack;
};
  


/*
 * This function should allocate and initialize an iterator over a specified
 * BST and return a pointer to that iterator.
 *
 * Params:
 *   bst - the BST for over which to create an iterator.  May not be NULL.
 */
struct bst_iterator *bst_iterator_create(struct bst *bst)
{
  return NULL;
}

/*
 * This function should free all memory allocated to a given BST iterator.
 * It should NOT free any memory associated with the BST itself.  This is the
 * responsibility of the caller.
 *
 * Params:
 *   iter - the BST iterator to be destroyed.  May not be NULL.
 */
void bst_iterator_free(struct bst_iterator *iter)
{
  return;
}

/*
 * This function should indicate whether a given BST iterator has more nodes
 * to visit.  It should specifically return 1 (true) if the iterator has at
 * least one more node to visit or 0 (false) if it does not have any more
 * nodes to visit.
 *
 * Param:
 *   iter - the BST iterator to be checked for remaining nodes to visit.  May
 *     not be NULL.
 */
int bst_iterator_has_next(struct bst_iterator *iter)
{
  return 0;
}

/*
 * This function should return both the value and key associated with the
 * current node pointed to by the specified BST iterator and advnce the
 * iterator to point to the next node in the BST (in in-order order).
 *
 * Because a function can't return two things, the key associated with the
 * current node should be returned the normal way, while its value should be
 * returned via the argument `value`.  Specifically, the argument `value`
 * is a pointer to a void pointer.  The current BST node's value (a void
 * pointer) should be stored at the address represented by `value` (i.e. by
 * dereferencing `value`).  This will look something like this:
 *
 *   *value = current_node->value;
 *
 * Parameters:
 *   iter - BST iterator.  The key and value associated with this iterator's
 *     current node should be returned, and the iterator should be updated to
 *     point to the next node in the BST (in in-order order).  May not be NULL.
 *   value - pointer at which the current BST node's value should be stored
 *     before this function returns.
 *
 * Return:
 *   This function should return the key associated with the current BST node
 *   pointed to by `iter`.
 */
int bst_iterator_next(struct bst_iterator *iter, void **value)
{
  if (value)
  {
    *value = NULL;
  }
  return 0;
}

//Taken from Geeksforgeeks https://www.geeksforgeeks.org/deletion-in-binary-search-tree/#
// Finds leftmost leaf in right branch as successor.
struct bst_node* get_successor(struct bst_node* node) {
    struct bst_node* current = node->right;
    while (current != NULL && current->left != NULL) {
        current = current->left;
    }
    return current;
}


int node_height(struct bst_node* node){
  if (node == NULL){
    return 0;
  }
  int height_left = node_height(node->left);
  int height_right = node_height(node->right);
  if (height_left > height_right)
  {
    printf("Height: %d\n", height_left);
    return height_left + 1;
  }
  else
  {
    printf("Height: %d\n", height_right);
    return height_right + 1;
  }
}

//Debugging functions
void inorder_node(struct bst_node* root){
  if (root != NULL) {
        inorder_node(root->left);
        printf("%d ", root->key);
        inorder_node(root->right);
    }
}

void inorder(struct bst* bst) {
    inorder_node(bst->root);
    printf("\n");
}



