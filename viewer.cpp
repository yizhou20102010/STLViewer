#include "viewer.h"
#include "manipulatedFrame.h"
#include <QGLViewer/manipulatedFrame.h>
#include<math.h>
#include <QMessageBox>
#include <QDebug>

using namespace std;

Viewer::Viewer(QWidget *parent) : QGLViewer(parent)
  , curselindex(-1)
{
    models.clear();
}


void Viewer::show_help_contents()
{
    help();
}

void Viewer::drawWithNames()
{
    for(auto k=0;k<models.count();k++)
    {
        glPushName(k);
        models.at(k)->draw();
        glPopName();
    }
}


void Viewer::postSelection(const QPoint &point)
{
    curselindex = selectedName();
    if(curselindex >= 0 && curselindex < models.count())
    {
        setManipulatedFrame(&models[curselindex]->mpFrame);
    }
}

void Viewer::onshowModel(TriangleMesh *mesh, bool is_reload)
{
    Object *m_object = new Object(mesh);
    QVector3D midpoint = (mesh->pmax + mesh->pmin)/2;
    m_object->mpFrame.setPosition(Vec(midpoint.x(), midpoint.y(), midpoint.z()));
    models.append(m_object);

    //给出物体所处的框架范围
    pmin.setValue(mesh->pmin.x(), mesh->pmin.y(), mesh->pmin.z());
    pmax.setValue(mesh->pmax.x(), mesh->pmax.y(), mesh->pmax.z());

    update();
}

void Viewer::DrawBasicPrintStructure()
{
    glColor3d(170.0/255, 213.0/255, 255.0/255);
    double framepoints[8][3] ={{0.0, 0.0, 0.0}, {250, 0.0, 0.0},{250.0, 250.0, 0.0},{0.0,250.0,0.0},
                               {0.0, 0.0, 300.0}, {250, 0.0, 300.0},{250.0, 250.0, 300.0},{0.0,250.0,300.0}};
        //底面
        glBegin(GL_POLYGON);
        for(auto i=0;i<4;i++)
        {
            glNormal3d(0.0, 0.0, 1.0);
            glVertex3d(framepoints[i][0], framepoints[i][1], framepoints[i][2]);
        }
        glEnd();


        glColor3b(0.0, 0.0, 0.0);

        glLineWidth(2.0);
        //框架
        glBegin(GL_LINE_LOOP);
        for(auto i=0;i<4;i++)
            glVertex3d(framepoints[i][0], framepoints[i][1], framepoints[i][2]);
        glEnd();

        glBegin(GL_LINE_LOOP);
        for(auto i=4;i<8;i++)
            glVertex3d(framepoints[i][0], framepoints[i][1], framepoints[i][2]);
        glEnd();


        for(auto i=0;i<4;i++)
        {
            glBegin(GL_LINES);
            glVertex3d(framepoints[i][0], framepoints[i][1], framepoints[i][2]);
            glVertex3d(framepoints[i+4][0], framepoints[i+4][1], framepoints[i+4][2]);
            glEnd();
        }
        glLineWidth(1.0);
}


void Viewer::loadProjectionType(const int type)
{
    if(type == 0)
        camera()->setType(Camera::PERSPECTIVE);
    else
        camera()->setType(Camera::ORTHOGRAPHIC);

    double radius = (pmax-pmin).norm();
    setSceneRadius(radius*0.5);
    setSceneCenter((pmin+pmax)/2);
    showEntireScene();

    restoreStateFromFile();
    updateGL();
}

void Viewer::loadStandardCamera(const int index)
{
    Vec pos, upV, viewD ,centerpos;
    centerpos = sceneCenter();
    pos = camera()->position();
    upV = camera()->upVector();
    viewD = camera()->viewDirection();

    Vec newpos, newupV, newviewD;
    switch (index) 
    {
    case 0: //Home
        newviewD = Vec(-0.5, 1, -0.5).unit();
        newupV = cross(newviewD, cross(Vec(0,0,1), newviewD)).unit();
        break;
    case 1: //Front View
        newviewD = Vec(0, 1, 0);
        newupV = Vec(0, 0, 1);
        break;
    case 2: //Back View
        newviewD = Vec(0, -1, 0);
        newupV = Vec(0, 0, 1);
        break;
    case 3: //Left View
        newviewD = Vec(1, 0, 0);
        newupV = Vec(0, 0, 1);
        break;
    case 4: //Right View
        newviewD = Vec(-1, 0, 0);
        newupV = Vec(0, 0, 1);
        break;
    case 5:  //Up View
        newviewD = Vec(0 ,0, -1);
        newupV = Vec(0, 1, 0);
        break;
    case 6: //Bottom View
        newviewD = Vec(0, 0, 1);
        newupV = Vec(0, -1, 0);
        break;
    default:
        break;
    }    

    newpos = centerpos - newviewD*((pos - centerpos).norm());

    camera()->setPosition(newpos);
    camera()->setViewDirection(newviewD);
    camera()->setUpVector(newupV);
    
    updateGL();
}

void Viewer::init()
{
//    glClearColor(0.73,0.87,0.99,1.0);
    setBackgroundColor(QColor(204,255,255));
    // Light setup

//       Light default parameters
      const GLfloat light_ambient[4] = {0.0, 0.0, 0.0, 1.0};
      const GLfloat light_specular[4] = {1.0, 1.0, 1.0, 1.0};
      const GLfloat light_diffuse[4] = {1.0, 1.0, 1.0, 1.0};
      const GLfloat position0[]={1.0,1.0,1.0, 0.0};
      const GLfloat position1[]={-1.0, -1.0, -1.0, 0.0};

      glLightfv(GL_LIGHT0, GL_POSITION, position0);


      glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
      glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
      glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
      glLightfv(GL_LIGHT1, GL_POSITION, position1);
      glEnable(GL_LIGHT1);


      pmin.setValue(0, 0, 0);
      pmax.setValue(250, 250, 300);
      loadProjectionType(1);


      loadStandardCamera();
//      setMouseTracking(true);

      restoreStateFromFile();
}

void Viewer::draw()
{
    glPushMatrix();
//    Vec center = sceneCenter();
//    glTranslatef(center[0], center[1], center[2]);
    drawAxis(100);
//    drawGrid(100,10);
    glPopMatrix();

    DrawBasicPrintStructure();

    glColor3d(0.5,0.5,0.5);

    for(auto k=0;k<models.count();k++)
    {
        if(curselindex == k)
            glColor3d(0.6, 0.85, 0.92);
        else
            glColor3d(0.5,0.5,0.5);
        models.at(k)->draw();
    }

    if(curselindex >= 0 && curselindex < models.count())
    {
        if (manipulatedFrame()->isManipulated())
          {
            glPushMatrix();
            glMultMatrixd(manipulatedFrame()->matrix());
            drawAxis(50);
            glPopMatrix();
          }
    }

    glPushMatrix();
    Vec center = sceneCenter();
    glTranslatef(center[0], center[1], center[2]);
    drawAxis(100);
//    drawGrid(100,10);
    glPopMatrix();
}

QString Viewer::helpString()const
{
    QString text("<h2>STL Viewer</h2>");
      text += "Left click while pressing the <b>Shift</b> key to select an object "
              "of the scene.<br><br>";
      return text;
}


