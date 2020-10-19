#include "MessangerClient.h"

MessangerClient::MessangerClient()
{
    socket = new QTcpSocket(this);
    qmlRegisterType<MessangerClient>("MessangerClient", 1, 0, "MessangerClient");
}
MessangerClient::~MessangerClient()
{

}

bool MessangerClient::connectToHost(QString host, int port)
{
    qDebug() << "host : " << host << "\n";
    qDebug() << "port : " << port << "\n";
    socket->connectToHost(host, port);
//    return socket->waitForConnected();
    return true;
}

void MessangerClient::requestSubmit(QString nickname, QString password)
{
    qDebug() << "nickname : " << nickname << "\n";
    qDebug() << "password : " << password << "\n";
}
