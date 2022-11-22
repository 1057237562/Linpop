#ifndef ETCPSOCKET_H
#define ETCPSOCKET_H
#include<QTcpSocket>
#include<QtGlobal>

// Encrypted TCP Socket
class ETcpSocket: public QTcpSocket
{
public:
    QByteArray readAll();
    qint64 write(const char *data);
    qint64 write(const char *data, qint64 maxSize);
};

#endif // ETCPSOCKET_H
