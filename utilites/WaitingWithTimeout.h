#ifndef WAITINGWITHTIMEOUT_H
#define WAITINGWITHTIMEOUT_H

#include <QEventLoop>
#include <chrono>

class WaitingWithTimeout : public QEventLoop
{
    Q_OBJECT
public:
    WaitingWithTimeout(std::chrono::milliseconds timeout, QObject *parent = nullptr);
    virtual ~WaitingWithTimeout();

    static void wait(std::chrono::milliseconds timeout, ProcessEventsFlags flags = AllEvents);

protected:
    virtual void timerEvent(QTimerEvent *) override;

private:
    int m_timerId;
};

#endif // WAITINGWITHTIMEOUT_H
