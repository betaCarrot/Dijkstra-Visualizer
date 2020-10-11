#ifndef RBTREE_H_
#define RBTREE_H_

enum RBTColor {
    RED, BLACK
};

template<class T>
class RBTNode {
public:
    RBTNode *left;
    RBTNode *right;
    RBTNode *parent;
    RBTColor color;
    T key;

    RBTNode(T value, RBTColor c = BLACK, RBTNode *p = nullptr, RBTNode *l =
            nullptr, RBTNode *r = nullptr) :
            left(l), right(r), parent(p), color(c), key(value) {
    }
};

template<class T>
class RBTree {
private:
    RBTNode<T> *root = nullptr;
    RBTNode<T>* search(RBTNode<T> *x, T key) const;
    void insert(RBTNode<T> *node);
    void insertFixUp(RBTNode<T> *node);
    void remove(RBTNode<T> *node);
    void removeFixUp(RBTNode<T> *node, RBTNode<T> *parent);
    void leftRotate(RBTNode<T> *x);
    void rightRotate(RBTNode<T> *y);

public:
    RBTree() = default;
    ~RBTree();
    bool is_empty() {
        return root == nullptr;
    }
    bool contains(T key) const;
    const T& find_min() const;
    void insert(T key);
    void remove(T key);
};

#endif // RBTREE_H
