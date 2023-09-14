#ifndef NATIVEHANDLE_H
#define NATIVEHANDLE_H

#include <QMWidgetTools/QMWindowHandle.h>

#include <widgetssharedhelper_p.h>

class CWindowBarV2;

class NativeHandle : public QMWindowHandle {
    Q_OBJECT
public:
    explicit NativeHandle(QMainWindow *parent);
    ~NativeHandle();

public:
    void setup() override;

    void setMenuBar(QMenuBar *menuBar) override;
    QMenuBar *menuBar() const override;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

    void updateTitleBar() override;

private:
    QScopedPointer<wangwenx190::FramelessHelper::WidgetsSharedHelper> m_helper;
    CWindowBarV2 *titleBar;
};

#endif // NATIVEHANDLE_H
