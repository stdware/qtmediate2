#ifndef QMSVGX_H
#define QMSVGX_H

#include <QHash>
#include <QIcon>
#include <QStringList>

#include <QMCore/QMNamespace.h>
#include <QMGui/QMGuiGlobal.h>

namespace QMSvgx {

    QMGUI_EXPORT QIcon create(const QHash<QM::ButtonState, QString> &fileMap,
                              const QHash<QM::ButtonState, QString> &colorMap);

    QMGUI_EXPORT QIcon create(const QString &file, const QString &checkedFile,
                              const QString &color = "auto");

    QMGUI_EXPORT bool setCurrentState(QIcon *icon, QM::ButtonState state);

    QMGUI_EXPORT QM::ButtonState getCurrentState(QIcon *icon);

    QMGUI_EXPORT void setColor(QIcon *icon, const QString &color);

    QMGUI_EXPORT QString queryColor(QIcon *icon, QM::ButtonState state);

}

#endif // QMSVGX_H
