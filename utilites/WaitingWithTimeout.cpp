#include "WaitingWithTimeout.h"
#include <QDebug>
WaitingWithTimeout::WaitingWithTimeout(std::chrono::milliseconds timeout, QObject *parent)
    : QEventLoop(parent)
    , m_timerId(0)
{
    m_timerId = startTimer(static_cast<int>(timeout.count()));
}

WaitingWithTimeout::~WaitingWithTimeout()
{
    qDebug() << "destr";
}

void WaitingWithTimeout::wait(std::chrono::milliseconds timeout, QEventLoop::ProcessEventsFlags flags)
{
    WaitingWithTimeout loop(timeout);
    loop.exec(flags);
}

void WaitingWithTimeout::timerEvent(QTimerEvent *)
{
    killTimer(m_timerId);
    quit();
}
