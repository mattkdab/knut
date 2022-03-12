#include "dir.h"

#include "logger.h"

#include <QVariant>

namespace Core {

/*!
 * \qmltype Dir
 * \brief Singleton with methods to handle directories.
 * \instantiates Core::Dir
 * \inqmlmodule Script
 * \since 4.0
 *
 * The `Dir` singleton implements most of the static methods from `QDir`, check [QDir](https://doc.qt.io/qt-5/qdir.html)
 * documentation.
 */

/*!
 * \qmlproperty char Dir::separator
 */
/*!
 * \qmlproperty string Dir::currentPath
 */
/*!
 * \qmlproperty string Dir::currentScriptPath
 */
/*!
 * \qmlproperty string Dir::homePath
 */
/*!
 * \qmlproperty string Dir::rootPath
 */
/*!
 * \qmlproperty string Dir::tempPath
 */

Dir::Dir(const QString &currentScriptPath, QObject *parent)
    : QObject(parent)
    , m_currentScriptPath(currentScriptPath)
{
}

Dir::~Dir() { }

/*!
 * \qmlmethod string Dir::toNativeSeparators( string pathName)
 */
QString Dir::toNativeSeparators(const QString &pathName)
{
    LOG("Dir::toNativeSeparators", pathName);
    return QDir::toNativeSeparators(pathName);
}

/*!
 * \qmlmethod string Dir::fromNativeSeparators( string pathName)
 */
QString Dir::fromNativeSeparators(const QString &pathName)
{
    LOG("Dir::fromNativeSeparators", pathName);
    return QDir::fromNativeSeparators(pathName);
}

/*!
 * \qmlmethod bool Dir::isRelativePath( string path)
 */
bool Dir::isRelativePath(const QString &path)
{
    LOG("Dir::isRelativePath", path);
    return QDir::isRelativePath(path);
}

QChar Dir::separator() const
{
    return QDir::separator();
}

bool Dir::setCurrentPath(const QString &path)
{
    LOG("Dir::setCurrentPath", path);
    if (path != QDir::currentPath()) {
        if (QDir::setCurrent(path)) {
            emit currentPathChanged(path);
            return true;
        }
    }
    return false;
}

/*!
 * \qmlmethod QDirValueType Dir::current()
 */
QString Dir::currentPath() const
{
    return QDir::currentPath();
}

/*!
 * \qmlmethod QDirValueType Dir::currentScript()
 */
QString Dir::currentScriptPath() const
{
    return m_currentScriptPath;
}

/*!
 * \qmlmethod QDirValueType Dir::home()
 */
QString Dir::homePath() const
{
    return QDir::homePath();
}

/*!
 * \qmlmethod QDirValueType Dir::root()
 */
QString Dir::rootPath() const
{
    return QDir::rootPath();
}

/*!
 * \qmlmethod QDirValueType Dir::temp()
 */
QString Dir::tempPath() const
{
    return QDir::tempPath();
}

/*!
 * \qmlmethod bool Dir::match( array<string> filters, string fileName)
 * \qmlmethod bool Dir::match( string filter, string fileName)
 */
bool Dir::match(const QStringList &filters, const QString &fileName)
{
    LOG("Dir::match", filters, fileName);
    return QDir::match(filters, fileName);
}

bool Dir::match(const QString &filter, const QString &fileName)
{
    LOG("Dir::match", filter, fileName);
    return QDir::match(filter, fileName);
}

/*!
 * \qmlmethod string Dir::cleanPath( string path)
 */
QString Dir::cleanPath(const QString &path)
{
    LOG("Dir::cleanPath", path);
    return QDir::cleanPath(path);
}

/*!
 * \qmlmethod QDirValueType Dir::create( string path)
 */
QDirValueType Dir::create(const QString &path)
{
    LOG("Dir::create", path);
    return QDirValueType(path);
}

} // namespace Core
