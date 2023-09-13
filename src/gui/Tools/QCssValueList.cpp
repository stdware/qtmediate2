#include "QCssValueList.h"

#include <QDebug>

#include "QMCss.h"

QCssValueList QCssValueList::fromStringList(const QStringList &stringList) {
    if (stringList.size() != 2 || stringList.front().compare(metaFunctionName(), Qt::CaseInsensitive) != 0) {
        return {};
    }
    const auto &strData = stringList.at(1);

    QCssValueList res;
    QStringList valueList = QMCss::parseStringValueList(strData);
    for (const auto &item : valueList) {
        res.get().append(QMCssType::parse(item.trimmed()));
    }

    return res;
}

const char *QCssValueList::metaFunctionName() {
    return "qlist";
}

QDebug operator<<(QDebug debug, const QCssValueList &list) {
    debug << list.get();
    return debug;
}