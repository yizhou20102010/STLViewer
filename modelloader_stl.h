#ifndef MODELLOADER_STL_H
#define MODELLOADER_STL_H

#include <QObject>
#include <QThread>
#include <QFile>
#include <QVector3D>
#include "trianglemesh.h"

struct SingleVertex{
    QVector3D point;
    int index;
};

class ModelLoader_STL : public QThread
{
    Q_OBJECT
public:
    explicit ModelLoader_STL(QObject *parent = nullptr, const QString &filename = tr(""), bool is_reload = false);
    void run();

protected:
    TriangleMesh * load_stl();
    /*  Reads an ASCII stl, starting from the start of the file*/
    TriangleMesh* read_stl_ascii(QFile& file);
    /*  Reads a binary stl, assuming we're at the end of the header */
    TriangleMesh* read_stl_binary(QFile& file);

signals:
    void loaded_file(QString filename);
    void got_mesh(TriangleMesh* m, bool is_reload);

    void error_info(int, QString);
public slots:

private:
    QString filename;
    bool is_reload;
    /*  Used to warn on binary STLs that begin with the word 'solid'" */
    bool confusing_stl;

    TriangleMesh *mesh_from_verts(QVector<SingleVertex>&verts);
    static bool compareData(const SingleVertex &T1, const SingleVertex &T2);
//    void parallel_sort(SingleTriangle * begin, SingleTriangle* end, int threads);
};

#endif // MODELLOADER_STL_H
