#ifndef QMSYSTEM_H
#define QMSYSTEM_H

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QString>
#include <QTextCodec>

#include <QMCore/QMGlobal.h>

namespace QMFs {

    inline bool isPathRelative(const QString &path) {
        return QDir::isRelativePath(path);
    }

    inline bool isPathExist(const QString &path) {
        return (!isPathRelative(path) && QFileInfo::exists(path));
    }

    inline bool isFileExist(const QString &path) {
        return (!isPathRelative(path) && QFileInfo(path).isFile());
    }

    inline bool isDirExist(const QString &path) {
        return (!isPathRelative(path) && QFileInfo(path).isDir());
    }

    inline bool isSamePath(const QString &path1, const QString &path2) {
        return (QFileInfo(path1).canonicalFilePath() == QFileInfo(path2).canonicalFilePath());
    }

    inline QString PathFindSuffix(const QString &path) {
        return QFileInfo(path).suffix();
    }

    inline QString PathFindDirPath(const QString &path) {
        return QFileInfo(path).absolutePath();
    }

    QMCORE_EXPORT QString PathFindFileName(const QString &path);

    QMCORE_EXPORT QString PathFindNextDir(const QString &path, const QString &dir);

    inline QDateTime PathGetModifyTime(const QString &path) {
        return QFileInfo(path).lastModified().toLocalTime();
    }

    inline bool mkDir(const QString &dirname) {
        return isDirExist(dirname) || QDir().mkpath(dirname);
    }

    inline bool rmDir(const QString &dirname) {
        return isDirExist(dirname) && QDir(dirname).removeRecursively();
    }

    inline bool rmFile(const QString &filename) {
        return QFile::remove(filename);
    }

    inline bool copy(const QString &fileName, const QString &newName) {
        QFile file(newName);
        return (!file.exists() || file.remove()) && QFile::copy(fileName, newName);
    }

    QMCORE_EXPORT bool combine(const QString &fileName1, const QString &fileName2,
                               const QString &newName);

    QMCORE_EXPORT void reveal(const QString &filename);

    QMCORE_EXPORT int rmPreStr(const QString &dirname, const QString &prefix);

    QMCORE_EXPORT int rmPreNum(const QString &dirname, int prefix);

    QMCORE_EXPORT QString removeTailSlashes(const QString &dirname);

    QMCORE_EXPORT QString appDataPath();

}

namespace QMOs {

    Q_DECL_CONSTEXPR inline int unitDpi() {
#ifdef Q_OS_MACOS
        return 72;
#else
        return 96;
#endif
    }

    QMCORE_EXPORT bool isUserRoot();

    QMCORE_EXPORT QString fileManagerName();

    QMCORE_EXPORT QString rootUserName();

    QMCORE_EXPORT QString allFilesFilter();

    QMCORE_EXPORT QString toLibFile(const QString &dir, const QString &name);

}

#endif // QMSYSTEM_H
