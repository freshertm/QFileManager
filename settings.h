#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QShortcut>
#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QWidget *parent);

public slots:
    void setupDefault();
    void load();
    void save();
public:
    void setQSettings(QSettings * settings);
private:
    void init();

    QSettings *settings;
    QWidget *parentWidget;

public:
    QShortcut* leftPanelChangeDrive;
    QShortcut* rightPanelChangeDrive;
    QShortcut* exitAction;

};

#endif // SETTINGS_H
