#ifndef CONVERSATION_WIDGET_H
#define CONVERSATION_WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace Ui {
class conversation_widget;
}

class conversation_widget : public QWidget
{
    Q_OBJECT

public:
    explicit conversation_widget(QString userName,QString friendName,QTcpSocket* m_soc,QString msg = NULL,QWidget *parent = nullptr);
    ~conversation_widget();

private slots:
    void on_send_clicked();

    void dialog_receive_a_signal(QString main_to_dia_msg);//定义子窗口接收主窗口信号的槽函数

private:
    Ui::conversation_widget *ui;
    QString userName;
    QString friendName;
    QTcpSocket* m_soc;
    QString msg;
};

#endif // CONVERSATION_WIDGET_H
