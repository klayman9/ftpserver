#include "FtpServer.h"
#include "ftp.h"

FtpServer::FtpServer(QObject *parent) : QTcpServer(parent)
{

}

FtpServer::~FtpServer()
{

}

void FtpServer::start()
{
    if (listen(QHostAddress::LocalHost, FTP::DEFAULT_CONNECT_PORT))
        emit serverEvent(tr("Сервер успешно запущен"), Qt::darkGreen);
    else
        emit serverEvent(tr("Ошибка при запуске сервера"), Qt::red);
}

void FtpServer::stop()
{
    close();
    emit serverEvent(tr("Сервер остановлен"), Qt::darkGreen);
}

void FtpServer::incomingConnection(int socketDescriptor)
{
    qDebug() << QString("FtpServer new connection: %1").arg(socketDescriptor);
    FtpServerThread *ftpThread = new FtpServerThread(socketDescriptor);
    connect(ftpThread, SIGNAL(finished()),
            ftpThread, SLOT(deleteLater()));
    m_ftpThreadList.append(ftpThread);
    ftpThread->start();
}

void FtpServer::readyRead()
{

}
