#ifndef QMSHORTCUTCONTEXT_P_H
#define QMSHORTCUTCONTEXT_P_H

//
//  W A R N I N G !!!
//  -----------------
//
// This file is not part of the QtMediate API. It is used purely as an
// implementation detail. This header file may change from version to
// version without notice, or may even be removed.
//

#include <QHash>
#include <QSet>

#include <QMChronSet.h>

#include "QMShortcutContext.h"

class QMShortcutContextPrivate : public QObject {
    Q_OBJECT
public:
    QMShortcutContextPrivate(QMShortcutContext *q);
    ~QMShortcutContextPrivate();

    QMShortcutContext *q;

    QMap<int, QMChronSet<QWidget *>> contexts;

    struct WidgetData {
        int priority;
        QMChronSet<QAction *> actions;
    };

    QHash<QWidget *, WidgetData> widgets;

    struct ActionData {
        QSet<QWidget *> widgets;
        QList<QKeySequence> keys;
    };
    QHash<QAction *, ActionData> actions;

    bool actionChanged;
    bool fixing;

    void addWidget(QWidget *w, int priority);
    void removeWidget(QWidget *w);
    void postFixTask();

    void doFix();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void _q_widgetDestroyed(QObject *obj);
};

#endif // QMSHORTCUTCONTEXT_P_H
