#include "LocalClient.h"

#include <QLocalSocket>

using namespace std::placeholders;

#include "Log.h"
#include "check.h"

#include "qt_utilites/QRegister.h"
#include "qt_utilites/SlotWrapper.h"

namespace localconnection
{

LocalClient::LocalClient(const QString& localServerName, QObject* parent) :
  QObject(parent), localServerName(localServerName)
{
    Q_REG(LocalClient::ReturnCallback, "LocalClient::ReturnCallback");
}

void LocalClient::sendRequest(
    const QByteArray& request, const LocalClient::ClientCallback& callback)
{
    const size_t currId = id.get();
    callbacks[currId] = callback;

    QLocalSocket* socket = new QLocalSocket(this);

    Q_CONNECT(
        socket,
        &QLocalSocket::readyRead,
        this,
        std::bind(&LocalClient::onTextMessageReceived, this, currId));
    Q_CONNECT(
        socket,
        QOverload<QLocalSocket::LocalSocketError>::of(&QLocalSocket::error),
        this,
        std::bind(&LocalClient::onErrorMessageReceived, this, currId, _1));

    QDataStream& outStream = buffers[currId].dataStream;
    outStream.setVersion(QDataStream::Qt_5_10);
    outStream.setDevice(socket);

    socket->connectToServer(localServerName);
    if (socket->isValid())
    {
        QByteArray block;
        QDataStream outStream(&block, QIODevice::WriteOnly);
        outStream.setVersion(QDataStream::Qt_5_10);
        outStream << static_cast<quint32>(request.size());
        outStream << request;
        socket->write(block);
        socket->flush();
    }
}

template <typename... Message>
void LocalClient::runCallback(size_t id, Message&&... messages)
{
    const auto foundCallback = callbacks.find(id);
    CHECK(
        foundCallback != callbacks.end(),
        "not found callback on id " + std::to_string(id));
    const auto callback =
        std::bind(foundCallback->second, std::forward<Message>(messages)...);
    emit callbackCall(callback);
    callbacks.erase(foundCallback);
    buffers.erase(id);
}

void LocalClient::onTextMessageReceived(size_t id)
{
    BEGIN_SLOT_WRAPPER
    QLocalSocket* socket = qobject_cast<QLocalSocket*>(sender());
    qDebug() << "EEEE";
    CHECK(buffers.find(id) != buffers.end(), "Incorrect response");
    Buffer& currentBuffer = buffers[id];

    if (currentBuffer.size == 0)
    {
        if (socket->bytesAvailable() < (qint64)sizeof(quint32))
        {
            return;
        }

        currentBuffer.dataStream >> currentBuffer.size;
    }
    qDebug() << currentBuffer.size << currentBuffer.size;

    if (socket->bytesAvailable() < currentBuffer.size
        || currentBuffer.dataStream.atEnd())
    {
        return;
    }

    Response resp;
    resp.response.resize(currentBuffer.size);
    currentBuffer.dataStream >> resp.response;

    runCallback(id, resp);

    socket->deleteLater();
    END_SLOT_WRAPPER
}

void LocalClient::onErrorMessageReceived(
    size_t id, QLocalSocket::LocalSocketError socketError)
{
    BEGIN_SLOT_WRAPPER
    QLocalSocket* socket = qobject_cast<QLocalSocket*>(sender());

    Response resp;

    switch (socketError)
    {
        case QLocalSocket::ServerNotFoundError:
        {
            resp.exception = ServerException(1, "Server not found");
            break;
        }
        case QLocalSocket::ConnectionRefusedError:
        {
            resp.exception = ServerException(2, "Connection refused");
            break;
        }
        case QLocalSocket::PeerClosedError:
        {
            resp.exception = ServerException(3, "Peer closed");
            break;
        }
        default:
        {
            resp.exception =
                ServerException(4, socket->errorString().toStdString());
        }
    }

    runCallback(id, resp);

    socket->deleteLater();
    END_SLOT_WRAPPER
}

} // namespace localconnection
