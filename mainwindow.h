#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include "abstractpanel.h"
#include <QShortcut>
#include <QShortcutEvent>
#include "settings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setSettings(Settings * settings);

protected:
    //virtual void keyPressEvent(QKeyEvent *);

private:
    void setLeft(AbstractPanel * panel);
    void setRight(AbstractPanel * panel);

private slots:
    void leftPanelChangeDrive();
    void rightPanelChangeDrive();
private:
    Ui::MainWindow *ui;
    Settings * set;

    AbstractPanel *left;
    AbstractPanel *right;

private slots:

};

#endif // MAINWINDOW_H
