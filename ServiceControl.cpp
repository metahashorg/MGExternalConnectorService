#include "ServiceControl.h"

#include <QDebug>
#include <QSettings>
#include <QThread>
#include <QTimer>

#include <QJsonDocument>
#include <QJsonObject>

#include "LocalClient.h"
#include "LocalServer.h"
#include "Log.h"
#include "MGExternalConnector.h"
#include "Paths.h"
#include "check.h"
#include "duration.h"
#include "qt_utilites/QRegister.h"
#include "qt_utilites/SlotWrapper.h"

SET_LOG_NAMESPACE("EXTCONN");

namespace
{
const QString UrlChangedMethod = QLatin1String("UrlChanged");
const QString GetUrlMethod = QLatin1String("GetUrl");
const QString SetUrlMethod = QLatin1String("SetUrl");

struct GetUrlResponse
{
    bool error = false;
    QString url;
};

QByteArray makeGetUrlRequest()
{
    QJsonObject json;
    json.insert(QStringLiteral("method"), GetUrlMethod);
    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

QByteArray makeSetUrlRequest(const QString& url)
{
    QJsonObject json;
    json.insert(QStringLiteral("method"), SetUrlMethod);
    json.insert(QStringLiteral("url"), url);
    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

GetUrlResponse parseGetUrlResponse(const QByteArray& message)
{
    GetUrlResponse res;
    const QJsonDocument jsonResponse = QJsonDocument::fromJson(message);
    CHECK(jsonResponse.isObject(), "Incorrect json");
    const QJsonObject& root = jsonResponse.object();

    CHECK(
        root.contains(QLatin1String("result"))
            && root.value(QLatin1String("result")).isString(),
        "result field not found");
    const QString rt = root.value(QLatin1String("result")).toString();
    if (rt == QLatin1String("OK"))
        res.error = false;
    else
        res.error = true;
    if (!res.error)
    {
        CHECK(
            root.contains(QLatin1String("url"))
                && root.value(QLatin1String("url")).isString(),
            "result field not found");
        res.url = root.value(QLatin1String("url")).toString();
    }
    return res;
}

bool parseSetUrlResponse(const QByteArray& message)
{
    bool error = true;
    const QJsonDocument jsonResponse = QJsonDocument::fromJson(message);
    CHECK(jsonResponse.isObject(), "Incorrect json");
    const QJsonObject& root = jsonResponse.object();

    CHECK(
        root.contains(QLatin1String("result"))
            && root.value(QLatin1String("result")).isString(),
        "result field not found");
    const QString rt = root.value(QLatin1String("result")).toString();
    if (rt == QLatin1String("OK"))
        error = false;
    else
        error = true;
    return error;
}

QString parseMethodAtRequest(const QJsonDocument& request)
{
    CHECK(request.isObject(), "Request field not found");
    const QJsonObject root = request.object();
    CHECK(
        root.contains("method") && root.value("method").isString(),
        "'method' field not found");
    const QString method = root.value("method").toString();
    return method;
}

QString parseUrlChangedRequest(const QJsonDocument& request)
{
    CHECK(request.isObject(), "Request field not found");
    const QJsonObject root = request.object();
    CHECK(
        root.contains("url") && root.value("url").isString(),
        "'url' field not found");
    return root.value("url").toString();
}

QByteArray makeUrlChangedResponse()
{
    QJsonObject json;
    json.insert(QStringLiteral("result"), QStringLiteral("OK"));
    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

} // namespace

namespace mgexternalconnector
{

ServiceControl::ServiceControl(QObject* parent) :
  QObject(parent),
  localServer(new localconnection::LocalServer(getOutLocalServerPath(), this)),
  localClient(new localconnection::LocalClient(getInLocalServerPath(), this)),
  connector(new MGExternalConnector(this))
{
    connector->start();
    Q_CONNECT(
        localServer,
        &localconnection::LocalServer::request,
        this,
        &ServiceControl::onRequest);

    Q_CONNECT3(
        localClient,
        &localconnection::LocalClient::callbackCall,
        [](localconnection::LocalClient::ReturnCallback callback) {
            callback();
        });

    Q_REG(ServiceControl::GetUrlCallback, "ServiceControl::GetUrlCallback");
    Q_REG(ServiceControl::SetUrlCallback, "ServiceControl::SetUrlCallback");
    qDebug() <<  QThread::currentThread();

}

ServiceControl::~ServiceControl()
{
}

void ServiceControl::getUrl(const GetUrlCallback& callback)
{
    qDebug() <<  QThread::currentThread();

    localClient->sendRequest(
        makeGetUrlRequest(),
        [callback](const localconnection::LocalClient::Response& response) {
            BEGIN_SLOT_WRAPPER
            qDebug() << "ok";
            const GetUrlResponse result = parseGetUrlResponse(response.response);
            callback.emitCallback(result.error, result.url);
            END_SLOT_WRAPPER
        });
}

void ServiceControl::setUrl(
    const QString& url, const SetUrlCallback& callback)
{
    qDebug() << "setUrl";
    localClient->sendRequest(
        makeSetUrlRequest(url),
        [callback](const localconnection::LocalClient::Response& response) {
            BEGIN_SLOT_WRAPPER
            qDebug() << "ok";
            QString status;
            /*const TypedException exception = apiVrapper2([&] {
                const GetUrlResponse result =
            parseGetUrlResponse(response.response);
                //CHECK_TYPED(!result,
            TypeErrors::EXTERCTORCONNECTOR_URLCHANGED_ERROR, "UrlChanged
            error");
            });*/
            callback.emitCallback(true);
            END_SLOT_WRAPPER
        });
}

void ServiceControl::onRequest(
    std::shared_ptr<localconnection::LocalServerRequest> request)
{
    qDebug() << request->request();
    const QJsonDocument reqJson = QJsonDocument::fromJson(request->request());
    const QString method = parseMethodAtRequest(reqJson);
    if (method == QStringLiteral("UrlChanged"))
    {
        const QString url = parseUrlChangedRequest(reqJson);
        request->response(makeUrlChangedResponse());
        QMetaObject::invokeMethod(
            connector,
            "onUrlChanged",
            Qt::QueuedConnection,
            Q_ARG(QString, url));
    }
}

} // namespace mgexternalconnector
