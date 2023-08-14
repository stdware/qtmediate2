#ifndef NATIVEWINDOWPLUGIN_H
#define NATIVEWINDOWPLUGIN_H

#include <QMWindowHandlePlugin.h>

class NativeWindowPlugin : public QMWindowHandlePlugin {
    Q_OBJECT
    Q_INTERFACES(QMWindowHandlePlugin)
    Q_PLUGIN_METADATA(IID QMWindowHandleFactoryInterface_IID FILE "plugin.json")
public:
    explicit NativeWindowPlugin(QObject *parent = nullptr);
    ~NativeWindowPlugin();

public:
    QMWindowHandle *create(const QString &key, QMainWindow *win) override;
};

#endif // NATIVEWINDOWPLUGIN_H
