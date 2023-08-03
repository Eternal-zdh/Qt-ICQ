#include "conversation_widget.h"
#include "ui_conversation_widget.h"

conversation_widget::conversation_widget(QString userName,QString friendName,QTcpSocket* m_soc,QString msg,QWidget *parent)
    :QWidget(parent),
    ui(new Ui::conversation_widget)
{
    ui->setupUi(this);
    conversation_widget::userName = userName;
    conversation_widget::friendName = friendName;
    conversation_widget::m_soc = m_soc;
    conversation_widget::msg = msg;
    this->setWindowTitle(friendName);
    //ui->getMSG->append(conversation_widget::msg);

    //用信号和槽的方式，将信息发送到子窗口
}

conversation_widget::~conversation_widget()
{
    delete ui;
}

void conversation_widget::on_send_clicked()
{
    //获取想要发送的信息
    QString msg = ui->sendMSG->toPlainText();

    //向服务器发送一段json
    QString userMsg = "{\"msgType\":\"talkToFd\",\"userName\":\""+userName+"\",\"fdName\":\""+friendName+"\",\"message\":\""+msg+"\"}";
    m_soc->write(userMsg.toUtf8());
    ui->getMSG->append(msg);
    ui->sendMSG->clear();
}

//接收主窗口发来的数据，并在子窗口textedit上显示
void conversation_widget::dialog_receive_a_signal(QString main_to_dia_msg)
{
    ui->getMSG->append(main_to_dia_msg);
}
