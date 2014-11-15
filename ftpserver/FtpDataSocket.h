#ifndef FTPDATASOCKET_H
#define FTPDATASOCKET_H

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QFile>

class FtpDataSocket : public QTcpServer
{
    Q_OBJECT

public:
    FtpDataSocket(int id, QObject *parent = 0);
    ~FtpDataSocket();

    void closeDataSocket();
    void sendData(QByteArray data);
    int id();

signals:
    void goodConnect();

protected:
    void incomingConnection(int socketDescriptor);

private slots:
    void readyRead();
    void disconnected();

private:
    int m_id;
    QTcpSocket *m_dataSocket;
};


#endif // FTPDATASOCKET_H
