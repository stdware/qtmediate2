#include "QMCustomWindow.h"

#include "QMWindowHandle.h"
#include "QMWindowHandlePlugin.h"


#include <QApplication>

#include <private/qfactoryloader_p.h>

class WindowHandleFactory {
public:
    static QStringList keys();
    static QString requested();
    static QMWindowHandle *create(const QString &key, QMainWindow *parent);
    static QMWindowHandle *create(QMainWindow *parent);
};

#if QT_CONFIG(settings)
Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, loader,
                          (QMWindowHandleFactoryInterface_IID, QLatin1String("/windowhandles"), Qt::CaseInsensitive))
#endif

QStringList WindowHandleFactory::keys() {
#if QT_CONFIG(settings)
    return loader()->keyMap().values();
#else
    return QStringList();
#endif
}

QString WindowHandleFactory::requested() {
    QByteArray env = qgetenv("QTMEDIATE_WINDOW_HANDLE");
    return env.isNull() ? "NativeWindow" : QString::fromLocal8Bit(env);
}

QMWindowHandle *WindowHandleFactory::create(const QString &key, QMainWindow *parent) {
#if QT_CONFIG(settings)
    if (!key.isEmpty()) {
        QMWindowHandle *inst = qLoadPlugin<QMWindowHandle, QMWindowHandlePlugin>(loader(), key, parent);
        if (inst)
            return inst;
        delete inst;
    }
#else
    Q_UNUSED(key);
#endif
    return nullptr;
}

QMWindowHandle *WindowHandleFactory::create(QMainWindow *parent) {
    return create(requested(), parent);
}

class QMCustomWindowPrivate {
    Q_DECLARE_PUBLIC(QMCustomWindow)
public:
    QMCustomWindowPrivate();
    virtual ~QMCustomWindowPrivate();

    void init();

    QMCustomWindow *q_ptr;
    QMWindowHandle *winHandle;
};


QMCustomWindowPrivate::QMCustomWindowPrivate() {
}

QMCustomWindowPrivate::~QMCustomWindowPrivate() {
    if (winHandle) {
        delete winHandle;
        winHandle = nullptr;
    }
}

void QMCustomWindowPrivate::init() {
    Q_Q(QMCustomWindow);
    winHandle = WindowHandleFactory::create(q);
    if (winHandle) {
        winHandle->setup();
    }

    q->setWindowTitle(qApp->applicationName());
    q->setWindowIcon(qApp->windowIcon());
}

QMCustomWindow::QMCustomWindow(QWidget *parent) : QMCustomWindow(*new QMCustomWindowPrivate(), parent) {
}

QMCustomWindow::~QMCustomWindow() {
}

void QMCustomWindow::setMenuBar(QMenuBar *menuBar) {
    Q_D(QMCustomWindow);
    if (d->winHandle) {
        d->winHandle->setMenuBar(menuBar);
    } else {
        QMainWindow::setMenuBar(menuBar);
    }
}

QMenuBar *QMCustomWindow::menuBar() const {
    Q_D(const QMCustomWindow);
    if (d->winHandle) {
        return d->winHandle->menuBar();
    } else {
        return QMainWindow::menuBar();
    }
}

QMCustomWindow::QMCustomWindow(QMCustomWindowPrivate &d, QWidget *parent) : QMainWindow(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}