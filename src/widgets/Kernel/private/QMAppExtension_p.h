#ifndef QMAPPEXTENSION_P_H
#define QMAPPEXTENSION_P_H

#include "../QMAppExtension.h"
#include "private/QMGuiAppExtension_p.h"

class QMAppExtensionPrivate : public QMGuiAppExtensionPrivate {
    Q_DECLARE_PUBLIC(QMAppExtension)
public:
    QMAppExtensionPrivate();
    ~QMAppExtensionPrivate();

    void init();
    
    QMCoreDecoratorV2 *createDecorator(QObject *parent) override;
};

#endif // QMAPPEXTENSION_P_H
