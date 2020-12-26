#include "MGExternalConnector.h"

#include "ServiceControl.h"

#include "check.h"
#include "Log.h"

#include <QRegularExpression>
#include <QDebug>
#include <QUrl>
#include <QThread>

SET_LOG_NAMESPACE("EXTCONN");

namespace mgexternalconnector
{

MGExternalConnector::MGExternalConnector(ServiceControl* serviceControl) :
  TimerClass(1min, nullptr), serviceControl(serviceControl)
{
    moveToThread(TimerClass::getThread());
}

MGExternalConnector::~MGExternalConnector()
{
    TimerClass::exit();
}

void MGExternalConnector::getUrl(const GetUrlCallback &callback)
{
    LOG << "Get url";
    CHECK(serviceControl, "service control error");
    const auto cb = ServiceControl::GetUrlCallback(
        [callback](bool res, const QString& url) {
            LOG << "Get url res" << res << url;
            callback(url);
        },
        signalFunc);
    QMetaObject::invokeMethod(serviceControl, "getUrl", Qt::QueuedConnection,
        Q_ARG(ServiceControl::GetUrlCallback, cb));
}

void MGExternalConnector::setUrl(const QString& url)
{
    LOG << "Set url " << url;
    CHECK(serviceControl, "service control error");
    const auto cb = ServiceControl::SetUrlCallback(
        [](bool res) {
            qDebug() << res << QThread::currentThread();
        },
        signalFunc);
    QMetaObject::invokeMethod(serviceControl, "setUrl", Qt::QueuedConnection,
        Q_ARG(QString, url),
        Q_ARG(ServiceControl::SetUrlCallback, cb));
}

void MGExternalConnector::onUrlChanged(const QString& url)
{
    LOG << "Url changed " << url;
    QUrl purl(url);
    if (purl.host() == QLatin1String("example.com") &&
            purl.query() == QLatin1String("q=Hello")) {

        qDebug() << "host " << purl.host() << purl.query();
        purl.setQuery(purl.query() + QLatin1String("Kitty"));
        qDebug() << purl.toString();
        setUrl(purl.toString());
    }
}

void MGExternalConnector::onUrlEntered(const QString &url)
{
    LOG << "Url entered " << url;
    qDebug() << "!";
    QUrl purl(url);
    if (purl.host() != QLatin1String("example.com"))
        return;

    QString query = purl.query();
    const QRegularExpression regexp(QStringLiteral("q=(\\d+)"));
    QRegularExpressionMatch match = regexp.match(query);
    if (match.hasMatch()) {
        const int num = match.captured(1).toInt() + 1;
        query.replace(regexp, QString("q=%1").arg(num));
    }
    purl.setQuery(query);
    qDebug() << purl.toString();
    setUrl(purl.toString());
}

void MGExternalConnector::startMethod()
{
}

void MGExternalConnector::timerMethod()
{
    /* example on timer
     * getUrl([this](const QString &url){
        qDebug() << "Get URL:" << url;
        setUrl(url + "...a");
    });
    */
}

void MGExternalConnector::finishMethod()
{
}

} // namespace mgexternalconnector
