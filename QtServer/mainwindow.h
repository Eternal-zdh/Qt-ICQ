#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QRadioButton>
#include <QMap>
#include <QJsonDocument>
#include <QJsonObject>

#include <QStringListModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct clientInFo
{
    //成员列表
    QString username;  //用户名
    QString port;      //端口号
    QTcpSocket* m_soc;    //套接字对象
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_setListen_clicked();

    void on_sendMsg_clicked();

    void on_friendList_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QTcpServer* m_ser;    //创建指针，和上面的ui一样
    QRadioButton* m_status; //状态栏
    QList <clientInFo> tcpSocket;   //用一个列表存储所有连接的套接字对象
    QString userList = "";  //发送给其他客户端的用户列表

    //将数组里的数据放到QListView控件里
    QStringList list;       //创建数据显示列表
};
#endif // MAINWINDOW_H
