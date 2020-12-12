TARGET   = MGExternalConnectorService
TEMPLATE = app
QT = core network

CONFIG += c++17
CONFIG += console qt
unix:!macx: CONFIG += static
#CONFIG -= app_bundle


DEFINES += VERSION_STRING=\\\"1.20.4\\\"

#DEFINES += PRODBUILD

INCLUDEPATH = ./ ./qtservice

SOURCES  = main.cpp \
        LocalServer.cpp \
        LocalClient.cpp \
        MGExternalConnectorService.cpp \
        ServiceControl.cpp \
        MGExternalConnector.cpp \
        Paths.cpp \
        Log.cpp \
        qt_utilites/TimerClass.cpp \
        qt_utilites/QRegister.cpp \
        qt_utilites/CallbackCallWrapper.cpp \
        qt_utilites/ManagerWrapper.cpp \
        utilites/WaitingWithTimeout.cpp \
        utilites/utils.cpp

HEADERS = \
        LocalServer.h \
        LocalClient.h \
        Logging.h \
        MGExternalConnectorService.h \
        ServiceControl.h \
        MGExternalConnector.h \
        Paths.h \
        Log.h \
        qt_utilites/TimerClass.h \
        qt_utilites/QRegister.h \
        qt_utilites/CallbackCallWrapper.h \
        qt_utilites/ManagerWrapper.h \
        utilites/WaitingWithTimeout.h

#win32: QMAKE_CXXFLAGS_RELEASE -= -Zc:strictStrings
#win32: QMAKE_CFLAGS_RELEASE -= -Zc:strictStrings
#win32: QMAKE_CFLAGS -= -Zc:strictStrings
#win32: QMAKE_CXXFLAGS -= -Zc:strictStrings

#win32: LIBS += -lIphlpapi


unix:!macx: QMAKE_LFLAGS += -static-libstdc++ -static-libgcc

#unix:!macx: DEFINES += TARGET_OS_MAC
win32: DEFINES += TARGET_WINDOWS
macx: DEFINES += TARGET_OS_MAC
macx: LIBS += -framework DiskArbitration -framework CoreFoundation

include(qtservice/qtservice.pri)
