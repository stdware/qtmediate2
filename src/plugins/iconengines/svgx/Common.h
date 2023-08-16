#ifndef COMMON_H
#define COMMON_H

#include <QStringList>

struct DataUrl {
    QString fmt;
    QString charset;
    QString data;
};

DataUrl fromDataUrl(const QString &s);

QByteArray parseDataUrl(const DataUrl &url);

#endif // COMMON_H
