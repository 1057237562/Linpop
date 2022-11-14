#include "sendfile.h"

sendFile::sendFile(QObject* parent) : QObject(parent)
{

}

void sendFile::connectHost(QString ip,qint16 port){
    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress(ip),port);
    connect(socket,&QTcpSocket::connected,this,&sendFile::connected);
    connect(socket,&QTcpSocket::readyRead,this,[=](){
        int progress = socket->readAll().toInt();
        qDebug()<<progress;
        emit update(progress);
        if(progress == 100){
            emit sent();
        }
    });
}

void sendFile::send(QString path){

    QFile file(path);
    file.open(QIODevice::ReadOnly);

    QFileInfo info = QFileInfo(path);
    QString command = "Action:Upload File:"+info.fileName()+"<"+QString::number(info.size(),10);
    socket->write(command.toUtf8().data());
    socket->flush();
    socket->waitForReadyRead();
    while(!file.atEnd()){
        QByteArray qb = file.read(65536);
        socket->write(qb);
        socket->waitForReadyRead();
    }
    socket->flush();
}
