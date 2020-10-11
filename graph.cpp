#include "graph.h"
#include "vertex.h"
#include "edge.h"
#include "hashtable.cpp"

Graph::Graph(Impl *h) :
        host(h) {
    ver.pb(0), edge.pb(0), nxt.pb(0);
    unvisited_vertices_set.init(100);
    delV.init(100);
    delE.init(100);
}

void Graph::addVertex() {
    head.pb(0), dist.pb(0), prv.pb(0);
}

void Graph::addEdge(int x, int y, int z) {
    ver.pb(y), edge.pb(z), nxt.pb(head[x]), head[x] = ++tot;
}

void Graph::init() {
    fill(dist.begin(), dist.end(), 0x3f3f3f3f);
    unvisited_vertices_set.clear();
    for (int i = 0; i < ver.size(); i++) {
        unvisited_vertices_set.add(ver[i]);
    }
    fill(prv.begin(), prv.end(), -1);
}

void Graph::step() {
    for (Edge *e : host->edges) {
        if (e->status == Edge::PASSED) {
            e->status = Edge::DEFAULT;
            e->myUpdate();
        }
    }
    bool ok = false;
    while (!ok) {
        if (status == NEW) {
            if (x != -1) {
                host->vertices[x]->status = Vertex::GRAY;
                host->vertices[x]->update();
                x = -1;
            }
            if (y != -1) {
                host->vertices[y]->status = Vertex::CYAN;
                host->vertices[y]->update();
                y = -1;
            }
            if (priority_queue.is_empty()) {
                status = FINISHED;
                return;
            }
            x = priority_queue.find_min().second;
            priority_queue.remove(priority_queue.find_min());
            if (!unvisited_vertices_set.contains(x) || delV.contains(x))
                continue;
            unvisited_vertices_set.remove(x);
            ok = true;
            host->vertices[x]->status = Vertex::BLUE;
            host->vertices[x]->raise();
            host->vertices[x]->update();
            i = head[x];
            if (i) {
                status = RUNNING;
                y = -1;
            }
        } else {
            if (y != -1) {
                host->vertices[y]->status = Vertex::CYAN;
                host->vertices[y]->update();
                y = -1;
            }
            if (delV.contains(ver[i]) || delE.contains(i)
                    || !unvisited_vertices_set.contains(ver[i])) {
                i = nxt[i];
                if (!i) {
                    y = -1;
                    status = NEW;
                }
                continue;
            }
            y = ver[i];
            int z = edge[i];
            for (Edge *e : host->edges) {
                if (e->status == Edge::DELETED)
                    continue;
                if ((e->vertex1 == x && e->vertex2 == y)
                        || (e->vertex2 == x && e->vertex1 == y)) {
                    e->status = Edge::PASSED;
                    e->myUpdate();
                    e->raise();
                }
            }
            host->raise_vertices();
            ok = true;
            if (dist[y] > dist[x] + z) {
                dist[y] = dist[x] + z;
                prv[y] = x;
                host->vertices[y]->status = Vertex::GREEN;
                host->vertices[y]->raise();
                host->vertices[y]->update();
                priority_queue.insert(make_pair(dist[y], y));
            } else {
                host->vertices[y]->status = Vertex::RED;
                host->vertices[y]->raise();
                host->vertices[y]->update();
            }
            i = nxt[i];
            if (!i) {
                status = NEW;
            }
        }
    }
}

void Graph::prepare(int s) {
    source = s;
    dist[source] = 0;
    while (!priority_queue.is_empty()) {
        priority_queue.remove(priority_queue.find_min());
    }
    priority_queue.insert(make_pair(0, source));
    status = NEW;
    x = -1;
    y = -1;
}
