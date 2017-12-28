#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    left = new QFileSystemModel(this);
    left->setRootPath("C:/Windows");
    left->setFilter(QDir::NoDot | QDir::AllEntries);
    ui->leftPanel->setModel(left);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_leftPanel_doubleClicked(const QModelIndex &index)
{
    QString path = left->filePath(index);
    left->setRootPath(path);
    ui->leftPanel->setRootIndex(left->index(path));
}
