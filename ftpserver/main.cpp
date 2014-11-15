
#include <QApplication>
#include <QTextCodec>
#include <QDateTime>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    qDebug() << QDir::rootPath();

    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    int exitCode = app.exec();
    return exitCode;
}
