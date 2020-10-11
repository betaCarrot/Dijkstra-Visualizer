#ifndef EDGE_H
#define EDGE_H

#include <QLabel>
#include <QPainter>
#include "impl.h"

class Edge: public QLabel {
    Q_OBJECT
public:
    enum STATUS {
        DEFAULT, DELETED, PASSED
    } status;
    Edge(int v1, int v2, int w, Impl *h, QWidget *parent = nullptr);
    int vertex1, vertex2, weight;
    bool movable;
    bool updated = false;
    void myUpdate();
protected slots:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *ev) override;
private:
    Impl *host;
};

#endif // EDGE_H
