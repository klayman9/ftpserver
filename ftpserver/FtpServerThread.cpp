#include <QDateTime>

#include "FtpServerThread.h"
#include "commands.h"

FtpServerThread::FtpServerThread(int socketDescritor, QObject *parent)
    : QThread(parent), m_socketDescriptor(socketDescritor),
      m_commandSocket(NULL),
      m_dataSocket(NULL)
{

}

FtpServerThread::~FtpServerThread()
{

}

void FtpServerThread::run()
{
    qDebug() << QString("Run socket[%1] thread").arg(m_socketDescriptor);

    m_dataSocket = new FtpDataSocket(m_socketDescriptor);

    m_commandSocket = new QTcpSocket();
    m_commandSocket->setSocketDescriptor(m_socketDescriptor);

    connect(m_commandSocket, SIGNAL(readyRead()),
            this, SLOT(readyRead()), Qt::DirectConnection);

    connect(m_commandSocket, SIGNAL(disconnected()),
            this, SLOT(commandSocketDisconnected()), Qt::DirectConnection);

    sendCommand(QString("220 ProFTPD 1.3.2rc3 Server (STREAM NNOV FTP Server) "
                        "[::ffff:127.0.0.1]\n").toLatin1());

    m_currentDirectory = QDir::root();

    exec();
}

void FtpServerThread::readyRead()
{
    QByteArray data(m_commandSocket->readAll());
    qDebug() << QString("Socket[%1] read data: %2")
                .arg(m_commandSocket->socketDescriptor())
                .arg(QString(data));

    QRegExp regExp("(\\w+)");

    QString buff = QString(data).section(" ", 1);
    QString argument;

    for (int i = 0; i < buff.count() - 2; i++)
        argument.append(buff[i]);

    qDebug() << argument;

    if (data.isEmpty())
        qDebug() << "Empty data";

    if (data.indexOf(FTP_QUERY::USER) == 0) {

        sendCommand(QString("330 Anonymous login ok.\n").toLatin1());
        return;
    }

    if (data.indexOf(FTP_QUERY::PASS) == 0) {

        sendCommand(FTP_ANSWER::AUTH);
        return;
    }

    if (data.indexOf(FTP_QUERY::SYST) == 0) {

        sendCommand(QString("215 UNIX Type: L8\n").toLatin1());
        return;
    }

    if (data.indexOf(FTP_QUERY::FEAT) == 0) {

        sendCommand(QString("211-Features:\n").toLatin1());
        sendCommand(QString(" MDTM\n").toLatin1());
        sendCommand(QString(" MFMT\n").toLatin1());
        sendCommand(QString(" TVFS\n").toLatin1());
        sendCommand(QString(" MFF modify;UNIX.group;UNIX.mode;\n").toLatin1());
        sendCommand(QString(" MLST modify*;size*;type*;\n").toLatin1());
        sendCommand(QString(" REST STREAM\n").toLatin1());
        sendCommand(QString(" SIZE\n").toLatin1());
        sendCommand(QString("211 End\n").toLatin1());

        return;
    }

    if (data.indexOf(QString("OPTS").toLatin1()) == 0) {

        sendCommand(QString("200 UTF8 set to on\n").toLatin1());
        return;
    }

    else if (data.indexOf(FTP_QUERY::PWD) == 0) {

        //m_commandSocket->write(QString("257 \"/\" is the current directory\n").toAscii());
        QString directoryName = m_currentDirectory.dirName();

        if (directoryName.isEmpty())
            directoryName = "/";

        sendCommand(QString("257 \"%1\"\n").arg(directoryName));
        return;
    }

    else if (data.indexOf(FTP_QUERY::CWD) == 0) {

        if (argument.isEmpty()) {

        }

        qDebug() << m_currentDirectory.path();

        if (argument.contains("/"))
            m_currentDirectory.setPath(QDir::rootPath() + argument);
        else
            m_currentDirectory.setPath(QString(m_currentDirectory.path()) +
                                       "/" + argument);

        qDebug() << m_currentDirectory.path();

        sendCommand(QString("250 CWD command successful\n").toLatin1());
        //m_commandSocket->write(QString("257 \"/\" is the current directory\n").toAscii());
        return;
    }

    else if (data.indexOf(FTP_QUERY::TYPE) == 0) {

        sendCommand(QString("200 Type set to I\n").toLatin1());
        return;
    }

    else if (data.indexOf(FTP_QUERY::CDUP) == 0) {

        if (m_currentDirectory.cdUp())
            sendCommand(QString("250 Directory successfully changed.\n")
                        .toLatin1());
        else
            sendCommand(QString("257 \"/\" is the current directory\n")
                        .toLatin1());

        qDebug() << m_currentDirectory.path();

        return;
    }

    else if (data.indexOf(FTP_QUERY::PASV) == 0) {

        if (m_dataSocket->isListening())
            m_dataSocket->close();

        if (m_dataSocket->listen(QHostAddress::LocalHost, 0)) {

            qDebug() << QString("Server port: %1")
                        .arg(m_dataSocket->serverPort());

            qDebug() << m_dataSocket->serverPort()/256;
            qDebug() << m_dataSocket->serverPort()%256;

            qDebug() << "Good listening.";

            sendCommand(QString("227 Entering Passive Mode (127,0,0,1,%1,%2)\n")
                        .arg(m_dataSocket->serverPort()/256)
                        .arg(m_dataSocket->serverPort()%256));
        }

        return;
    }

    else if (data.indexOf(FTP_QUERY::PORT) == 0) {

        /*	PORT n1,n2,n3,n4,n5,n6
         *	IP адрес клиента (n1.n2.n3.n4)
         *	Порт (n5 x 256 + n6)
         */
        int port = QString(data).section(",", 4, 4).toInt() * 256 +
                QString(data).section(",", 5, 5).toInt();

        sendCommand(QString("250 port ok\n"));
        return;
    }

    else if (data.indexOf(FTP_QUERY::NOOP) == 0) {

        sendCommand(QString("200 NOOP command successful\n"));
        return;
    }

    else if (data.indexOf(FTP_QUERY::MLSD) == 0) {

        QEventLoop eventLoop;
        connect(m_dataSocket, SIGNAL(goodConnect()),
                &eventLoop, SLOT(quit()));

        sendCommand(QString("150 Opening ASCII mode data connection for MLSD\n")
                    .toLatin1());

        eventLoop.exec();

        disconnect(m_dataSocket, SIGNAL(goodConnect()),
                &eventLoop, SLOT(quit()));

        qDebug() << "sendlist";
        sendDirectoryList();

        m_dataSocket->closeDataSocket();

        sendCommand(QString("226 Transfer complete\n").toLatin1());

        return;
    }

    else if (data.indexOf(FTP_QUERY::MKD) == 0) {

        m_currentDirectory.cd(m_currentDirectory.absolutePath());

        if (m_currentDirectory.mkdir(argument))
            sendCommand(QString("257 Directory [%1] created.")
                        .arg(argument));
        else
            sendCommand(QString("451 Directory [%1] not created.")
                        .arg(argument));

        return;
    }

    else if (data.indexOf(FTP_QUERY::RETR) == 0) {

        qDebug() << "RETR";
        qDebug() << QString(data).section(" ", -1);

        QEventLoop eventLoop;

        sendCommand(QString("150 Opening ASCII mode data connection "
                            "for video.mp4 (224 bytes)\n").toLatin1());

        connect(m_dataSocket, SIGNAL(goodConnect()),
                &eventLoop,	SLOT(quit()));

        eventLoop.exec();

        disconnect(m_dataSocket, SIGNAL(goodConnect()),
                   &eventLoop, SLOT(quit()));

        qDebug() << "sending";
        sendFile(argument);

        sendCommand(QString("226 Transfer complete\n").toLatin1());

        return;
    }

    sendCommand(QString("450 Bad command\n").toLatin1());
}

void FtpServerThread::dataReadyRead()
{
    qDebug() << "FtpThread ready read data.";
}

void FtpServerThread::commandSocketDisconnected()
{
    qDebug() << QString("Close socket[%1] thread").arg(m_socketDescriptor);
    m_commandSocket->deleteLater();

    exit();
}

void FtpServerThread::dataSocketDisconnect()
{

}

void FtpServerThread::sendFile(QString fileName)
{
    qDebug() << "Sending file";

    QString path = m_currentDirectory.filePath(fileName);
    qDebug() << path;

    QFile file(path);
    QByteArray read ;
    if (!file.open(QIODevice::ReadOnly)) {

        qDebug() << QString("File [%1] not open!").arg(path);
        return;
    }

    while (1) {

        read.clear();
        read = file.read(32768*8);
        qDebug() << "Read : " << read.size();
        if (read.size() == 0)
           break;

        m_dataSocket->sendData(read);
        read.clear();
    }

    file.close();
    m_dataSocket->closeDataSocket();
}

void FtpServerThread::sendCommand(QByteArray command)
{
    if (command.indexOf("\n") == -1)
        command.append("\n");

    m_commandSocket->write(command);
}

void FtpServerThread::sendCommand(QString command)
{
    sendCommand(command.toLatin1());
}

void FtpServerThread::sendData(QByteArray data)
{
    m_dataSocket->sendData(data);
}

QStringList FtpServerThread::directoryList()
{
    QStringList directoryList;

    qDebug() << m_currentDirectory.path();

    QFileInfoList fileInfoList = m_currentDirectory.entryInfoList();

    for (int i = 0; i < fileInfoList.count(); i++) {

        QFileInfo fileInfo = fileInfoList[i];

        QString type;
        if (fileInfo.isDir())
            type = QString("dir");
        else
            type = QString("file");

        QDate lastModifiedDate = fileInfo.lastModified().date();
        QTime lastModidiedTime = fileInfo.lastModified().time();

        QString item;
        item.append(QString("modify=%1%2%3%4%5%6;")
                    .arg(lastModifiedDate.year())
                    .arg(lastModifiedDate.month())
                    .arg(lastModifiedDate.day())
                    .arg(lastModidiedTime.hour())
                    .arg(lastModidiedTime.minute())
                    .arg(lastModidiedTime.second()));

        item.append(QString("perm=%1;").arg("ReadOnly"));
        item.append(QString("size=%1;").arg(fileInfo.size()));
        item.append(QString("type=%1;").arg(type));
        item.append(QString(" %1\n").arg(fileInfo.fileName()));

        directoryList.append(item);
    }

    return directoryList;
}

void FtpServerThread::sendDirectoryList()
{
    QStringList sendingList = directoryList();

    for (int i = 0; i < sendingList.count(); i++)
        m_dataSocket->sendData(sendingList[i].toLatin1());
}

