#ifndef LINCLIENT_H
#define LINCLIENT_H
#include<QObject>
#include<util.cpp>
#include<QList>
#include<QMap>
#include<QString>
#include<QCoreApplication>
#include<etcpsocket.h>
#include<QHostAddress>

class Linclient:public QObject
{
    Q_OBJECT
signals:
    void dataReceived(Data);
public:
    ETcpSocket* socket;
    QString ip;
    qint16 port = 25565;
    //QList<Data> content;
    QString filename;
    bool fetching = true;

    Linclient(QString,qint16);
    void ChangeNickname(QString);
    void SendMessage(QString);
};

#endif // LINCLIENT_H
