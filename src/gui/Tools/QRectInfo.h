#ifndef QRECTINFO_H
#define QRECTINFO_H

#include <QPen>
#include <QSharedData>

#include <QMCore/QMNamespace.h>
#include <QMGui/QMGuiGlobal.h>

class QRectInfoData;

class QMGUI_EXPORT QRectInfo {
public:
    QRectInfo();
    ~QRectInfo();

    QRectInfo(const QRectInfo &other);
    QRectInfo(QRectInfo &&other) noexcept;

    QRectInfo &operator=(const QRectInfo &other);
    QRectInfo &operator=(QRectInfo &&other) noexcept;

public:
    QRect rect() const;
    void setRect(const QRect &rect);

    QMargins margins() const;
    void setMargins(const QMargins &margins);

    QColor color(QM::ButtonState state = QM::ButtonNormal) const;
    void setColor(const QColor &color, QM::ButtonState state = QM::ButtonNormal);
    void setColors(const QList<QColor> &colors);

    int radius() const;
    void setRadius(int r);

public:
    static QRectInfo fromStringList(const QStringList &stringList);

    static const char *metaFunctionName();

    QMGUI_EXPORT friend QDebug operator<<(QDebug debug, const QRectInfo &info);

private:
    QSharedDataPointer<QRectInfoData> d;
};

Q_DECLARE_TYPEINFO(QRectInfo, Q_MOVABLE_TYPE);

Q_DECLARE_METATYPE(QRectInfo)

#endif // QRECTINFO_H
