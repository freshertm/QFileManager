#include "fileviewer.h"
#include "ui_fileviewer.h"
#include <QKeyEvent>

FileViewer::FileViewer(QDir dir, QWidget *parent) :
    AbstractPanel(dir, parent),
    ui(new Ui::FileViewer),
    dir(dir)
{
    ui->setupUi(this);

    model = new QFileSystemModel(this);
    model->setRootPath(dir.absolutePath());
    model->setFilter(QDir::NoDot | QDir::AllEntries);
    ui->listView->setModel(model);
    changeDir(dir);
}

FileViewer::~FileViewer()
{
    delete ui;
}

QDir FileViewer::currentDir()
{
    return dir;
}

void FileViewer::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Enter ||
        event->key() == Qt::Key_Return)
    {
        on_listView_doubleClicked(ui->listView->currentIndex());
    }

    if (event->key() == Qt::Key_Backspace)
    {
        QDir c = dir;
        c.cdUp();
        on_listView_doubleClicked(model->index(c.absolutePath()));
    }
}

void FileViewer::changeDir(QDir dir)
{
    QString path = dir.absolutePath();
    model->setRootPath(path);
    ui->listView->setRootIndex(model->index(path));
    this->dir = dir;
    ui->currentDirectory->setText(QDir::toNativeSeparators(dir.absolutePath()));
    emit dirChanged(dir);
}

void FileViewer::on_listView_doubleClicked(const QModelIndex &index)
{
    QString path = model->filePath(index);
    QFileInfo fi(path);
    if (fi.isDir()){
        changeDir(path);
    } else {
        emit fileAction(fi);
    }
}
