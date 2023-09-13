#ifndef QSCROLLABLETABWIDGETPRIVATE_H
#define QSCROLLABLETABWIDGETPRIVATE_H

//
//  W A R N I N G !!!
//  -----------------
//
// This file is not part of the QtMediate API. It is used purely as an
// implementation detail. This header file may change from version to
// version without notice, or may even be removed.
//

#include <QDockWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QtGlobal>

#include "QScrollableTabBar.h"

class QScrollableTabWidget;

class QScrollableTabWidgetPrivate {
    Q_DECLARE_PUBLIC(QScrollableTabWidget)
public:
    QScrollableTabWidgetPrivate();
    virtual ~QScrollableTabWidgetPrivate();

    void init();

    QScrollableTabWidget *q_ptr;

    QHBoxLayout *barLayout;
    QWidget *tabBarWidget;

    QVBoxLayout *mainLayout;

    QScrollableTabBar *tabBar;
    QStackedWidget *stack;
};

#endif // QSCROLLABLETABWIDGETPRIVATE_H
