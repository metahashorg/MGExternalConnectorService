#ifndef MGEXTERNALCONNECTORSERVICE_H
#define MGEXTERNALCONNECTORSERVICE_H

#include <QtService>

namespace mgexternalconnector
{
class ServiceControl;

class MGExternalConnectorService : public QtService<QCoreApplication>
{
public:
    MGExternalConnectorService(int argc, char **argv);

protected:
    void start();
    void stop();

    ServiceControl *serviceControl;
};

}

#endif // MGEXTERNALCONNECTORSERVICE_H
