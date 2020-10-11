#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include <QMessageBox>
#include "impl.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow: public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_vertexButton_toggled(bool checked);

    void on_edgeButton_toggled(bool checked);

    void on_startButton_toggled(bool checked);

    void on_clearButton_pressed();

    void on_pathButton_toggled(bool checked);

    void on_generateButton_pressed();

    void on_stepButton_pressed();

protected:
    bool eventFilter(QObject *obj, QEvent *ev);

private:
    enum status {
        IDLE, VERTEX, EDGE, PREPARING, RUNNING, FINISHED, SHOWING
    } status;

    Ui::MainWindow *ui;
    Impl impl;
    int vertex1 = -1, vertex2 = -1;
    int source = -1;
    void active_reset();
    void idle_reset();
    void install_vertex_event_filter();
    void remove_vertex_event_filter();
    void install_edge_event_filter();
    void remove_edge_event_filter();
};
#endif // MAINWINDOW_H
