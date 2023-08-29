#include "QMCoreAppExtension.h"
#include "private/QMCoreAppExtension_p.h"

#include <QCoreApplication>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QMessageLogger>
#include <QSettings>

#include "QMSystem.h"

#ifdef Q_OS_WINDOWS
#    include <Windows.h>
#elif defined(Q_OS_MACOS)
#    include <CoreFoundation/CoreFoundation.h>
#endif

Q_LOGGING_CATEGORY(qAppExtLog, "qtmediate")

Q_SINGLETON_DECLARE(QMCoreAppExtension);

#ifdef Q_OS_MAC
#    define QT_CONFIG_FILE_DIR  appUpperDir() + "/Resources"
#    define QT_CONFIG_BASE_DIR  appUpperDir()
#    define DEFAULT_LIBRARY_DIR "Frameworks"
#    define DEFAULT_SHARE_DIR   "Resources"
#else
#    define QT_CONFIG_FILE_DIR  QCoreApplication::applicationDirPath()
#    define QT_CONFIG_BASE_DIR  QCoreApplication::applicationDirPath()
#    define DEFAULT_LIBRARY_DIR "lib"
#    define DEFAULT_SHARE_DIR   "share"
#endif

static QString appUpperDir() {
    static QString dir = QDir::cleanPath(QCoreApplication::applicationDirPath() + "/..");
    return dir;
}

QMCoreAppExtensionPrivate::QMCoreAppExtensionPrivate() {
    isAboutToQuit = false;
}

QMCoreAppExtensionPrivate::~QMCoreAppExtensionPrivate() {
}

void QMCoreAppExtensionPrivate::init() {
    Q_Q(QMCoreAppExtension);

    // Basic directories
    appDataDir = QMFs::appDataPath() + "/ChorusKit/" + qApp->applicationName();
    tempDir = QDir::tempPath() + "/ChorusKit/" + qApp->applicationName();

    libDir = appUpperDir() + "/" + DEFAULT_LIBRARY_DIR;
    shareDir = appUpperDir() + "/" + DEFAULT_SHARE_DIR;

    configVars.addHash(QMSimpleVarExp::SystemValues());
    configVars.add("DEFAULT_APPDATA", appDataDir);
    configVars.add("DEFAULT_TEMP", tempDir);

    // Read configurations
    if (readConfiguration(QMCoreAppExtension::configurationPath(QSettings::SystemScope))) {
        qCDebug(qAppExtLog) << "system configuration file found";
    } else {
        qCDebug(qAppExtLog) << "system configuration file not found";
    }

    if (readConfiguration(QMCoreAppExtension::configurationPath(QSettings::UserScope))) {
        qCDebug(qAppExtLog) << "user configuration file found";
    } else {
        qCDebug(qAppExtLog) << "user configuration file not found";
    }

    // Polymorphic factory

    // Create instances
    s_dec = createDecorator(q);
    qCDebug(qAppExtLog) << s_dec->metaObject()->className() << "initializing.";

    QObject::connect(qApp, &QCoreApplication::aboutToQuit, this, [this]() {
        isAboutToQuit = true; //
    });

    // Add plugin paths
    for (const auto &path : qAsConst(pluginPaths))
        QCoreApplication::addLibraryPath(path);

    // Add translation paths
    for (const auto &path : qAsConst(translationPaths))
        s_dec->addTranslationPath(path);

    // Set default app share dir and app plugins dir
    appShareDir = shareDir
#ifndef Q_OS_MAC
                  + "/" + qApp->applicationName()
#endif
        ;

    appPluginsDir =
#ifdef Q_OS_MAC
        appUpperDir() + "/Plugins"
#else
        libDir + "/" + QCoreApplication::applicationName() + "/plugins"
#endif
        ;
}

bool QMCoreAppExtensionPrivate::readConfiguration(const QString &fileName) {
    // Read
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    auto data = file.readAll();
    file.close();

    // Parse
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject()) {
        return false;
    }

    auto obj = doc.object();

    QJsonValue value;

    value = obj.value("AppFont");
    if (!value.isUndefined()) {
        if (value.isString()) {
            appFont.insert("Family", value.toString());
        } else if (value.isObject()) {
            appFont = value.toObject();
        }
    }

    QString prefix = QMCoreAppExtension::configurationBasePrefix();
    value = obj.value("Prefix");
    if (value.isString()) {
        auto dir = configVars.parse(value.toString());
        if (QMFs::isPathRelative(dir)) {
            dir = QT_CONFIG_BASE_DIR + "/" + dir;
        }

        QFileInfo info(dir);
        if (info.isDir()) {
            prefix = info.canonicalFilePath();
        }
    }

    auto getDir = [prefix, this](QString path) {
        path = configVars.parse(path);
        if (QMFs::isPathRelative(path)) {
            path = prefix + "/" + path;
        }
        QFileInfo info(path);
        if (!info.isDir())
            return QString();
        return info.canonicalFilePath();
    };

    auto getDirs = [getDir](QStringList &paths, const QJsonValue &value) {
        switch (value.type()) {
            case QJsonValue::String: {
                auto dir = getDir(value.toString());
                if (!dir.isEmpty()) {
                    paths << dir;
                }
                break;
            }
            case QJsonValue::Array: {
                for (const auto &item : value.toArray()) {
                    if (!item.isString())
                        continue;

                    auto dir = getDir(item.toString());
                    if (!dir.isEmpty()) {
                        paths << dir;
                    }
                }
                break;
            }
            default:
                break;
        }
    };

    value = obj.value("Temp");
    if (value.isString()) {
        auto dir = getDir(value.toString());
        if (!dir.isEmpty()) {
            tempDir = dir;
        }
    }

    value = obj.value("Libraries");
    if (value.isString()) {
        auto dir = getDir(value.toString());
        if (!dir.isEmpty()) {
            libDir = dir;
        }
    }

    value = obj.value("Share");
    if (value.isString()) {
        auto dir = getDir(value.toString());
        if (!dir.isEmpty()) {
            shareDir = dir;
        }
    }

    getDirs(pluginPaths, obj.value("Plugins"));
    getDirs(translationPaths, obj.value("Translations"));
    getDirs(themePaths, obj.value("Themes"));
    getDirs(fontPaths, obj.value("Fonts"));

    return true;
}

QMCoreDecoratorV2 *QMCoreAppExtensionPrivate::createDecorator(QObject *parent) {
    return new QMCoreDecoratorV2(parent);
}

#if defined(Q_OS_WINDOWS) || defined(Q_OS_MAC)

void QMCoreAppExtensionPrivate::osMessageBox_helper(void *winHandle, QMCoreAppExtension::MessageBoxFlag flag,
                                                    const QString &title, const QString &text) const {
#    ifdef Q_OS_WINDOWS
    int winFlag;
    switch (flag) {
        case QMCoreAppExtension::Critical:
            winFlag = MB_ICONERROR;
            break;
        case QMCoreAppExtension::Warning:
            winFlag = MB_ICONWARNING;
            break;
        case QMCoreAppExtension::Question:
            winFlag = MB_ICONQUESTION;
            break;
        case QMCoreAppExtension::Information:
            winFlag = MB_ICONINFORMATION;
            break;
    };

    ::MessageBoxW(static_cast<HWND>(winHandle), text.toStdWString().data(), title.toStdWString().data(),
                  MB_OK
#        ifdef QTMEDIATE_WIN32_MSGBOX_TOPMOST
                      | MB_TOPMOST
#        endif
                      | MB_SETFOREGROUND | winFlag);
#    else
    // From
    // https://web.archive.org/web/20111127025605/http://jorgearimany.blogspot.com/2010/05/messagebox-from-windows-to-mac.html
    CFOptionFlags result;
    int level = 0;
    switch (flag) {
        case QMCoreConsole::Critical:
            level = 2;
            break;
        case QMCoreConsole::Warning:
            level = 1;
            break;
        case QMCoreConsole::Question:
            level = 3;
            break;
        case QMCoreConsole::Information:
            level = 0;
            break;
    };
    CFUserNotificationDisplayAlert(0,     // no timeout
                                   level, // change it depending message_type flags ( MB_ICONASTERISK.... etc.)
                                   NULL,  // icon url, use default, you can change it depending message_type flags
                                   NULL,  // not used
                                   NULL,  // localization of strings
                                   title.toCFString(), // header text
                                   text.toCFString(),  // message text
                                   NULL,               // default "ok" text in button
                                   NULL,               // alternate button title
                                   NULL,               // other button title, null--> no other button
                                   &result             // response flags
    );
#    endif
}

#endif

QMCoreAppExtension::QMCoreAppExtension(QObject *parent) : QMCoreAppExtension(*new QMCoreAppExtensionPrivate(), parent) {
}

QMCoreAppExtension::~QMCoreAppExtension() {
    destruct();
}

void QMCoreAppExtension::MsgBox(QObject *parent, MessageBoxFlag flag, const QString &title, const QString &text) const {
    Q_UNUSED(parent);

#if defined(Q_OS_WINDOWS) || defined(Q_OS_MAC)
    Q_D(const QMCoreAppExtension);
    d->osMessageBox_helper(nullptr, flag, title, text);
#else
    switch (flag) {
        case Critical:
        case Warning:
            fputs(qPrintable(text), stderr);
            break;
        case Question:
        case Information:
            fputs(qPrintable(text), stdout);
            break;
    };
#endif
}

bool QMCoreAppExtension::isAboutToQuit() const {
    Q_D(const QMCoreAppExtension);
    return d->isAboutToQuit;
}

QString QMCoreAppExtension::appDataDir() const {
    Q_D(const QMCoreAppExtension);
    return d->appDataDir;
}

void QMCoreAppExtension::setAppDataDir(const QString &dir) {
    Q_D(QMCoreAppExtension);
    d->appDataDir = dir;
}

QString QMCoreAppExtension::tempDir() const {
    Q_D(const QMCoreAppExtension);
    return d->tempDir;
}

void QMCoreAppExtension::setTempDir(const QString &dir) {
    Q_D(QMCoreAppExtension);
    d->tempDir = dir;
}

QString QMCoreAppExtension::libDir() const {
    Q_D(const QMCoreAppExtension);
    return d->libDir;
}

void QMCoreAppExtension::setLibDir(const QString &dir) {
    Q_D(QMCoreAppExtension);
    d->libDir = dir;
}

QString QMCoreAppExtension::shareDir() const {
    Q_D(const QMCoreAppExtension);
    return d->shareDir;
}

void QMCoreAppExtension::setShareDir(const QString &dir) {
    Q_D(QMCoreAppExtension);
    d->shareDir = dir;
}

QString QMCoreAppExtension::appShareDir() const {
    Q_D(const QMCoreAppExtension);
    return d->appShareDir;
}

void QMCoreAppExtension::setAppShareDir(const QString &dir) {
    Q_D(QMCoreAppExtension);
    d->appShareDir = dir;
}

QString QMCoreAppExtension::appPluginsDir() const {
    Q_D(const QMCoreAppExtension);
    return d->appPluginsDir;
}

void QMCoreAppExtension::setAppPluginsDir(const QString &dir) {
    Q_D(QMCoreAppExtension);
    d->appPluginsDir = dir;
}

bool QMCoreAppExtension::createDataAndTempDirs() const {
    auto func = [this](const QString &path) {
        qCDebug(qAppExtLog) << "qmcorehost:" << (QMFs::isDirExist(path) ? "find" : "create") << "directory" << path;
        if (!QMFs::mkDir(path)) {
            MsgBox(nullptr, Critical, qApp->applicationName(),
                   QString("Failed to create %1 directory!").arg(QMFs::PathFindFileName(path)));
            return false;
        }
        return true;
    };

    if (!func(appDataDir())) {
        return false;
    }

    if (!func(tempDir())) {
        return false;
    }

    return true;
}

QString QMCoreAppExtension::configurationPath(QSettings::Scope scope) {
    return QT_CONFIG_FILE_DIR + "/" + (scope == QSettings::SystemScope ? "qtmediate.json" : "qtmediate.user.json");
}

QString QMCoreAppExtension::configurationBasePrefix() {
    return QT_CONFIG_BASE_DIR;
}

QMCoreAppExtension::QMCoreAppExtension(QMCoreAppExtensionPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
    construct();

    d.q_ptr = this;
    d.init();
}