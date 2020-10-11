#include "impl.h"
#include "vertex.h"
#include "edge.h"
#include "graph.h"
#include "hashtable.cpp"

Impl::Impl() {
    graph = new Graph(this);
    edgeSet.init(10000);
}

Impl::~Impl() {
    for (Vertex *v : vertices) {
        delete v;
    }
    for (Edge *e : edges) {
        delete e;
    }
    delete graph;
}

void Impl::addVertex(QWidget *parent) {
    Vertex *vertex = new Vertex(this, vertices.size(), parent);
    vertex->show();
    vertices.push_back(vertex);
    graph->addVertex();
}

void Impl::addEdge(int vertex1, int vertex2, int weight, QWidget *parent) {
    if (vertex1 == vertex2)
        return;
    edgeSet.add(vertex1 * 1007 + vertex2);
    Edge *edge = new Edge(vertex1, vertex2, weight, this, parent);
    edge->show();
    edge->myUpdate();
    edges.push_back(edge);
    vertices[vertex1]->status = Vertex::DEFAULT;
    vertices[vertex2]->status = Vertex::DEFAULT;
    for (int i = 0; i < vertices.size(); i++) {
        vertices[i]->raise();
    }
    graph->addEdge(vertex1, vertex2, weight);
    graph->addEdge(vertex2, vertex1, weight);
}

void Impl::run(int source) {
    graph->init();
    for (int i = 0; i < vertices.size(); i++) {
        if (vertices[i]->status == Vertex::DELETED) {
            graph->delV.add(i);
        } else {
            vertices[i]->status = Vertex::DEFAULT;
        }
    }
    for (int i = 0; i < edges.size(); i++) {
        if (edges[i]->status == Edge::DELETED) {
            graph->delE.add(i * 2 + 1);
            graph->delE.add(i * 2 + 2);
        }
    }
    graph->prepare(source);
}

void Impl::step() {
    graph->step();
}

bool Impl::checkFinished() {
    return graph->status == Graph::FINISHED;
}

bool Impl::show(int source, int to) {
    if (vertices[to]->status != Vertex::GRAY) {
        return false;
    }
    reset_edges();
    int nxt;
    for (int curr = to; curr != source; curr = nxt) {
        nxt = graph->prv[curr];
        for (int i = 0; i < edges.size(); i++) {
            if (edges[i]->status == Edge::DELETED)
                continue;
            if ((edges[i]->vertex1 == curr && edges[i]->vertex2 == nxt)
                    || (edges[i]->vertex2 == curr && edges[i]->vertex1 == nxt)) {
                edges[i]->status = Edge::PASSED;
                edges[i]->raise();
                edges[i]->myUpdate();
            }
        }
    }
    raise_vertices();
    return true;
}

void Impl::move_vertices() {
    for (Vertex *v : vertices) {
        if (v->status == Vertex::DELETED)
            continue;
        v->movable = true;
    }
}

void Impl::freeze_vertices() {
    for (Vertex *v : vertices) {
        if (v->status == Vertex::DELETED)
            continue;
        v->movable = false;
    }
}

void Impl::raise_vertices() {
    for (Vertex *v : vertices) {
        if (v->status == Vertex::DELETED)
            continue;
        v->raise();
    }
}

void Impl::lower_vertices() {
    for (Vertex *v : vertices) {
        if (v->status == Vertex::DELETED)
            continue;
        v->lower();
    }
}

void Impl::reset_vertices() {
    for (Vertex *v : vertices) {
        if (v->status == Vertex::DELETED)
            continue;
        v->status = Vertex::DEFAULT;
        v->update();
    }
}

void Impl::move_edges() {
    for (Edge *e : edges) {
        if (e->status == Edge::DELETED)
            continue;
        e->movable = true;
    }
}

void Impl::freeze_edges() {
    for (Edge *e : edges) {
        if (e->status == Edge::DELETED)
            continue;
        e->movable = false;
    }
}

void Impl::raise_edges() {
    for (Edge *e : edges) {
        if (e->status == Edge::DELETED)
            continue;
        e->raise();
    }
}

void Impl::lower_edges() {
    for (Edge *e : edges) {
        if (e->status == Edge::DELETED)
            continue;
        e->lower();
    }
}

void Impl::reset_edges() {
    for (Edge *e : edges) {
        if (e->status == Edge::DELETED)
            continue;
        e->status = Edge::DEFAULT;
        e->myUpdate();
    }
}
