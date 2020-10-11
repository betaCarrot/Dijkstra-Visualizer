#include "vertex.h"
#include "edge.h"
#include "graph.h"
#include "hashtable.cpp"

Vertex::Vertex(Impl *h, int ind, QWidget *parent) :
        QLabel(parent), host(h), index(ind) {
    status = DEFAULT;
    setGeometry(QRect(0, 0, 40, 40));
}

void Vertex::paintEvent(QPaintEvent *ev) {
    if (status == DELETED)
        return;
    QPainter painter(this);
    painter.setPen(QPen(Qt::white));
    painter.drawRect(QRect(0, 0, 40, 40));
    QPen pen;
    pen.setWidth(3);
    if (status == SELECTED)
        pen.setColor(Qt::yellow);
    else if (status == BLUE)
        pen.setColor(Qt::blue);
    else if (status == GREEN)
        pen.setColor(Qt::green);
    else if (status == RED)
        pen.setColor(Qt::red);
    else if (status == GRAY)
        pen.setColor(Qt::gray);
    else if (status == CYAN)
        pen.setColor(Qt::cyan);
    painter.setPen(pen);
    painter.setBrush(Qt::white);
    painter.drawEllipse(QRect(5, 5, 30, 30));
    if (status == BLUE || status == GREEN || status == RED || status == GRAY
            || status == CYAN) {
        int value = host->graph->dist[index];
        std::string str = std::to_string(value);
        painter.drawText(rect(), Qt::AlignCenter, str.c_str());
    }
}

void Vertex::mousePressEvent(QMouseEvent *ev) {
    if (!movable || status == DELETED)
        return;
    if (ev->button() == Qt::LeftButton) {
        moving = true;
        startPoint = ev->globalPos();
        windowPoint = frameGeometry().topLeft();
    } else if (ev->button() == Qt::RightButton) {
        status = DELETED;
        for (int i = 0; i < host->edges.size(); i++) {
            if (host->edges[i]->vertex1 == index
                    || host->edges[i]->vertex2 == index) {
                host->edges[i]->status = Edge::DELETED;
                host->edgeSet.remove(
                        host->edges[i]->vertex1 * 1007
                                + host->edges[i]->vertex2);
                host->edges[i]->myUpdate();
            }
        }
        update();
    }
}

void Vertex::mouseMoveEvent(QMouseEvent *ev) {
    if (!movable || status == DELETED)
        return;
    if (moving && ev->buttons() & Qt::LeftButton) {
        QPoint relativePos = ev->globalPos() - startPoint;
        move(windowPoint + relativePos);
        for (int i = 0; i < host->edges.size(); i++) {
            if (host->edges[i]->vertex1 == index
                    || host->edges[i]->vertex2 == index) {
                    host->edges[i]->myUpdate();
            }
        }
    }
}

void Vertex::mouseReleaseEvent(QMouseEvent *ev) {
    if (!movable || status == DELETED)
        return;
    if (ev->button() == Qt::LeftButton) {
        moving = false;
    }
}
