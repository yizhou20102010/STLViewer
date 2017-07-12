#include "object.h"
#include <QDebug>

Object::Object()
    : isSelected(false)
{

}

Object::Object(const TriangleMesh *mesh)
    : mesh(mesh)
    , isSelected(false)
{

}

void Object::draw() const
{
    QVector3D point[3], normalv;
    QVector3D midpoint;
    midpoint = (mesh->pmax+mesh->pmin)/2;

    glPushMatrix();
    glMultMatrixd(mpFrame.matrix());

    if(mesh != nullptr)
    {
        if(mesh->indices.count()>0)
        {
            for(auto i=0; i<mesh->indices.count();i+=3)
            {
                for(auto j=0;j<3;j++)
                    point[j] = mesh->vertices.at(mesh->indices.at(i+j)) - midpoint;
                normalv = QVector3D::crossProduct(point[2]-point[1], point[1]-point[0]);
                normalv.normalize();
                glBegin(GL_POLYGON);
                glNormal3d(normalv.x(), normalv.y(), normalv.z());
                glVertex3d(point[0].x(), point[0].y(), point[0].z());
                glNormal3d(normalv.x(), normalv.y(), normalv.z());
                glVertex3d(point[1].x(), point[1].y(), point[1].z());
                glNormal3d(normalv.x(), normalv.y(), normalv.z());
                glVertex3d(point[2].x(), point[2].y(), point[2].z());
                glEnd();
            }
        }
    }
    glPopMatrix();
}

Vec Object::getMidPoint()
{
    QVector3D midpoint;
    midpoint = (mesh->pmax+mesh->pmin)/2;
    return Vec(midpoint.x(), midpoint.y(), midpoint.z());
}
