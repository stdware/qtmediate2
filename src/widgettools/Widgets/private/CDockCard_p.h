#ifndef CDOCKCARD_P_H
#define CDOCKCARD_P_H

//
//  W A R N I N G !!!
//  -----------------
//
// This file is not part of the QtMediate API. It is used purely as an
// implementation detail. This header file may change from version to
// version without notice, or may even be removed.
//

#include "QMView.h"

#include "../CDockCard.h"
#include "QMFloatingWindowHelper.h"

class CDockCardPrivate {
    Q_DECLARE_PUBLIC(CDockCard)
public:
    CDockCardPrivate();
    ~CDockCardPrivate();

    void init();

    QPixmap cardShot() const;

    CDockCard *q_ptr;

    CDockTabBar *m_tabBar;
    QRect oldGeometry;

    Qt::Orientation m_orientation;
    QSizePolicy m_sizePolicyH, m_sizePolicyV;

    QPoint m_dragPos;
    QSize m_dragOffset;

    bool m_readyDrag;

    QWidget *m_container;
    QWidget *m_widget;

    bool m_closing;
    CDockCard::ViewMode m_viewMode;

    QMFloatingWindowHelper *m_floatingHelper;

    static QMenu *createViewModeMenu(CDockCard *card);
};

#endif // CDOCKCARD_P_H
