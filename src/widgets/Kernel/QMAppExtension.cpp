#include "QMAppExtension.h"

#include <QApplication>
#include <QDebug>
#include <QDir>

#include "QMConsole.h"
#include "QMDecoratorV2.h"
#include "QMSystem.h"
#include "private/QMAppExtension_p.h"

#ifdef _WIN32
#    include <Windows.h>
#    define OS_MAX_PATH MAX_PATH
#else
#    include <dlfcn.h>
#    include <limits.h>
#    include <stdio.h>
#    include <string.h>

#    define OS_MAX_PATH PATH_MAX
#endif

static QString GetLibraryPath() {
#ifdef _WIN32
    wchar_t buf[OS_MAX_PATH + 1] = {0};
    HMODULE hm = nullptr;
    if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
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

QMCoreConsole *QMInitFactory::createConsole(QObject *parent) {
    return new QMConsole(parent);
}

QMCoreDecoratorV2 *QMInitFactory::createDecorator(QObject *parent) {
    return new QMDecoratorV2(parent);
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
    QApplication::addLibraryPath(QDir::cleanPath(GetLibraryPath() + "/../../lib/qtmediate/plugins"));
}

QMCoreInitFactory *QMAppExtensionPrivate::createFactory() {
    return new QMInitFactory();
}

QMAppExtension::QMAppExtension(QObject *parent) : QMAppExtension(*new QMAppExtensionPrivate(), parent) {
}

QMAppExtension::~QMAppExtension() {
}

QMAppExtension::QMAppExtension(QMAppExtensionPrivate &d, QObject *parent) : QMGuiAppExtension(d, parent) {
    d.init();
}