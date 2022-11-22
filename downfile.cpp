#include "downfile.h"

downFile::downFile(QObject *parent) : QObject(parent)
{

}

void downFile::connectHost(QString ip,qint16 port){
    socket = new ETcpSocket(this);
    socket->connectToHost(QHostAddress(ip),port);
    connect(socket,&ETcpSocket::connected,this,&downFile::connected);
}

void downFile::fetch(QString path,QString filename){
    QString command = "Action:Download File:"+filename;
    socket->write(command.toUtf8().data());
    socket->flush();
    connect(socket,&ETcpSocket::readyRead,this,[=]{
        if(readyReceive){
            QFile file(path);
            file.open(QIODevice::Append);
            if(restsize > 0){
                QByteArray arr = socket->readAll();
                restsize -= arr.size();
                socket->flush();
                file.write(arr);
                emit update(100-(int)((double)restsize/filesize*100.0));
            }
            if(restsize <= 0){
                readyReceive = false;
                socket->disconnect();
                emit downed();
            }
        }else{
            QString strsize = socket->readAll();
            restsize = filesize = strsize.toLongLong();
            QString command = "Action:Response:";
            socket->write(command.toUtf8().data());
            socket->flush();
            readyReceive = true;
        }
    });
}
