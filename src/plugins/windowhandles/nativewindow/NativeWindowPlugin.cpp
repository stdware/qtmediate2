#include "NativeWindowPlugin.h"
#include "NativeHandle.h"

#include <framelessconfig_p.h>

NativeWindowPlugin::NativeWindowPlugin(QObject *parent) : QMWindowHandlePlugin(parent) {
    using namespace wangwenx190::FramelessHelper;
    
    FramelessConfig::instance()->set(Global::Option::WindowUseRoundCorners, true);
}

NativeWindowPlugin::~NativeWindowPlugin() {
}

QMWindowHandle *NativeWindowPlugin::create(const QString &key, QMainWindow *win) {
    Q_UNUSED(key);
    return new NativeHandle(win);
}
