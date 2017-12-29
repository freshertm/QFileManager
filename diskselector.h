#ifndef DISKSELECTOR_H
#define DISKSELECTOR_H

#include <QWidget>
#include <QFileSystemModel>
#include <QListView>

namespace Ui {
class DiskSelector;
}

class DiskSelector : public QListView
{
    Q_OBJECT

public:
    explicit DiskSelector(QWidget *parent = 0);
    ~DiskSelector();

protected:
    virtual void focusOutEvent(QFocusEvent *);
    virtual void showEvent(QShowEvent *event);

private:
    Ui::DiskSelector *ui;
    QFileSystemModel *model;
};

#endif // DISKSELECTOR_H
