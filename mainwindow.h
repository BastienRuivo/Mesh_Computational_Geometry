#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "meshhelper.h"
#include <chrono>
#include <sstream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_Tetra_clicked();

    void on_SaveOff_clicked();

    void on_Wireframe_toggled(bool checked);

    void on_Axis_toggled(bool checked);

    void on_Pyramid_clicked();

    void on_Queen_clicked();

    void on_Cube_clicked();

    void on_Wireframe_2_toggled(bool checked);

    void on_CircularValue_valueChanged(int arg1);

    void on_Circular_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
