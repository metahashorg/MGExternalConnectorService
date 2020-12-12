#ifndef SERVICECONTROL_H
#define SERVICECONTROL_H

#include "MGExternalConnector.h"

#include <QObject>
#include <memory>

namespace localconnection
{
class LocalServer;
class LocalServerRequest;
class LocalClient;
}

namespace mgexternalconnector
{
class MGExternalConnector;

class ServiceControl : public QObject
{
    Q_OBJECT
public:
    using GetUrlCallback = CallbackWrapper<void(bool res, const QString &url)>;
    using SetUrlCallback = CallbackWrapper<void(bool res)>;

    ServiceControl(QObject* parent = nullptr);
    virtual ~ServiceControl();

public slots:
    void getUrl(const ServiceControl::GetUrlCallback &callback);
    void setUrl(const QString &url, const ServiceControl::SetUrlCallback &callback);

private slots:
    void onRequest(std::shared_ptr<localconnection::LocalServerRequest> request);

private:
    localconnection::LocalServer* localServer;
    localconnection::LocalClient* localClient;

    MGExternalConnector *connector;
};

} // namespace mgexternalconnector

#endif // SERVICECONTROL_H
