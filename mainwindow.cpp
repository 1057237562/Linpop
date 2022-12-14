#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpServer>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileInfo>
#include <QFileDialog>
#include<linserver.h>
#include<linclient.h>
#include<sendfile.h>
#include<downfile.h>
#include<etcpsocket.h>
#include<QThread>
#include<QKeyEvent>

Linserver* lserver;
Linclient* lclient;

QString sip;
qint16 sport;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    switch(event->key()){
        case Qt::Key_Enter:
        case Qt::Key_Return:
    {
        on_sendbtn_clicked();
    }
        break;
    }
}

void MainWindow::ProceedData(Data data){
    ui->textBrowser->moveCursor(QTextCursor::End);
    ui->textBrowser->setTextColor(Qt::blue);
    ui->textBrowser->append(data.fline());
    ui->textBrowser->setTextColor(Qt::black);
    ui->textBrowser->append(data.sline());
    QApplication::alert(this);
}

void MainWindow::on_sendbtn_clicked()
{
    if(lclient == NULL){
        QMessageBox::warning(this,"错误:","你没有连接到任何服务器。");
        return;
    }
    lclient->SendMessage(ui->sendText->document()->toPlainText());
    ui->sendText->document()->clear();
}

void MainWindow::on_actionClient_triggered()
{
    QFile chsave(QCoreApplication::applicationDirPath()+"/config.cfg");
    if(chsave.exists()){
        chsave.open(QIODevice::ReadOnly);
        QString str = chsave.readLine().trimmed();
        sip = QInputDialog::getText(this, "输入字符串", "请输入IP：", QLineEdit::Normal,str.split(":").at(0));
        sport = QInputDialog::getInt(this, "输入字符串", "请输入端口号：",str.split(":").at(1).toInt());
        chsave.close();
    }else{
        sip = QInputDialog::getText(this, "输入字符串", "请输入IP：", QLineEdit::Normal,"127.0.0.1");
        sport = QInputDialog::getInt(this, "输入字符串", "请输入端口号：",25565);
    }
    lclient = new Linclient(sip,sport);
    ui->connectInfo->setText("Current Connect IP:\n"+sip);
    chsave.open(QIODevice::ReadWrite | QIODevice::Truncate);
    chsave.write((sip+":"+QString::number(sport)).toStdString().c_str());
    chsave.close();
    connect(lclient,SIGNAL(dataReceived(Data)),this,SLOT(ProceedData(Data)));
}

void MainWindow::on_actionHost_triggered()
{
    sport = QInputDialog::getInt(this, "输入字符串", "请输入端口号：",25565);
    QThread* t = new QThread;
    lserver = new Linserver(sport);
    lserver->moveToThread(t);
    t->start();
    sip = "127.0.0.1";
    ui->connectInfo->setText("Current Connect IP:\n"+sip);
    lclient = new Linclient(sip,sport);
    connect(lclient,SIGNAL(dataReceived(Data)),this,SLOT(ProceedData(Data)));
}

void MainWindow::on_actionChange_NickName_triggered()
{
    if(lclient == NULL){
        QMessageBox::warning(this,"错误:","你没有连接到任何服务器。");
        return;
    }
    lclient->ChangeNickname(QInputDialog::getText(this, "输入字符串", "请输入昵称：", QLineEdit::Normal,"Guest"));
}

void MainWindow::on_sendFile_clicked()
{
    if(lclient == NULL){
        QMessageBox::warning(this,"错误:","你没有连接到任何服务器。");
        return;
    }
    QString path= QFileDialog::getOpenFileName(this,"打开文件","");
    QThread* t = new QThread;
    sendFile* worker = new sendFile;
    worker->moveToThread(t);
    connect(this,&MainWindow::send,worker,&sendFile::send);
    connect(this,&MainWindow::startConnect,worker,&sendFile::connectHost);
    connect(worker,&sendFile::connected,this,[=](){
        emit send(path);
    });
    ui->progressBar->setVisible(true);
    connect(worker,&sendFile::update,ui->progressBar,&QProgressBar::setValue);
    t->start();
    emit startConnect(sip,sport);
    connect(worker,&sendFile::sent,this,[=](){
        t->quit();
        t->wait();
        worker->deleteLater();
        t->deleteLater();
        ui->progressBar->setVisible(false);
        ui->progressBar->setValue(0);
    });
}

void MainWindow::on_textBrowser_anchorClicked(const QUrl &arg1)
{
    if(lclient == NULL){
        QMessageBox::warning(this,"错误:","你没有连接到任何服务器。");
        return;
    }
    QString fname = QFileDialog::getSaveFileName(this,"保存文件",arg1.toString());
    QThread* t = new QThread;
    downFile* worker = new downFile;
    worker->moveToThread(t);
    connect(this,&MainWindow::fetch,worker,&downFile::fetch);
    connect(this,&MainWindow::startConnect,worker,&downFile::connectHost);
    connect(worker,&downFile::update,ui->progressBar,&QProgressBar::setValue);
    ui->progressBar->setVisible(true);
    connect(worker,&downFile::connected,this,[=](){
        emit fetch(fname,arg1.toString());
    });
    t->start();
    emit startConnect(sip,sport);
    connect(worker,&downFile::downed,this,[=](){
        t->quit();
        t->wait();
        t->deleteLater();
        worker->deleteLater();
        ui->progressBar->setVisible(false);
        ui->progressBar->setValue(0);
    });
}

void MainWindow::on_actionDelete_Chat_history_triggered()
{
    QFile file(QCoreApplication::applicationDirPath()+"/Chat.history");
    if(file.exists()){
        file.remove();
    }
}

void MainWindow::on_actionAdd_Plugins_triggered()
{

}
