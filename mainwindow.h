#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include "abstractpanel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void keyPressEvent(QKeyEvent *);

private:
    void setLeft(AbstractPanel * panel);
    void setRight(AbstractPanel * panel);
private:
    Ui::MainWindow *ui;

    AbstractPanel *left;
    AbstractPanel *right;
};

#endif // MAINWINDOW_H
