#include "sendfile.h"

#include <QFile>
#include <QFileInfo>

SendFile::SendFile(QObject *parent)
    : QObject{parent}
{

}

void SendFile::sendFiles(QString path)
{
    QFile file(path);
    QFileInfo info(path);
    int filesize = info.size();  //获取文件大小
    file.open(QFile::ReadOnly);
    //当文件没有读完
    while (!file.atEnd()) {
        static int n = 0;
        if(n == 0){
            //通知服务器，将要发送的文件大小
            m_soc->write((char*)&filesize,4);
        }

        QByteArray line = file.readLine();  //读一行数据
        m_soc->write(line);
        n += line.size();   //当前已发送的数据
        int percent = n*100/filesize;   //计算百分比
        emit curPercent(percent);   //触发信号，更改进度条
    }
}

void SendFile::connectSever(QString ip, unsigned short port)
{


    //告诉主线程，服务器连接成功，主线程会执行一些界面UI的操作
    connect(m_soc,&QTcpSocket::connected,this,&SendFile::connectOK);

}

void SendFile::sendMsgs(QString msg)
{
    m_soc->write(msg.toUtf8());
}

void SendFile::endSever()
{
    m_soc->disconnectFromHost();    //断开连接
    connect(m_soc,&QTcpSocket::disconnected,this,[=](){
        m_soc->close();
        //m_soc->deleteLater();     //手动释放会导致无法再次连接
        emit connectEnd();      //启动信号，告诉主线程连接已断开
    });
}
