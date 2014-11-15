#ifndef COMMANDS_H
#define COMMANDS_H

#include <QString>

namespace FTP_QUERY {

    const QByteArray ABOR = "ABOT"; // Прервать передачу файла.
    const QByteArray CDUP = "CDUP"; // Сменить директорию на вышестоящую.
    const QByteArray CWD  = "CWD";  // Сменить директорию.
    const QByteArray DELE = "DELE"; // Удалить файл (DELE filename).
    const QByteArray EPSV = "EPSV"; // Войти в расширенный пассивный режим. Применяется вместо PASV.
    const QByteArray HELP = "HELP"; // Выводит список команд, принимаемых сервером.
    const QByteArray LIST = "LIST"; // Возвращает список файлов директории. Список передаётся через соединение данных.
    const QByteArray MDTM = "MDTM"; // Возвращает время модификации файла.
    const QByteArray MKD  = "MKD" ; // Создать директорию.
    const QByteArray NLST = "NLST"; // Возвращает список файлов директории в более кратком формате, чем LIST. Список передаётся через соединение данных.
    const QByteArray NOOP = "NOOP"; // Пустая операция.
    const QByteArray PASV = "PASV"; // Войти в пассивный режим. Сервер вернёт адрес и порт, к которому нужно подключиться, чтобы забрать данные. Передача начнётся при введении следующих команд: RETR, LIST и т.д.
    const QByteArray PORT = "PORT"; // Войти в активный режим. Например PORT 12,34,45,56,78,89. В отличие от пассивного режима для передачи данных сервер сам подключается к клиенту.
    const QByteArray PWD  = "PWD";  // Возвращает текущую директорию.
    const QByteArray QUIT = "QUIT"; // Отключиться.
    const QByteArray REIN = "REIN"; // Реинициализировать подключение.
    const QByteArray RETR = "RETR"; // Скачать файл. Перед RETR должна быть команда PASV или PORT.
    const QByteArray RMD  = "EMD";  // Удалить директорию.
    const QByteArray RNFR = "RNFR"; // Переименовать файл. RNFR — что переименовывать.
    const QByteArray RNTO = "RNTO"; // Переименовать файл. RNTO — во что переименовывать.
    const QByteArray SIZE = "SIZE"; // Возвращает размер файла.
    const QByteArray STOR = "STOR"; // Закачать файл. Перед STOR должна быть команда PASV или PORT.
    const QByteArray SYST = "SYST"; // Возвращает тип системы (UNIX, WIN, …).
    const QByteArray TYPE = "TYPE"; // Установить тип передачи файла (бинарный, текстовый).
    const QByteArray USER = "USER"; // Имя пользователя для входа на сервер.
    const QByteArray FEAT = "FEAT"; // Список расширений сервера.
    const QByteArray PASS = "PASS"; //
    const QByteArray MLSD = "MLSD";
}

namespace FTP_ANSWER {

    const QByteArray READY = "220 FTP server ready.\n"; // Сервер готов.
    const QByteArray LOGIN = "230 Login successful.\n"; // Логин подтверждён.
    const QByteArray UNIX  = "215 UNIX Type: L8\n";     // UNIX.
    const QByteArray WIN   = "215 WIN.\n";              // WIN.
    const QByteArray FEAT  = "211-Features:\n";
    const QByteArray FEATE = "211 End\n";
    const QByteArray CD    = "257 \"/\" is the current directory\n";
    const QByteArray TYPE  = "200 Type set to I\n";
    const QByteArray PASV  = "227 Entering Passive Mode (127,0,0,1,184,120)\n";
    const QByteArray PORT  = " \n";
    const QByteArray SNDB  = "150 Here comes the directory listing.\n";
    const QByteArray SNDE  = "226 Directory send OK.\n";
    const QByteArray AUTH  = "230 *** Welcome to this anonymous ftp server! ***\n";
}


#endif // COMMANDS_H
