#ifndef CALLBACKCALLWRAPPER_H
#define CALLBACKCALLWRAPPER_H

#include <QObject>

#include <functional>

using SignalFunc = std::function<void(const std::function<void()> &callback)>;

template<typename Callback>
class CallbackWrapper
{

private:


public:

    CallbackWrapper() = default;

    CallbackWrapper(const std::function<Callback> &callback, const SignalFunc &signal)
        : callback(callback)
        , signal(signal)
    {}

    template<typename ...Args>
    void emitCallback(Args&& ...args) const
    {
        const auto cb = std::bind(callback, std::forward<Args>(args)...);
        emit signal(cb);
    }
/*
    template<typename ...Args>
    void operator() (Args&& ...args) const {
        emitFunc(exception, std::forward<Args>(args)...);
    }
*/
private:
    std::function<Callback> callback;
    SignalFunc signal;
};



class CallbackCallWrapper : public QObject {
    Q_OBJECT
public:

    using Callback = std::function<void()>;

public:
    explicit CallbackCallWrapper(QObject *parent = nullptr);

    virtual ~CallbackCallWrapper();

signals:

    void callbackCall(const CallbackCallWrapper::Callback &callback);

private slots:

    void onCallbackCall(const CallbackCallWrapper::Callback &callback);

protected:

    SignalFunc signalFunc;
};

#endif // CALLBACKCALLWRAPPER_H
