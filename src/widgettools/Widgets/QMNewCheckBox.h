#ifndef QMNEWCHECKBOX_H
#define QMNEWCHECKBOX_H

#include <QAbstractButton>
#include <QHBoxLayout>
#include <QLabel>

#include <QMWidgets/QMWidgetsMacros.h>
#include <QMWidgetTools/QMWidgetToolsGlobal.h>

class QMNewCheckBoxPrivate;

class QMWTOOLS_EXPORT QMNewCheckBox : public QAbstractButton {
    Q_OBJECT
    Q_LAYOUT_PROPERTY_DELCARE
public:
    explicit QMNewCheckBox(QWidget *parent = nullptr);
    QMNewCheckBox(const QString &text, QWidget *parent = nullptr);
    ~QMNewCheckBox();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QMNewCheckBoxPrivate *d;
};

#endif // QMNEWCHECKBOX_H
