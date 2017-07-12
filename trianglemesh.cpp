#include "trianglemesh.h"
#include <math.h>
TriangleMesh::TriangleMesh(QVector<QVector3D> vertices, QVector<int> indices, QString filename)
    : vertices(vertices), indices(indices), name(filename)
{
    double xmin(0), ymin(0), zmin(0), xmax(0), ymax(0), zmax(0);
    for(auto i=0;i<vertices.count();i++)
    {
        if(i==0)
        {
            xmin = xmax = vertices.at(0).x();
            ymin = ymax = vertices.at(0).y();
            zmin = zmax = vertices.at(0).z();
        }
        else
        {
            xmin = fmin(xmin, vertices.at(i).x());
            xmax = fmax(xmax, vertices.at(i).x());
            ymin = fmin(ymin, vertices.at(i).y());
            ymax = fmax(ymax, vertices.at(i).y());
            zmin = fmin(zmin, vertices.at(i).z());
            zmax = fmax(zmax, vertices.at(i).z());
        }
    }
    pmin.setX(xmin);
    pmin.setY(ymin);
    pmin.setZ(zmin);
    pmax.setX(xmax);
    pmax.setY(ymax);
    pmax.setZ(zmax);
}

bool TriangleMesh::empty() const
{
    return vertices.count() == 0;
}
