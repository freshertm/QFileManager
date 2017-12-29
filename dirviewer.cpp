#include "dirviewer.h"
#include "ui_dirviewer.h"
#include <QKeyEvent>
#include "customevent.h"

DirViewer::DirViewer(QDir dir, QWidget *parent) :
    AbstractPanel(dir, parent),
    ui(new Ui::DirViewer),
    dir(dir)
{
    ui->setupUi(this);

    model = new QFileSystemModel(this);
    model->setRootPath(dir.absolutePath());
    model->setFilter(QDir::NoDot | QDir::AllEntries);
    ui->listView->setModel(model);
    changeDir(dir);

    selector = new DiskSelector(this);
    selector->setVisible(false);

    selector->installEventFilter(this);

    connect(selector, &DiskSelector::driveSelected, this, &DirViewer::changeDir);
}

DirViewer::~DirViewer()
{
    delete ui;
}

QDir DirViewer::currentDir()
{
    return dir;
}

void DirViewer::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Enter ||
        event->key() == Qt::Key_Return)
    {
        on_listView_doubleClicked(ui->listView->currentIndex());
        event->accept();
        return;
    }

    if (event->key() == Qt::Key_Backspace)
    {
        QDir c = dir;
        c.cdUp();
        on_listView_doubleClicked(model->index(c.absolutePath()));
        event->accept();
        return;
    }
    event->ignore();
}

bool DirViewer::event(QEvent *event)
{
    if (event->type() == OpenDiskList) {
        openDiskList();
        event->accept();
        return true;
    }
    return QWidget::event(event);
}

bool DirViewer::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == selector &&
            event->type() == QEvent::Hide) {
        ui->listView->setFocus();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void DirViewer::changeDir(QDir dir)
{
    QString path = dir.absolutePath();
    model->setRootPath(path);
    ui->listView->setRootIndex(model->index(path));
    this->dir = dir;
    ui->currentDirectory->setText(QDir::toNativeSeparators(dir.absolutePath()));
    emit dirChanged(dir);
}

void DirViewer::on_listView_doubleClicked(const QModelIndex &index)
{
    QString path = model->filePath(index);
    QFileInfo fi(path);
    if (fi.isDir()){
        changeDir(path);
    } else {
        emit fileAction(fi);
    }
}

void DirViewer::openDiskList()
{
    QSize size = selector->size();
    selector->setGeometry(0,0,size.width(), size.height());
    selector->setVisible(true);   
}
