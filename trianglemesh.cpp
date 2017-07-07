#include "trianglemesh.h"

TriangleMesh::TriangleMesh(QVector<QVector3D> vertices, QVector<int> indices)
    : vertices(vertices), indices(indices)
{

}

bool TriangleMesh::empty() const
{
    return vertices.count() == 0;
}
