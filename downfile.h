#ifndef DOWNFILE_H
#define DOWNFILE_H

#include <QObject>
#include<QTcpSocket>
#include<QFile>
#include<QFileInfo>
#include<QHostAddress>

class downFile : public QObject
{
    Q_OBJECT
public:
    explicit downFile(QObject *parent = nullptr);
    void connectHost(QString ip,qint16 port);
    void fetch(QString path,QString name);
private:
    QTcpSocket* socket;
signals:
    void connected();
public slots:
};

#endif // DOWNFILE_H
