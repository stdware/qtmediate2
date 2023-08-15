#include <QApplication>
#include <QDebug>

#include <QMCustomWindow.h>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QApplication::addLibraryPath(QApplication::applicationDirPath() + "/plugins");

    QMCustomWindow w;
    w.show();

    return a.exec();
}