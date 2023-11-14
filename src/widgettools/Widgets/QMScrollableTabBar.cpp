#include "QMScrollableTabBar.h"
#include "QMScrollableTabWidget.h"
#include "QMScrollableTabBar_p.h"

#include <QApplication>
#include <QDebug>
#include <QDrag>
#include <QMouseEvent>
#include <QTimerEvent>
#include <QPainter>
#include <QStyle>

#include <QMGui/QMGraphs.h>
#include <QMWidgets/CToolButton.h>

static inline QString id_format() {
    return QString("application/%1.pid%2.data")
        .arg(qApp->applicationName(), QString::number(qApp->applicationPid()));
}

QMScrollableTabBar *QMScrollableTabBarPrivate::draggedTabBar = nullptr;

QMScrollableTabBarPrivate::QMScrollableTabBarPrivate() {
}

QMScrollableTabBarPrivate::~QMScrollableTabBarPrivate() {
}

void QMScrollableTabBarPrivate::init() {
    Q_Q(QMScrollableTabBar);
    q->setAttribute(Qt::WA_StyledBackground);

    autoHide = false;

    tabs = nullptr;

    current = nullptr;
    previous = nullptr;

    selectionBehaviorOnRemove = QTabBar::SelectPreviousTab;
    draggedIndex = -1;
    needAutoScroll = false;

    entityLayout = new QHBoxLayout();
    entityLayout->setMargin(0);
    entityLayout->setSpacing(0);

    entity = new QWidget(q);
    entity->setObjectName("entity");
    entity->setLayout(entityLayout);
    entity->installEventFilter(q);

    scrollBar = new QScrollBar(Qt::Horizontal, q);
    scrollBar->setObjectName("scroll-bar");
    scrollBar->setContextMenuPolicy(Qt::NoContextMenu);

    scrollBar->installEventFilter(q);

    QObject::connect(scrollBar, &QScrollBar::valueChanged, q,
                     &QMScrollableTabBar::_q_scrollBarValueChanged);
    QObject::connect(scrollBar, &QScrollBar::rangeChanged, q,
                     &QMScrollableTabBar::_q_scrollBarRangeChanged);

    opacityEffect = new QGraphicsOpacityEffect(scrollBar);
    scrollBar->setGraphicsEffect(opacityEffect);

    opacityTween = new QPropertyAnimation(q, "scrollOpacity");
    opacityTween->setEasingCurve(QEasingCurve::OutCubic);

    timerId = q->startTimer(100);
    underMouse = false;

    updateScroll();
}

void QMScrollableTabBarPrivate::layoutScroll() {
    Q_Q(QMScrollableTabBar);
    scrollBar->setGeometry(0, q->height() - scrollBar->height(), q->width(), scrollBar->height());
}

void QMScrollableTabBarPrivate::updateScroll() {
    Q_Q(QMScrollableTabBar);

    int dw = entity->width() - q->width();
    if (dw <= 0) {
        scrollBar->setValue(0);
        scrollBar->hide();

        // Stop animation
        underMouse = false;
        lastResized = QTime();

        opacityTween->stop();
        opacityEffect->setOpacity(0);
    } else {
        scrollBar->show();
        scrollBar->setRange(0, dw);
        scrollBar->setPageStep(dw);
        scrollBar->setSingleStep(qMax(int(dw / 40), 1));
    }
}

void QMScrollableTabBarPrivate::runOpacityTween(bool visible) {
    opacityTween->stop();

    double endValue = visible ? 1 : 0;
    double duration = visible ? 200 : 1000;

    opacityTween->setStartValue(opacityEffect->opacity());
    opacityTween->setEndValue(endValue);
    opacityTween->setDuration(qAbs(opacityEffect->opacity() - visible) * duration);

    opacityTween->start();
}

void QMScrollableTabBarPrivate::startDrag(QMScrollableTabBarTab *tab) {
    if (current != tab) {
        setCurrentTab(tab);
    }

    Q_Q(QMScrollableTabBar);

    QDrag *drag = new QDrag(tab);
    QMimeData *mime = new QMimeData();

    // Bind Data
    QVariant var = tab->data();
    if (var.type() == QVariant::String) {
        mime->setData("text/plain", var.toString().toUtf8());
    }
    mime->setData(id_format(), tabs->metaObject()->className());

    QPixmap pixmap = QMGraphs::createPixmap(tab->size(), q->window()->windowHandle());
    pixmap.fill(Qt::transparent);
    tab->render(&pixmap);

    drag->setMimeData(mime);
    drag->setPixmap(pixmap);
    drag->setHotSpot(tab->mapFromGlobal(QCursor::pos()));

    draggedTabBar = q;

    // Block
    Qt::DropAction res = drag->exec(Qt::ActionMask);

    draggedTabBar = nullptr;

    Q_UNUSED(res)
}

QMScrollableTabBarTab *QMScrollableTabBarPrivate::tabAtIndex(int index) const {
    auto item = entityLayout->itemAt(index);
    if (!item) {
        return nullptr;
    }
    return qobject_cast<QMScrollableTabBarTab *>(item->widget());
}

void QMScrollableTabBarPrivate::setCurrentTab(QMScrollableTabBarTab *tab) {
    Q_Q(QMScrollableTabBar);

    int orgIndex = -1;
    if (current) {
        if (current == tab) {
            goto out;
        }
        orgIndex = entityLayout->indexOf(current);
        current->setSelected(false);
    }
    tab->setSelected(true);
    previous = current;
    current = tab;

out:
    needAutoScroll = true;
    entityLayout->invalidate();

    emit q->currentChanged(entityLayout->indexOf(tab), orgIndex);
}

void QMScrollableTabBarPrivate::autoScrollToCurrent() const {
    Q_Q(const QMScrollableTabBar);

    auto tab = current;
    if (!tab) {
        return;
    }

    // Move View Port
    if (tab->x() + entity->x() < 0) {
        scrollBar->setValue(tab->x());
    } else if (tab->x() + tab->width() + entity->x() > q->width()) {
        scrollBar->setValue(tab->x() + tab->width() - q->width());
    }
}

void QMScrollableTabBarPrivate::updateVisibility() {
    Q_Q(QMScrollableTabBar);
    if (autoHide && q->count() == 0) {
        q->hide();
    } else {
        q->show();
    }
}

QMScrollableTabBar::QMScrollableTabBar(QWidget *parent)
    : QMScrollableTabBar(*new QMScrollableTabBarPrivate(), parent) {
}

QMScrollableTabBar::~QMScrollableTabBar() {
}

int QMScrollableTabBar::addTab(const QString &text) {
    return insertTab(count(), text);
}

int QMScrollableTabBar::addTab(const QIcon &icon, const QString &text) {
    return insertTab(count(), icon, text);
}

int QMScrollableTabBar::insertTab(int index, const QString &text) {
    return insertTab(index, QIcon(), text);
}

int QMScrollableTabBar::insertTab(int index, const QIcon &icon, const QString &text) {
    auto tab = new QMScrollableTabBarTab();
    tab->installEventFilter(this);
    tab->setIcon(icon);
    tab->setText(text);

    Q_D(QMScrollableTabBar);
    d->entityLayout->insertWidget(qMin(d->entityLayout->count(), index), tab);

    connect(tab->closeButton(), &QAbstractButton::clicked, this, &QMScrollableTabBar::_q_closeTab);

    int res = d->entityLayout->indexOf(tab);
    tabInserted(res);
    if (count() == 1) {
        setCurrentIndex(0);
    }
    d->updateVisibility();
    return res;
}

void QMScrollableTabBar::removeTab(int index) {
    Q_D(QMScrollableTabBar);
    auto item = d->entityLayout->takeAt(index);
    if (!item) {
        return;
    }

    auto tab = qobject_cast<QMScrollableTabBarTab *>(item->widget());
    tab->hide();
    tab->deleteLater();
    delete item;

    if (d->previous == tab) {
        d->previous = nullptr;
    }

    int cnt = count();

    if (d->current == tab) {
        d->current = nullptr;
        switch (d->selectionBehaviorOnRemove) {
            case QTabBar::SelectPreviousTab:
                if (d->previous) {
                    d->setCurrentTab(d->previous);
                    break;
                }
            case QTabBar::SelectLeftTab: {
                if (index > 0) {
                    setCurrentIndex(index - 1);
                } else if (cnt > 0) {
                    setCurrentIndex(0);
                }
                break;
            }
            case QTabBar::SelectRightTab: {
                if (index < cnt - 1) {
                    setCurrentIndex(index + 1);
                } else if (cnt > 0) {
                    setCurrentIndex(cnt - 1);
                }
                break;
            }
        }
    }
    d->updateVisibility();

    if (cnt == 0) {
        emit currentChanged(-1, index);
    }

    tabRemoved(index);
}

void QMScrollableTabBar::moveTab(int from, int to) {
    Q_D(QMScrollableTabBar);
    auto item = d->entityLayout->takeAt(from);
    if (!item) {
        return;
    }
    d->entityLayout->insertItem(qMin(d->entityLayout->count(), to), item);
    emit tabMoved(from, to);
}

QString QMScrollableTabBar::tabText(int index) const {
    Q_D(const QMScrollableTabBar);
    auto tab = d->tabAtIndex(index);
    if (!tab) {
        return QString();
    }
    return tab->text();
}

void QMScrollableTabBar::setTabText(int index, const QString &text) {
    Q_D(QMScrollableTabBar);
    auto tab = d->tabAtIndex(index);
    if (!tab) {
        return;
    }
    tab->setText(text);
}

QIcon QMScrollableTabBar::tabIcon(int index) const {
    Q_D(const QMScrollableTabBar);
    auto tab = d->tabAtIndex(index);
    if (!tab) {
        return QIcon();
    }
    return tab->icon();
}

void QMScrollableTabBar::setTabIcon(int index, const QIcon &icon) {
    Q_D(QMScrollableTabBar);
    auto tab = d->tabAtIndex(index);
    if (!tab) {
        return;
    }
    tab->setIcon(icon);
}

QString QMScrollableTabBar::tabToolTip(int index) const {
    Q_D(const QMScrollableTabBar);
    auto tab = d->tabAtIndex(index);
    if (!tab) {
        return QString();
    }
    return tab->toolTip();
}

void QMScrollableTabBar::setTabToolTip(int index, const QString &tip) {
    Q_D(QMScrollableTabBar);
    auto tab = d->tabAtIndex(index);
    if (!tab) {
        return;
    }
    tab->setToolTip(tip);
}

QVariant QMScrollableTabBar::tabData(int index) const {
    Q_D(const QMScrollableTabBar);
    auto tab = d->tabAtIndex(index);
    if (!tab) {
        return QVariant();
    }
    return tab->data();
}

void QMScrollableTabBar::setTabData(int index, const QVariant &data) {
    Q_D(QMScrollableTabBar);
    auto tab = d->tabAtIndex(index);
    if (!tab) {
        return;
    }
    tab->setData(data);
}

QRect QMScrollableTabBar::tabRect(int index) const {
    Q_D(const QMScrollableTabBar);
    auto tab = d->tabAtIndex(index);
    if (!tab) {
        return QRect();
    }
    return QRect(d->entity->pos() + tab->pos(), tab->size());
}

int QMScrollableTabBar::tabAt(const QPoint &pos) const {
    const int max = count();
    for (int i = 0; i < max; ++i) {
        if (tabRect(i).contains(pos)) {
            return i;
        }
    }
    return -1;
}

int QMScrollableTabBar::totalWidth() const {
    Q_D(const QMScrollableTabBar);
    return d->entity->width();
}

int QMScrollableTabBar::currentIndex() const {
    Q_D(const QMScrollableTabBar);
    return d->entityLayout->indexOf(d->current);
}

int QMScrollableTabBar::count() const {
    Q_D(const QMScrollableTabBar);
    return d->entityLayout->count();
}

QSize QMScrollableTabBar::sizeHint() const {
    Q_D(const QMScrollableTabBar);
    return QSize(QWidget::sizeHint().width(), d->entity->sizeHint().height());
}

QSize QMScrollableTabBar::minimumSizeHint() const {
    return this->sizeHint();
}

QSize QMScrollableTabBar::iconSize(int index) const {
    Q_D(const QMScrollableTabBar);
    auto tab = d->tabAtIndex(index);
    if (!tab) {
        return QSize();
    }
    return tab->iconSize();
}

void QMScrollableTabBar::setIconSize(int index, const QSize &size) {
    Q_D(QMScrollableTabBar);
    auto tab = d->tabAtIndex(index);
    if (!tab) {
        return;
    }
    tab->setIconSize(size);
}

QTabBar::SelectionBehavior QMScrollableTabBar::selectionBehaviorOnRemove() const {
    Q_D(const QMScrollableTabBar);
    return d->selectionBehaviorOnRemove;
}

void QMScrollableTabBar::setSelectionBehaviorOnRemove(QTabBar::SelectionBehavior behavior) {
    Q_D(QMScrollableTabBar);
    d->selectionBehaviorOnRemove = behavior;
}

void QMScrollableTabBar::setCurrentIndex(int index) {
    Q_D(QMScrollableTabBar);
    auto tab = d->tabAtIndex(index);
    if (!tab) {
        return;
    }
    d->setCurrentTab(tab);
}

void QMScrollableTabBar::tabInserted(int index) {
    Q_UNUSED(index)
}

void QMScrollableTabBar::tabRemoved(int index) {
    Q_UNUSED(index)
}

void QMScrollableTabBar::resizeEvent(QResizeEvent *event) {
    Q_D(QMScrollableTabBar);
    d->layoutScroll();
    d->updateScroll();

    // Save Time
    if (event->oldSize().width() != event->size().width() && d->scrollBar->isVisible()) {
        d->lastResized = QTime::currentTime();
    }

    QFrame::resizeEvent(event);
}

void QMScrollableTabBar::wheelEvent(QWheelEvent *event) {
    Q_D(QMScrollableTabBar);
    if (d->scrollBar->isVisible()) {
        QApplication::sendEvent(d->scrollBar, event);
    }
    QFrame::wheelEvent(event);
}

void QMScrollableTabBar::timerEvent(QTimerEvent *event) {
    Q_D(QMScrollableTabBar);
    if (event->timerId() == d->timerId) {
        if (d->scrollBar->isVisible()) {
            if ((d->lastResized.isValid() && d->lastResized.msecsTo(QTime::currentTime()) < 1500) ||
                d->scrollBar->isSliderDown() ||
                this->rect().contains(this->mapFromGlobal(QCursor::pos()))) {
                if (!d->underMouse) {
                    d->underMouse = true;
                    d->runOpacityTween(true);
                }
            } else {
                if (d->underMouse) {
                    d->underMouse = false;
                    d->runOpacityTween(false);
                }
            }
        }
    }
}

bool QMScrollableTabBar::eventFilter(QObject *obj, QEvent *event) {
    Q_D(QMScrollableTabBar);

    if (qstrcmp(obj->metaObject()->className(), "QMScrollableTabBarTab") == 0) {
        auto tab = qobject_cast<QMScrollableTabBarTab *>(obj);
        switch (event->type()) {
            case QEvent::MouseButtonPress: {
                QMouseEvent *e = static_cast<QMouseEvent *>(event);
                if (e->button() == Qt::LeftButton) {
                    d->setCurrentTab(tab);
                }
                break;
            }
            case QEvent::MouseMove: {
                QMouseEvent *e = static_cast<QMouseEvent *>(event);
                if (e->buttons() & Qt::LeftButton) {
                    // Release Mouse
                    QMouseEvent newEvent(QEvent::MouseButtonRelease, e->pos(), Qt::LeftButton,
                                         Qt::NoButton, Qt::NoModifier);
                    QApplication::sendEvent(this, &newEvent);
                    // Start Drag
                    d->draggedIndex = d->entityLayout->indexOf(tab);
                    d->startDrag(tab);
                    d->draggedIndex = -1;
                }
                break;
            }
            case QEvent::MouseButtonRelease: {
                if (d->draggedIndex < 0) {
                    QMouseEvent *e = static_cast<QMouseEvent *>(event);
                    emit tabBarClicked(e->button(), d->entityLayout->indexOf(tab));
                }
                break;
            }
            default:
                break;
        }
    } else if (obj == d->scrollBar) {
        switch (event->type()) {
            case QEvent::Show: {
                d->layoutScroll();
                break;
            }
            case QEvent::Resize: {
                if (d->scrollBar->isVisible()) {
                    d->layoutScroll();
                }
                break;
            }
            default:
                break;
        }
    } else if (obj == d->entity) {
        switch (event->type()) {
            case QEvent::Resize: {
                auto e = static_cast<QResizeEvent *>(event);
                d->updateScroll();
                if (e->oldSize().height() != e->size().height()) {
                    updateGeometry();
                }
                break;
            }
            case QEvent::LayoutRequest: {
                d->entity->adjustSize();
                if (d->needAutoScroll) {
                    d->autoScrollToCurrent();
                    d->needAutoScroll = false;
                }
                break;
            }
            default:
                break;
        }
    }

    return QFrame::eventFilter(obj, event);
}

QMScrollableTabBar::QMScrollableTabBar(QMScrollableTabBarPrivate &d, QWidget *parent)
    : QFrame(parent), d_ptr(&d) {
    d.q_ptr = this;
    d.init();
}

double QMScrollableTabBar::scrollOpacity() const {
    Q_D(const QMScrollableTabBar);
    return d->opacityEffect->opacity();
}

void QMScrollableTabBar::setScrollOpacity(double opacity) {
    Q_D(QMScrollableTabBar);
    d->opacityEffect->setOpacity(opacity);
    emit styleChanged();
}

bool QMScrollableTabBar::autoHide() const {
    Q_D(const QMScrollableTabBar);
    return d->autoHide;
}

void QMScrollableTabBar::setAutoHide(bool autoHide) {
    Q_D(QMScrollableTabBar);
    d->autoHide = autoHide;
    d->updateVisibility();
}

QScrollBar *QMScrollableTabBar::scrollBar() const {
    Q_D(const QMScrollableTabBar);
    return d->scrollBar;
}

int QMScrollableTabBar::currentDraggedIndex() {
    Q_D(const QMScrollableTabBar);
    return d->draggedIndex;
}

QMScrollableTabBar *QMScrollableTabBar::currentDraggedTabBar() {
    return QMScrollableTabBarPrivate::draggedTabBar;
}

QAbstractButton *QMScrollableTabBar::closeButton(int index) const {
    Q_D(const QMScrollableTabBar);
    auto tab = d->tabAtIndex(index);
    if (!tab) {
        return nullptr;
    }
    return tab->closeButton();
}

void QMScrollableTabBar::setCloseButton(int index, QAbstractButton *button) {
    Q_D(QMScrollableTabBar);
    auto tab = d->tabAtIndex(index);
    if (!tab) {
        return;
    }
    tab->setCloseButton(button);
    connect(button, &QAbstractButton::clicked, this, &QMScrollableTabBar::_q_closeTab);
}

void QMScrollableTabBar::_q_scrollBarValueChanged(int value) {
    Q_D(QMScrollableTabBar);
    d->entity->move(-value, d->entity->y());
}

void QMScrollableTabBar::_q_scrollBarRangeChanged(int min, int max) {
    Q_UNUSED(min);
    Q_UNUSED(max);
}

void QMScrollableTabBar::_q_closeTab() {
    QObject *obj = sender();
    int index = -1;

    Q_D(QMScrollableTabBar);
    for (int i = 0; i < d->entityLayout->count(); ++i) {
        if (obj->parent() == d->entityLayout->itemAt(i)->widget()) {
            index = i;
            break;
        }
    }

    if (index < 0) {
        return;
    }

    emit tabCloseRequested(index);
}

// ======================================================================================
// QMScrollableTabBarTab

void QMScrollableTabBarTab::updateIconAndText() {
    QFontMetrics font(this->font());
    QSize fontSize(font.horizontalAdvance(m_text), font.height());

    int w =
        (m_icon.isNull() ? 0
                         : (m_iconSize.width() + m_iconMargins.left() + m_iconMargins.right())) +
        (m_text.isEmpty() ? 0
                          : (fontSize.width() + m_textMargins.left() + m_iconMargins.right() * 2));
    int h = qMax(
        m_icon.isNull() ? 0 : (m_iconSize.height() + m_iconMargins.top() + m_iconMargins.bottom()),
        m_text.isEmpty() ? 0 : (fontSize.height() + m_textMargins.top() + m_iconMargins.bottom()));

    QSizePolicy policy = m_iconTextItem->sizePolicy();
    m_iconTextItem->changeSize(w, h, policy.horizontalPolicy(), policy.verticalPolicy());
    m_layout->invalidate();

    update();
}

QMScrollableTabBarTab::QMScrollableTabBarTab(QWidget *parent) : QFrame(parent) {
    setAttribute(Qt::WA_StyledBackground);

    m_layout = new QHBoxLayout();
    m_layout->setMargin(0);
    m_layout->setSpacing(0);

    m_iconSize = QSize(12, 12);
    m_iconMargins = QMargins(0, 0, 0, 0);
    m_textMargins = QMargins(0, 0, 0, 0);

    m_iconTextItem = new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Preferred);

    m_closeButton = new CToolButton();
    m_closeButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_closeButton->setObjectName("close-button");

    m_layout->addItem(m_iconTextItem);
    m_layout->addWidget(m_closeButton);

    setLayout(m_layout);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    setProperty("selected", false);
}

QMScrollableTabBarTab::~QMScrollableTabBarTab() {
}

QIcon QMScrollableTabBarTab::icon() const {
    return m_icon;
}

void QMScrollableTabBarTab::setIcon(const QIcon &icon) {
    m_icon = icon;
    updateIconAndText();
}

QSize QMScrollableTabBarTab::iconSize() const {
    return m_iconSize;
}

void QMScrollableTabBarTab::setIconSize(const QSize &iconSize) {
    m_iconSize = iconSize;
    updateIconAndText();
}

QMargins QMScrollableTabBarTab::iconMargins() const {
    return m_iconMargins;
}

void QMScrollableTabBarTab::setIconMargins(const QMargins &iconMargins) {
    m_iconMargins = iconMargins;
    updateIconAndText();
}

QString QMScrollableTabBarTab::text() const {
    return m_text;
}

void QMScrollableTabBarTab::setText(const QString &text) {
    m_text = text;
    updateIconAndText();
}

QMargins QMScrollableTabBarTab::textMargins() const {
    return m_textMargins;
}

void QMScrollableTabBarTab::setTextMargins(const QMargins &textMargins) {
    m_textMargins = textMargins;
    updateIconAndText();
}

QVariant QMScrollableTabBarTab::data() const {
    return m_data;
}

void QMScrollableTabBarTab::setData(const QVariant &data) {
    m_data = data;
}

bool QMScrollableTabBarTab::selected() const {
    return property("selected").toBool();
}

void QMScrollableTabBarTab::setSelected(bool selected) {
    m_closeButton->setChecked(selected);

    setProperty("selected", selected);
    style()->polish(this);
    update();
}

QAbstractButton *QMScrollableTabBarTab::closeButton() const {
    return m_closeButton;
}

void QMScrollableTabBarTab::setCloseButton(QAbstractButton *button) {
    m_layout->replaceWidget(m_closeButton, button);
    delete m_closeButton;
    m_closeButton = button;
}

void QMScrollableTabBarTab::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setClipRegion(event->region());
    painter.translate(contentsRect().topLeft());

    int h = contentsRect().height();

    // Draw Icon
    if (!m_icon.isNull()) {
        painter.drawPixmap(
            QRect(QPoint(m_iconMargins.left(),
                         (h - m_iconSize.height() + m_iconMargins.top() - m_iconMargins.bottom()) /
                             2),
                  m_iconSize),
            m_icon.pixmap(m_iconSize));
    }

    // Draw Text
    if (!m_text.isEmpty()) {
        QFontMetrics font(this->font());
        QRect rect(m_iconMargins.left() + m_iconSize.width() + m_iconMargins.right() +
                       m_textMargins.left(),
                   (h - font.height() + m_textMargins.top() - m_textMargins.bottom()) / 2,
                   font.horizontalAdvance(m_text), font.height());
        if (m_icon.isNull()) {
            rect.moveLeft(m_textMargins.left());
        }
        painter.setPen(QPen(palette().windowText().color()));
        painter.setFont(this->font());
        painter.drawText(rect, Qt::AlignCenter, m_text);
    }

    QFrame::paintEvent(event);
}

// QMScrollableTabBarTab
// ======================================================================================