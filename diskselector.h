#ifndef DISKSELECTOR_H
#define DISKSELECTOR_H

#include <QWidget>
#include <QFileSystemModel>
#include <QListView>

namespace Ui {
class DiskSelector;
}

class DiskSelector : public QWidget
{
    Q_OBJECT

public:
    explicit DiskSelector(QWidget *parent = 0);
    ~DiskSelector();

signals:
    void driveSelected(QDir drive);

protected:   
    virtual void showEvent(QShowEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void on_listView_clicked(const QModelIndex &index);
    void onCustomContextMenu(const QPoint &point);

private:
    Ui::DiskSelector *ui;
    QFileSystemModel *model;
};

#endif // DISKSELECTOR_H
