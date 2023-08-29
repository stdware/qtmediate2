#ifndef QMCOREAPPEXTENSION_P_H
#define QMCOREAPPEXTENSION_P_H

#include <QJsonObject>

#include "../QMCoreAppExtension.h"
#include "../QMCoreDecoratorV2.h"

#include "QMSimpleVarExp.h"

class QMCORE_EXPORT QMCoreAppExtensionPrivate : public QObject {
    Q_OBJECT
    Q_DECLARE_PUBLIC(QMCoreAppExtension)
public:
    QMCoreAppExtensionPrivate();
    ~QMCoreAppExtensionPrivate();

    void init();

    bool readConfiguration(const QString &fileName);

    QMCoreAppExtension *q_ptr;

    QMSimpleVarExp configVars;

    bool isAboutToQuit;

    QString appDataDir;
    QString tempDir;
    QString libDir;
    QString shareDir;

    QString appShareDir;
    QString appPluginsDir;

    QStringList pluginPaths;
    QStringList translationPaths;
    QStringList themePaths;
    QStringList fontPaths;

    QJsonObject appFont;

    virtual QMCoreDecoratorV2 *createDecorator(QObject *parent);

#if defined(Q_OS_WINDOWS) || defined(Q_OS_MAC)
    void osMessageBox_helper(void *winHandle, QMCoreAppExtension::MessageBoxFlag flag, const QString &title,
                             const QString &text) const;
#endif

private:
    QMCoreDecoratorV2 *s_dec;
};



#endif // QMCOREAPPEXTENSION_P_H
