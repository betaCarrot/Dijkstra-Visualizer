/* Goal: To balance an AVL tree */
#include "avl.h"
template<typename T>
void AVL<T>::balance() {
    if (is_empty())
        return;

    fix_height();
    int balance_factor = bfactor();

    if (balance_factor == 2)       // Right subtree is taller by 2
            {
        if (right_subtree().bfactor() < 0) // Case 4: insertion to the L of RT
            right_subtree().rotate_right();
        return rotate_left();      // Cases 1 or 4: Insertion to the R/L of RT
    } else if (balance_factor == -2) // Left subtree is taller by 2
            {
        if (left_subtree().bfactor() > 0) // Case 3: insertion to the R of LT
            left_subtree().rotate_left();
        return rotate_right();     // Cases 2 or 3: insertion to the L/R of LT
    }
    // Balancing is not required for the remaining cases
}
/* balance factor = height of right sub-tree - height of left sub-tree */
template<typename T>
int AVL<T>::bfactor() const {
    return is_empty() ? 0 : right_subtree().height() - left_subtree().height();
}
// Goal: To search for an item x in an AVL tree
// Return: (bool) true if found, otherwise false
template<typename T>
bool AVL<T>::contains(const T &x) const {
    if (is_empty())               // Base case #1
        return false;

    else if (x == root->value) // Base case #2
        return true;

    else if (x < root->value)   // Recursion on the left subtree
        return left_subtree().contains(x);

    else
        // Recursion on the right subtree
        return right_subtree().contains(x);
}
/* To find the minimum value stored in an AVL tree. */

template<typename T>
const T& AVL<T>::find_min() const {
    // It is assumed that the calling tree is not empty
    const AVL &left_avl = left_subtree();

    if (left_avl.is_empty())    // Base case: Found!
        return root->value;

    return left_avl.find_min(); // Recursion on the left subtree
}
/* Goal: To rectify the height values of each AVL node */
template<typename T>
void AVL<T>::fix_height() const {
    if (!is_empty()) {
        int left_avl_height = left_subtree().height();
        int right_avl_height = right_subtree().height();
        root->height = 1 + std::max(left_avl_height, right_avl_height);
    }
}
/* To find the height of an AVL tree */
template<typename T>
int AVL<T>::height() const {
    return is_empty() ? -1 : root->height;
}
/* To insert an item x to AVL tree and keep the tree balanced */

template<typename T>
void AVL<T>::insert(const T &x) {
    if (is_empty())                // Base case
        root = new AVLnode(x);

    else if (x < root->value)
        left_subtree().insert(x);  // Recursion on the left sub-tree

    else if (x > root->value)
        right_subtree().insert(x); // Recursion on the left sub-tree

    balance(); // Re-balance the tree at every visited node
}
/* Goal: To print an AVL
 * Remark: The output is the AVL rotated -90 degrees
 */

template<typename T> /* File: avl-print.cpp */
void AVL<T>::print(int depth) const {
    if (is_empty())                 // Base case
        return;

    root->right.print(depth + 1);     // Recursion: right sub-tree

    for (int j = 0; j < depth; j++) // Print the node value
        std::cout << '\t';
    std::cout << root->value.first << " " << root->value.second << std::endl;

    root->left.print(depth + 1);      // Recursion: left sub-tree
}
/* To remove an item x in AVL tree and keep the tree balanced */

template<typename T>
void AVL<T>::remove(const T &x) {
    if (is_empty())                // Item is not found; do nothing
        return;

    if (x < root->value)
        left_subtree().remove(x);  // Recursion on the left sub-tree

    else if (x > root->value)
        right_subtree().remove(x); // Recursion on the right sub-tree

    else {
        AVL &left_avl = left_subtree();
        AVL &right_avl = right_subtree();

        // Found node has 2 children
        if (!left_avl.is_empty() && !right_avl.is_empty()) {
            root->value = right_avl.find_min(); // Copy the min value
            right_avl.remove(root->value); // Remove node with min value
        }

        else  // Found node has 0 or 1 child
        {
            AVLnode *node_to_remove = root; // Save the node first
            *this = left_avl.is_empty() ? right_avl : left_avl;

            // Reset the node to be removed with empty children
            right_avl.root = left_avl.root = nullptr;
            delete node_to_remove;
        }
    }

    balance(); // Re-balance the tree at every visited node
}
/* Goal: To perform a single left (anti-clocwise) rotation */
template<typename T>
void AVL<T>::rotate_left() // The calling AVL node is node a
{
    AVLnode *b = right_subtree().root; // Points to node b
    right_subtree() = b->left;
    b->left = *this;    // Note: *this is node a
    fix_height();       // Fix the height of node a
    this->root = b;     // Node b becomes the new root
    fix_height();       // Fix the height of node b, now the new root
}
/* Goal: To perform right (clockwise) rotation */
template<typename T>
void AVL<T>::rotate_right() // The calling AVL node is node a
{
    AVLnode *b = left_subtree().root; // Points to node b
    left_subtree() = b->right;
    b->right = *this;   // Note: *this is node a
    fix_height();       // Fix the height of node a
    this->root = b;     // Node b becomes the new root
    fix_height();       // Fix the height of node b, now the new root
}
