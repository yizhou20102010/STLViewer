#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include <QVector>
#include <QVector3D>
class TriangleMesh
{
public:
    TriangleMesh(QVector<QVector3D> vertices, QVector<int>indices, QString filename);

private:

public:
    QVector<QVector3D>vertices;
    QVector<int>indices;
    QString name;
    QVector3D pmin,pmax;

public:
    bool empty() const;
};

#endif // TRIANGLEMESH_H
