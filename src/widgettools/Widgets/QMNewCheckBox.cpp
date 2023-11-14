#include "QMNewCheckBox.h"

#include <QMouseEvent>

#include <QMWidgets/CToolButton.h>

class QMNewCheckBoxPrivate {
public:
    QMNewCheckBox *q;

    CToolButton *m_box;
    QLabel *m_label;
    QHBoxLayout *m_layout;

    explicit QMNewCheckBoxPrivate(QMNewCheckBox *q) : q(q) {
        q->setAttribute(Qt::WA_StyledBackground);

        m_box = new CToolButton();
        m_box->setObjectName("box");
        m_box->setFocusPolicy(Qt::StrongFocus);
        m_box->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        m_label = new QLabel();
        m_label->setObjectName("label");
        m_label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        m_label->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        m_layout = new QHBoxLayout();
        m_layout->setMargin(0);
        m_layout->setSpacing(0);

        m_layout->addWidget(m_box);
        m_layout->addWidget(m_label);

        q->setLayout(m_layout);
        q->setCheckable(true);

        QObject::connect(m_box, &CToolButton::toggled, q, &QMNewCheckBox::setChecked);
    }
};

/*!
    \class QMNewCheckBox
    \brief QMNewCheckBox is a combination of a label and a tool button and appears like a check box.
*/

/*!
    Constructs with no text.
*/
QMNewCheckBox::QMNewCheckBox(QWidget *parent) : QAbstractButton(parent), d(new QMNewCheckBoxPrivate(this)) {
}

/*!
    Constructs with the given text.
*/
QMNewCheckBox::QMNewCheckBox(const QString &text, QWidget *parent) : QMNewCheckBox(parent) {
    setText(text);
}

/*!
    Destructor.
*/
QMNewCheckBox::~QMNewCheckBox() {
    delete d;
}

void QMNewCheckBox::paintEvent(QPaintEvent *event) {
    bool checkable = this->isCheckable();
    bool checked = this->isChecked();
    QString text = this->text();

    if (checkable != d->m_box->isCheckable()) {
        d->m_box->setCheckable(checkable);
    }

    if (checked != d->m_box->isChecked()) {
        d->m_box->setChecked(checked);
    }

    if (text != d->m_label->text()) {
        d->m_label->setText(text);
    }

    return QWidget::paintEvent(event);
}
