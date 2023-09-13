#ifndef QSCROLLABLETABBARTABPRIVATE_H
#define QSCROLLABLETABBARTABPRIVATE_H

//
//  W A R N I N G !!!
//  -----------------
//
// This file is not part of the QtMediate API. It is used purely as an
// implementation detail. This header file may change from version to
// version without notice, or may even be removed.
//

#include <QAbstractButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QVariant>

class QScrollableTabBarTab;

class QScrollableTabBarTabPrivate {
    Q_DECLARE_PUBLIC(QScrollableTabBarTab)
public:
    QScrollableTabBarTabPrivate();
    virtual ~QScrollableTabBarTabPrivate();

    void init();

    void updateIconAndText();

    QScrollableTabBarTab *q_ptr;

    QHBoxLayout *layout;

    QIcon icon;
    QSize iconSize;
    QMargins iconMargins;

    QString text;
    QMargins textMargins;

    QSpacerItem *iconTextItem;
    QAbstractButton *closeButton;

    QVariant data;
};

#endif // QSCROLLABLETABBARTABPRIVATE_H
