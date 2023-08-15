#include "SystemButton.h"

#include <QApplication>
#include <QDateTime>
#include <QMouseEvent>

SystemButton::SystemButton(QWidget *parent) : CToolButton(parent) {
}

SystemButton::~SystemButton() {
}

void SystemButton::setHovered(bool value) {
    auto pos = QCursor::pos();
    if (value) {
        if (underMouse()) {
            QHoverEvent e(QEvent::HoverMove, mapFromGlobal(pos), mapFromGlobal(pos),
                          QApplication::keyboardModifiers());
            QApplication::sendEvent(this, &e);
        } else {
            QEnterEvent e(mapFromGlobal(pos), mapFrom(window(), pos), pos);
            QHoverEvent e1(QEvent::HoverEnter, mapFromGlobal(pos), mapFromGlobal(pos),
                           QApplication::keyboardModifiers());
            QApplication::sendEvent(this, &e);
            QApplication::sendEvent(this, &e1);
        }
    } else {
        if (underMouse()) {
            QEvent e(QEvent::Leave);
            QHoverEvent e1(QEvent::HoverLeave, mapFromGlobal(pos), mapFromGlobal(pos),
                           QApplication::keyboardModifiers());
            QApplication::sendEvent(this, &e);
            QApplication::sendEvent(this, &e1);
        }
    }
}

void SystemButton::setPressed(bool value) {
    if (isDown() == value) {
        return;
    }

    QMouseEvent e(value ? QEvent::MouseButtonPress : QEvent::MouseButtonRelease,
                  mapFromGlobal(QCursor::pos()), Qt::LeftButton, Qt::NoButton,
                  QApplication::keyboardModifiers());
    QApplication::sendEvent(this, &e);
}

void SystemButton::setActive(bool value) {
    qDebug() << "set active" << value;
    setEnabled(value);
}

bool SystemButton::event(QEvent *event) {
    switch (event->type()) {
        case QEvent::HoverEnter:
            qDebug() << QDateTime::currentDateTime() << "hover enter";
            break;
        case QEvent::HoverMove:
            qDebug() << QDateTime::currentDateTime() << "hover move";
            break;
        case QEvent::HoverLeave:
            qDebug() << QDateTime::currentDateTime() << "hover leave";
            break;
        default:
            break;
    }
    return CToolButton::event(event);
}
