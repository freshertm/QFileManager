#ifndef ABSTRACTPANEL_H
#define ABSTRACTPANEL_H

#include <QWidget>
#include <QDir>
#include <QFileInfo>

class AbstractPanel : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractPanel(QDir rootDir, QWidget *parent = nullptr): QWidget(parent){}
    virtual ~AbstractPanel(){}
    virtual QDir currentDir()=0;

signals:
    void dirChanged(const QDir &dir);
    void fileAction(const QFileInfo& fileInfo);

public slots:
    virtual void changeDir(QDir dir)=0;
};

#endif // ABSTRACTPANEL_H
