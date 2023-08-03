#ifndef SENDFILE_H
#define SENDFILE_H

#include <QObject>
#include <QTcpSocket>

//传输文件用的，先不做了
class SendFile : public QObject
{
    Q_OBJECT
public:
    explicit SendFile(QObject *parent = nullptr);

    //发送文件，在子线程里做
    void sendFiles(QString path);

    //连接服务器
    void connectSever(QString ip,unsigned short port);

    //向服务器发送信息
    void sendMsgs(QString msg);

    void endSever();

signals:
    void curPercent(int percent);   //进度条百分比
    void connectOK();   //告诉主线程连接成功
    void connectEnd();  //告诉主线程断开连接
private:
    QTcpSocket* m_soc;

};

#endif // SENDFILE_H
