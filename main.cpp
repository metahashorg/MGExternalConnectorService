#include <QCoreApplication>
#include <QSettings>
#include <QDir>

#include "utilites/machine_uid.h"
#include "Log.h"
#include "MGExternalConnectorService.h"

using namespace mgexternalconnector;

int main(int argc, char **argv)
{
#if !defined(Q_OS_WIN)
    // QtService stores service settings in SystemScope, which normally require root privileges.
    // To allow testing this example as non-root, we change the directory of the SystemScope settings file.
    //QSettings::setPath(QSettings::NativeFormat, QSettings::SystemScope, QDir::tempPath());
    //qWarning("(Example uses dummy settings file: %s/QtSoftware.conf)", QDir::tempPath().toLatin1().constData());
#endif
    MGExternalConnectorService service(argc, argv);
    return service.exec();
}
