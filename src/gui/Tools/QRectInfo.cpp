#include "QRectInfo.h"

#include <QColor>
#include <QDebug>

#include "QMButtonState_p.h"
#include "QMCss.h"
#include "QPixelSize.h"

class QRectInfoData : public QSharedData {
public:
    QRectInfoData() {
        for (auto &item : nums)
            item = -1;
        numSize = 0;
        radius = 0;

        colors.setValueAll(Qt::transparent);
    }

    int nums[4];
    int numSize;

    QMButtonAttributes<QColor> colors;

    int radius;
};

QRectInfo::QRectInfo() : d(new QRectInfoData()) {
}

QRectInfo::~QRectInfo() {
}

QRectInfo::QRectInfo(const QRectInfo &other) : d(other.d) {
}

QRectInfo::QRectInfo(QRectInfo &&other) noexcept : d(other.d) {
    other.d = nullptr;
}

QRectInfo &QRectInfo::operator=(const QRectInfo &other) {
    if (this == &other)
        return *this;
    d = other.d;
    return *this;
}

QRectInfo &QRectInfo::operator=(QRectInfo &&other) noexcept {
    qSwap(d, other.d);
    return *this;
}

QRect QRectInfo::rect() const {
    switch (d->numSize) {
        case 1:
            return {0, 0, d->nums[0], d->nums[0]};
            break;
        case 2:
            return {0, 0, d->nums[0], d->nums[1]};
            break;
        case 3:
            return {d->nums[0], d->nums[1], d->nums[2], d->nums[2]};
            break;
        case 4:
            return {d->nums[0], d->nums[1], d->nums[2], d->nums[3]};
            break;
        default:
            break;
    }
    return {};
}

void QRectInfo::setRect(const QRect &rect) {
    d->nums[0] = rect.x();
    d->nums[1] = rect.y();
    d->nums[2] = rect.width();
    d->nums[3] = rect.height();
    d->numSize = 4;
}

QMargins QRectInfo::margins() const {
    switch (d->numSize) {
        case 1:
            return {d->nums[0], d->nums[0], d->nums[0], d->nums[0]};
            break;
        case 2:
        case 3:
            return {d->nums[1], d->nums[0], d->nums[1], d->nums[0]};
            break;
        case 4:
            return {d->nums[0], d->nums[1], d->nums[2], d->nums[3]};
            break;
        default:
            break;
    }
    return {};
}

void QRectInfo::setMargins(const QMargins &margins) {
    d->nums[0] = margins.left();
    d->nums[1] = margins.top();
    d->nums[2] = margins.right();
    d->nums[3] = margins.bottom();
    d->numSize = 4;
}

QColor QRectInfo::color(QM::ButtonState state) const {
    return d->colors.value(state);
}

void QRectInfo::setColor(const QColor &color, QM::ButtonState state) {
    d->colors.setValue(color, state);
}

void QRectInfo::setColors(const QList<QColor> &colors) {
    d->colors.setValues(colors);
}

int QRectInfo::radius() const {
    return d->radius;
}

void QRectInfo::setRadius(int r) {
    d->radius = r;
}

QRectInfo QRectInfo::fromStringList(const QStringList &stringList) {
    if (stringList.size() != 2 || stringList.front().compare(metaFunctionName(), Qt::CaseInsensitive) != 0) {
        return {};
    }
    const auto &strData = stringList.at(1);

    auto args = QMCss::parseArgList(strData.trimmed(),
                                    {
                                        "color",
                                        "numbers",
                                        "radius",
                                    },
                                    {});

    auto it = args.find("color");
    if (it == args.end())
        return {};

    QRectInfo res;

    QString colorStrings[8];
    const auto &colorExpressions = it->trimmed();
    if (colorExpressions.startsWith('(') && colorExpressions.endsWith(')')) {
        QMCss::parseButtonStateList(colorExpressions.mid(1, colorExpressions.size() - 2), colorStrings, false);

        for (int i = 0; i < 8; ++i) {
            if (colorStrings[i].isEmpty())
                continue;
            res.setColor(QMCss::parseColor(colorStrings[i]), static_cast<QM::ButtonState>(i));
        }
    } else {
        res.setColor(QMCss::parseColor(colorExpressions));
    }

    it = args.find("numbers");
    if (it != args.end()) {
        auto list = QMCss::parseSizeValueList(it.value());
        auto sz = qMin(4, list.size());
        for (int i = 0; i < sz; ++i) {
            res.d->nums[i] = list.at(i);
        }
        res.d->numSize = sz;
    }

    it = args.find("radius");
    if (it != args.end()) {
        res.setRadius(QPixelSize::fromString(it.value()));
    }

    return res;
}

const char *QRectInfo::metaFunctionName() {
    return "qrect";
}

QDebug operator<<(QDebug debug, const QRectInfo &info) {
    QDebugStateSaver saver(debug);

    debug.nospace().noquote() << "QRectInfo(" << info.color().name() << ", " << info.rect() << ", " << info.radius()
                              << ")";
    return debug;
}