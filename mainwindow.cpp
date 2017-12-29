#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "fileviewer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QDir dir("C:/");

    FileViewer *l = new FileViewer(dir, this);
    FileViewer *r = new FileViewer(dir, this);
    setLeft(l);
    setRight(r);
}

MainWindow::~MainWindow()
{
    delete ui;
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


