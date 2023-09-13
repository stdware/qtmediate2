#ifndef SVGXICONENGINE_P_H
#define SVGXICONENGINE_P_H

//
//  W A R N I N G !!!
//  -----------------
//
// This file is not part of the QtMediate API. It is used purely as an
// implementation detail. This header file may change from version to
// version without notice, or may even be removed.
//

#include <QIcon>
#include <QSvgRenderer>

#include "QMNamespace.h"

class SvgxIconEnginePrivate {
public:
    SvgxIconEnginePrivate();
    ~SvgxIconEnginePrivate();

public:
    QString pmcKey(const QSize &size, QIcon::Mode mode, QIcon::State state);

    void stepSerialNum();
    bool tryLoad(QSvgRenderer *renderer, QIcon::Mode mode, QIcon::State state);
    QIcon::Mode loadDataForModeAndState(QSvgRenderer *renderer, QIcon::Mode mode, QIcon::State state);

public:
    int serialNum;
    static QAtomicInt lastSerialNum;

    // Custom
    QByteArray svgContents[8];
    QString colors[8];

    mutable int contentIndexes[8];
    mutable QString realColors[8];

    QM::ClickState currentState;

    QString salt;
};

#endif // SVGXICONENGINE_P_H
