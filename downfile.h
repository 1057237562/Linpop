#ifndef DOWNFILE_H
#define DOWNFILE_H

#include <QObject>
#include<etcpsocket.h>
#include<QFile>
#include<QFileInfo>
#include<QHostAddress>

class downFile : public QObject
{
    Q_OBJECT
public:
    bool readyReceive = false;
    qint64 restsize = 0;
    qint64 filesize = 0;

    explicit downFile(QObject *parent = nullptr);
    void connectHost(QString ip,qint16 port);
    void fetch(QString path,QString name);
private:
    ETcpSocket* socket;
signals:
    void connected();
    void downed();
    void update(int percentage);
public slots:
};

#endif // DOWNFILE_H
