#ifndef QMGRAPHS_H
#define QMGRAPHS_H

#include <QPixmap>
#include <QRect>
#include <QPen>

#include <QMGui/QMGuiGlobal.h>

namespace QMGraphs {

    QMGUI_EXPORT QPixmap createPixmap(const QSize &logicalPixelSize, QWindow *window = nullptr);

    QMGUI_EXPORT void drawBorderShadow(QPainter &painter, const QMargins &margin, const QSize &size,
                                       const QColor &color = Qt::black);

    QMGUI_EXPORT void drawBorderLines(QPainter &painter, const QMargins &margin, const QSize &size,
                                      const QPen &pen = QPen(Qt::black));

}

#endif // QMGRAPHS_H
