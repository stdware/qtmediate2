#ifndef QMSCROLLABLETABWIDGET_P_H
#define QMSCROLLABLETABWIDGET_P_H

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

#include <QMWidgetTools/QMScrollableTabWidget.h>

class QMScrollableTabWidget;

class QMScrollableTabWidgetPrivate {
    Q_DECLARE_PUBLIC(QMScrollableTabWidget)
public:
    QMScrollableTabWidgetPrivate();
    virtual ~QMScrollableTabWidgetPrivate();

    void init();

    QMScrollableTabWidget *q_ptr;

    QHBoxLayout *barLayout;
    QWidget *tabBarWidget;

    QVBoxLayout *mainLayout;

    QMScrollableTabBar *tabBar;
    QStackedWidget *stack;
};

#endif // QMSCROLLABLETABWIDGET_P_H
