#ifndef VIEWER_H
#define VIEWER_H

#include <QWidget>
#include <QGLViewer/qglviewer.h>
#include "trianglemesh.h"

using namespace qglviewer;

class Viewer : public QGLViewer
{
    Q_OBJECT
public:
    explicit Viewer(QWidget *parent = nullptr);

    virtual void init();
    virtual void draw();
    virtual QString helpString() const;
signals:

public slots:
    void onshowModel(TriangleMesh* mesh, bool is_reload);

private:
    qglviewer::Vec orig, dir, selectedPoint;
    TriangleMesh *modelmesh;
    Vec pmin, pmax; //显示bound

public:
    void show_help_contents();
    void reLoadViewFrame(const int type=0);
};

#endif // VIEWER_H
