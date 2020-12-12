#include "Paths.h"

#include <QCoreApplication>
#include <QDebug>
#include <QSettings>
#include <QStandardPaths>
#include "utilites/utils.h"

#include "QtService"

const static QString LOCALSERVERIN_NAME = "metahash.externalconnector.in";
const static QString LOCALSERVEROUT_NAME = "metahash.externalconnector.out";


const static QString LOG_PATH = "logs/";

static QString metagatePath = "/tmp";


QString getInLocalServerPath()
{
#ifdef Q_OS_UNIX
    return makePath(metagatePath, LOCALSERVERIN_NAME);
#else
    return LOCALSERVERIN_NAME;
#endif
}

QString getOutLocalServerPath()
{
#ifdef Q_OS_UNIX
    return makePath(metagatePath, LOCALSERVEROUT_NAME);
#else
    return LOCALSERVEROUT_NAME;
#endif
}

QString getLogPath()
{
    const QString res = makePath(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation), LOG_PATH);
    createFolder(res);
    qDebug() << res;
    return res;
}
