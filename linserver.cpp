#include "linserver.h"

Linserver::Linserver(qint16 p = 25565)
{
    port = p;
    server = new QTcpServer(this);
    server->listen(QHostAddress::Any,port);

    connect(server,&QTcpServer::newConnection,[=](){
        QTcpSocket* tcpsocket = server->nextPendingConnection();
        clientlist.push_back(tcpsocket);

        QString ip = tcpsocket->peerAddress().toString();
        qint16 port = tcpsocket->peerPort();

        nmap[QString("%1:%2").arg(ip).arg(port)] = QString("%1:%2").arg(ip).arg(port);

        connect(tcpsocket,&QTcpSocket::readyRead,[=](){
            if(servermode == 0){
                QString readData = tcpsocket->readAll();

                if(readData.contains("Action:CN:")){
                    nmap[QString("%1:%2").arg(ip).arg(port)] = readData.replace("Action:CN:","");
                }else if(readData.contains("Action:Upload File:")){
                    filename = readData.replace("Action:Upload File:","");
                    QString ip = tcpsocket->peerAddress().toString();
                    qint16 port = tcpsocket->peerPort();
                    Data data = {"<a href="+filename+">"+filename+"</a>",nmap[QString("%1:%2").arg(ip).arg(port)],QDateTime::fromTime_t(QDateTime::currentSecsSinceEpoch())};
                    content.push_back(data);

                    for(QTcpSocket* client:clientlist){
                        client->write(data.convert().toUtf8().data());
                    }
                    servermode = 1;
                }else if(readData.contains("Action:Download File:")){
                    QString fn = QCoreApplication::applicationDirPath()+"/"+readData.replace("Action:Download File:","");
                    QFile file(fn);
                    file.open(QIODevice::ReadOnly);
                    QByteArray qb = file.readAll();
                    tcpsocket->write(qb);
                }else{
                    QString ip = tcpsocket->peerAddress().toString();
                    qint16 port = tcpsocket->peerPort();
                    Data data = {readData,nmap[QString("%1:%2").arg(ip).arg(port)],QDateTime::fromTime_t(QDateTime::currentSecsSinceEpoch())};
                    content.push_back(data);

                    for(QTcpSocket* client:clientlist){
                        client->write(data.convert().toUtf8().data());
                    }
                }
            }else if(servermode == 1){
                QByteArray arr = tcpsocket->readAll();
                QString path = QCoreApplication::applicationDirPath()+"/"+filename;
                QFile file(path);
                file.open(QIODevice::ReadWrite);
                file.write(arr);
                servermode = 0;
            }
        });
    });
}
