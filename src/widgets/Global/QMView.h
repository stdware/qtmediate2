#ifndef QMVIEW_H
#define QMVIEW_H

#include <QPen>
#include <QPixmap>
#include <QKeyEvent>
#include <QAbstractButton>

#include <QMCore/QMNamespace.h>
#include <QMWidgets/QMWidgetsGlobal.h>

namespace QMView {

    QMWIDGETS_EXPORT void waitToShow(QWidget *window);

    QMWIDGETS_EXPORT void centralizeWindow(QWidget *window, QSizeF ratio = QSizeF(-1, -1));

    QMWIDGETS_EXPORT void raiseWindow(QWidget *window);

    QMWIDGETS_EXPORT void fixWindowPos(QWidget *window);

    QMWIDGETS_EXPORT void forwardShortcut(QKeyEvent *event, QWidget *window);

    QMWIDGETS_EXPORT QM::ButtonState buttonState(QAbstractButton *button);

}

#endif // QMVIEW_H
