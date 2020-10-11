#include "edge.h"
#include "vertex.h"
#include <QImage>
#include <QBitmap>
#include <cmath>
#include "hashtable.cpp"

Edge::Edge(int v1, int v2, int w, Impl *h, QWidget *parent) :
        QLabel(parent), vertex1(v1), vertex2(v2), weight(w), host(h) {
    status = DEFAULT;
    movable = false;
    setGeometry(parent->rect());
}

void Edge::myUpdate() {
    updated = false;
    update();
}

void Edge::mousePressEvent(QMouseEvent *ev) {
    if (movable && ev->button() == Qt::RightButton) {
        if (status == DELETED)
            return;
        status = DELETED;
        update();
        host->edgeSet.remove(vertex1 * 1007 + vertex2);
    }
}

void Edge::paintEvent(QPaintEvent *ev) {
    if (status == DELETED || host->vertices[vertex1]->status == Vertex::DELETED
            || host->vertices[vertex2]->status == Vertex::DELETED)
        return;
    QLabel::paintEvent(ev);
    if (updated) {
        return;
    }
    updated = true;
    QImage img(this->rect().width(), this->rect().height(),
            QImage::Format_RGBA8888);
    QPainter painter(&img);
    QPen pen(Qt::SolidPattern, 3);
    if (status == PASSED) {
        pen.setColor(Qt::blue);
    }
    painter.setPen(pen);
    QPoint p1 = host->vertices[vertex1]->pos() + QPoint(20, 20);
    QPoint p2 = host->vertices[vertex2]->pos() + QPoint(20, 20);
    double x1 = p1.x();
    double y1 = p1.y();
    double x2 = p2.x();
    double y2 = p2.y();
    double dx = (x2 - x1);
    double dy = (y2 - y1);
    double hypo = sqrt(dx * dx + dy * dy);
    int dist = 20;
    QRectF rec(x1 + dist * dy / hypo - dist, y1 - dist * dx / hypo - dist,
            dx + 2 * dist, dy + 2 * dist);
    QLine line(p1, p2);
    painter.drawLine(p1, p2);
    pen.setColor(Qt::black);
    painter.setPen(pen);
    std::string str = std::to_string(weight);
    painter.drawText(rec, Qt::AlignCenter, str.c_str());
    setPixmap(QPixmap::fromImage(img));
    setMask(QPixmap::fromImage(img).mask());
}
