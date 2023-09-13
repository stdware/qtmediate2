#ifndef CHORUSKIT_QMSVG_P_H
#define CHORUSKIT_QMSVG_P_H

//
//  W A R N I N G !!!
//  -----------------
//
// This file is not part of the QtMediate API. It is used purely as an
// implementation detail. This header file may change from version to
// version without notice, or may even be removed.
//

#include <QIcon>

namespace QMSvgPrivate {

    enum IconEngineHook {
        Create = 0x1000,
        Update,
        GetColor,
        SetColor,
    };

}

#endif // CHORUSKIT_QMSVG_P_H
