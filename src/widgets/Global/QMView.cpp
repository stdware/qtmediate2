#include "QMView.h"

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QPainter>
#include <QScreen>
#include <QSvgRenderer>
#include <QWidget>
#include <QWindow>
#include <QElapsedTimer>

#include <QMGui/QMGuiAppExtension.h>

#include <private/qapplication_p.h>
#include <private/qshortcutmap_p.h>

#ifdef Q_OS_WINDOWS
#  include <Windows.h>
#endif

extern Q_DECL_IMPORT QWidget *qt_button_down;

// Copy from `qsplashscreen.cpp`
inline static bool waitForWindowExposed(QWindow *window, int timeout = 1000) {
    enum { TimeOutMs = 10 };
    QElapsedTimer timer;
    timer.start();
    while (!window->isExposed()) {
        const int remaining = timeout - int(timer.elapsed());
        if (remaining <= 0)
            break;
        QCoreApplication::processEvents(QEventLoop::AllEvents, remaining);
        QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
#if defined(Q_OS_WINRT)
        WaitForSingleObjectEx(GetCurrentThread(), TimeOutMs, false);
#elif defined(Q_OS_WIN)
        Sleep(uint(TimeOutMs));
#else
        struct timespec ts = {TimeOutMs / 1000, (TimeOutMs % 1000) * 1000 * 1000};
        nanosleep(&ts, nullptr);
#endif
    }
    return window->isExposed();
}


namespace {

    class ShortcutFilter : public QObject {
    public:
        ShortcutFilter(QWidget *org) : m_org(org), m_handled(false) {
        }

        inline bool handled() const {
            return m_handled;
        }

    protected:
        bool eventFilter(QObject *watched, QEvent *event) override {
            if (event->type() == QEvent::Shortcut) {
                QApplicationPrivate::active_window = m_org;
                m_handled = true;
            }
            return QObject::eventFilter(watched, event);
        }

    private:
        QWidget *m_org;
        bool m_handled;
    };

}

/*!
    \namespace QMView
    \brief Namespace of widgets and windows utilities.
*/

namespace QMView {

    /*!
        Wait until the widget shows.
    */
    void waitToShow(QWidget *window) {
        if (window) {
            if (!window->windowHandle())
                window->createWinId();
            waitForWindowExposed(window->windowHandle());
        }
    }

    /*!
        Sets the size of the window using a percentage of the desktop size and makes the
        window show in center.
    */
    void centralizeWindow(QWidget *window, QSizeF ratio) {
        QSize desktopSize;
        if (window->parentWidget()) {
            desktopSize = window->parentWidget()->size();
        } else {
            desktopSize = window->screen()->size();
        }

        int dw = desktopSize.width();
        int dh = desktopSize.height();

        double rw = ratio.width();
        double rh = ratio.height();

        QSize size = window->size();
        if (rw > 0 && rw <= 1) {
            size.setWidth(dw * rw);
        }
        if (rh > 0 && rh <= 1) {
            size.setHeight(dh * rh);
        }

        window->setGeometry((dw - size.width()) / 2, (dh - size.height()) / 2, size.width(),
                            size.height());
    }

    /*!
        Bring the given window to the top.
    */
    void raiseWindow(QWidget *window) {
        if (!window->isWindow())
            return;

        // Make sure the window isn't minimized
        // TODO: this always puts it in the "normal" state but it might have been maximized
        // before minimized...so either a flag needs stored or find a Qt call to do it appropriately
        if (window->isMinimized())
            window->showNormal();

#ifdef Q_OS_WINDOWS
        // TODO: there doesn't seem to be a cross platform way to force the window
        // to the foreground. So this will need moved to a platform specific file

        HWND hWnd = reinterpret_cast<HWND>(window->effectiveWinId());
        if (hWnd) {
            // I have no idea what this does but it works mostly
            // https://www.codeproject.com/Articles/1724/Some-handy-dialog-box-tricks-tips-and-workarounds

            ::AttachThreadInput(::GetWindowThreadProcessId(::GetForegroundWindow(), nullptr),
                                ::GetCurrentThreadId(), TRUE);

            ::SetForegroundWindow(hWnd);
            ::SetFocus(hWnd);

            ::AttachThreadInput(GetWindowThreadProcessId(GetForegroundWindow(), nullptr),
                                GetCurrentThreadId(), FALSE);
        }
#endif
    }

    /*!
        Makes the window completely inside the screen if part of the window is outside.
    */
    void fixWindowPos(QWidget *window) {
        QPoint target(window->pos());
        if (target.x() < 0) {
            target.setX(0);
        }
        if (target.y() < 0) {
            target.setY(0);
        }

        const auto &desktopSize = window->screen()->size();
        if (target.x() + window->width() > desktopSize.width()) {
            target.setX(desktopSize.width() - window->width());
        }
        if (target.y() + window->height() > desktopSize.height()) {
            target.setY(desktopSize.height() - window->height());
        }
        window->move(target);
    }

    /*!
        Redirect the key event as a shortcut to the given window.
    */
    void forwardShortcut(QKeyEvent *event, QWidget *window) {
        // This function hacks the QApplication data structure and simply changes
        // the `active_window` pointer temporarily to make the shortcut map transmit the
        // event to the target window.

        if (!window || !window->isWindow()) {
            return;
        }

        // Hack `active_window` temporarily
        auto org = QApplicationPrivate::active_window;
        QApplicationPrivate::active_window = window;

        // Make sure to restore `active_window` right away if shortcut matches
        ShortcutFilter filter(org);
        qApp->installEventFilter(&filter);

        // Retransmit event
        QKeyEvent keyEvent(QEvent::ShortcutOverride, event->key(), event->modifiers(),
                           event->nativeScanCode(), event->nativeVirtualKey(),
                           event->nativeModifiers(), event->text(), event->isAutoRepeat(),
                           event->count());
        QGuiApplicationPrivate::instance()->shortcutMap.tryShortcut(&keyEvent);

        if (!filter.handled()) {
            QApplicationPrivate::active_window = org;
        }
    }

    /*!
        Returns the current state of a button.
    */
    QM::ButtonState buttonState(QAbstractButton *button) {
        if (button->isChecked()) {
            if (!button->isEnabled()) {
                return QM::ButtonDisabledChecked;
            }
            if (button->isDown()) {
                return QM::ButtonPressedChecked;
            }
            if (button->underMouse()) {
                return QM::ButtonHoverChecked;
            }
            return QM::ButtonNormalChecked;
        }

        if (!button->isEnabled()) {
            return QM::ButtonDisabled;
        }
        if (button->isDown()) {
            return QM::ButtonPressed;
        }
        if (button->underMouse()) {
            return QM::ButtonHover;
        }
        return QM::ButtonNormal;
    }

}