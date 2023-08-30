#ifndef QMWINDOWHANDLE_H
#define QMWINDOWHANDLE_H

#include <QMainWindow>

#include "QMWidgetToolsGlobal.h"

class QMWindowHandlePrivate;

class QMWTOOLS_EXPORT QMWindowHandle : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMWindowHandle)
public:
    explicit QMWindowHandle(QMainWindow *parent);
    ~QMWindowHandle();
    
    QMainWindow *window() const;

public:
    virtual void setup();

    virtual void setMenuBar(QMenuBar *menuBar);
    virtual QMenuBar *menuBar() const;

    enum TitleBarFlag {
        WindowIcon = 0x1,
        WindowTitle = 0x2,
        WindowMinimizeButton = 0x4,
        WindowMaximizeButton = 0x8,
        WindowCloseButton = 0x10,
        TitleBarFlagMask = WindowIcon | WindowTitle |                    //
                           WindowMinimizeButton | WindowMaximizeButton | //
                           WindowCloseButton,
    };
    Q_DECLARE_FLAGS(TitleBarFlags, TitleBarFlag);

    void setTitleBarFlags(TitleBarFlags flags);
    TitleBarFlags titleBarFlags();

protected:
    virtual void updateTitleBar() = 0;

private:
    QScopedPointer<QMWindowHandlePrivate> d_ptr;
};

#endif // QMWINDOWHANDLE_H
