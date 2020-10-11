#ifndef GRAPH_H
#define GRAPH_H

#include "avl.cpp"
#include "rbtree.cpp"
#include "hashtable.h"
#include "impl.h"

using namespace std;

#define pb push_back

class Vertex;
class Edge;

class Graph {
public:
    enum STATUS {
        NEW, RUNNING, FINISHED
    } status;
    QVector<int> head, ver, edge, nxt, dist, prv;
    HashTable<int> unvisited_vertices_set, delV, delE;
    int source, x, y, i;
    Graph(Impl *h);
    void addVertex();
    void addEdge(int x, int y, int z);
    void prepare(int s);
    void step();
    void init();
private:
    // AVL<pair<int, int>> priority_queue;
    RBTree<pair<int, int>> priority_queue;
    Impl *host;
    int tot = 0;
};

#endif // GRAPH_H
