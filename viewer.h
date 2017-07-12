#ifndef VIEWER_H
#define VIEWER_H

#include <QWidget>
#include <QGLViewer/qglviewer.h>
#include "trianglemesh.h"
#include "object.h"
#include <QList>

using namespace qglviewer;

class Viewer : public QGLViewer
{
    Q_OBJECT
public:
    explicit Viewer(QWidget *parent = nullptr);

    virtual void init();
    virtual void draw();
    virtual QString helpString() const;
    virtual void drawWithNames();
    virtual void postSelection(const QPoint &point);
signals:

public slots:
    void onshowModel(TriangleMesh* mesh, bool is_reload);

private:
    Vec orig, dir, selectedPoint;
    Vec pmin, pmax; //显示bound
    QList<Object *> models;
    int curselindex; //选择

    void DrawBasicPrintStructure();

public:
    void show_help_contents();
    void loadProjectionType(const int type=0);
    void loadStandardCamera(const int index = 0);
};

#endif // VIEWER_H
