#ifndef QMCUSTOMWINDOW_H
#define QMCUSTOMWINDOW_H

#include <QMainWindow>

#include "QMWidgetToolsGlobal.h"

class QMCustomWindowPrivate;

class QMWTOOLS_EXPORT QMCustomWindow : public QMainWindow {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMCustomWindow)
public:
    explicit QMCustomWindow(QWidget *parent = nullptr);
    ~QMCustomWindow();

public:
    void setMenuBar(QMenuBar *menuBar);
    QMenuBar *menuBar() const;

protected:
    QMCustomWindow(QMCustomWindowPrivate &d, QWidget *parent = nullptr);

    QScopedPointer<QMCustomWindowPrivate> d_ptr;
};

#endif // QMCUSTOMWINDOW_H
