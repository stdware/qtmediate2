#ifndef QMVIEW_H
#define QMVIEW_H

#include <QPen>
#include <QPixmap>

#include <QMWidgets/QMWidgetsGlobal.h>

namespace QMView {

    QMWIDGETS_EXPORT void waitToShow(QWidget *w);

    QMWIDGETS_EXPORT void centralizeWindow(QWidget *w, QSizeF ratio = QSizeF(-1, -1));

    QMWIDGETS_EXPORT void raiseWindow(QWidget *w);

    QMWIDGETS_EXPORT void fixWindowPos(QWidget *w);

    QMWIDGETS_EXPORT QWidget *implicitMouseGrabber();

    QMWIDGETS_EXPORT void setImplicitMouseGrabber(QWidget *w);

}

#endif // QMVIEW_H
