#ifndef FTPSERVER_H
#define FTPSERVER_H

#include <QtNetwork/QTcpServer>
#include <QColor>

#include "FtpServerThread.h"

class FtpServer : public QTcpServer
{
    Q_OBJECT

public:
    FtpServer(QObject *parent = 0);
    ~FtpServer();

    void start();
    void stop();

signals:
    void serverEvent(QString, QColor color = Qt::black);

protected:
    void incomingConnection(int socketDescriptor);
    void readyRead();

private:
    int m_port;
    FtpServerThread *m_ftpThread;
    QList<FtpServerThread *> m_ftpThreadList;
};

#endif // FTPSERVER_H
