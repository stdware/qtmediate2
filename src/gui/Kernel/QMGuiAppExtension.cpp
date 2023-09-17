#include "QMGuiAppExtension.h"

#include <QDir>
#include <QFontDatabase>
#include <QGuiApplication>
#include <QScreen>

#include <QMCore/QMSystem.h>

#include "QMGuiDecoratorV2.h"

#include "QMGuiAppExtension_p.h"

// ======================================================================================
// QMetaTypeImpl

#include "QFontInfoEx.h"
#include "QMarginsImpl.h"
#include "QPenInfo.h"
#include "QPixelSize.h"
#include "QRectInfo.h"

#include "QCssValueList.h"
#include "QCssValueMap.h"

#include "QMCss.h"

namespace QMetaTypeImpl {

    template <class T>
    static void registerFromString() {
        QMetaType::registerConverter<QString, T>(T::fromString);
    }

    template <class T>
    static void registerFromStringList() {
        QMetaType::registerConverter<QStringList, T>(T::fromStringList);
    }

    template <class T>
    static void registerMetaTypeName() {
        QMCssType::registerMetaTypeName(qMetaTypeId<T>(), T::metaFunctionName());
    }

    void Register() {
        static bool m_registered = false;
        if (m_registered) {
            return;
        }
        m_registered = true;

        // Register converters
        QMetaType::registerConverter<QStringList, QMargins>(QMarginsImpl::fromStringList);
        QMetaType::registerConverter<QString, QMargins>(QMarginsImpl::fromString);

        QMetaType::registerConverter<QPixelSize, int>([](const QPixelSize &size) -> int {
            return size.value(); //
        });
        QMetaType::registerConverter<QPixelSize, double>([](const QPixelSize &size) -> double {
            return size.valueF(); //
        });
        registerFromString<QPixelSize>();

        registerFromStringList<QPenInfo>();
        registerFromStringList<QRectInfo>();
        registerFromStringList<QFontInfoEx>();

        registerFromStringList<QCssValueList>();
        registerFromStringList<QCssValueMap>();

        // Register names
        QMCssType::registerMetaTypeName(qMetaTypeId<QMargins>(), QMarginsImpl::metaFunctionName());

        registerMetaTypeName<QPenInfo>();
        registerMetaTypeName<QRectInfo>();
        registerMetaTypeName<QFontInfoEx>();
        registerMetaTypeName<QCssValueList>();
        registerMetaTypeName<QCssValueMap>();
    }

}

// QMetaTypeImpl
// ======================================================================================

QMGuiAppExtensionPrivate::QMGuiAppExtensionPrivate() {
}

QMGuiAppExtensionPrivate::~QMGuiAppExtensionPrivate() {
}

void QMGuiAppExtensionPrivate::init() {
    // This is necessary for macOS platforms, so that QIcon will return a
    // pixmap with correct devicePixelRatio when using QIcon::pixmap().
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    // Must register all meta-types converters in advance
    QMetaTypeImpl::Register();

    for (const auto &path : qAsConst(fontPaths)) {
        QDir directory(path);
        QStringList fontFiles = directory.entryList({"*.ttf", "*.otf"}, QDir::Files);
        foreach (const QString &fontFile, fontFiles) {
            QString fontFilePath = directory.absoluteFilePath(fontFile);
            int fontId = QFontDatabase::addApplicationFont(fontFilePath);
            if (fontId != -1) {
                QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
                qCDebug(qAppExtLog) << "add font families: " << fontFamilies.join(", ");
            }
        }
    }

    QFont font = QGuiApplication::font();

#ifdef Q_OS_WINDOWS
    // font.setFamily("Microsoft YaHei"); // Consider not using MSYH on non-Chinese Windows platform
#endif

    // Init font
    if (!appFont.isEmpty()) {
        QJsonValue value;
        value = appFont.value("Family");
        if (value.isString()) {
            font.setFamily(value.toString());
        }

        value = appFont.value("Size");
        if (value.isDouble()) {
            double ratio = QGuiApplication::primaryScreen()->logicalDotsPerInch() / QMOs::unitDpi();
            font.setPixelSize(int(value.toDouble() * ratio));
        }

        value = appFont.value("Weight");
        if (value.isDouble()) {
            font.setWeight(value.toInt());
        }

        value = appFont.value("Italic");
        if (value.isBool()) {
            font.setItalic(value.toBool());
        }
    }

    font.setStyleStrategy(QFont::PreferAntialias);
    qApp->setFont(font);
}

QAtomicInt QMGuiAppExtensionPrivate::globalImageCacheSerialNum;

QMCoreDecoratorV2 *QMGuiAppExtensionPrivate::createDecorator(QObject *parent) {
    return new QMGuiDecoratorV2(parent);
}

/*!
    \class QMGuiAppExtension
    \brief The QMGuiAppExtension class is the global resources manager for \c qtmediate framework.

    All QtMediate non-qt types will be registered to QMetaType system when QMGuiAppExtension
    constructs.
*/

/*!
    Constructor.
*/
QMGuiAppExtension::QMGuiAppExtension(QObject *parent)
    : QMGuiAppExtension(*new QMGuiAppExtensionPrivate(), parent) {
}

/*!
    Destructor.
*/
QMGuiAppExtension::~QMGuiAppExtension() {
}

/*!
    Returns the default desktop available geometry.
*/
QRect QMGuiAppExtension::desktopRect() {
    return QGuiApplication::primaryScreen()->availableGeometry();
}

/*!
    \internal
*/
QMGuiAppExtension::QMGuiAppExtension(QMGuiAppExtensionPrivate &d, QObject *parent)
    : QMCoreAppExtension(d, parent) {
    d.init();
}