#ifndef SENDFILE_H
#define SENDFILE_H
#include<QObject>
#include<etcpsocket.h>
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
    ETcpSocket* socket;

signals:
    void connected();
    void sent();
    void update(int percentage);
};

#endif // SENDFILE_H
