#include "rbtree.h"

template<class T>
RBTree<T>::~RBTree() {
    if (root == nullptr)
        return;
    if (root->left != nullptr)
        delete root->left;
    if (root->right != nullptr)
        delete root->right;
    delete root;
}

template<class T>
bool RBTree<T>::contains(T key) const {
    return search(root, key) == nullptr;
}

template<class T>
const T& RBTree<T>::find_min() const {
    RBTNode<T> *p = root;
    while (p->left != nullptr)
        p = p->left;
    return p->key;
}

template<class T>
void RBTree<T>::insert(T key) {
    RBTNode<T> *z = nullptr;
    if ((z = new RBTNode<T>(key)) == nullptr)
        return;
    insert(z);
}

template<class T>
void RBTree<T>::remove(T key) {
    RBTNode<T> *node;
    if ((node = search(root, key)) != nullptr)
        remove(node);
}

template<class T>
RBTNode<T>* RBTree<T>::search(RBTNode<T> *x, T key) const {
    if (x == nullptr || x->key == key)
        return x;
    if (key < x->key)
        return search(x->left, key);
    else
        return search(x->right, key);
}

template<class T>
void RBTree<T>::insert(RBTNode<T> *node) {
    RBTNode<T> *y = nullptr;
    RBTNode<T> *x = root;
    while (x != nullptr) {
        y = x;
        if (node->key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    node->parent = y;
    if (y != nullptr) {
        if (node->key < y->key)
            y->left = node;
        else
            y->right = node;
    } else
        root = node;
    node->color = RED;
    insertFixUp(node);
}

template<class T>
void RBTree<T>::insertFixUp(RBTNode<T> *node) {
    RBTNode<T> *parent, *gparent;
    while ((parent = node->parent) && parent->color == RED) {
        gparent = parent->parent;
        if (parent == gparent->left) {
            {
                RBTNode<T> *uncle = gparent->right;
                if (uncle && uncle->color == RED) {
                    uncle->color = BLACK;
                    parent->color = BLACK;
                    gparent->color = RED;
                    node = gparent;
                    continue;
                }
            }
            if (parent->right == node) {
                RBTNode<T> *tmp;
                leftRotate(parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }
            parent->color = BLACK;
            gparent->color = RED;
            rightRotate(gparent);
        } else {
            RBTNode<T> *uncle = gparent->left;
            if (uncle && uncle->color == RED) {
                uncle->color = BLACK;
                parent->color = BLACK;
                gparent->color = RED;
                node = gparent;
                continue;
            }
            if (parent->left == node) {
                RBTNode<T> *tmp;
                rightRotate(parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }
            parent->color = BLACK;
            gparent->color = RED;
            leftRotate(gparent);
        }
    }
    root->color = BLACK;
}

template<class T>
void RBTree<T>::remove(RBTNode<T> *node) {
    RBTNode<T> *child, *parent;
    RBTColor color;
    if ((node->left != nullptr) && (node->right != nullptr)) {
        RBTNode<T> *replace = node;
        replace = replace->right;
        while (replace->left != nullptr)
            replace = replace->left;
        if (node->parent) {
            if (node->parent->left == node)
                node->parent->left = replace;
            else
                node->parent->right = replace;
        } else
            root = replace;
        child = replace->right;
        parent = replace->parent;
        color = replace->color;
        if (parent == node) {
            parent = replace;
        } else {
            if (child)
                child->parent = parent;
            parent->left = child;

            replace->right = node->right;
            node->right->parent = replace;
        }
        replace->parent = node->parent;
        replace->color = node->color;
        replace->left = node->left;
        node->left->parent = replace;
        if (color == BLACK)
            removeFixUp(child, parent);
        delete node;
        return;
    }
    if (node->left != nullptr)
        child = node->left;
    else
        child = node->right;
    parent = node->parent;
    color = node->color;
    if (child)
        child->parent = parent;
    if (parent) {
        if (parent->left == node)
            parent->left = child;
        else
            parent->right = child;
    } else
        root = child;
    if (color == BLACK)
        removeFixUp(child, parent);
    delete node;
}

template<class T>
void RBTree<T>::removeFixUp(RBTNode<T> *node, RBTNode<T> *parent) {
    RBTNode<T> *other;
    while ((!node || node->color == BLACK) && node != root) {
        if (parent->left == node) {
            other = parent->right;
            if (other->color == RED) {
                other->color = BLACK;
                parent->color = RED;
                leftRotate(parent);
                other = parent->right;
            }
            if ((!other->left || other->left->color == BLACK)
                    && (!other->right || other->right->color == BLACK)) {
                other->color = RED;
                node = parent;
                parent = node->parent;
            } else {
                if (!other->right || other->right->color == BLACK) {
                    other->left->color = BLACK;
                    other->color = RED;
                    rightRotate(other);
                    other = parent->right;
                }
                other->color = parent->color;
                parent->color = BLACK;
                other->right->color = BLACK;
                leftRotate(parent);
                node = root;
                break;
            }
        } else {
            other = parent->left;
            if (other->color == RED) {
                other->color = BLACK;
                parent->color = RED;
                rightRotate(parent);
                other = parent->left;
            }
            if ((!other->left || other->left->color == BLACK)
                    && (!other->right || other->right->color == BLACK)) {
                other->color = RED;
                node = parent;
                parent = node->parent;
            } else {
                if (!other->left || other->left->color == BLACK) {
                    other->right->color = BLACK;
                    other->color = RED;
                    leftRotate(other);
                    other = parent->left;
                }
                other->color = parent->color;
                parent->color = BLACK;
                other->left->color = BLACK;
                rightRotate(parent);
                node = root;
                break;
            }
        }
    }
    if (node)
        node->color = BLACK;
}

template<class T>
void RBTree<T>::leftRotate(RBTNode<T> *x) {
    RBTNode<T> *y = x->right;
    x->right = y->left;
    if (y->left != nullptr)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr) {
        root = y;
    } else {
        if (x->parent->left == x)
            x->parent->left = y;
        else
            x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

template<class T>
void RBTree<T>::rightRotate(RBTNode<T> *y) {
    RBTNode<T> *x = y->left;
    y->left = x->right;
    if (x->right != nullptr)
        x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == nullptr) {
        root = x;
    } else {
        if (y == y->parent->right)
            y->parent->right = x;
        else
            y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}
