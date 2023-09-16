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

/*!
    \namespace QMView
    \brief Namespace of widgets and windows utilities.
*/

namespace QMView {

    void waitToShow(QWidget *w) {
        if (w) {
            if (!w->windowHandle())
                w->createWinId();
            waitForWindowExposed(w->windowHandle());
        }
    }

    void centralizeWindow(QWidget *w, QSizeF ratio) {
        QSize desktopSize;
        if (w->parentWidget()) {
            desktopSize = w->parentWidget()->size();
        } else {
            desktopSize = w->screen()->size();
        }

        int dw = desktopSize.width();
        int dh = desktopSize.height();

        double rw = ratio.width();
        double rh = ratio.height();

        QSize size = w->size();
        if (rw > 0 && rw <= 1) {
            size.setWidth(dw * rw);
        }
        if (rh > 0 && rh <= 1) {
            size.setHeight(dh * rh);
        }

        w->setGeometry((dw - size.width()) / 2, (dh - size.height()) / 2, size.width(),
                       size.height());
    }

    void raiseWindow(QWidget *w) {
        // Make sure the window isn't minimized
        // TODO: this always puts it in the "normal" state but it might have been maximized
        // before minimized...so either a flag needs stored or find a Qt call to do it appropriately
        if (w->isMinimized())
            w->showNormal();

#ifdef Q_OS_WINDOWS
        // TODO: there doesn't seem to be a cross platform way to force the window
        // to the foreground. So this will need moved to a platform specific file

        HWND hWnd = reinterpret_cast<HWND>(w->effectiveWinId());
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

    void fixWindowPos(QWidget *w) {
        QPoint target(w->pos());
        if (target.x() < 0) {
            target.setX(0);
        }
        if (target.y() < 0) {
            target.setY(0);
        }
        int dw = qApp->desktop()->width();
        int dh = qApp->desktop()->height();
        if (target.x() + w->width() > dw) {
            target.setX(dw - w->width());
        }
        if (target.y() + w->height() > dh) {
            target.setY(dh - w->height());
        }
        w->move(target);
    }

    QWidget *implicitMouseGrabber() {
        return qt_button_down;
    }

    void setImplicitMouseGrabber(QWidget *w) {
        qt_button_down = w;
    }

}