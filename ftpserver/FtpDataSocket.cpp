#include "FtpDataSocket.h"

FtpDataSocket::FtpDataSocket(int id, QObject *parent)
    : QTcpServer(parent), m_id(id)
{
    m_dataSocket = new QTcpSocket();

    connect(m_dataSocket, SIGNAL(readyRead()),
            this, SLOT(readyRead()), Qt::DirectConnection);

    connect(m_dataSocket, SIGNAL(disconnected()),
            this, SLOT(disconnected()), Qt::DirectConnection);
}

FtpDataSocket::~FtpDataSocket()
{

}

void FtpDataSocket::closeDataSocket()
{
    m_dataSocket->waitForBytesWritten();
    m_dataSocket->close();
    m_dataSocket->waitForDisconnected();
}

void FtpDataSocket::sendData(QByteArray data)
{
    m_dataSocket->write(data);
    m_dataSocket->waitForBytesWritten();
}

int FtpDataSocket::id()
{
    return m_id;
}

void FtpDataSocket::incomingConnection(int socketDescriptor)
{
    qDebug() << QString("FtpDataSocket[%1] incoming connection [%2]")
                .arg(m_id).arg(socketDescriptor);

    m_dataSocket->setSocketDescriptor(socketDescriptor);
    emit goodConnect();
}

void FtpDataSocket::readyRead()
{
    qDebug() << QString("FtpDataSocket[%1] read data: %2")
                .arg(m_dataSocket->peerPort())
                .arg(QString(m_dataSocket->readAll()));
}

void FtpDataSocket::disconnected()
{
    qDebug() << QString("Data socket[%1] disconnected[%2]")
                .arg(m_id)
                .arg(m_dataSocket->socketDescriptor());
    //m_dataSocket->deleteLater();
}
