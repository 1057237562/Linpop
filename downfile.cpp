#include "downfile.h"

downFile::downFile(QObject *parent) : QObject(parent)
{

}

void downFile::connectHost(QString ip,qint16 port){
    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress(ip),port);
    connect(socket,&QTcpSocket::connected,this,&downFile::connected);
}

void downFile::fetch(QString path,QString filename){
    QString command = "Action:Download File:"+filename;
    socket->write(command.toUtf8().data());
    socket->connect(socket,&QTcpSocket::readyRead,this,[=]{
        socket->flush();
        QByteArray arr = socket->readAll();
        QFile file(path);
        file.open(QIODevice::ReadWrite);
        file.write(arr);
        socket->disconnect();
    });
}
