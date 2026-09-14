#include "mainwindow.h"

#include <QApplication>

#ifdef Q_OS_LINUX
#include "linuxdragshakedetector.h"
#endif

int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATFORM", "xcb");
    QApplication a(argc, argv);

#ifdef Q_OS_LINUX
    LinuxDragShakeDetector detector;
    QObject::connect(&detector, &LinuxDragShakeDetector::shakeDetected, &a, [] {
        qInfo() << "Shake detected";
    });
#endif

    MainWindow w;
    w.show();
    return a.exec();
}
