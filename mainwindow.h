#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionHelpOperator_triggered();
    void on_actionOpenFile_triggered();
    void on_state_show(int errorindex, QString errorstr);
    void enable_openfile();
    void disable_openfile();

    void on_actionHomeView_triggered();
    
    void on_actionFrontView_triggered();
    
    void on_actionBackView_triggered();
    
    void on_actionLeftView_triggered();
    
    void on_actionRightView_triggered();
    
    void on_actionUpView_triggered();
    
    void on_actionBottomView_triggered();
    
    void on_actionProjectionPerspective_triggered();
    
    void on_actionProjectionOrthogonal_triggered();

protected:
    void contextMenuEvent(QContextMenuEvent *event);//右键菜单
private:
    Ui::MainWindow *ui;
    bool open_stl(const QString &filename);
    QAction *curCheckedAction;


};

#endif // MAINWINDOW_H
