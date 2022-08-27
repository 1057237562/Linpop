#ifndef LINSERVER_H
#define LINSERVER_H
#include<QObject>
#include<util.cpp>
#include<QTcpServer>
#include<QTcpSocket>
#include<QList>
#include<QMap>
#include<QCoreApplication>

class Linserver:public QObject
{
    Q_OBJECT
public:
    QTcpServer* server;
    QList<QTcpSocket*> clientlist;

    qint16 port = 25565;

    QList<Data> content;
    QMap<QString,QString> nmap;
    QString filename;

    int servermode = 0;

    Linserver(qint16);
};

#endif // LINSERVER_H
