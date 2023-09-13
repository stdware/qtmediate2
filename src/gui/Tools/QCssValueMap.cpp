#include "QCssValueMap.h"

#include <QDebug>

#include <QMCore/QMBatch.h>

#include "QMCss.h"

QCssValueMap QCssValueMap::fromStringList(const QStringList &stringList) {
    if (stringList.size() != 2 || stringList.front().compare(metaFunctionName(), Qt::CaseInsensitive) != 0) {
        return {};
    }
    const auto &strData = stringList.at(1);

    QCssValueMap res;
    QStringList valueList = QMCss::parseStringValueList(strData);

    for (auto item : qAsConst(valueList)) {
        item = item.trimmed();
        auto eq = QMCss::indexOfEqSign(item);
        if (eq < 0)
            continue;
        res.get().insert(QMBatch::strRemoveSideQuote(item.left(eq).trimmed(), true),
                         QMCssType::parse(item.mid(eq + 1).trimmed()));
    }
    return res;
}

const char *QCssValueMap::metaFunctionName() {
    return "qmap";
}

QDebug operator<<(QDebug debug, const QCssValueMap &map) {
    debug << map.get();
    return debug;
}