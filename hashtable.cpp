#include "hashtable.h"

template<typename T>
void HashTable<T>::init(int size) {
    for (int i = 0; i < size; i++) {
        lists.push_back(QLinkedList<T>());
    }
}

template<typename T>
bool HashTable<T>::contains(const T &x) const {
    return find(lists[x % lists.size()].begin(), lists[x % lists.size()].end(),
            x) != lists[x % lists.size()].end();
}

template<typename T>
void HashTable<T>::add(const T &x) {
    lists[x % lists.size()].clear();
    if (contains(x)) {
        return;
    }
    lists[x % lists.size()].push_back(x);
    if (++size > lists.size())
        rehash();
}

template<typename T>
void HashTable<T>::remove(const T &x) {
    if (!contains(x))
        return;
    lists[x % lists.size()].erase(
            find(lists[x % lists.size()].begin(), lists[x % lists.size()].end(),
                    x));
    size--;
}

template<typename T>
void HashTable<T>::clear() {
    for (QLinkedList<T> list : lists) {
        list.clear();
    }
}

template<typename T>
void HashTable<T>::rehash() {
    QVector < QLinkedList < T >> temp = lists;
    lists.resize(2 * lists.size());
    clear();
    size = 0;
    for (QLinkedList<T> list : temp) {
        for (T item : list) {
            add(item);
        }
    }
}
