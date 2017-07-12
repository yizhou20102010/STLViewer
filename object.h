#ifndef OBJECT_H
#define OBJECT_H
//用于模型显示
#include "trianglemesh.h"

#include <QGLViewer/qglviewer.h>
#include <QGLViewer/manipulatedFrame.h>

using namespace qglviewer;

class Object
{
private:
    const TriangleMesh *mesh;

    bool isSelected; //是否被选中
public:
    Object();
    Object(const TriangleMesh *mesh);
    void draw() const;//绘制三角面片
    Vec getMidPoint(); //获取模型中心点

    ManipulatedFrame mpFrame;
    void 	setPosition (const Vec &position) { mpFrame.setPosition(position);}

};

#endif // OBJECT_H
