#include "mainwindow.h"
#include "linuxdragshakedetector.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATFORM", "xcb");
    QApplication a(argc, argv);

    LinuxDragShakeDetector detector;
    QObject::connect(&detector, &LinuxDragShakeDetector::shakeDetected, &a, [] {
        qInfo() << "Shake detected";
    });

    MainWindow w;
    w.show();
    return a.exec();
}
