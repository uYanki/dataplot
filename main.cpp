#include "mainwindow.h"

#include <QApplication>
#include "devicewatcher.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifdef Q_OS_WIN
     initDeviceWatcher();
#endif
    MainWindow w;
    w.show();
    return a.exec();
}
