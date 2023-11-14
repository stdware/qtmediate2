#ifndef QMNAVWIDGET_H
#define QMNAVWIDGET_H

#include <QAbstractButton>
#include <QFrame>
#include <QSplitter>

#include <QMWidgetTools/QMWidgetToolsGlobal.h>

class QMNavWidgetPrivate;

class QMWTOOLS_EXPORT QMNavWidget : public QFrame {
    Q_OBJECT
public:
    explicit QMNavWidget(QWidget *parent = nullptr);
    ~QMNavWidget();

    QSplitter *splitter() const;

public:
    QWidget *topWidget() const;
    void setTopWidget(QWidget *w);
    QWidget *takeTopWidget();

    QWidget *bottomWidget() const;
    void setBottomWidget(QWidget *w);
    QWidget *takeBottomWidget();

    QAbstractButton *addWidget(QWidget *w);
    QAbstractButton *insertWidget(int index, QWidget *w);
    void removeWidget(QWidget *w);
    void removeAt(int index);

    QWidget *widget(int index);
    QWidget *currentWidget() const;
    int currentIndex() const;
    int count() const;
    int indexOf(QWidget *w) const;

    void setCurrentIndex(int index);
    void setCurrentWidget(QWidget *w);

signals:
    void currentIndexChanged(int index);

protected:
    QScopedPointer<QMNavWidgetPrivate> d_ptr;

    friend class QMNavWidgetPrivate;

private:
    void _q_currentChanged(int index);
    void _q_buttonClicked(int id);
};

#endif // QMNAVWIDGET_H
