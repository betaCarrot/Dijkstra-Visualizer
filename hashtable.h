#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <QVector>
#include <QLinkedList>
#include <algorithm>
using namespace std;
template<typename T>
class HashTable {
public:
    HashTable() = default;
    void init(int size);
    bool contains(const T &x) const;
    void add(const T &x);
    void remove(const T &x);
    void clear();

private:
    QVector<QLinkedList<T>> lists;
    int size = 0;
    void rehash();
};

#endif // HASHTABLE_H
