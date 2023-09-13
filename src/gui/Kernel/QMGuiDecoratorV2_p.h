#ifndef QMGUIDECORATORV2_P_H
#define QMGUIDECORATORV2_P_H

//
//  W A R N I N G !!!
//  -----------------
//
// This file is not part of the QtMediate API. It is used purely as an
// implementation detail. This header file may change from version to
// version without notice, or may even be removed.
//

#include <QMCore/private/QMCoreDecoratorV2_p.h>

#include <QMGui/QMGuiDecoratorV2.h>

class QMGUI_EXPORT QMGuiDecoratorV2Private : public QMCoreDecoratorV2Private {
    Q_DECLARE_PUBLIC(QMGuiDecoratorV2)
public:
    QMGuiDecoratorV2Private();
    ~QMGuiDecoratorV2Private();

    void init();
};

#endif // QMGUIDECORATORV2_P_H
