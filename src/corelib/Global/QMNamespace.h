#ifndef QMNAMESPACE_H
#define QMNAMESPACE_H

#include <QObject>

#include <QMCore/QMGlobal.h>

namespace QM {

    Q_NAMESPACE_EXPORT(QMCORE_EXPORT)

    enum Direction {
        Forward,
        Backward,
    };
    Q_ENUM_NS(Direction)

    enum Priority {
        Primary = 16,
        Secondary = 32,
    };
    Q_ENUM_NS(Priority)

    enum ButtonState {
        ButtonNormal = 1,
        ButtonHover,
        ButtonPressed,
        ButtonDisabled,
        ButtonNormalChecked,
        ButtonHoverChecked,
        ButtonPressedChecked,
        ButtonDisabledChecked,
    };
    Q_ENUM_NS(ButtonState)

}

#endif // QMNAMESPACE_H
