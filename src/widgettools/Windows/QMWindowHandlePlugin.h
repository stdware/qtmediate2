#ifndef QMWINDOWHANDLEPLUGIN_H
#define QMWINDOWHANDLEPLUGIN_H

#include <QMainWindow>

#include "QMWidgetToolsGlobal.h"

class QMWindowHandle;

class QMWTOOLS_EXPORT QMWindowHandlePlugin : public QObject {
    Q_OBJECT
public:
    explicit QMWindowHandlePlugin(QObject *parent = nullptr);
    ~QMWindowHandlePlugin();

    virtual QMWindowHandle *create(const QString &key, QMainWindow *parent) = 0;
};

#define QMWindowHandleFactoryInterface_IID "org.qt-project.QtMediate.WindowHandleFactoryInterface"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(QMWindowHandlePlugin, QMWindowHandleFactoryInterface_IID)
QT_END_NAMESPACE


#endif // QMWINDOWHANDLEPLUGIN_H
