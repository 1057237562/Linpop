#ifndef SENDFILE_H
#define SENDFILE_H
#include<QObject>
#include<QTcpSocket>
#include<QFile>
#include<QFileInfo>
#include<QHostAddress>
class sendFile:public QObject
{
    Q_OBJECT
public:

    explicit sendFile(QObject *parent = nullptr);
    void connectHost(QString ip,qint16 port);
    void send(QString path);
private:
    QTcpSocket* socket;

signals:
    void connected();
};

#endif // SENDFILE_H
