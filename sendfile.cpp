#include "sendfile.h"

sendFile::sendFile(QObject* parent) : QObject(parent)
{

}

void sendFile::connectHost(QString ip,qint16 port){
    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress(ip),port);
    connect(socket,&QTcpSocket::connected,this,&sendFile::connected);
}

void sendFile::send(QString path){
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QByteArray qb = file.readAll();
    QString command = "Action:Upload File:"+QFileInfo(path).fileName();
    socket->write(command.toUtf8().data());
    socket->flush();
    socket->write(qb);
    socket->flush();
    socket->disconnect();
}
