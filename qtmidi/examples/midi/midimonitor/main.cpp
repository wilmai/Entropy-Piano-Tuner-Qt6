#include "mainwindow.h"
#include <QApplication>

#ifdef Q_OS_ANDROID
//#include <QAndroidJniObject>
namespace {
    //QAndroidJniObject forceLinking;
}
#endif

int main(int argc, char *argv[])
{
    // fix to run examples out of qt creator
    QCoreApplication::addLibraryPath("./../../../plugins");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
