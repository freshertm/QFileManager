#include "mainwindow.h"
#include <QApplication>
#include "settings.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    Settings set(&w);
    w.setSettings(&set);
    w.show();

    return a.exec();
}
