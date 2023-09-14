#ifndef SVGXICONENGINE_P_H
#define SVGXICONENGINE_P_H

#include <QSvgRenderer>
#include <QSharedData>
#include <QIcon>

class SvgxIconEnginePrivate : public QSharedData {
public:
    SvgxIconEnginePrivate() : svgBuffers(0), addedPixmaps(0) {
        stepSerialNum();
    }

    ~SvgxIconEnginePrivate() {
        delete addedPixmaps;
        delete svgBuffers;
    }

    static int hashKey(QIcon::Mode mode, QIcon::State state) {
        return (((mode) << 4) | state);
    }

    QString pmcKey(const QSize &size, QIcon::Mode mode, QIcon::State state) {
        return QLatin1String("$qt_svgicon_") + QString::number(serialNum, 16).append(QLatin1Char('_')) +
               QString::number((((((qint64(size.width()) << 11) | size.height()) << 11) | mode) << 4) | state, 16);
    }

    void stepSerialNum() {
        serialNum = lastSerialNum.fetchAndAddRelaxed(1);
    }

    bool tryLoad(QSvgRenderer *renderer, QIcon::Mode mode, QIcon::State state);
    QIcon::Mode loadDataForModeAndState(QSvgRenderer *renderer, QIcon::Mode mode, QIcon::State state);

    QHash<int, QString> svgFiles;
    QHash<int, QByteArray> *svgBuffers;
    QHash<int, QPixmap> *addedPixmaps;
    int serialNum;
    static QAtomicInt lastSerialNum;
};

#endif // SVGXICONENGINE_P_H
