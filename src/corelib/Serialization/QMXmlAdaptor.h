#ifndef QMXMLADAPTOR_H
#define QMXMLADAPTOR_H

#include <QJsonObject>
#include <QXmlStreamWriter>

#include "QMGlobal.h"

// Example

/*

    {
        "name": "document",
        "properties": {
            "key1": "val1",
            ...
        },
        "value": "...",
        "children": [
            ...
        ]
    }

*/

class QMCORE_EXPORT QMXmlAdaptorElement {
public:
    using Ref = QSharedPointer<QMXmlAdaptorElement>;

    QString name;                      // Tag name
    QMap<QString, QString> properties; // Tag properties
    QString value;                     // Characters if no children
    QList<Ref> children;               // Children

    QMXmlAdaptorElement(){};
    ~QMXmlAdaptorElement() = default;

    QJsonObject toObject() const;
    static QMXmlAdaptorElement fromObject(const QJsonObject &obj);

    void writeXml(QXmlStreamWriter &writer) const;
};

class QMCORE_EXPORT QMXmlAdaptor {
public:
    QMXmlAdaptor();
    ~QMXmlAdaptor();

public:
    bool load(const QString &filename);
    bool save(const QString &filename) const;

    QMXmlAdaptorElement root;
};

#endif // QMXMLADAPTOR_H
