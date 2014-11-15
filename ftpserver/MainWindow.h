#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "FtpServer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void translateGui();

public slots:
    void startServer();
    void stopServer();
    void serverEvent(QString event, QColor = Qt::black);

private:
    void connection();

private:
    Ui::MainWindow *ui;
    FtpServer *m_ftpServer;
};

#endif // MAINWINDOW_H
