#ifndef LINSERVER_H
#define LINSERVER_H
#include<QObject>
#include<util.cpp>
#include<QTcpServer>
#include<etcpsocket.h>
#include<QList>
#include<QMap>
#include<QCoreApplication>
#include<QFileInfo>

class Linserver:public QObject
{
    Q_OBJECT
public:
    QTcpServer* server;
    QList<ETcpSocket*> clientlist;

    qint16 port = 25565;

    QList<Data> content;
    QMap<QString,QString> nmap;
    QString filename;
    qint64 filesize;
    qint64 restsize;

    int servermode = 0;

    Linserver(qint16);
    void saveContent();
signals:
    void receiveFile(qint64 fsize);
};

#endif // LINSERVER_H
