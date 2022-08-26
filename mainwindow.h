#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
