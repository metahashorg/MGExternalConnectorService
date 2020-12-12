#include "MGExternalConnectorService.h"

#include "ServiceControl.h"
#include <QDebug>

#include "Paths.h"
#include <QDir>
#include <QThread>
#include "Log.h"
#include "utilites/machine_uid.h"

namespace mgexternalconnector {

MGExternalConnectorService::MGExternalConnectorService(int argc, char **argv)
    : QtService<QCoreApplication>(argc, argv, QStringLiteral("MG External Connector Service"))
    , serviceControl(nullptr)
{
    setServiceDescription(QStringLiteral("MG External Connector Service"));
    //setServiceFlags(QtServiceBase::CannotBeStopped);
    setStartupType(QtServiceController::AutoStartup);
}


void MGExternalConnectorService::start()
{
    QCoreApplication *app = application();
    QtServiceBase::instance()->logMessage(QStringLiteral("Service dir %1").arg(getLogPath()), QtServiceBase::Information);
    initLog();

    serviceControl = new ServiceControl(app);
}

void MGExternalConnectorService::stop()
{
    if (serviceControl) {
        delete serviceControl;
    }
}

}
