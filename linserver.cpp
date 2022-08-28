#include "linserver.h"
QString chpath;

Linserver::Linserver(qint16 p = 25565)
{
    port = p;
    chpath = QCoreApplication::applicationDirPath()+"/Chat.history";
    QFile chsave(chpath);
    chsave.open(QIODevice::ReadWrite);
    if(chsave.exists()){
        while(!chsave.atEnd()){
            QString str = chsave.readLine().trimmed();
            content.push_back(FromString(str));
        }
    }
    chsave.close();

    server = new QTcpServer(this);
    server->listen(QHostAddress::Any,port);

    connect(server,&QTcpServer::newConnection,[=](){
        QTcpSocket* tcpsocket = server->nextPendingConnection();
        clientlist.push_back(tcpsocket);

        QString ip = tcpsocket->peerAddress().toString();
        qint16 port = tcpsocket->peerPort();

        nmap[QString("%1:%2").arg(ip).arg(port)] = QString("%1:%2").arg(ip).arg(port);

        for(Data d:content){
            tcpsocket->write(d.convert().toUtf8().data());
            tcpsocket->waitForReadyRead();
            tcpsocket->flush();
        }

        connect(tcpsocket,&QTcpSocket::readyRead,[=](){
            if(servermode == 0){
                QString readData = tcpsocket->readAll();
                if(readData.contains("Action:Response:")){
                    return;
                }
                if(readData.contains("Action:CN:")){
                    nmap[QString("%1:%2").arg(ip).arg(port)] = readData.replace("Action:CN:","");
                }else if(readData.contains("Action:Upload File:")){
                    filename = readData.replace("Action:Upload File:","").split("<")[0];
                    restsize = filesize = readData.replace("Action:Upload File:","").split("<")[1].toInt();
                    QString path = QCoreApplication::applicationDirPath()+"/"+filename;
                    QFile file(path);
                    if(file.exists()){
                        file.remove();
                    }
                    QString ip = tcpsocket->peerAddress().toString();
                    qint16 port = tcpsocket->peerPort();
                    Data data = {"<a href=\""+filename+"\">"+filename+"</a>",nmap[QString("%1:%2").arg(ip).arg(port)],QDateTime::fromTime_t(QDateTime::currentSecsSinceEpoch())};
                    content.push_back(data);
                    saveContent();

                    for(QTcpSocket* client:clientlist){
                        client->write(data.convert().toUtf8().data());
                    }
                    servermode = 1;
                }else if(readData.contains("Action:Download File:")){
                    QString fn = QCoreApplication::applicationDirPath()+"/"+readData.replace("Action:Download File:","");
                    QFile file(fn);
                    QFileInfo info(file);
                    file.open(QIODevice::ReadOnly);
                    QByteArray qb = file.readAll();
                    tcpsocket->write(QString::number(info.size(),10).toUtf8().data());
                    tcpsocket->flush();
                    tcpsocket->waitForReadyRead();
                    tcpsocket->write(qb);
                    tcpsocket->flush();
                }else{
                    QString ip = tcpsocket->peerAddress().toString();
                    qint16 port = tcpsocket->peerPort();
                    Data data = {readData,nmap[QString("%1:%2").arg(ip).arg(port)],QDateTime::fromTime_t(QDateTime::currentSecsSinceEpoch())};
                    content.push_back(data);
                    saveContent();

                    for(QTcpSocket* client:clientlist){
                        client->write(data.convert().toUtf8().data());
                    }
                }
            }else if(servermode == 1){
                QString path = QCoreApplication::applicationDirPath()+"/"+filename;
                QFile file(path);
                file.open(QIODevice::Append);
                if(restsize > 0){
                    QByteArray arr = tcpsocket->readAll();
                    restsize -= arr.size();
                    file.write(arr);
                    tcpsocket->write(QString::number(100-(int)((double)restsize/filesize*100.0),10).toUtf8().data());
                }
                if(restsize <= 0){
                    servermode = 0;
                }
            }
        });
    });
}

void Linserver::saveContent(){

    QFile chsave(chpath);
    chsave.open(QIODevice::ReadWrite);
    QString chdata = "";
    for(Data data:content){
        chdata += data.convert("\n");
    }
    chsave.write(chdata.toStdString().c_str());
    chsave.close();
}
