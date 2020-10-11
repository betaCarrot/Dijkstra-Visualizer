#ifndef IMPL_H
#define IMPL_H

#include <QWidget>
#include "hashtable.h"
class Vertex;
class Edge;
class Graph;

class Impl {
public:
    Impl();
    ~Impl();
    QVector<Vertex*> vertices;
    QVector<Edge*> edges;
    void addVertex(QWidget *parent);
    void addEdge(int vertex1, int vertex2, int weight, QWidget *parent);
    void run(int source);
    bool show(int source, int to);
    void step();
    bool checkFinished();
    Graph *graph;
    HashTable<int> edgeSet;
    void move_vertices();
    void freeze_vertices();
    void move_edges();
    void freeze_edges();
    void raise_vertices();
    void lower_vertices();
    void raise_edges();
    void lower_edges();
    void reset_vertices();
    void reset_edges();
    void repaint_edges();
};

#endif // IMPL_H
