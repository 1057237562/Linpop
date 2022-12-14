#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<util.cpp>
#include <QKeyEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void startConnect(QString,qint16);
    void send(QString path);
    void fetch(QString path,QString name);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_sendbtn_clicked();

    void on_actionClient_triggered();

    void on_actionHost_triggered();

    void on_actionChange_NickName_triggered();

    void on_sendFile_clicked();

    void on_textBrowser_anchorClicked(const QUrl &arg1);

    void ProceedData(Data data);

    void on_actionDelete_Chat_history_triggered();

    void on_actionAdd_Plugins_triggered();

private:
    Ui::MainWindow *ui;
protected:
    virtual void keyPressEvent(QKeyEvent *ev);
};

#endif // MAINWINDOW_H
