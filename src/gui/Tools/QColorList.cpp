#include "QColorList.h"

#include <QDebug>

#include <QMCore/QMBatch.h>

#include "QMCss.h"

QColorList QColorList::fromStringList(const QStringList &stringList) {
    if (stringList.size() != 2 || stringList.front().compare(metaFunctionName(), Qt::CaseInsensitive) != 0) {
        return {};
    }
    const auto &strData = stringList.at(1);

    QColorList res;
    QStringList valueList = QMCss::parseStringValueList(strData);
    for (const auto &item : qAsConst(valueList)) {
        res.get().append(QMCss::parseColor(QMBatch::strRemoveSideQuote(item.simplified())));
    }

    return res;
}

const char *QColorList::metaFunctionName() {
    return "qcolors";
}

QDebug operator<<(QDebug debug, const QColorList &list) {
    debug << list.get();
    return debug;
}
