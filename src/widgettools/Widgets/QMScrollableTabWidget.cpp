#include "QMScrollableTabWidget.h"
#include "QMScrollableTabWidget_p.h"
#include "QMScrollableTabBar_p.h"

#include <QApplication>
#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>

static inline QString id_format() {
    return QString("application/%1.pid%2.data")
        .arg(qApp->applicationName(), QString::number(qApp->applicationPid()));
}

QMScrollableTabWidgetPrivate::QMScrollableTabWidgetPrivate() {
}

QMScrollableTabWidgetPrivate::~QMScrollableTabWidgetPrivate() {
}

void QMScrollableTabWidgetPrivate::init() {
    Q_Q(QMScrollableTabWidget);
    q->setAttribute(Qt::WA_StyledBackground);
    q->setAcceptDrops(true);
    mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    q->setLayout(mainLayout);

    // Init Tab Bar Widget
    tabBarWidget = new QWidget();
    tabBarWidget->setAttribute(Qt::WA_StyledBackground);

    barLayout = new QHBoxLayout();
    barLayout->setMargin(0);
    barLayout->setSpacing(0);
    tabBarWidget->setLayout(barLayout);

    mainLayout->addWidget(tabBarWidget);

    // Init Stack
    stack = new QStackedWidget();
    stack->setObjectName(QLatin1String("stack"));
    stack->setLineWidth(0);
    stack->setSizePolicy(
        QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred, QSizePolicy::TabWidget));
    QObject::connect(stack, &QStackedWidget::widgetRemoved, q, &QMScrollableTabWidget::_q_removeTab);
    mainLayout->addWidget(stack);

    // Init Tab Bar
    tabBar = nullptr;
    auto bar = new QMScrollableTabBar();
    bar->setObjectName("tab-bar");
    q->setTabBar(bar);
}

QMScrollableTabWidget::QMScrollableTabWidget(QWidget *parent)
    : QMScrollableTabWidget(*new QMScrollableTabWidgetPrivate(), parent) {
}

QMScrollableTabWidget::~QMScrollableTabWidget() {
}

QMScrollableTabBar *QMScrollableTabWidget::tabBar() const {
    Q_D(const QMScrollableTabWidget);
    return d->tabBar;
}

int QMScrollableTabWidget::addTab(QWidget *widget, const QString &label) {
    return insertTab(-1, widget, label);
}

int QMScrollableTabWidget::addTab(QWidget *widget, const QIcon &icon, const QString &label) {
    return insertTab(-1, widget, icon, label);
}

int QMScrollableTabWidget::insertTab(int index, QWidget *widget, const QString &label) {
    return insertTab(index, widget, QIcon(), label);
}

int QMScrollableTabWidget::insertTab(int index, QWidget *widget, const QIcon &icon,
                                    const QString &label) {
    Q_D(QMScrollableTabWidget);
    if (!widget) {
        return -1;
    }

    index = d->stack->insertWidget(index, widget);
    d->tabBar->insertTab(index, icon, label);
    tabInserted(index);
    return index;
}

void QMScrollableTabWidget::removeTab(int index) {
    Q_D(QMScrollableTabWidget);
    if (QWidget *w = d->stack->widget(index)) {
        d->stack->removeWidget(w);
    }
}

QString QMScrollableTabWidget::tabText(int index) const {
    Q_D(const QMScrollableTabWidget);
    return d->tabBar->tabText(index);
}

void QMScrollableTabWidget::setTabText(int index, const QString &label) {
    Q_D(QMScrollableTabWidget);
    d->tabBar->setTabText(index, label);
}

QIcon QMScrollableTabWidget::tabIcon(int index) const {
    Q_D(const QMScrollableTabWidget);
    return d->tabBar->tabIcon(index);
}

void QMScrollableTabWidget::setTabIcon(int index, const QIcon &icon) {
    Q_D(QMScrollableTabWidget);
    d->tabBar->setTabIcon(index, icon);
}

QString QMScrollableTabWidget::tabToolTip(int index) const {
    Q_D(const QMScrollableTabWidget);
    return d->tabBar->tabToolTip(index);
}

void QMScrollableTabWidget::setTabToolTip(int index, const QString &tip) {
    Q_D(QMScrollableTabWidget);
    d->tabBar->setTabToolTip(index, tip);
}

QVariant QMScrollableTabWidget::tabData(int index) const {
    Q_D(const QMScrollableTabWidget);
    return d->tabBar->tabData(index);
}

void QMScrollableTabWidget::setTabData(int index, const QVariant &data) {
    Q_D(QMScrollableTabWidget);
    d->tabBar->setTabData(index, data);
}

int QMScrollableTabWidget::currentIndex() const {
    Q_D(const QMScrollableTabWidget);
    return d->tabBar->currentIndex();
}

void QMScrollableTabWidget::setCurrentIndex(int index) {
    Q_D(QMScrollableTabWidget);
    d->tabBar->setCurrentIndex(index);
}

QWidget *QMScrollableTabWidget::currentWidget() const {
    Q_D(const QMScrollableTabWidget);
    return d->stack->currentWidget();
}

void QMScrollableTabWidget::setCurrentWidget(QWidget *widget) {
    Q_D(const QMScrollableTabWidget);
    d->tabBar->setCurrentIndex(indexOf(widget));
}

QWidget *QMScrollableTabWidget::widget(int index) const {
    Q_D(const QMScrollableTabWidget);
    return d->stack->widget(index);
}

int QMScrollableTabWidget::indexOf(QWidget *w) const {
    Q_D(const QMScrollableTabWidget);
    return d->stack->indexOf(w);
}

int QMScrollableTabWidget::count() const {
    Q_D(const QMScrollableTabWidget);
    return d->tabBar->count();
}

QSize QMScrollableTabWidget::iconSize(int index) const {
    return d_func()->tabBar->iconSize(index);
}

void QMScrollableTabWidget::setIconSize(int index, const QSize &size) {
    d_func()->tabBar->setIconSize(index, size);
}

void QMScrollableTabWidget::clear() {
    while (count() != 0) {
        removeTab(0);
    }
}

void QMScrollableTabWidget::tabInserted(int index) {
    Q_UNUSED(index)
}

void QMScrollableTabWidget::tabRemoved(int index) {
    Q_UNUSED(index)
}

void QMScrollableTabWidget::tabSelected(int index, int orgIndex) {
    Q_UNUSED(index)
    Q_UNUSED(orgIndex)
}

bool QMScrollableTabWidget::tabIncoming(const QMimeData *mime) const {
    Q_UNUSED(mime);
    return true;
}

void QMScrollableTabWidget::setTabBar(QMScrollableTabBar *tabBar) {
    Q_D(QMScrollableTabWidget);

    if (d->tabBar) {
        d->barLayout->replaceWidget(d->tabBar, tabBar);
        d->tabBar->deleteLater();
    } else {
        d->barLayout->insertWidget(0, tabBar);
        tabBar->d_func()->tabs = this;
    }

    d->tabBar = tabBar;
    tabBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    tabBar->show();
    setFocusProxy(tabBar);

    connect(d->tabBar, &QMScrollableTabBar::currentChanged, this, &QMScrollableTabWidget::_q_showTab);
    connect(d->tabBar, &QMScrollableTabBar::tabMoved, this, &QMScrollableTabWidget::_q_tabMoved);

    // Proxy Signals
    connect(d->tabBar, &QMScrollableTabBar::tabBarClicked, this,
            &QMScrollableTabWidget::tabBarClicked);
    connect(d->tabBar, &QMScrollableTabBar::tabCloseRequested, this,
            &QMScrollableTabWidget::tabCloseRequested);
}

void QMScrollableTabWidget::dragEnterEvent(QDragEnterEvent *event) {
    const QMimeData *mime = event->mimeData();
    QString idFormat = id_format();
    if (mime->hasFormat(idFormat) && !mime->data(idFormat).compare(metaObject()->className())) {
        event->acceptProposedAction();
        return;
    }
    return QWidget::dragEnterEvent(event);
}

void QMScrollableTabWidget::dropEvent(QDropEvent *event) {
    const QMimeData *mime = event->mimeData();
    QString idFormat = id_format();
    if (mime->hasFormat(idFormat) && !mime->data(idFormat).compare(metaObject()->className())) {
        auto orgBar = QMScrollableTabBar::currentDraggedTabBar();
        auto orgTabs = orgBar->d_func()->tabs;
        int orgIndex;
        if (orgBar && orgTabs && (orgIndex = orgBar->currentDraggedIndex()) >= 0 &&
            tabIncoming(mime)) {
            // Calculate New Index
            auto bar = tabBar();
            QPoint p = bar->mapFromGlobal(QCursor::pos());
            int index = bar->tabAt(p);
            if (index < 0 && (p.y() >= 0 && p.y() <= bar->height())) {
                if (p.x() < 0) {
                    index = 0;
                } else if (p.x() > bar->totalWidth()) {
                    index = bar->count();
                }
            }
            if (index < 0) {
                if (orgBar != bar) {
                    index = count();
                } else {
                    index = orgIndex;
                }
            }

            if (orgBar == bar) {
                if (index != orgIndex) {
                    if (index >= bar->count()) {
                        index = bar->count() - 1;
                    }
                    bar->moveTab(orgIndex, index);
                }
            } else {
                // Record And Remove
                auto tab = orgTabs->widget(orgIndex);
                auto icon = orgBar->tabIcon(orgIndex);
                auto label = orgBar->tabText(orgIndex);
                orgTabs->removeTab(orgIndex);

                // Insert And Switch
                insertTab(index, tab, icon, label);
            }

            setCurrentIndex(index);
            event->acceptProposedAction();
            return;
        }
    }
    return QWidget::dropEvent(event);
}

QMScrollableTabWidget::QMScrollableTabWidget(QMScrollableTabWidgetPrivate &d, QWidget *parent)
    : QFrame(parent), d_ptr(&d) {
    d.q_ptr = this;
    d.init();
}

QAbstractButton *QMScrollableTabWidget::closeButton(int index) const {
    Q_D(const QMScrollableTabWidget);
    return d->tabBar->closeButton(index);
}

void QMScrollableTabWidget::setCloseButton(int index, QAbstractButton *button) {
    Q_D(const QMScrollableTabWidget);
    d->tabBar->setCloseButton(index, button);
}

QWidget *QMScrollableTabWidget::tabBarWidget() {
    Q_D(const QMScrollableTabWidget);
    return d->tabBarWidget;
}

QWidget *QMScrollableTabWidget::moveOutTabBarWidget() {
    Q_D(QMScrollableTabWidget);
    auto w = d->tabBarWidget;
    if (w->parentWidget() != this) {
        return nullptr;
    }
    d->mainLayout->removeWidget(w);
    return w;
}

void QMScrollableTabWidget::moveInTabBarWidget() {
    Q_D(const QMScrollableTabWidget);
    auto w = d->tabBarWidget;
    if (w->parentWidget() == this) {
        return;
    }
    d->mainLayout->insertWidget(0, d->tabBarWidget);
}

void QMScrollableTabWidget::_q_showTab(int index, int orgIndex) {
    Q_D(QMScrollableTabWidget);
    if (index >= 0 && index < d->stack->count()) {
        d->stack->setCurrentIndex(index);
        tabSelected(index, orgIndex);
    }
    emit currentChanged(index, orgIndex);
}

void QMScrollableTabWidget::_q_removeTab(int index) {
    Q_D(QMScrollableTabWidget);
    d->tabBar->removeTab(index);
    tabRemoved(index);
}

void QMScrollableTabWidget::_q_tabMoved(int from, int to) {
    Q_D(QMScrollableTabWidget);
    const QSignalBlocker blocker(d->stack);
    QWidget *w = d->stack->widget(from);
    d->stack->removeWidget(w);
    d->stack->insertWidget(to, w);
}
