#ifndef GLDISPLAYWIDGET_H
#define GLDISPLAYWIDGET_H

#include <QOpenGLWidget> // Module QtOpengL (classes QGLxxx in Qt4),
                         // with widget and rendering classes descending from QGLWidget,
                         // deprecated for module QtGui (classes QOpenGLxxx )
                         // and module widgets (QWidget, QGraphicsView).

#include <QtWidgets>
#include <QTimer>
#include "mesh.h"  // Model
#include "parameters.h"

class GLDisplayWidget : public QOpenGLWidget
{
public:
    explicit GLDisplayWidget(QWidget *parent = 0);

    void initializeGL(); // The scene may be initialized in this function since the GeometricWorld is a data member...
    void paintGL(); // Display the scene Gl
    void resizeGL(int width, int height);

    GeometricWorld _geomWorld; // The scene to be displayed

    Parameters param = Parameters(false, true);

protected:
    // Mouse Management
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:

    QTimer _timer; // To update the scene
    float _X = 0.f, _Y = 0.f ,_Z = 0.f; // Translation
    float _angleX; // Rotation
    float _angleY = 0;

    QPoint _lastPosMouse; // To keep the last position of the mouse
};

#endif // GLDISPLAYWIDGET_H
