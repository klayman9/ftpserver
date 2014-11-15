TEMPLATE = app

CONFIG += qt gui

QT += widgets

SOURCES += \
    main.cpp \
    FtpDataSocket.cpp \
    FtpServer.cpp \
    FtpServerThread.cpp \
    MainWindow.cpp

HEADERS += \
    Commands.h \
    Ftp.h \
    FtpDataSocket.h \
    FtpServer.h \
    FtpServerThread.h \
    MainWindow.h

FORMS += \
    MainWindow.ui
