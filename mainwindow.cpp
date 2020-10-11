#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "vertex.h"
#include "edge.h"
#include "hashtable.cpp"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    idle_reset();
}

MainWindow::~MainWindow() {
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *ev) {
    if (ev->type() == QEvent::MouseButtonRelease && status == SHOWING) {
        QMouseEvent *e = static_cast<QMouseEvent*>(ev);
        if (e->button() == Qt::LeftButton) {
            for (int i = 0; i < impl.vertices.size(); i++) {
                if (obj == impl.vertices[i]) {
                    if (impl.vertices[i]->status == Vertex::DELETED)
                        return false;
                    if (!impl.show(source, i)) {
                        QMessageBox msgBox;
                        msgBox.setText("Path does not exist!");
                        msgBox.exec();
                    }
                }
            }
        }
    }
    if (ev->type() == QEvent::MouseButtonPress) {
        QMouseEvent *e = static_cast<QMouseEvent*>(ev);
        if (status == VERTEX) {
            if (e->button() == Qt::LeftButton) {
                impl.addVertex(ui->graphicsView);
                impl.vertices[impl.vertices.size() - 1]->move(
                        e->pos() - QPoint(20, 20));
            }
        } else if (status == EDGE) {
            if (e->button() == Qt::LeftButton) {
                for (int i = 0; i < impl.vertices.size(); i++) {
                    if (obj == impl.vertices[i]) {
                        if (impl.vertices[i]->status == Vertex::DELETED)
                            return false;
                        impl.vertices[i]->status = Vertex::SELECTED;
                        impl.vertices[i]->update();
                        if (vertex1 == -1) {
                            vertex1 = i;
                        } else {
                            if (vertex1 == i)
                                return false;
                            int edgeDeleted = 0;
                            for (Edge *e : impl.edges) {
                                if (e->status == Edge::DELETED)
                                    edgeDeleted++;
                            }
                            if (impl.edges.size() - edgeDeleted >= 500) {
                                QMessageBox msgBox;
                                msgBox.setText(
                                        "Edge number greater than 500. Out of Memory >.<");
                                msgBox.exec();
                                impl.vertices[i]->status = Vertex::DEFAULT;
                                impl.vertices[vertex1]->status =
                                        Vertex::DEFAULT;
                                vertex1 = -1;
                                return false;
                            }
                            bool ok = false;
                            int weight = QInputDialog::getInt(this,
                                    "Input Dialog", "Input Weight", 1, 1, 256,
                                    1, &ok);
                            if (!ok) {
                                impl.vertices[i]->status = Vertex::DEFAULT;
                                impl.vertices[vertex1]->status =
                                        Vertex::DEFAULT;
                                vertex1 = -1;
                                return false;
                            }
                            vertex2 = i;
                            impl.addEdge(vertex1, vertex2, weight,
                                    ui->graphicsView);
                            vertex1 = -1;
                            vertex2 = -1;
                        }
                        return false;
                    }
                }
            }
        } else if (status == PREPARING) {
            if (e->button() == Qt::LeftButton) {
                for (int i = 0; i < impl.vertices.size(); i++) {
                    if (obj == impl.vertices[i]) {
                        if (impl.vertices[i]->status == Vertex::DELETED)
                            return false;
                        source = i;
                        impl.run(i);
                        impl.step();
                        ui->vertexButton->setCheckable(false);
                        ui->edgeButton->setCheckable(false);
                        ui->startButton->setCheckable(false);
                        status = RUNNING;
                    }
                }
            }
        } else if (status == RUNNING) {
            if (e->button() == Qt::RightButton) {
                return true;
            }
        } else if (status == FINISHED) {
            if (e->button() == Qt::RightButton) {
                return true;
            }
        } else if (status == SHOWING) {
            if (e->button() == Qt::LeftButton) {
                for (int i = 0; i < impl.vertices.size(); i++) {
                    if (obj == impl.vertices[i]) {
                        if (impl.vertices[i]->status == Vertex::DELETED)
                            return false;
                        if (impl.show(source, i))
                            impl.vertices[i]->raise();
                    }
                }
            } else if (e->button() == Qt::RightButton) {
                return true;
            }
        } else {
            if (e->button() == Qt::RightButton) {
                for (int i = 0; i < impl.edges.size(); i++) {
                    if (obj == impl.edges[i]) {
                        if (impl.edges[i]->status == Edge::DELETED)
                            return false;
                        impl.edges[i]->lower();
                    }
                }
                for (int i = 0; i < impl.vertices.size(); i++) {
                    if (obj == impl.vertices[i]) {
                        if (impl.vertices[i]->status == Vertex::DELETED)
                            return false;
                        impl.vertices[i]->lower();
                        for (Edge *e : impl.edges) {
                            if (e->status == Edge::DELETED)
                                continue;
                        }
                    }
                }
            }
        }
    }
    return false;
}

void MainWindow::on_vertexButton_toggled(bool checked) {
    if (status == RUNNING)
        return;
    if (checked) {
        active_reset();
        ui->graphicsView->viewport()->installEventFilter(this);
        ui->graphicsView->viewport()->raise();
        status = VERTEX;
    } else {
        idle_reset();
    }
}

void MainWindow::on_edgeButton_toggled(bool checked) {
    if (checked) {
        active_reset();
        install_vertex_event_filter();
        status = EDGE;
    } else {
        if (status == RUNNING)
            return;
        idle_reset();
    }
}

void MainWindow::on_generateButton_pressed() {
    if (status != IDLE && status != VERTEX && status != EDGE
            && status != PREPARING)
        return;
    int vertexDeleted = 0;
    for (Vertex *v : impl.vertices) {
        if (v->status == Vertex::DELETED)
            vertexDeleted++;
    }
    int edgeDeleted = 0;
    for (Edge *e : impl.edges) {
        if (e->status == Edge::DELETED)
            edgeDeleted++;
    }
    active_reset();
    bool ok = false;
    int vertexNumber = QInputDialog::getInt(this, "Input Dialog",
            "Number of New Vertices", 0, 0, 32, 1, &ok);
    if (!ok) {
        idle_reset();
        return;
    }
    int totalVertices = vertexNumber + impl.vertices.size() - vertexDeleted;
    int maxEdges = totalVertices * (totalVertices - 1) / 2;
    double minDensity = (impl.edges.size() - edgeDeleted + 0.0) / maxEdges;
    if (maxEdges == 0)
        minDensity = 0;
    double percent = QInputDialog::getDouble(this, "Input Dialog",
            "Density of All Edges", minDensity, minDensity, 1, 2, &ok,
            Qt::WindowFlags(), 0.01);
    if (!ok) {
        idle_reset();
        return;
    }
    int edgeNumber = static_cast<int>(maxEdges * percent + 1e-6)
            - impl.edges.size() + edgeDeleted;
    if (edgeNumber + impl.edges.size() - edgeDeleted > 500) {
        QMessageBox msgBox;
        msgBox.setText("Edge number greater than 500. Out of Memory >.<");
        msgBox.exec();
        idle_reset();
        return;
    }
    int maxEdgeWeight = QInputDialog::getInt(this, "Input Dialog",
            "Maximum Weight of New Edges", 1, 1, 256, 1, &ok);
    if (!ok) {
        idle_reset();
        return;
    }
    srand (time(NULL));for
(	int i=0;i<vertexNumber;i++) {
        impl.addVertex(ui->graphicsView);
        impl.vertices[impl.vertices.size() - 1]->move(
        rand()%(ui->graphicsView->width()-40),rand()%(ui->graphicsView->height()-40));
    }
    for (int i = 0; i < edgeNumber; i++) {
        int a = rand() % impl.vertices.size();
        int b = rand() % impl.vertices.size();
        while (a == b || impl.vertices[a]->status == Vertex::DELETED
                || impl.vertices[b]->status == Vertex::DELETED
                || impl.edgeSet.contains(a * 1007 + b)
                || impl.edgeSet.contains(b * 1007 + a)) {
            a = rand() % impl.vertices.size();
            b = rand() % impl.vertices.size();
        }
        impl.addEdge(a, b, rand() % maxEdgeWeight + 1, ui->graphicsView);
    }
    idle_reset();
}

void MainWindow::on_startButton_toggled(bool checked) {
    if (status == FINISHED)
        return;
    if (checked) {
        active_reset();
        install_vertex_event_filter();
        impl.move_vertices();
        impl.move_edges();
        impl.raise_edges();
        impl.raise_vertices();
        status = PREPARING;
    } else {
        idle_reset();
    }
}

void MainWindow::on_stepButton_pressed() {
    if (status != RUNNING)
        return;
    if (impl.checkFinished()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Finished");
        msgBox.setText("The algorithm has finished");
        msgBox.exec();
        status = FINISHED;
        ui->pathButton->setCheckable(true);
        return;
    }
    impl.step();
}

void MainWindow::on_clearButton_pressed() {
    if (status != PREPARING && status != RUNNING && status != FINISHED
            && status != SHOWING)
        return;
    if (status == PREPARING) {
        ui->startButton->toggle();
    }
    if (status == SHOWING) {
        ui->pathButton->toggle();
    }
    for (Edge *e : impl.edges) {
        if (e->status == Edge::PASSED) {
            e->status = Edge::DEFAULT;
            e->myUpdate();
        }
    }
    idle_reset();
    source = -1;
}

void MainWindow::on_pathButton_toggled(bool checked) {
    if (checked) {
        if (status != FINISHED)
            return;
        status = SHOWING;
    } else {
        impl.show(source, source);
        status = FINISHED;
    }
}

void MainWindow::active_reset() {
    if (status == VERTEX)
        ui->vertexButton->setChecked(false);
    if (status == EDGE)
        ui->edgeButton->setChecked(false);
    if (status == PREPARING)
        ui->startButton->setChecked(false);
    ui->graphicsView->viewport()->removeEventFilter(this);
    remove_vertex_event_filter();
    remove_edge_event_filter();
    impl.freeze_vertices();
    impl.freeze_edges();
    impl.reset_vertices();
    impl.reset_edges();
    vertex1 = -1;
    vertex2 = -1;
}

void MainWindow::idle_reset() {
    install_vertex_event_filter();
    install_edge_event_filter();
    impl.move_vertices();
    impl.move_edges();
    impl.reset_vertices();
    impl.reset_edges();
    impl.raise_edges();
    impl.raise_vertices();
    vertex1 = -1;
    vertex2 = -1;
    ui->startButton->setCheckable(true);
    ui->vertexButton->setCheckable(true);
    ui->edgeButton->setCheckable(true);
    ui->pathButton->setCheckable(false);
    status = IDLE;
}

void MainWindow::install_vertex_event_filter() {
    for (Vertex *v : impl.vertices) {
        if (v->status == Vertex::DELETED)
            continue;
        v->installEventFilter(this);
    }
}

void MainWindow::remove_vertex_event_filter() {
    for (Vertex *v : impl.vertices) {
        if (v->status == Vertex::DELETED)
            continue;
        v->removeEventFilter(this);
    }
}

void MainWindow::install_edge_event_filter() {
    for (Edge *e : impl.edges) {
        if (e->status == Edge::DELETED)
            continue;
        e->installEventFilter(this);
    }
}

void MainWindow::remove_edge_event_filter() {
    for (Edge *e : impl.edges) {
        if (e->status == Edge::DELETED)
            continue;
        e->removeEventFilter(this);
    }
}
