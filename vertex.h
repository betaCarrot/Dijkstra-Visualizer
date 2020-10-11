#ifndef VERTEX_H
#define VERTEX_H

#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include "impl.h"

class Graph;

class Vertex: public QLabel {
    Q_OBJECT

public:
    enum STATUS {
        DEFAULT, SELECTED, DELETED, BLUE, GREEN, RED, GRAY, CYAN
    } status;
    Vertex(Impl *h, int ind, QWidget *parent = nullptr);
    bool movable;

protected slots:
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void paintEvent(QPaintEvent*) override;

private:
    bool moving;
    QPoint startPoint, windowPoint;
    Impl *host;
    int index;
};

#endif // VERTEX_H
