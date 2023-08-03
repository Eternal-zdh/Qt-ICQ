#ifndef CLINETSOCKET_H
#define CLINETSOCKET_H

#include <QObject>
#include <QTcpSocket>

class ClinetSocket : public QObject
{
    Q_OBJECT
public:
    explicit ClinetSocket(QTcpSocket* socket,QObject *parent = nullptr);

private:
    QTcpSocket* socket;
signals:

};

#endif // CLINETSOCKET_H
