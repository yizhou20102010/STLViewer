#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "viewer.h"
#include "modelloader_stl.h"
#include <QFileDialog>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    curCheckedAction = ui->actionHomeView;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionHelpOperator_triggered()
{
    ui->Canvas->help();
}

void MainWindow::on_actionOpenFile_triggered()
{
    QString filename = QFileDialog::getOpenFileName(
                this, "Load .stl file", QString(), "*.stl");
    if(!filename.isNull())
    {
        if(!ui->actionOpenFile->isEnabled()) return;

        open_stl(filename);
    }
}

void MainWindow::on_state_show(int errorindex, QString errorstr)
{

}

void MainWindow::enable_openfile()
{

}

void MainWindow::disable_openfile()
{

}


bool MainWindow::open_stl(const QString &filename)
{
    ui->statusBar->showMessage(tr("Loading ")+filename);

    ModelLoader_STL* loader = new ModelLoader_STL(this, filename);

    connect(loader, SIGNAL(error_info(int,QString)), this, SLOT(on_state_show(int,QString)));
    connect(loader, SIGNAL(got_mesh(TriangleMesh*,bool)), ui->Canvas, SLOT(onshowModel(TriangleMesh*,bool)));

    connect(loader, SIGNAL(started()), this, SLOT(disable_openfile()));
    connect(loader, SIGNAL(finished()), this, SLOT(enable_openfile()));

    loader->start();
    return true;
}


void MainWindow::on_actionHomeView_triggered()
{
    curCheckedAction->setChecked(false);
    curCheckedAction = ui->actionHomeView;
    curCheckedAction->setChecked(true);
    ui->Canvas->loadStandardCamera(0);
}

void MainWindow::on_actionFrontView_triggered()
{
    curCheckedAction->setChecked(false);
    curCheckedAction = ui->actionFrontView;
    curCheckedAction->setChecked(true);
    ui->Canvas->loadStandardCamera(1);
}

void MainWindow::on_actionBackView_triggered()
{
    curCheckedAction->setChecked(false);
    curCheckedAction = ui->actionBackView;
    curCheckedAction->setChecked(true);
    ui->Canvas->loadStandardCamera(2);
}

void MainWindow::on_actionLeftView_triggered()
{
    curCheckedAction->setChecked(false);
    curCheckedAction = ui->actionLeftView;
    curCheckedAction->setChecked(true);
    ui->Canvas->loadStandardCamera(3);
}

void MainWindow::on_actionRightView_triggered()
{
    curCheckedAction->setChecked(false);
    curCheckedAction = ui->actionRightView;
    curCheckedAction->setChecked(true);
    ui->Canvas->loadStandardCamera(4);
}

void MainWindow::on_actionUpView_triggered()
{
    curCheckedAction->setChecked(false);
    curCheckedAction = ui->actionUpView;
    curCheckedAction->setChecked(true);
    ui->Canvas->loadStandardCamera(5);
}

void MainWindow::on_actionBottomView_triggered()
{
    curCheckedAction->setChecked(false);
    curCheckedAction = ui->actionBottomView;
    curCheckedAction->setChecked(true);
    ui->Canvas->loadStandardCamera(6);
}

void MainWindow::on_actionProjectionPerspective_triggered()
{
    ui->actionProjectionOrthogonal->setChecked(false);
    ui->actionProjectionPerspective->setChecked(true);
    ui->Canvas->loadProjectionType(0);
}

void MainWindow::on_actionProjectionOrthogonal_triggered()
{
    ui->actionProjectionPerspective->setChecked(false);
    ui->actionProjectionOrthogonal->setChecked(true);

    ui->Canvas->loadProjectionType(1);
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = new QMenu();
    menu->addAction(ui->actionHomeView);
    menu->addSeparator();
    menu->addAction(ui->actionFrontView);
    menu->addAction(ui->actionBackView);
    menu->addAction(ui->actionLeftView);
    menu->addAction(ui->actionRightView);
    menu->addAction(ui->actionUpView);
    menu->addAction(ui->actionBottomView);

    menu->exec(QCursor::pos());

    delete menu;
}
