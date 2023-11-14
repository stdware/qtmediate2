#include "NativeHandle.h"

#include <QEvent>

#include <FramelessWidgetsHelper>

#include <QMWidgets/CMenuBar.h>

#include "Widgets/CWindowBarV2.h"

NativeHandle::NativeHandle(QMainWindow *parent) : QMWindowHandle(parent) {
    titleBar = nullptr;
}

NativeHandle::~NativeHandle() {
}

void NativeHandle::setup() {
    using namespace wangwenx190::FramelessHelper;

    auto w = window();

    // Not necessary, but better call this function, before the construction
    // of any Q(Core|Gui)Application instances.
    // FramelessHelper::Widgets::initialize();

    auto hFrameless = FramelessWidgetsHelper::get(w);
    hFrameless->extendsContentIntoTitleBar();

    m_helper.reset(new WidgetsSharedHelper(this));
    m_helper->setup(w);

    // Avoid displaying the top white line
    // w->setContentsMargins(QMargins());
    // hFrameless->windowBorder()->setThickness(0);

    // Insert Menu Bar To Title Bar
    auto bar = new CMenuBar();

    // New Window Bar
    titleBar = new CWindowBarV2(bar);
    titleBar->installEventFilter(w);

    // setMenuWidget(): make the menu widget become the first row of the window.
    w->setMenuWidget(titleBar);
    titleBar->setWidget(w);

    hFrameless->setTitleBarWidget(titleBar);

#ifndef Q_OS_MAC
    hFrameless->setSystemButton(titleBar->minButton(), Global::SystemButtonType::Minimize);
    hFrameless->setSystemButton(titleBar->maxButton(), Global::SystemButtonType::Maximize);
    hFrameless->setSystemButton(titleBar->closeButton(), Global::SystemButtonType::Close);
#endif

    hFrameless->setHitTestVisible(bar); // IMPORTANT!

    w->installEventFilter(this);
}

void NativeHandle::setMenuBar(QMenuBar *menuBar) {
    using namespace wangwenx190::FramelessHelper;

    auto helper = FramelessWidgetsHelper::get(window());
    auto orgBar = this->menuBar();
    if (orgBar) {
        helper->setHitTestVisible(orgBar, false);
    }
    helper->setHitTestVisible(menuBar);
    titleBar->setMenuBar(menuBar);
}

QMenuBar *NativeHandle::menuBar() const {
    return titleBar->menuBar();
}

bool NativeHandle::eventFilter(QObject *obj, QEvent *event) {
    if (obj == window()) {
        switch (event->type()) {
            case QEvent::WindowActivate:
                titleBar->setTitleBarActive(true);
                break;
            case QEvent::WindowDeactivate:
                titleBar->setTitleBarActive(false);
                break;
#ifndef Q_OS_MAC
            case QEvent::WindowIconChange:
                titleBar->iconButton()->setIcon(window()->windowIcon());
                break;
#endif
            default:
                break;
        }
    }
    return QObject::eventFilter(obj, event);
}

void NativeHandle::updateTitleBar() {
    auto titleBarFlags = this->titleBarFlags();
    titleBar->m_iconButton->setVisible(titleBarFlags & WindowIcon);
    titleBar->m_closeButton->setVisible(titleBarFlags & WindowCloseButton);
    titleBar->m_maxButton->setVisible(titleBarFlags & WindowMaximizeButton);
    titleBar->m_minButton->setVisible(titleBarFlags & WindowMinimizeButton);
    titleBar->setTitleVisible(titleBarFlags & WindowTitle);
}