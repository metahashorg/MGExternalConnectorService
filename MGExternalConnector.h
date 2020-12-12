#ifndef MGEXTERNALCONNECTOR_H
#define MGEXTERNALCONNECTOR_H

#include "qt_utilites/CallbackCallWrapper.h"
#include "qt_utilites/TimerClass.h"

namespace mgexternalconnector
{

class ServiceControl;

class MGExternalConnector : public CallbackCallWrapper, public TimerClass
{
    Q_OBJECT
public:
    using GetUrlCallback = std::function<void(const QString &url)>;

    explicit MGExternalConnector(ServiceControl *serviceControl);
    ~MGExternalConnector() override;

public slots:
    void getUrl(const GetUrlCallback &callback);
    void setUrl(const QString &url);
    void onUrlChanged(const QString &url);
    void onUrlEntered(const QString &url);

protected:
    void startMethod() override;
    void timerMethod() override;
    void finishMethod() override;

private:
    ServiceControl *serviceControl = nullptr;
};

}

#endif // MGEXTERNALCONNECTOR_H
