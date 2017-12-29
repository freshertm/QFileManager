#include "diskselector.h"
#include "ui_diskselector.h"
#include <QFocusEvent>
#include <QDebug>

//#include <QEvent

DiskSelector::DiskSelector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DiskSelector)
{
    ui->setupUi(this);
    model = new QFileSystemModel(this);
    model->setRootPath("");
    ui->listView->setModel(model);
    ui->listView->installEventFilter(this);
}

DiskSelector::~DiskSelector()
{
    delete ui;
}

void DiskSelector::showEvent(QShowEvent *event)
{
    int w = 50 + ui->listView->sizeHintForColumn(0) ;
    int h = 50 + ui->horizontalLayout->margin() + ui->listView->sizeHintForRow(0) * ui->listView->children().size();
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
    }

    if (event->key() == Qt::Key_Escape) {
        hide();
    }
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
