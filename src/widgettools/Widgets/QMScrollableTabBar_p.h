#ifndef QMSCROLLABLETABBAR_P_H
#define QMSCROLLABLETABBAR_P_H

//
//  W A R N I N G !!!
//  -----------------
//
// This file is not part of the QtMediate API. It is used purely as an
// implementation detail. This header file may change from version to
// version without notice, or may even be removed.
//

#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

#include <QDateTime>
#include <QHBoxLayout>
#include <QLabel>
#include <QAbstractButton>
#include <QScrollBar>
#include <QSlider>
#include <QTabBar>
#include <QtGlobal>

class QMScrollableTabBarTab;
class QMScrollableTabWidget;
class QMScrollableTabBar;

class QMScrollableTabBarPrivate {
    Q_DECLARE_PUBLIC(QMScrollableTabBar)
public:
    QMScrollableTabBarPrivate();
    virtual ~QMScrollableTabBarPrivate();

    void init();

    void layoutScroll();
    void updateScroll();

    void runOpacityTween(bool visible);

    void startDrag(QMScrollableTabBarTab *tab);

    QMScrollableTabBarTab *tabAtIndex(int index) const;
    void setCurrentTab(QMScrollableTabBarTab *tab);

    void autoScrollToCurrent() const;

    void updateVisibility();

    QMScrollableTabBar *q_ptr;

    // Properties
    bool autoHide;

    QMScrollableTabWidget *tabs;

    // Tabs Entity
    QHBoxLayout *entityLayout;
    QWidget *entity;

    // Scroll Bar
    QScrollBar *scrollBar;

    QPropertyAnimation *opacityTween;
    QGraphicsOpacityEffect *opacityEffect;

    int timerId;
    bool underMouse;
    QTime lastResized;

    // Tab Bar Properties
    QTabBar::SelectionBehavior selectionBehaviorOnRemove;

    // Realtime Status
    QMScrollableTabBarTab *current, *previous;

    // Drag
    int draggedIndex;
    bool needAutoScroll;

    static QMScrollableTabBar *draggedTabBar;
};

// ======================================================================================
// QMScrollableTabBarTab

class QMScrollableTabBarTab : public QFrame {
    Q_OBJECT

    Q_PROPERTY(QIcon icon READ icon WRITE setIcon)
    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize)
    Q_PROPERTY(QMargins iconMargins READ iconMargins WRITE setIconMargins)
    Q_PROPERTY(QMargins textMargins READ textMargins WRITE setTextMargins)
public:
    explicit QMScrollableTabBarTab(QWidget *parent = nullptr);
    ~QMScrollableTabBarTab();

    friend class QMScrollableTabWidget;

public:
    QIcon icon() const;
    void setIcon(const QIcon &icon);

    QSize iconSize() const;
    void setIconSize(const QSize &iconSize);

    QMargins iconMargins() const;
    void setIconMargins(const QMargins &iconMargins);

    QString text() const;
    void setText(const QString &text);

    QMargins textMargins() const;
    void setTextMargins(const QMargins &textPadding);

    QVariant data() const;
    void setData(const QVariant &data);

    bool selected() const;
    void setSelected(bool selected);

public:
    QAbstractButton *closeButton() const;
    void setCloseButton(QAbstractButton *button);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QHBoxLayout *m_layout;

    QIcon m_icon;
    QSize m_iconSize;
    QMargins m_iconMargins;

    QString m_text;
    QMargins m_textMargins;

    QSpacerItem *m_iconTextItem;
    QAbstractButton *m_closeButton;

    QVariant m_data;

    void updateIconAndText();
};

// QMScrollableTabBarTab
// ======================================================================================

#endif // QMSCROLLABLETABBAR_P_H
