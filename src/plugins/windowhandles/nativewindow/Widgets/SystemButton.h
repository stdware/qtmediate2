#ifndef SYSTEMBUTTON_H
#define SYSTEMBUTTON_H

#include <QMWidgets/CToolButton.h>

class SystemButton : public CToolButton {
    Q_OBJECT
public:
    explicit SystemButton(QWidget *parent = nullptr);
    ~SystemButton();

public:
    Q_INVOKABLE void setHovered(bool value);
    Q_INVOKABLE void setPressed(bool value);
    Q_INVOKABLE void setActive(bool value);

protected:
    bool event(QEvent *event) override;

    void nextCheckState() override;
};

#endif // SYSTEMBUTTON_H
