#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //创建实际的QTcpSocket对象，不设置父类对象，内存手动释放
    m_soc = new QTcpSocket;       //窗口关闭时自动释放内存

    ui->port->setText("8888");          //设置服务器端口号文本为8888
    ui->IP->setText("127.0.0.1");       //设置服务器IP
    ui->disconnect->setEnabled(0);      //断开连接按钮不可用

    ui->progressBar->setRange(0,100);   //设置进度条的范围
    ui->progressBar->setValue(0);       //设置进度条的初始值
    ui->progressBar->setHidden(1);      //设置进度条隐藏

    //设置listview不可编译
    ui->friendList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //设置状态栏
    m_status = new QRadioButton;    //用选择按钮来展示状态栏
    m_status->setEnabled(0);        //设置不可用
    m_status->setText("连接状态：");
    m_status->setLayoutDirection(Qt::RightToLeft);
    //ui->statusbar->addWidget(new QLabel("连接状态："));
    ui->statusbar->addWidget(m_status); //将控件添加到状态栏中


    /*
    //开启子线程
    QThread* t = new QThread;
    SendFile* worker = new SendFile;
    worker->moveToThread(t);
    t->start();

    //发送文件---信号和槽
    connect(this,&MainWindow::sendFile,worker,&SendFile::sendFiles);    //主窗口发出信号，类中执行槽函数，发送文件
    connect(worker,&SendFile::curPercent,ui->progressBar,&QProgressBar::setValue);  //更改进度条进度

    //这里涉及到多线程的相关内容，暂时用不上，后续会更新

    //断开连接---信号和槽
    connect(this,&MainWindow::endConnect,worker,&SendFile::endSever);
    //接收断开连接的信号，改状态栏，弹出提示
    connect(worker,&SendFile::connectEnd,this,[=](){
        m_status->setChecked(0);    //状态栏 连接成功
        ui->record->append("服务器连接断开！");
        ui->connect->setEnabled(1);     //连接按钮不可用
        ui->disconnect->setEnabled(0);     //断开连接按钮可用
    });
    */

    connect(m_soc,&QTcpSocket::connected,this,[=](){
        //客户端连接成功后，告诉服务器你是谁
        QString json = "{\"msgType\":\"login\",\"userName\":\""+ui->userName->text()+"\"}";
        m_soc->write(json.toUtf8());

        m_status->setChecked(1);    //状态栏 连接成功
        ui->record->append("服务器连接成功！");
        ui->connect->setEnabled(0);     //连接按钮不可用
        ui->disconnect->setEnabled(1);     //断开连接按钮可用
    });




    //获取服务器发送的内容
    connect(m_soc,&QTcpSocket::readyRead,this,[=](){
        //读取所有客户端发送来的数据,解析发送过来的json字符串
        QByteArray data = m_soc->readAll();
        QString  strJson(data);
        QJsonDocument jsonDocument = QJsonDocument::fromJson(strJson.toStdString().data());
        QJsonObject json = jsonDocument.object();
        //QString  login = json["msgType"].toString();

        //判断消息类型，如果消息类型为用户列表
        if (json["msgType"].toString() == "userList")
        {
            //将返回的用户列表转换为json数组
            QJsonArray userList =  json.value("userList").toArray();

            //将数组里的数据放到QListView控件里
            QStringList list;       //创建数据显示列表

            //循环放到list列表
            for (int var = 0; var < userList.count(); var++) {
                list.append(userList[var].toString());
            }

            //使用数据列表创建数据显示模型
            QStringListModel *listmodel = new QStringListModel(list);
            ui->friendList->setModel(listmodel);
        }

        //判断消息类型，如果消息类型为说话
        if (json["msgType"].toString() == "talk")
        {
            //解析json,将信息打在屏幕上
            ui->record->append(json["userName"].toString()+"："+json["message"].toString());
        }

        //判断消息类型，如果消息类型为用户交流
        if (json["msgType"].toString() == "talkToFd")
        {
            //将得到的消息放入全局变量中，再传到另外一个窗体内
            message = json["message"].toString();
            //将接收到的消息传递到对应的子窗口中
            //emit mainwindow_send_a_signal(message);
            //message = "";
            //解析json,将信息打在屏幕上
            //ui->record->append(json["userName"].toString()+" 对你说："+json["message"].toString());
        }
        //ui->record->append("服务器："+data);
    });



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connect_clicked()   //连接服务器
{
    //判断是否输入了登录名
    if(ui->userName->text() == ""){
        ui->userName->setPlaceholderText("请输入用户名");
        return;
    }

    //获取端口号,类型转换为非负整数
    unsigned short port = ui->port->text().toUShort();
    QString ip = ui->IP->text();

    //调用socket的函数连接服务器
    m_soc->connectToHost(QHostAddress(ip),port);

    ui->userName->setEnabled(0);      //用户名输入框不可用

    //emit startConnect(ip,port);     //启动信号，连接服务器
    //emit sendMsg(ui->userName->text());     //启动信号，将用户名发送到服务器，告诉服务器是谁连接的
}

void MainWindow::on_disconnect_clicked()    //断开连接
{
    //emit endConnect();      //启动信号，断开连接
    m_soc->disconnectFromHost();
    ui->connect->setEnabled(1);        //连接按钮可用
    ui->disconnect->setEnabled(0);     //断开连接按钮不可用
    ui->userName->setEnabled(1);       //用户名输入框可用
    ui->friendList->model()->removeRows(0,ui->friendList->model()->rowCount());     //清空右侧列表
}

void MainWindow::on_selFile_clicked()   //选择文件路径
{
    QString path = QFileDialog::getOpenFileName();
    if(path.isEmpty()){
        return;
    }
    ui->filePath->setText(path);
}

void MainWindow::on_sendMsg_clicked()   //发送信息和文件
{
    //如果文件路径不为空，发送文件（没做完）
    if(ui->filePath->text().isEmpty() == 0){
        ui->progressBar->setHidden(0);      //显示进度条
        emit sendFile(ui->filePath->text());    //触发信号
    }

    QString msg = ui->Msg->toPlainText();
    QString user = ui->userName->text();  //获取用户名称
    //向服务器发送一段json
    QString userMsg = "{\"msgType\":\"talk\",\"userName\":\""+user+"\",\"message\":\""+msg+"\"}";

    QString myfriend = "派大星";   //获取好友名称

    //emit sendMsg(port_msg);

    m_soc->write(userMsg.toUtf8());
    ui->record->append(user+"："+msg);
    ui->Msg->clear();

}


void MainWindow::on_friendList_doubleClicked(const QModelIndex &index)  //打开私聊窗口
{
    //判断选择的是否为自己，不能自己和自己对话
    if(ui->userName->text() == index.data().toString())
    {
        return;
    }
    ui->friendList->model()->setData(index,QBrush(Qt::red),Qt::ForegroundRole);
    if(message == "")
    {
        conversation_widget *configWindow = new conversation_widget(ui->userName->text(),index.data().toString(),m_soc);
        //限制窗体只能打开一次
        configWindow->setWindowModality(Qt::ApplicationModal);
        configWindow->setAttribute(Qt::WA_ShowModal,true);

        configWindow->show();
        connect(this,SIGNAL(mainwindow_send_a_signal(QString)),configWindow,SLOT(dialog_receive_a_signal(QString)));
    }
    else{
        conversation_widget *configWindow = new conversation_widget(ui->userName->text(),index.data().toString(),m_soc,message);
        //限制窗体只能打开一次
        configWindow->setWindowModality(Qt::ApplicationModal);
        configWindow->setAttribute(Qt::WA_ShowModal,true);

        configWindow->show();
        connect(this,SIGNAL(mainwindow_send_a_signal(QString)),configWindow,SLOT(dialog_receive_a_signal(QString)));
        emit mainwindow_send_a_signal(message);
        message = "";
    }

}

