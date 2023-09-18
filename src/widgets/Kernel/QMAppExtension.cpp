#include "QMAppExtension.h"
#include "QMAppExtension_p.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QMessageBox>

#include "QMDecoratorV2.h"

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

/*!
    \class QMAppExtension
    
    The QMGuiAppExtension class is the global resources manager for \c QtMediate framework.
*/

/*
    Constructor.
*/
QMAppExtension::QMAppExtension(QObject *parent)
    : QMAppExtension(*new QMAppExtensionPrivate(), parent) {
}

/*!
    Destructor.
*/
QMAppExtension::~QMAppExtension() {
}

/*!
    Shows system message box if supported, otherwise shows Qt message box.
*/
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

/*!
    \internal
*/
QMAppExtension::QMAppExtension(QMAppExtensionPrivate &d, QObject *parent)
    : QMGuiAppExtension(d, parent) {
    d.init();
}