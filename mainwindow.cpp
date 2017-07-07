#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "modelloader_stl.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->actionOpenFile->setEnabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
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
    ui->statusBar->showMessage(errorstr);
}

void MainWindow::enable_openfile()
{
    ui->actionOpenFile->setEnabled(true);
}

void MainWindow::disable_openfile()
{
    ui->actionOpenFile->setEnabled(false);
}

bool MainWindow::open_stl(const QString &filename, const bool is_reload)
{
    ui->statusBar->showMessage(tr("Loading ")+filename);

    ModelLoader_STL* loader = new ModelLoader_STL(this, filename, is_reload);

    connect(loader, SIGNAL(error_info(int,QString)), this, SLOT(on_state_show(int,QString)));
    connect(loader, SIGNAL(got_mesh(TriangleMesh*,bool)), ui->Canvas, SLOT(onshowModel(TriangleMesh*,bool)));

    connect(loader, SIGNAL(started()), this, SLOT(disable_openfile()));
    connect(loader, SIGNAL(finished()), this, SLOT(enable_openfile()));

    loader->start();
    return true;
}

void MainWindow::on_actionViewer_Help_triggered()
{
    ui->Canvas->show_help_contents();
}



void MainWindow::on_actionOrthographic_triggered()
{
    ui->actionPerspective->setChecked(false);
    ui->Canvas->reLoadViewFrame(1);
}

void MainWindow::on_actionPerspective_triggered()
{
    ui->actionOrthographic->setChecked(false);
    ui->Canvas->reLoadViewFrame(0);
}
