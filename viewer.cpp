#include "viewer.h"
#include "manipulatedFrame.h"
#include <QGLViewer/manipulatedFrame.h>
#include<math.h>
#include <QMessageBox>
#include <QDebug>

using namespace std;

Viewer::Viewer(QWidget *parent) : QGLViewer(parent)
{
    modelmesh = nullptr;
}


void Viewer::show_help_contents()
{
    help();
}

void Viewer::onshowModel(TriangleMesh *mesh, bool is_reload)
{
    qDebug()<<"mesh info: "<<mesh->vertices.count()<<" "<<mesh->indices.count();
    modelmesh = mesh;

    //给出物体所处的框架范围
    double xmin,xmax,ymin,ymax,zmin,zmax;
    if(modelmesh->vertices.count()>0)
    {
        xmin = xmax = modelmesh->vertices.at(0).x();
        ymin = ymax = modelmesh->vertices.at(0).y();
        zmin = zmax = modelmesh->vertices.at(0).z();
        for(auto point : modelmesh->vertices)
        {
            xmin = fmin(xmin, point.x());
            xmax = fmax(xmax, point.x());
            ymin = fmin(ymin, point.y());
            ymax = fmax(ymax, point.y());
            zmin = fmin(zmin, point.z());
            zmax = fmax(zmax, point.z());
        }
        pmin.setValue(xmin, ymin, zmin);
        pmax.setValue(xmax, ymax, zmax);
        reLoadViewFrame();
    }

    update();
}



void Viewer::reLoadViewFrame(const int type)
{
    double radius = (pmax-pmin).norm();

    if(type == 0)
        camera()->setType(Camera::PERSPECTIVE);
    else
        camera()->setType(Camera::ORTHOGRAPHIC);

    setSceneRadius(radius*0.618);
    setSceneCenter((pmin+pmax)/2);
    showEntireScene();

    restoreStateFromFile();
}

void Viewer::init()
{
    glClearColor(0.73,0.87,0.99,1.0);

    // Light setup
      glEnable(GL_LIGHT0);
      glEnable(GL_LIGHT1);

      // Light default parameters
      const GLfloat light_ambient[4] = {0.9, 0.9, 0.9, 1.0};
      const GLfloat light_specular[4] = {0.9, 0.9, 0.9, 1.0};
      const GLfloat light_diffuse[4] = {0.9, 0.9, 0.9, 1.0};
      const GLfloat position0[]={1.0,1.0,1.0, 0.0};
      const GLfloat position1[]={-1.0, -1.0, -1.0, 0.0};

      glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
      glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
      glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
      glLightfv(GL_LIGHT0, GL_POSITION, position0);

      glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
      glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
      glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
      glLightfv(GL_LIGHT1, GL_POSITION, position1);


      glEnable(GL_NORMALIZE);

      pmin.setValue(-1,-1,-1);
      pmax.setValue(1,1,1);
      reLoadViewFrame();
      restoreStateFromFile();
}

void Viewer::draw()
{

    QVector3D point[3], normalv;
    // Draw ten spirals
    glColor3d(0.15,0.15,0.15);

    GLfloat material_ambient[]={0.5, 0.5, 0.5, 1.0};
    GLfloat material_diffuse[]={0.5, 0.5, 0.5, 1.0};
    GLfloat material_specular[]={0.5, 0.5, 0.5, 1.0};
    GLfloat material_shininess=5.0;
    glMaterialfv(GL_FRONT, GL_AMBIENT,material_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, material_shininess);
    if(modelmesh != nullptr)
    {
        if(modelmesh->indices.count()>0)
        {
            for(auto i=0; i<modelmesh->indices.count();i+=3)
            {
                for(auto j=0;j<3;j++)
                    point[j] = modelmesh->vertices.at(modelmesh->indices.at(i+j));
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
}

QString Viewer::helpString()const
{
    QString text("<h2>STL Viewer</h2>");
     return text;
}
