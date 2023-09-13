#ifndef QMGUIAPPEXTENSION_P_H
#define QMGUIAPPEXTENSION_P_H

//
//  W A R N I N G !!!
//  -----------------
//
// This file is not part of the QtMediate API. It is used purely as an
// implementation detail. This header file may change from version to
// version without notice, or may even be removed.
//

#include <QAtomicInt>

#include <QMCore/private/QMCoreAppExtension_p.h>

#include <QMGui/QMGuiAppExtension.h>

class QMGUI_EXPORT QMGuiAppExtensionPrivate : public QMCoreAppExtensionPrivate {
    Q_DECLARE_PUBLIC(QMGuiAppExtension)
public:
    QMGuiAppExtensionPrivate();
    ~QMGuiAppExtensionPrivate();

    void init();

    QMCoreDecoratorV2 *createDecorator(QObject *parent) override;

    static QAtomicInt globalImageCacheSerialNum;
};

#endif // QMGUIAPPEXTENSION_P_H
