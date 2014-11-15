#ifndef FTP_H
#define FTP_H

#include <QObject>

namespace FTP {

    // Порт для подключения.
    const quint16 DEFAULT_CONNECT_PORT = 21;

    // Порт для подключения.
    const quint16 TRANSFER_BLOCK_SIZE = 32768;
}

#endif // FTP_H
