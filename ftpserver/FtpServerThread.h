#ifndef FTPSERVERTHREAD_H
#define FTPSERVERTHREAD_H

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QStringList>
#include <QEventLoop>
#include <QThread>
#include <QTimer>
#include <QDir>
#include "FtpDataSocket.h"

class FtpServerThread : public QThread
{
    Q_OBJECT

public:
    FtpServerThread(int socketDescriptor, QObject *parent = 0);
    ~FtpServerThread();

    void setCurrentDirectory(QString directoryName);

protected:
    void run();

private slots:
    void readyRead();
    void dataReadyRead();
    void commandSocketDisconnected();
    void dataSocketDisconnect();
    void sendFile(QString fileName);
    void sendCommand(QByteArray command);
    void sendCommand(QString command);
    void sendData(QByteArray data);

private:
    QStringList directoryList();
    void sendDirectoryList();

private:
    //! Дескриптор потока.
    int m_socketDescriptor;

    //! Сокет команд.
    QTcpSocket *m_commandSocket;

    //! Сокет данных.
    FtpDataSocket *m_dataSocket;

    //! Текущая директория.
    QDir m_currentDirectory;
};

#endif // FTPSERVERTHREAD_H
