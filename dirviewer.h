#ifndef FILEVIEWER_H
#define FILEVIEWER_H

#include <QWidget>
#include "abstractpanel.h"
#include <QFileSystemModel>
#include "diskselector.h"

namespace Ui {
class DirViewer;
}

class DirViewer : public AbstractPanel
{
    Q_OBJECT

public:
    explicit DirViewer(QDir dir, QWidget *parent = 0);
    ~DirViewer();
    virtual QDir currentDir();

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual bool event(QEvent * event);

    bool eventFilter(QObject *obj, QEvent *event);

public slots:
    virtual void changeDir(QDir dir);

private slots:
    void on_listView_doubleClicked(const QModelIndex &index);
    void openDiskList();
    void directoryLoaded(const QString &path);
    void onCustomContextMenu(const QPoint &point);
    void openCurrentItem();

private:
    bool openShellContextMenuForObject(const std::wstring &path, int xPos, int yPos, void * parentWindow);
    QDir dir;
    Ui::DirViewer *ui;
    QFileSystemModel * model;
    DiskSelector *selector;
};

#endif // FILEVIEWER_H
