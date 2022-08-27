#ifndef LINCLIENT_H
#define LINCLIENT_H
#include<QObject>
#include<util.cpp>
#include<QList>
#include<QMap>
#include<QString>
#include<QCoreApplication>
#include<QTcpSocket>
#include<QHostAddress>

class Linclient:public QObject
{
    Q_OBJECT
signals:
    void dataReceived(Data);
public:
    QTcpSocket* socket;
    QString ip;
    qint16 port = 25565;
    QList<Data> content;
    QString filename;

    Linclient(QString,qint16);
    void ChangeNickname(QString);
    void SendMessage(QString);
};

#endif // LINCLIENT_H