#ifndef QMCSS_H
#define QMCSS_H

#include <QColor>
#include <QHash>
#include <QSize>
#include <QStringList>
#include <QVariant>

#include <QMGui/QMGuiGlobal.h>

namespace QMCss {

    QMGUI_EXPORT QList<int> parseSizeValueList(const QString &s);

    QMGUI_EXPORT QList<double> parseSizeFValueList(const QString &s);

    QMGUI_EXPORT QStringList parseStringValueList(const QString &s);

    QMGUI_EXPORT bool parseBoolean(const QString &s, bool *ok = nullptr);

    QMGUI_EXPORT QColor parseColor(const QString &s);

    QMGUI_EXPORT QString colorName(const QColor &color);

    QMGUI_EXPORT int indexOfEqSign(const QString &s);

    enum FallbackOption {
        FO_Value,
        FO_Reference,
    };
    QMGUI_EXPORT QHash<QString, QString> parseArgList( //
        const QString &s,                              //
        const QStringList &expectedKeys,               //
        const QHash<QString, QPair<QString, FallbackOption>> &fallbacks);

    QMGUI_EXPORT bool parseButtonStateList(const QString &s, QString arr[], bool resolveFallback);

}

class QMGUI_EXPORT QMCssType {
public:
    static bool registerMetaTypeName(int id, const std::string &name);
    static bool unregisterMetaTypeName(int id);
    static bool unregisterMetaTypeName(const std::string &name);

    static std::string metaTypeName(int id);
    static int metaTypeId(const std::string &name);

    static QVariant parse(const QString &s);
};

#endif // QMCSS_H
