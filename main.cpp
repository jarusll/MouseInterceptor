#include "mainwindow.h"
#include "linuxdraginput.h"

#include <QApplication>
#include <QThread>
#include <cstdio>

int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATFORM", "xcb");
    QApplication a(argc, argv);
    LinuxDragInput drag;
    exit(0);
    // QThread::create([] {
    //     LinuxDragInput drag;
    //     drag.onDrag([](int dx, int dy) {
    //         printf("drag delta: %d, %d\n", dx, dy);
    //         fflush(stdout);
    //     });
    //     drag.run();
    // })->start();

    MainWindow w;
    w.show();
    return a.exec();
}
