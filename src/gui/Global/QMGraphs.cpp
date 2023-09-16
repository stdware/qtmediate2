#include "QMGraphs.h"

#include <QWindow>
#include <QGuiApplication>
#include <QPainter>

/*!
    \namespace QMGraphs
    \brief Namespace of Qt graphics utilities.
*/

namespace QMGraphs {

    /*!
       Create a pixmap whose resolution and pixel ratio is suitable for rendering to screen.

       \param logicalPixelSize Desired size of pixmap, in OS logical pixels
       \param window Pointer to QWindow whose DPR is taken. If it's nullptr, use qApp's DPR instead

       \return pixmap created
     */
    QPixmap createPixmap(const QSize &logicalPixelSize, QWindow *window) {
#ifndef Q_OS_MACOS
        qreal targetDPR;
        if (window)
            targetDPR = window->devicePixelRatio();
        else
            targetDPR = qApp->devicePixelRatio();
        QPixmap ret(logicalPixelSize * targetDPR);
        ret.setDevicePixelRatio(targetDPR);
        return ret;
#else
        Q_UNUSED(window);
        return QPixmap(logicalPixelSize);
#endif
    }

    /*!
        Draw border shadow refering to the given \c margin and \c size parameters.
    */
    void drawBorderShadow(QPainter &painter, const QMargins &margin, const QSize &size,
                          const QColor &color) {
        int w = size.width();
        int h = size.height();

        int cw = w - margin.left() - margin.right();
        int ch = h - margin.top() - margin.bottom();

        painter.setPen(Qt::NoPen);

        // Left
        QRect r1(0, margin.top(), margin.left(), ch);
        QLinearGradient l1(r1.left(), 0, r1.right(), 0);
        l1.setColorAt(0, Qt::transparent);
        l1.setColorAt(1, color);

        painter.setBrush(QBrush(l1));
        painter.drawRect(r1);

        // Top
        QRect r2(margin.left(), 0, cw, margin.top());
        QLinearGradient l2(0, r2.top(), 0, r2.bottom());
        l2.setColorAt(0, Qt::transparent);
        l2.setColorAt(1, color);

        painter.setBrush(QBrush(l2));
        painter.drawRect(r2);

        // Right
        QRect r3(w - margin.right(), margin.top(), margin.right(), ch);
        QLinearGradient l3(r3.left(), 0, r3.right(), 0);
        l3.setColorAt(0, color);
        l3.setColorAt(1, Qt::transparent);

        painter.setBrush(QBrush(l3));
        painter.drawRect(r3);

        // Bottom
        QRect r4(margin.left(), h - margin.bottom(), cw, margin.bottom());
        QLinearGradient l4(0, r4.top(), 0, r4.bottom());
        l4.setColorAt(0, color);
        l4.setColorAt(1, Qt::transparent);

        painter.setBrush(QBrush(l4));
        painter.drawRect(r4);

        int maxRadius =
            qMax(qMax(margin.left(), margin.right()), qMax(margin.top(), margin.bottom()));
        QPixmap bmp(QSize(maxRadius * 2, maxRadius * 2));
        bmp.fill(Qt::transparent);

        QPainter tempPainter(&bmp);
        QRadialGradient rg(maxRadius, maxRadius, maxRadius, maxRadius, maxRadius);
        rg.setColorAt(0, color);
        rg.setColorAt(1, Qt::transparent);

        tempPainter.setPen(Qt::NoPen);
        tempPainter.setBrush(QBrush(rg));
        tempPainter.drawEllipse(0, 0, 2 * maxRadius, 2 * maxRadius);

        QPixmap topLeftBmp = bmp.copy(0, 0, maxRadius, maxRadius);
        QPixmap topRightBmp = bmp.copy(maxRadius, 0, maxRadius, maxRadius);
        QPixmap bottomLeftBmp = bmp.copy(0, maxRadius, maxRadius, maxRadius);
        QPixmap bottomRightBmp = bmp.copy(maxRadius, maxRadius, maxRadius, maxRadius);

        // Top Left
        painter.drawPixmap(0, 0, margin.left(), margin.top(),
                           topLeftBmp.scaled(margin.left(), margin.top()));
        // Top Right
        painter.drawPixmap(w - margin.right(), 0, margin.right(), margin.top(),
                           topRightBmp.scaled(margin.right(), margin.top()));
        // Bottom Left
        painter.drawPixmap(0, h - margin.bottom(), margin.left(), margin.top(),
                           bottomLeftBmp.scaled(margin.left(), margin.bottom()));
        // Bottom Right
        painter.drawPixmap(w - margin.right(), h - margin.bottom(), margin.right(), margin.bottom(),
                           bottomRightBmp.scaled(margin.right(), margin.bottom()));
    }

    /*!
        Draw border lines refering to the given \c margin and \c size parameters.
    */
    void drawBorderLines(QPainter &painter, const QMargins &margin, const QSize &size,
                         const QPen &pen) {
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);

        QRectF rect;
        rect.setLeft(margin.left() - pen.widthF());
        rect.setRight(size.width() - margin.right() + pen.widthF());
        rect.setTop(margin.top() - pen.widthF());
        rect.setBottom(size.height() - margin.bottom() + pen.widthF());
        painter.drawRect(rect);
    }

}