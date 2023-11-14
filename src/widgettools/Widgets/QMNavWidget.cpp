#include "QMNavWidget.h"

#include <QButtonGroup>
#include <QHBoxLayout>
#include <QSplitter>
#include <QStackedLayout>

#include <QMWidgets/CTabButton.h>

class QMNavWidgetPrivate {
public:
    QMNavWidget *q_ptr;

    QHBoxLayout *layout;
    QVBoxLayout *leftLayout;
    QVBoxLayout *buttonLayout;

    QWidget *topWidget;
    QWidget *bottomWidget;

    QStackedLayout *stack;

    QFrame *leftFrame;
    QFrame *rightFrame;

    QSplitter *splitter;

    QButtonGroup *btnGroup;
};

QMNavWidget::QMNavWidget(QWidget *parent) : QFrame(parent), d_ptr(new QMNavWidgetPrivate()) {
    auto &d = *d_ptr.data();
    d.q_ptr = this;

    // Left
    d.buttonLayout = new QVBoxLayout();
    d.buttonLayout->setMargin(0);
    d.buttonLayout->setSpacing(0);

    d.leftLayout = new QVBoxLayout();
    d.leftLayout->setMargin(0);
    d.leftLayout->setSpacing(0);

    d.leftLayout->addLayout(d.buttonLayout);
    d.leftLayout->addStretch();

    d.leftFrame = new QFrame();
    d.leftFrame->setObjectName("left-frame");
    d.leftFrame->setLayout(d.leftLayout);

    // Right
    d.stack = new QStackedLayout();
    d.stack->setMargin(0);

    d.rightFrame = new QFrame();
    d.rightFrame->setObjectName("right-frame");
    d.rightFrame->setLayout(d.stack);

    // Main
    d.splitter = new QSplitter();
    d.splitter->setObjectName("splitter");
    d.splitter->setChildrenCollapsible(false);
    d.splitter->addWidget(d.leftFrame);
    d.splitter->addWidget(d.rightFrame);

    d.splitter->setStretchFactor(0, 0);
    d.splitter->setStretchFactor(1, 1);

    d.layout = new QHBoxLayout();
    d.layout->setMargin(0);
    d.layout->setSpacing(0);

    d.layout->addWidget(d.splitter);
    setLayout(d.layout);

    d.btnGroup = new QButtonGroup(this);
    d.btnGroup->setExclusive(true);

    d.topWidget = nullptr;
    d.bottomWidget = nullptr;

    connect(d.stack, &QStackedLayout::currentChanged, this, &QMNavWidget::_q_currentChanged);
    connect(d.btnGroup, &QButtonGroup::idClicked, this, &QMNavWidget::_q_buttonClicked);
}

QMNavWidget::~QMNavWidget() {
}

QSplitter *QMNavWidget::splitter() const {
    return d_ptr->splitter;
}

QWidget *QMNavWidget::topWidget() const {
    return d_ptr->topWidget;
}

void QMNavWidget::setTopWidget(QWidget *w) {
    auto &d = *d_ptr.data();

    takeTopWidget();
    d.leftLayout->insertWidget(0, w);
    d.topWidget = w;
}

QWidget *QMNavWidget::takeTopWidget() {
    auto &d = *d_ptr.data();
    QWidget *w = nullptr;
    if (d.topWidget) {
        w = d.topWidget;
        d_ptr->leftLayout->removeWidget(w);
        d.topWidget = nullptr;
    }
    return w;
}

QWidget *QMNavWidget::bottomWidget() const {
    return d_ptr->bottomWidget;
}

void QMNavWidget::setBottomWidget(QWidget *w) {
    auto &d = *d_ptr.data();

    takeBottomWidget();
    d.leftLayout->addWidget(w);
    d.bottomWidget = w;
}

QWidget *QMNavWidget::takeBottomWidget() {
    auto &d = *d_ptr.data();
    QWidget *w = nullptr;
    if (d.bottomWidget) {
        w = d.bottomWidget;
        d_ptr->leftLayout->removeWidget(w);
        d.bottomWidget = nullptr;
    }
    return w;
}

QAbstractButton *QMNavWidget::addWidget(QWidget *w) {
    return insertWidget(count(), w);
}

QAbstractButton *QMNavWidget::insertWidget(int index, QWidget *w) {
    index = qMin(index, count());

    auto &d = *d_ptr.data();

    // New button
    auto btn = new CTabButton();
    btn->setCheckable(true);
    d.btnGroup->addButton(btn);

    // Insert to layouts
    d.buttonLayout->insertWidget(index, btn);
    d.stack->insertWidget(index, w);

    return btn;
}

void QMNavWidget::removeWidget(QWidget *w) {
    removeAt(d_ptr->stack->indexOf(w));
}

void QMNavWidget::removeAt(int index) {
    if (index >= count() || index < 0) {
        return;
    }
    auto &d = *d_ptr.data();

    // Find button
    auto btn = qobject_cast<QAbstractButton *>(d.buttonLayout->itemAt(index)->widget());
    d.btnGroup->removeButton(btn);

    // Remove from layouts
    d.buttonLayout->removeWidget(btn);
    d.stack->removeWidget(d.stack->widget(index));

    // Delete button
    btn->deleteLater();
}

QWidget *QMNavWidget::widget(int index) {
    return d_ptr->stack->widget(index);
}

QWidget *QMNavWidget::currentWidget() const {
    return d_ptr->stack->currentWidget();
}

int QMNavWidget::currentIndex() const {
    return d_ptr->stack->currentIndex();
}

int QMNavWidget::count() const {
    return d_ptr->stack->count();
}

int QMNavWidget::indexOf(QWidget *w) const {
    return d_ptr->stack->indexOf(w);
}

void QMNavWidget::setCurrentIndex(int index) {
    d_ptr->stack->setCurrentIndex(index);
}

void QMNavWidget::setCurrentWidget(QWidget *w) {
    d_ptr->stack->setCurrentWidget(w);
}

void QMNavWidget::_q_currentChanged(int index) {
    qobject_cast<QAbstractButton *>(d_ptr->buttonLayout->itemAt(index)->widget())->setChecked(true);
    emit currentIndexChanged(index);
}

void QMNavWidget::_q_buttonClicked(int id) {
    Q_UNUSED(id);

    auto &d = *d_ptr.data();
    setCurrentIndex(d.buttonLayout->indexOf(d.btnGroup->checkedButton()));
}
