#ifndef CMENUBAR_H
#define CMENUBAR_H

#include <QMenuBar>

#include <QMWidgets/QMWidgetsGlobal.h>

class CMenuBarPrivate;

class QMWIDGETS_EXPORT CMenuBar : public QMenuBar {
    Q_OBJECT
    Q_PROPERTY(QIcon extensionIcon READ extensionIcon WRITE setExtensionIcon)
public:
    explicit CMenuBar(QWidget *parent = nullptr);
    ~CMenuBar();

public:
    QIcon extensionIcon() const;
    void setExtensionIcon(const QIcon &extensionIcon);

    QMenu *extensionMenu() const;

private:
    CMenuBarPrivate *d;
};

#endif // CMENUBAR_H
