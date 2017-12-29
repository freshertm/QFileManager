#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dirviewer.h"
#include "diskselector.h"
#include <QKeyEvent>
#include "customevent.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QDir dir("C:/");

    DirViewer *l = new DirViewer(dir, this);
    DirViewer *r = new DirViewer(dir, this);
    setLeft(l);
    setRight(r);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
    if (event->key() == Qt::Key_F1 &&
        event->modifiers() == Qt::AltModifier){
        QCoreApplication::postEvent(left, new QEvent(OpenDiskList));
    }

    if (event->key() == Qt::Key_F2 &&
        event->modifiers() == Qt::AltModifier){
        QCoreApplication::postEvent(right, new QEvent(OpenDiskList));
    }
}

void MainWindow::setLeft(AbstractPanel *panel)
{
    if (left) {
        ui->layout->removeWidget(left);
    }
    ui->layout->addWidget(panel);
    left = panel;
}

void MainWindow::setRight(AbstractPanel *panel)
{
    if (right) {
        ui->layout->removeWidget(right);
    }
    ui->layout->addWidget(panel);
    right = panel;
}


