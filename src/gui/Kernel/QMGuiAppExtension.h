#ifndef QMGUIAPPEXTENSION_H
#define QMGUIAPPEXTENSION_H

#include <QMCore/QMCoreAppExtension.h>
#include <QMGui/QMGuiGlobal.h>

#ifdef qAppExt
#  undef qAppExt
#endif
#define qAppExt qobject_cast<QMGuiAppExtension *>(QMCoreAppExtension::instance())

class QMGuiAppExtensionPrivate;

class QMGUI_EXPORT QMGuiAppExtension : public QMCoreAppExtension {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMGuiAppExtension)
public:
    explicit QMGuiAppExtension(QObject *parent = nullptr);
    ~QMGuiAppExtension();

public:
    static QRect desktopRect();

protected:
    QMGuiAppExtension(QMGuiAppExtensionPrivate &d, QObject *parent = nullptr);
};

#endif // QMGUIAPPEXTENSION_H
