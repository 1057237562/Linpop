#include "etcpsocket.h"

ETcpSocket::ETcpSocket(QObject *parent): QTcpSocket(parent){
}

QByteArray ETcpSocket::readAll(){
    QByteArray ba = QTcpSocket::readAll();
    char mask = 0xFF;
    for(int i = 0; i < ba.size(); i++){
	char tmp = ba[i];
    ba[i] = ba[i] ^ mask;
	mask = tmp;
    }
    return ba;
}

qint64 ETcpSocket::write(const char *data){
    return write(data, qstrlen(data));
}

qint64 ETcpSocket::write(const char *data, qint64 maxSize){
    char *edata = new char[maxSize];
    char mask = 0xFF;
    for(int i = 0; i < maxSize; i++){
	edata[i] = data[i] ^ mask;
	mask = edata[i];
    }
    return QTcpSocket::write(edata, maxSize);
}
