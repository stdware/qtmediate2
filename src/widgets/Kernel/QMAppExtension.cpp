#include "QMAppExtension.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QMessageBox>

#include <private/qapplication_p.h>
#include <private/qshortcutmap_p.h>

#include "QMDecoratorV2.h"

#include "QMAppExtension_p.h"

#ifdef _WIN32
#  include <Windows.h>
#  define OS_MAX_PATH MAX_PATH
#else
#  include <dlfcn.h>
#  include <limits.h>
#  include <stdio.h>
#  include <string.h>

#  define OS_MAX_PATH PATH_MAX
#endif

static QString GetLibraryPath() {
#ifdef _WIN32
    wchar_t buf[OS_MAX_PATH + 1] = {0};
    HMODULE hm = nullptr;
    if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                                GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                            (LPCWSTR) &GetLibraryPath, &hm) ||
        !GetModuleFileNameW(hm, buf, sizeof(buf))) {
        return {};
    }
    return QDir::fromNativeSeparators(QString::fromStdWString(buf));
#else
    Dl_info dl_info;
    dladdr((void *) GetLibraryPath, &dl_info);
    auto buf = dl_info.dli_fname;
    return QString::fromStdString(buf);
#endif
}

namespace {

    class ShortcutFilter : public QObject {
    public:
        ShortcutFilter(QWidget *org) : m_org(org), m_handled(false) {
        }

        inline bool handled() const {
            return m_handled;
        }

    protected:
        bool eventFilter(QObject *watched, QEvent *event) override {
            if (event->type() == QEvent::Shortcut) {
                QApplicationPrivate::active_window = m_org;
                m_handled = true;
            }
            return QObject::eventFilter(watched, event);
        }

    private:
        QWidget *m_org;
        bool m_handled;
    };

    // This function hacks the QApplication data structure and simply changes
    // the `active_window` pointer temporarily to make the shortcut map transmit the
    // event to the target window.
    static void forwardShortcut(QKeyEvent *event, QWidget *window) {
        event->accept();

        // Hack `active_window` temporarily
        auto org = QApplicationPrivate::active_window;
        QApplicationPrivate::active_window = window;

        // Make sure to restore `active_window` right away if shortcut matches
        ShortcutFilter filter(org);
        qApp->installEventFilter(&filter);

        // Retransmit event
        QKeyEvent keyEvent(QEvent::ShortcutOverride, event->key(), event->modifiers(),
                           event->nativeScanCode(), event->nativeVirtualKey(),
                           event->nativeModifiers(), event->text(), event->isAutoRepeat(),
                           event->count());
        QGuiApplicationPrivate::instance()->shortcutMap.tryShortcut(&keyEvent);

        if (!filter.handled()) {
            QApplicationPrivate::active_window = org;
        }
    }

}

QMAppExtensionPrivate::QMAppExtensionPrivate() {
}

QMAppExtensionPrivate::~QMAppExtensionPrivate() {
}

void QMAppExtensionPrivate::init() {
    // Add theme paths
    for (const auto &path : qAsConst(themePaths))
        qIDec->addThemePath(path);

    // Add plugin path
    QApplication::addLibraryPath(
        QDir::cleanPath(GetLibraryPath() + "/../../lib/qtmediate/plugins"));
}

QMCoreDecoratorV2 *QMAppExtensionPrivate::createDecorator(QObject *parent) {
    return new QMDecoratorV2(parent);
}

QMAppExtension::QMAppExtension(QObject *parent)
    : QMAppExtension(*new QMAppExtensionPrivate(), parent) {
}

QMAppExtension::~QMAppExtension() {
}

void QMAppExtension::showMessage(QObject *parent, MessageBoxFlag flag, const QString &title,
                                 const QString &text) const {
    Q_D(const QMAppExtension);

    QWidget *w = nullptr;
    if (parent && parent->isWidgetType()) {
        w = qobject_cast<QWidget *>(parent)->window();
    }

#if defined(Q_OS_WINDOWS)
    d->osMessageBox_helper(w ? (HWND) w->winId() : nullptr, flag, title, text);
#elif defined(Q_OS_MAC)
    d->osMessageBox_helper(nullptr, flag, title, text);
#else
    switch (flag) {
        case Critical:
            QMessageBox::critical(w, title, text);
            break;
        case Warning:
            QMessageBox::warning(w, title, text);
            break;
        case Question:
            QMessageBox::question(w, title, text);
            break;
        case Information:
            QMessageBox::information(w, title, text);
            break;
    };
#endif
}


void QMAppExtension::installShortcutForwarder(QWidget *w, QWidget *target,
                                              const std::function<bool()> &predicate) {
    Q_D(QMAppExtension);
    if (!w) {
        return;
    }

    QMetaObject::Connection conn = connect(w, &QObject::destroyed, this, [this, w]() {
        removeEventFilter(w); //
    });
    d->shortcutForwarders.insert(w, {w, target, predicate, conn});
}

void QMAppExtension::removeShortcutForwarder(QWidget *w) {
    Q_D(QMAppExtension);
    auto it = d->shortcutForwarders.find(w);
    if (it == d->shortcutForwarders.end())
        return;
    disconnect(it->conn);
    d->shortcutForwarders.erase(it);
}

bool QMAppExtension::eventFilter(QObject *obj, QEvent *event) {
    Q_D(QMAppExtension);
    switch (event->type()) {
        case QEvent::KeyPress: {
            if (!obj->isWidgetType())
                break;

            auto w = static_cast<QWidget *>(obj);
            auto it = d->shortcutForwarders.find(w);
            if (it == d->shortcutForwarders.end())
                break;

            const auto &r = it.value();
            if (r.predicate && r.target && r.predicate()) {
                forwardShortcut(static_cast<QKeyEvent *>(event), r.target);
                return true;
            }
            break;
        }
        default:
            break;
    }
    return QMGuiAppExtension::eventFilter(obj, event);
}

QMAppExtension::QMAppExtension(QMAppExtensionPrivate &d, QObject *parent)
    : QMGuiAppExtension(d, parent) {
    d.init();
}