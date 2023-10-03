#-------------------------------------------------
#
# Project created by QtCreator 2018-08-28T10:55:17
#
#-------------------------------------------------

#QT       += core gui opengl
QT += core gui opengl openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mesh_Computational_Geometry
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    gldisplaywidget.cpp \
    mathhelper.cpp \
    mesh.cpp \
    meshhelper.cpp \
    parameters.cpp

HEADERS  += mainwindow.h \
    gldisplaywidget.h \
    mathhelper.h \
    mesh.h \
    meshhelper.h \
    parameters.h

FORMS    += mainwindow.ui

#---- Comment the following line on MacOS
#---- Uncomment it on Windows and Linux
LIBS = -lGLU

#---- Uncomment the following line on Windows
#---- Comment it on Linux and MacOS
#LIBS += -lglu32
#LIBS += -lOpengl32

