#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "clinetsocket.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //创建实际的QTcpServer对象，设置父类为主窗口this
    m_ser = new QTcpServer(this);
    //设置端口号文本为8888
    ui->port->setText("8888");
    //设置listview不可编译
    ui->friendList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //状态栏
    m_status = new QRadioButton;
    m_status->setEnabled(0);
    m_status->setText("连接状态：");
    m_status->setLayoutDirection(Qt::RightToLeft);
    //ui->statusbar->addWidget(new QLabel("连接状态："));
    ui->statusbar->addWidget(m_status);

    //连接客户端
    connect(m_ser,&QTcpServer::newConnection,this,[=](){
        //用QTcpSocket接收客户端发送的socket
        QTcpSocket* m_soc = m_ser->nextPendingConnection();

        //ClinetSocket* cs = new ClinetSocket(m_soc);

        //得到客户端的IP地址和端口号
        ui->record->append(m_soc->peerAddress().toString());
        QString m_soc_port=tr("%1").arg(m_soc->peerPort());
        ui->record->append(m_soc_port);

        //新建一个结构体
        clientInFo newClient;
        newClient.port = m_soc_port;
        newClient.m_soc = m_soc;
        //将结构体存放到列表
        this->tcpSocket.append(newClient);
        //int m = tcpSocket.count();

        //获取客户端发送的内容
        connect(m_soc,&QTcpSocket::readyRead,this,[=](){

            //读取所有客户端发送来的数据，解析发送过来的json字符串
            QByteArray data = m_soc->readAll();
            QString  strJson(data);
            QJsonDocument jsonDocument = QJsonDocument::fromJson(strJson.toStdString().data());
            QJsonObject json = jsonDocument.object();

            //QString  login = json["msgType"].toString();
            //判断消息类型，如果消息类型为登录
            if (json["msgType"].toString() == "login")
            {
                //创建一个字符串，记录已登录的用户列表
                userList = userList + "[";
                //遍历tcpSocket列表,找到对应的套接字
                for (int var = 0; var < tcpSocket.count(); var++) {
                    //将当前登录的用户名添加到列表中
                    if(tcpSocket[var].m_soc == m_soc)
                    {
                        tcpSocket[var].username = json["userName"].toString();
                    }
                    //将已登录的用户列表返还给客户端
                    if(var+1 == tcpSocket.count())
                    {
                        userList = userList+"\""+tcpSocket[var].username+"\""+"]";
                    }else{
                        userList = userList+"\""+tcpSocket[var].username+"\""+",";
                    }
                    //list用于显示在主窗口的列表中
                    list.append(tcpSocket[var].username);
                }

                QString sendJson = "{\"msgType\":\"userList\",\"userList\":"+userList+"}";
                //将当前登录的用户名列表返还给每个客户端;
                for (int var = 0; var < tcpSocket.count(); var++) {
                    tcpSocket[var].m_soc->write(sendJson.toUtf8());
                }
                //传输一次之后就清空列表
                userList = "";

                //使用数据列表创建数据显示模型
                QStringListModel *listmodel = new QStringListModel(list);
                ui->friendList->setModel(listmodel);
                list.clear();
            }

            //判断消息类型，如果消息类型为说话
            if (json["msgType"].toString() == "talk")
            {
                //解析json,将信息打在屏幕上
                ui->record->append(json["userName"].toString()+"："+json["message"].toString());
                //将该json转发给其他客户端
                for (int var = 0; var < tcpSocket.count(); var++) {
                    if(json["userName"].toString() != tcpSocket[var].username){
                       tcpSocket[var].m_soc->write(data);
                    }
                }
            }

            //判断消息类型，如果消息类型为对好友交谈
            if (json["msgType"].toString() == "talkToFd")
            {
                //遍历tcpSocket列表,找到需要接收信息的用户
                for (int var = 0; var < tcpSocket.count(); var++) {
                    //将当前登录的用户名添加到列表中
                    if(tcpSocket[var].username == json["fdName"].toString())
                    {
                       //将消息转发给指定用户
                       tcpSocket[var].m_soc->write(data);
                    }
                }
            }

            //判断json格式是用户登录，还是用户之间的通讯，还是用户对客户端的通讯
            //ui->record->append(m_soc_port+"客户端："+data);
        });



        //当客户端和服务器断开连接
        connect(m_soc,&QTcpSocket::disconnected,this,[=](){
            //创建一个字符串，记录已登录的用户列表
            userList = userList + "[";
            //迭代器指向list第一个元素
            auto del = tcpSocket.begin();
            //遍历tcpSocket列表,找到对应的套接字，将其剔除掉
            for (int var = 0; var < tcpSocket.count(); var++) {

                //将当前断开的元素从列表中删除,其余的再次添加到list中，并发送给剩余的客户端
                if(tcpSocket[var].m_soc == m_soc)
                {
                    //迭代器右移，指向对应的元素
                    del= del+var;
                }
                else{
                    //list用于显示在主窗口的列表中
                    list.append(tcpSocket[var].username);
                    userList = userList+"\""+tcpSocket[var].username+"\""+",";
                }
            }
            //删除指定迭代器的元素
            tcpSocket.erase(del);
            userList = userList.left(userList.size()-1) + "]";

            //重新向其他客户端发送登录列表
            QString sendJson = "{\"msgType\":\"userList\",\"userList\":"+userList+"}";
            //将当前登录的用户名列表返还给每个客户端;
            for (int var = 0; var < tcpSocket.count(); var++) {
                tcpSocket[var].m_soc->write(sendJson.toUtf8());
            }
            //传输一次之后就清空列表
            userList = "";

            //使用数据列表创建数据显示模型
            QStringListModel *listmodel = new QStringListModel(list);
            ui->friendList->setModel(listmodel);
            list.clear();

            //关闭socket，释放内存，修改状态栏
            m_soc->close();
            m_soc->deleteLater();


            //m_status->setChecked(0);
        });
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_setListen_clicked()
{
    //按钮启动监听
    //获取端口号,类型转换为非负整数
    unsigned short port = ui->port->text().toUShort();
    //启动监听，设置IP和端口号
    m_ser->listen(QHostAddress::Any,port);
    //将按钮设置为不可用
    ui->setListen->setDisabled(1);
    //修改状态栏
    m_status->setChecked(1);
}


void MainWindow::on_sendMsg_clicked()
{
    QString msg = ui->Msg->toPlainText();
    //m_soc->write(msg.toUtf8());
    //服务器不用主动发消息给客户端了，可以做出来
    ui->record->append("服务器："+msg);
}

//右侧列表的双击事件，先放着，后续再开发
void MainWindow::on_friendList_doubleClicked(const QModelIndex &index)
{
    //ui->record->append(index.data().toString());
}

