#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QTime>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_ftpServer = new FtpServer(this);
    translateGui();
    connection();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::translateGui()
{
    setWindowTitle(tr("Ftp server"));
    ui->startButton->setText(tr("Старт"));
    ui->stopButton->setText(tr("Стоп"));
}

void MainWindow::startServer()
{
    m_ftpServer->start();
}

void MainWindow::stopServer()
{
    m_ftpServer->stop();
}

void MainWindow::serverEvent(QString event, QColor color)
{
    ui->messageList->insertItem(0, QTime::currentTime().toString() + " " +
                                event);

    ui->messageList->item(0)->setTextColor(color);
}

void MainWindow::connection()
{
    connect(ui->startButton, SIGNAL(clicked()),
            this, SLOT(startServer()));

    connect(ui->stopButton, SIGNAL(clicked()),
            this, SLOT(stopServer()));

    connect(m_ftpServer, SIGNAL(serverEvent(QString, QColor)),
            this, SLOT(serverEvent(QString, QColor)));
}
