#include "diskselector.h"
#include "ui_diskselector.h"
#include <QFocusEvent>
#include <QDebug>

DiskSelector::DiskSelector(QWidget *parent) :
    QListView(parent),
    ui(new Ui::DiskSelector)
{
    ui->setupUi(this);
    model = new QFileSystemModel(this);
    model->setRootPath("");
    ui->listView->setModel(model);
}

DiskSelector::~DiskSelector()
{
    delete ui;
}

void DiskSelector::focusOutEvent(QFocusEvent * event)
{
    setVisible(false);
    event->accept();
}

void DiskSelector::showEvent(QShowEvent *event)
{

    int w = 50 + ui->listView->sizeHintForColumn(0) ;
    int h = 50 + ui->horizontalLayout->margin() + ui->listView->sizeHintForRow(0) * ui->listView->children().size();
    resize(w,h);
}
