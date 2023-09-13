#ifndef QMCOREAPPEXTENSION_P_H
#define QMCOREAPPEXTENSION_P_H

//
//  W A R N I N G !!!
//  -----------------
//
// This file is not part of the QtMediate API. It is used purely as an
// implementation detail. This header file may change from version to
// version without notice, or may even be removed.
//

#include <QJsonObject>

#include <QMCore/QMCoreAppExtension.h>
#include <QMCore/QMCoreDecoratorV2.h>
#include <QMCore/QMSimpleVarExp.h>

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
