#include "clinetsocket.h"

ClinetSocket::ClinetSocket(QTcpSocket* socket,QObject *parent)
    : QObject{parent},socket(socket)
{
    //暂时没用，后续可能用上
}
