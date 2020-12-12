#ifndef LOGGING_H
#define LOGGING_H

#include <QtService>
#include "Log.h"

inline void DLog(const QString &message, QtServiceBase::MessageType type)
{
    QtServiceBase::instance()->logMessage(message, type);
    LOG << message;
}

#endif // LOGGING_H
