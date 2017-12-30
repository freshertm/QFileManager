#include "diskselector.h"
#include "ui_diskselector.h"
#include <QFocusEvent>
#include <QDebug>



DiskSelector::DiskSelector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DiskSelector)
{
    ui->setupUi(this);
    model = new QFileSystemModel(this);
    model->setRootPath("");
    model->setFilter(QDir::Drives);
    ui->listView->setModel(model);
    ui->listView->installEventFilter(this);

    ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);

}

DiskSelector::~DiskSelector()
{
    delete ui;
}

void DiskSelector::showEvent(QShowEvent *event)
{
    int w = 50 + ui->listView->sizeHintForColumn(0) ;
    int h = 10 + ui->listView->sizeHintForRow(0) * ui->listView->model()->rowCount();

    qDebug() << "size:" <<ui->listView->model()->rowCount();

    resize(w,h);
    ui->listView->setFocus();
    ui->listView->setCurrentIndex(ui->listView->currentIndex());
}

void DiskSelector::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return ||
            event->key() == Qt::Key_Enter) {
        QString path = model->filePath(ui->listView->currentIndex());
        emit driveSelected(QDir(path));
        hide();
        event->accept();
        return;
    }

    if (event->key() == Qt::Key_Escape) {
        hide();
        event->accept();
        return;
    }
    event->ignore();
}

bool DiskSelector::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->listView && event->type() == QEvent::FocusOut){
        hide();
        event->accept();
        return true;
    }
    return QWidget::eventFilter(obj, event);
}

void DiskSelector::on_listView_clicked(const QModelIndex &index)
{
    QString path = model->filePath(index);
    emit driveSelected(QDir(path));
    hide();
}
