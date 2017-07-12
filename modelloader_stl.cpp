#include "modelloader_stl.h"
#include <QDataStream>
#include <QtAlgorithms>
#include <algorithm>
#include <QDebug>

ModelLoader_STL::ModelLoader_STL(QObject *parent, const QString &filepathname , bool is_reload)
    : QThread(parent), filepathname(filepathname), is_reload(is_reload)
{
    // extract file name
    filename = filepathname.mid(filepathname.lastIndexOf('/')+1);
    filename = filename.mid(0,filename.indexOf('.'));
}

void ModelLoader_STL::run()
{
    TriangleMesh* mesh = load_stl();
    if (mesh)
    {
        if (mesh->empty())
        {
            emit error_info(0x00, tr("empty mesh."));
            delete mesh;
        }
        else
        {
            emit got_mesh(mesh, is_reload);
            emit loaded_file(filepathname);
        }
    }
}

TriangleMesh *ModelLoader_STL::load_stl()
{
    QFile file(filepathname);
    if(!file.open(QIODevice::ReadOnly))
    {
        emit error_info(0x10, tr("file missing."));
        return nullptr;
    }
    // First, try to read the stl as an ASCII file
    if(file.read(5)=="solid")//ascii file
    {
        file.readLine();// skip solid name
        auto line = file.readLine().trimmed();
        if(line.startsWith("facet") || line.startsWith("endsolid"))
        {
            file.seek(0);
            return read_stl_ascii(file);
        }

        confusing_stl = true;
    }
    else confusing_stl =false;

    // Otherwise, skip the rest of the header material and read as binary
    file.seek(0);
    return read_stl_binary(file);
}

TriangleMesh *ModelLoader_STL::read_stl_ascii(QFile &file)
{
    file.readLine();
    uint32_t tri_count = 0;
    QVector<SingleVertex>verts;
    SingleVertex onevertex;
    
    bool okay = true;
    while(okay && !file.atEnd())
    {
        const auto line = file.readLine().simplified();
        if(line.startsWith("endsolid"))
            break;
        if((!line.startsWith("facet normal") && file.readLine().simplified().startsWith("outer loop")))
        {
            okay = false;
            break;
        }
        
        for(int i=0;i<3;i++)
        {
            auto line = file.readLine().simplified().split(' ');
            if(line[0] != "vertex")
            {
                okay = false;
                break;
            }
            double xvalue = line[1].toDouble(&okay);
            double yvalue = line[2].toDouble(&okay);
            double zvalue = line[3].toDouble(&okay);
            onevertex.point.setX(xvalue);
            onevertex.point.setY(yvalue);
            onevertex.point.setZ(zvalue);
            onevertex.index = tri_count*3+i;
            verts.push_back(onevertex);
        }
        if((!line.startsWith("endloop") && file.readLine().simplified().startsWith("endfacet")))
        {
            okay = false;
            break;
        }
        tri_count++;
    }
qDebug()<<"read completed.";
    if(okay)
        return mesh_from_verts(verts);
    else
    {
        emit error_info(0x20, tr("bad stl."));
        return nullptr;
    }
}

TriangleMesh *ModelLoader_STL::read_stl_binary(QFile &file)
{
    QDataStream data(&file);
    data.setByteOrder(QDataStream::LittleEndian);
    data.setFloatingPointPrecision(QDataStream::SinglePrecision);

    // Load the triangle count from the .stl file
    file.seek(80);
    uint32_t tri_count;
    data>>tri_count;

    // Verify that the file is the right size
    if(file.size() != 84 + tri_count*50)
    {
        emit error_info(0x20, tr("bad stl file."));
        return nullptr;
    }

    // Verify that the file is the right size
    QVector<SingleVertex>verts;
    SingleVertex onevertex;

    // Dummy array, because readRawData is faster than skipRawData
    uint8_t* buffer = (uint8_t*)malloc(tri_count * 50);
    data.readRawData((char*)buffer, tri_count * 50);
    // Store vertices in the array, processing one triangle at a time.
    auto b = buffer;
    for(auto i=0; i<tri_count; i++)
    {
        b += 3*4*sizeof(char);
        // Load vertex data from .stl file into vertices
        for(auto j=0; j<3; j++)
        {
            float value[3];

            memcpy(&value, b, 3*4*sizeof(char));
            b += 3*4*sizeof(char);

            onevertex.point.setX(value[0]);
            onevertex.point.setY(value[1]);
            onevertex.point.setZ(value[2]);
            onevertex.index = i*3+j;
            verts.push_back(onevertex);
        }
        // Skip face attribute
        b += sizeof(uint16_t);
    }

    if(confusing_stl)
        emit error_info(0x100, tr("warning confusing stl"));

    free(buffer);

    return mesh_from_verts(verts);
}

TriangleMesh *ModelLoader_STL::mesh_from_verts(QVector<SingleVertex> &verts)
{
    // Sort the set of vertices (to deduplicate)
    qSort(verts.begin(), verts.end(), compareData);


    QVector<QVector3D>vertices;
    QVector<int>indices(verts.count());

    for(auto st : verts)
    {
        if(!(vertices.count()>0 && st.point == vertices.constLast()))
        {
            vertices.push_back(st.point);
        }
        indices[st.index] = vertices.count()-1;
    }
    verts.clear();

    return new TriangleMesh(vertices, indices, filename);
}

bool ModelLoader_STL::compareData(const SingleVertex &T1, const SingleVertex &T2)
{
    if(T1.point.x() < T2.point.x())
        return true;
    else if(T1.point.x() > T2.point.x())
        return false;
    else if(T1.point.y() < T2.point.y())
        return true;
    else if(T1.point.y() > T2.point.y())
        return false;
    else if(T1.point.z() < T2.point.z())
        return true;
    else if(T1.point.z() > T2.point.z())
        return false;
    else return false;

}



