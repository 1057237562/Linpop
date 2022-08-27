#include "linclient.h"


Linclient::Linclient(QString serverip,qint16 p)
{
    port = p;
    ip = serverip;
    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress(ip),port);
    connect(socket, &QTcpSocket::readyRead, this, [=]()
    {
        QString msg = socket->readAll();
        Data data = FromString(msg);
        content.push_back(data);
        emit dataReceived(data);
    });
}

void Linclient::ChangeNickname(QString name){
    QString nn = "Action:CN:"+name;
    socket->write(nn.toUtf8().data());
}

void Linclient::SendMessage(QString msg){
    socket->write(msg.toUtf8().data());
}
