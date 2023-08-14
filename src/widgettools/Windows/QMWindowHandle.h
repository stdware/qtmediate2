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

public:
    /**
     * @brief Parent window, cast from QWidget::parentWidget
     *
     */
    QMainWindow *window() const;

    /**
     * @brief Initialize the window handle, must call to take effect
     *
     */
    virtual void setup();

    /**
     * @brief Set the window menu bar, default to QMainWindow::setMenuBar
     *
     */
    virtual void setMenuBar(QMenuBar *menuBar);

    /**
     * @brief Current window menu bar, default to QMainWindow::menuBar
     *
     * @return QMenuBar*
     */
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

    /**
     * @brief Similar to QWidget::setWindowFlags
     *
     */
    void setTitleBarFlags(TitleBarFlags flags);

    /**
     * @brief Similar to QWidget::windowFlags
     *
     */
    TitleBarFlags titleBarFlags();

protected:
    virtual void updateTitleBar() = 0;

private:
    QScopedPointer<QMWindowHandlePrivate> d_ptr;
};

#endif // QMWINDOWHANDLE_H
