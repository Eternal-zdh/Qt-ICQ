#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QRadioButton>
#include <QFileDialog>
#include <QThread>
#include "sendfile.h"
#include "conversation_widget.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QStringListModel>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void sendFile(QString path);    //发送文件的信号

    void startConnect(QString ip,unsigned short port);   //启动信号连接服务器

    void sendMsg(QString msg);     //向服务器发送信息

    void endConnect();      //断开连接信号

    void mainwindow_send_a_signal(QString main_to_dia_msg);//定义向子窗口发送的信号

private slots:
    void on_sendMsg_clicked();

    void on_connect_clicked();

    void on_disconnect_clicked();

    void on_selFile_clicked();

    void on_friendList_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    //创建指针，和上面的ui一样
    QTcpSocket* m_soc;
    QRadioButton* m_status; //状态栏
    QString message = "";    //存储从其他客户端发送过来的消息
};
#endif // MAINWINDOW_H
