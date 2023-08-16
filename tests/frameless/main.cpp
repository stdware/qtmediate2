#include <QApplication>
#include <QDebug>

#include <QMCustomWindow.h>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QApplication::addLibraryPath(QApplication::applicationDirPath() + "/plugins");

    QMCustomWindow w;
    w.show();

//     w.setStyleSheet(R"(

// QAbstractButton {
//     border: none;
//     background-color: blue;
// }

// QAbstractButton:hover {
//     background-color: red;
// }

// QAbstractButton:pressed {
//     background-color: green;
// }

// QAbstractButton:checked {
//     background-color: yellow;
// }

//     )");

    return a.exec();
}