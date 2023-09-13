#include "QFontInfoEx.h"

#include <QColor>
#include <QDebug>
#include <QGuiApplication>

#include <QMCore/QMBatch.h>

#include "QMButtonState_p.h"
#include "QMCss.h"

class QFontInfoExData : public QSharedData {
public:
    QFontInfoExData() {
        weight = -1;
        italic = -1;
        point = -1;
        pixel = -1;
    }

    int weight;
    int italic;
    double point;
    int pixel;
    QStringList families;
    QMButtonAttributes<QColor> colors;

    void copyAttributes(QFont &font) const {
        if (weight >= 0) {
            font.setWeight(weight);
        }

        if (italic >= 0) {
            font.setItalic(italic);
        }

        if (point > 0) {
            font.setPointSizeF(point);
        } else if (pixel > 0) {
            font.setPixelSize(pixel);
        }

        if (!families.isEmpty()) {
            if (families.size() == 1)
                font.setFamily(families.front());
            else
                font.setFamilies(families);
        }
    }
};

QFontInfoEx::QFontInfoEx() : d(new QFontInfoExData()) {
}

QFontInfoEx::QFontInfoEx(const QString &family, int pixelSize, int weight, bool italic) : QFontInfoEx() {
    d->families = QStringList{family};
    d->pixel = pixelSize;
    d->weight = weight;
    d->italic = italic;
}

QFontInfoEx::~QFontInfoEx() {
}

QFontInfoEx::QFontInfoEx(const QFontInfoEx &other) : d(other.d) {
}

QFontInfoEx::QFontInfoEx(QFontInfoEx &&other) noexcept : d(other.d) {
    other.d = nullptr;
}

QFont QFontInfoEx::toFont(const QObject *obj) const {
    QFont font = QGuiApplication::font();
    if (obj) {
        auto var = obj->property("font");
        if (var.type() == QVariant::Font) {
            font = var.value<QFont>();
        }
    }

    d->copyAttributes(font);

    return font;
}

QFontInfoEx &QFontInfoEx::operator=(const QFontInfoEx &other) {
    if (this == &other)
        return *this;
    d = other.d;
    return *this;
}

QFontInfoEx &QFontInfoEx::operator=(QFontInfoEx &&other) noexcept {
    qSwap(d, other.d);
    return *this;
}

int QFontInfoEx::weight() const {
    return d->weight;
}

void QFontInfoEx::setWeight(int weight) {
    d->weight = weight;
}

bool QFontInfoEx::italic() const {
    return d->italic < 0 ? false : bool(d->italic);
}

void QFontInfoEx::setItalic(bool italic) {
    d->italic = italic;
}

double QFontInfoEx::pointSize() const {
    return d->point;
}

void QFontInfoEx::setPointSize(double pointSize) {
    d->point = pointSize;
    d->pixel = -1;
}

int QFontInfoEx::pixelSize() const {
    return d->pixel;
}

void QFontInfoEx::setPixelSize(int pixelSize) {
    d->pixel = pixelSize;
    d->point = -1;
}

QStringList QFontInfoEx::families() const {
    return d->families;
}

void QFontInfoEx::setFamilies(const QStringList &families) {
    d->families = families;
}

QColor QFontInfoEx::color(QM::ButtonState state) const {
    return d->colors.value(state);
}

void QFontInfoEx::setColor(const QColor &color, QM::ButtonState state) {
    d->colors.setValue(color, state);
}

void QFontInfoEx::setColors(const QList<QColor> &colors) {
    d->colors.setValues(colors);
}

static int StringToWeight(const QString &str, int defaultValue) {
    int weight = defaultValue;
    if (!str.compare(QLatin1String("thin"), Qt::CaseInsensitive)) {
        weight = QFont::Thin;
    } else if (!str.compare(QLatin1String("extralight"), Qt::CaseInsensitive)) {
        weight = QFont::ExtraLight;
    } else if (!str.compare(QLatin1String("light"), Qt::CaseInsensitive)) {
        weight = QFont::Light;
    } else if (!str.compare(QLatin1String("normal"), Qt::CaseInsensitive)) {
        weight = QFont::Normal;
    } else if (!str.compare(QLatin1String("medium"), Qt::CaseInsensitive)) {
        weight = QFont::Medium;
    } else if (!str.compare(QLatin1String("demibold"), Qt::CaseInsensitive)) {
        weight = QFont::DemiBold;
    } else if (!str.compare(QLatin1String("bold"), Qt::CaseInsensitive)) {
        weight = QFont::Bold;
    } else if (!str.compare(QLatin1String("extrabold"), Qt::CaseInsensitive)) {
        weight = QFont::ExtraBold;
    } else if (!str.compare(QLatin1String("black"), Qt::CaseInsensitive)) {
        weight = QFont::Black;
    }
    return weight;
}

QFontInfoEx QFontInfoEx::fromStringList(const QStringList &stringList) {
    if (stringList.size() != 2 || stringList.front().compare(metaFunctionName(), Qt::CaseInsensitive) != 0) {
        return {};
    }
    const auto &strData = stringList.at(1);

    auto args = QMCss::parseArgList(strData.trimmed(),
                                    {
                                        "color",
                                        "size",
                                        "weight",
                                        "italic",
                                        "family",
                                    },
                                    {});

    auto it = args.find("color");
    if (it == args.end())
        return {};

    QFontInfoEx res;

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

    it = args.find("size");
    if (it != args.end()) {
        const auto &val = it.value();

        if (val.endsWith("pt")) {
            // pt
            res.setPointSize(val.chopped(2).toDouble());
        } else if (val.endsWith("px")) {
            // px
            res.setPixelSize(val.chopped(2).toInt());
        } else {
            // fallback as px
            res.setPixelSize(val.toInt());
        }
    }

    it = args.find("weight");
    if (it != args.end()) {
        const auto &val = it.value();

        int num;
        bool ok;

        num = val.toInt(&ok);
        if (!ok) {
            num = StringToWeight(val, res.weight());
        }
        res.setWeight(num);
    }

    it = args.find("italic");
    if (it != args.end()) {
        res.setItalic(QMCss::parseBoolean(it.value()));
    }

    it = args.find("family");
    if (it != args.end()) {
        const auto &val = it.value();
        QStringList list = (val.startsWith('(') && val.endsWith(')'))
                               ? QMCss::parseStringValueList(val.mid(1, val.size() - 2))
                               : QStringList{val};
        for (auto &item : list)
            item = QMBatch::strRemoveSideQuote(item.trimmed());
        res.setFamilies(list);
    }

    return res;
}

const char *QFontInfoEx::metaFunctionName() {
    return "qfont";
}

QDebug operator<<(QDebug debug, const QFontInfoEx &info) {
    QDebugStateSaver saver(debug);

    QStringList list;
    list << info.color().name();
    if (info.pointSize() > 0) {
        list << QString::number(info.pointSize()) + "pt";
    } else if (info.pixelSize() > 0) {
        list << QString::number(info.pixelSize()) + "px";
    }
    list << QString::number(info.weight());
    if (info.italic()) {
        list << "italic";
    }
    list << "[" + info.families().join(", ") + "]";

    debug.nospace().noquote() << "QFontInfoEx(" << list.join(", ") << ")";
    return debug;
}