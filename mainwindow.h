#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool open_stl(const QString &filename, const bool is_reload=false);
private slots:
    void on_actionOpenFile_triggered();
    void on_state_show(int errorindex, QString errorstr);
    void enable_openfile();
    void disable_openfile();

    void on_actionViewer_Help_triggered();

    void on_actionOrthographic_triggered();

    void on_actionPerspective_triggered();

private:
    Ui::MainWindow *ui;


};

#endif // MAINWINDOW_H
