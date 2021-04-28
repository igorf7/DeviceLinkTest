QT       += core gui network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    comport.cpp \
    comportview.cpp \
    comworkview.cpp \
    main.cpp \
    mainwindow.cpp \
    tcpclient.cpp \
    tcpportview.cpp \
    tcpworkview.cpp

HEADERS += \
    comport.h \
    comportview.h \
    comworkview.h \
    mainwindow.h \
    tcpclient.h \
    tcpportview.h \
    tcpworkview.h

FORMS += \
    comportview.ui \
    comworkview.ui \
    mainwindow.ui \
    tcpportview.ui \
    tcpworkview.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
