#include "settings.h"

Settings::Settings(QWidget *parent) : QObject(parent), parentWidget(parent)
{
    init();
    setupDefault();

    if (settings){
        setQSettings(settings);
        load();
    }
}

void Settings::setupDefault()
{
    leftPanelChangeDrive->setKey(QString("Alt+F1"));
    rightPanelChangeDrive->setKey(QString("Alt+F2"));
    exitAction->setKey(QString("Alt+F4"));
}

void Settings::load()
{

}

void Settings::save()
{

}

void Settings::setQSettings(QSettings *settings)
{
    this->settings = settings;
}

void Settings::init()
{
    leftPanelChangeDrive = new QShortcut(parentWidget);
    rightPanelChangeDrive = new QShortcut(parentWidget);
    exitAction = new QShortcut(parentWidget);
}
