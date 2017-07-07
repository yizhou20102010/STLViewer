#-------------------------------------------------
#
# Project created by QtCreator 2017-07-05T08:34:13
#
#-------------------------------------------------

QT       += core gui opengl

CONFIG +=opengl console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtSTLView
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    modelloader_stl.cpp \
    trianglemesh.cpp \
    viewer.cpp

HEADERS += \
        mainwindow.h \
    modelloader_stl.h \
    trianglemesh.h \
    viewer.h

FORMS += \
        mainwindow.ui

INCLUDEPATH +=$$quote(C:/Qt/libQGLViewer-2.6.4)

win32:CONFIG(release, debug|release): LIBS += -L$$quote(C:/Qt/libQGLViewer-2.6.4/QGLViewer/) -lQGLViewer2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$quote(C:/Qt/libQGLViewer-2.6.4/QGLViewer/) -lQGLViewerd2

INCLUDEPATH += $$quote(C:/Qt/libQGLViewer-2.6.4/QGLViewer)
DEPENDPATH += $$quote(C:/Qt/libQGLViewer-2.6.4/QGLViewer)

LIBS += -lopengl32 -lglu32
