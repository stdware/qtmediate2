#include "QMWindowHandle.h"

#include <QMainWindow>

class QMWindowHandlePrivate {
    Q_DECLARE_PUBLIC(QMWindowHandle)
public:
    QMWindowHandlePrivate(QMWindowHandle *q);
    virtual ~QMWindowHandlePrivate();

    QMWindowHandle *q_ptr;

    QMainWindow *w;

    QMWindowHandle::TitleBarFlags titleBarFlags;
};

QMWindowHandlePrivate::QMWindowHandlePrivate(QMWindowHandle *q) : q_ptr(q) {
    w = qobject_cast<QMainWindow *>(q->parent());
    titleBarFlags = QMWindowHandle::TitleBarFlagMask;
}

QMWindowHandlePrivate::~QMWindowHandlePrivate() {
    w = nullptr;
}

QMWindowHandle::QMWindowHandle(QMainWindow *parent) : QObject(parent), d_ptr(new QMWindowHandlePrivate(this)) {
}

QMWindowHandle::~QMWindowHandle() {
}

QMainWindow *QMWindowHandle::window() const {
    Q_D(const QMWindowHandle);
    return d->w;
}

void QMWindowHandle::setup() {
}

void QMWindowHandle::setMenuBar(QMenuBar *menuBar) {
    Q_D(QMWindowHandle);
    d->w->setMenuBar(menuBar);
}

QMenuBar *QMWindowHandle::menuBar() const {
    Q_D(const QMWindowHandle);
    return d->w->menuBar();
}

void QMWindowHandle::setTitleBarFlags(TitleBarFlags flags) {
    Q_D(QMWindowHandle);
    d->titleBarFlags = flags;

    updateTitleBar();
}

QMWindowHandle::TitleBarFlags QMWindowHandle::titleBarFlags() {
    Q_D(const QMWindowHandle);
    return d->titleBarFlags;
}
