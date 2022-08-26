#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <util.cpp>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileInfo>
#include <QFileDialog>

QTcpServer* server;
QList<QTcpSocket*> clientlist;
QTcpSocket* socket;

qint16 port = 25565;

QList<Data> content;
QMap<QString,QString> nmap;

int servermode = 0;
int clientmode = 0;
QString filename;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_sendbtn_clicked()
{
    if(NULL == socket){
        QMessageBox::warning(this,"错误:","你没有连接到任何服务器。");
        return;
    }
    socket->write(ui->sendText->document()->toPlainText().toUtf8().data());
    ui->sendText->document()->clear();
}

void MainWindow::on_actionClient_triggered()
{
    QString ip = QInputDialog::getText(this, "输入字符串", "请输入IP：", QLineEdit::Normal,"127.0.0.1");
    qint16 port = QInputDialog::getInt(this, "输入字符串", "请输入端口号：",25565);
    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress(ip),port);
    connect(socket, &QTcpSocket::readyRead, this, [=]()
    {
        if(clientmode == 0){
            QString msg = socket->readAll();
            Data data = FromString(msg);
            content.push_back(data);
            ui->textBrowser->setTextColor(Qt::blue);
            ui->textBrowser->append(data.fline());
            ui->textBrowser->setTextColor(Qt::black);
            ui->textBrowser->append(data.sline());
        }
        if(clientmode == 1){
            QByteArray arr = socket->readAll();
            QFile file(filename);
            file.open(QIODevice::ReadWrite);
            file.write(arr);
            clientmode = 0;
        }
    });
}

void MainWindow::on_actionHost_triggered()
{
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

    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress("127.0.0.1"),port);
    connect(socket, &QTcpSocket::readyRead, this, [=]()
    {
        if(clientmode == 0){
            QString msg = socket->readAll();
            Data data = FromString(msg);
            content.push_back(data);
            ui->textBrowser->setTextColor(Qt::blue);
            ui->textBrowser->append(data.fline());
            ui->textBrowser->setTextColor(Qt::black);
            ui->textBrowser->append(data.sline());
        }
        if(clientmode == 1){
            QByteArray arr = socket->readAll();
            QFile file(filename);
            file.open(QIODevice::ReadWrite);
            file.write(arr);
            clientmode = 0;
        }
    });
}

void MainWindow::on_actionChange_NickName_triggered()
{
    if(NULL == socket){
        QMessageBox::warning(this,"错误:","你没有连接到任何服务器。");
        return;
    }
    QString nn = "Action:CN:"+QInputDialog::getText(this, "输入字符串", "请输入昵称：", QLineEdit::Normal,"Guest");
    socket->write(nn.toUtf8().data());
}

void MainWindow::on_sendFile_clicked()
{
    if(NULL == socket){
        QMessageBox::warning(this,"错误:","你没有连接到任何服务器。");
        return;
    }
    QString path= QFileDialog::getOpenFileName(this,"打开文件","");
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QByteArray qb = file.readAll();
    QString command = "Action:Upload File:"+QFileInfo(path).fileName();
    socket->write(command.toUtf8().data());
    socket->flush();
    socket->write(qb);
}

void MainWindow::on_textBrowser_anchorClicked(const QUrl &arg1)
{
    clientmode = 1;
    filename = QFileDialog::getSaveFileName(this,"保存文件",arg1.toString());
    QString command = "Action:Download File:"+arg1.toString();
    socket->write(command.toUtf8().data());
}
