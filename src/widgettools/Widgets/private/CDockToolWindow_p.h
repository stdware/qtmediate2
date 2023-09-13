#ifndef CDOCKTOOLWINDOWPRIVATE_H
#define CDOCKTOOLWINDOWPRIVATE_H

//
//  W A R N I N G !!!
//  -----------------
//
// This file is not part of the QtMediate API. It is used purely as an
// implementation detail. This header file may change from version to
// version without notice, or may even be removed.
//

#include <QSet>

#include "CDockToolWindow.h"

class CDockToolWindowPrivate {
    Q_DECLARE_PUBLIC(CDockToolWindow)
public:
    CDockToolWindowPrivate();
    virtual ~CDockToolWindowPrivate();

    void init();

    void setCard(CDockCard *card);

    CDockToolWindow *q_ptr;

    CDockCard *card;

    QMargins resizeMargins;
    QMargins orgResizeMargins;
};

#endif // CDOCKTOOLWINDOWPRIVATE_H