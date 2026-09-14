#include "mainwindow.h"

#include <QApplication>

#ifdef Q_OS_LINUX
#include "linuxdragshakedetector.h"
#endif

#ifdef Q_OS_MAC
#include "macosdragshakedetector.h"
#endif

int main(int argc, char *argv[])
{
    #ifdef Q_OS_LINUX
    qputenv("QT_QPA_PLATFORM", "xcb");
#endif
    QApplication a(argc, argv);

#ifdef Q_OS_LINUX
    LinuxDragShakeDetector detector;
    QObject::connect(&detector, &LinuxDragShakeDetector::shakeDetected, &a, [] {
        qInfo() << "Shake detected";
    });
#endif

#ifdef Q_OS_MAC
    MacOSDragShakeDetector detector;
    QObject::connect(&detector, &MacOSDragShakeDetector::shakeDetected, &a, [] {
        qInfo() << "Shake detected";
    });
#endif

    MainWindow w;
    w.show();
    return a.exec();
}
