#ifndef FILEVIEWER_H
#define FILEVIEWER_H

#include <QWidget>
#include "abstractpanel.h"
#include <QFileSystemModel>

namespace Ui {
class FileViewer;
}

class FileViewer : public AbstractPanel
{
    Q_OBJECT

public:
    explicit FileViewer(QDir dir, QWidget *parent = 0);
    ~FileViewer();
    virtual QDir currentDir();

protected:
    virtual void keyPressEvent(QKeyEvent *event);

public slots:
    virtual void changeDir(QDir dir);

private slots:
    void on_listView_doubleClicked(const QModelIndex &index);
private:
    QDir dir;
    Ui::FileViewer *ui;
    QFileSystemModel * model;
};

#endif // FILEVIEWER_H
