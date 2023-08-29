#ifndef QMCORECONSOLE_H
#define QMCORECONSOLE_H

#include <QObject>

#include "QMGlobal.h"

#define qmCon QMCoreConsole::instance()

class QMCoreConsolePrivate;

class QMCORE_EXPORT QMCoreConsole : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMCoreConsole)
    Q_SINGLETON(QMCoreConsole)
public:
    explicit QMCoreConsole(QObject *parent = nullptr);
    ~QMCoreConsole();

public:
    enum MessageBoxFlag {
        Critical,
        Warning,
        Question,
        Information,
    };
    Q_ENUM(MessageBoxFlag)

    virtual void MsgBox(QObject *parent, MessageBoxFlag flag, const QString &title, const QString &text);
    
protected:
    QMCoreConsole(QMCoreConsolePrivate &d, QObject *parent = nullptr);

    QScopedPointer<QMCoreConsolePrivate> d_ptr;
};

#endif // QMCORECONSOLE_H
