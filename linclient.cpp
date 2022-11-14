#include "linclient.h"


Linclient::Linclient(QString serverip,qint16 p)
{
    port = p;
    ip = serverip;
    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress(ip),port);
    connect(socket,&QTcpSocket::connected,this,[=](){
        socket->write(QString("Action:Fetch Chat History:").toUtf8().data());
    });
    connect(socket, &QTcpSocket::readyRead, this, [=]()
    {
        QString msg = socket->readAll();
        if(msg.contains("Action:Finished:")){
            fetching = false;
        }else{
            Data data = FromString(msg);
            if(fetching)
                socket->write(QString("Action:Response:").toUtf8().data());
            emit dataReceived(data);
        }
    });
}

void Linclient::ChangeNickname(QString name){
    QString nn = "Action:CN:"+name;
    socket->write(nn.toUtf8().data());
}

void Linclient::SendMessage(QString msg){
    socket->write(msg.toUtf8().data());
}
