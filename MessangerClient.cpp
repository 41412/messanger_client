#include "MessangerClient.h"

MessangerClient::MessangerClient(McUser *mcuser)
    : QObject(), socket(new QTcpSocket(this)), mcuser(mcuser), debugger(new McDebug()), packet(new McPacket())
{
    connect(socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
//    connect(socket, SIGNAL(error(int, QString)), this, SLOT(error(int, QString)));

//    connectToHost("192.168.10.200", 35000);
    connectToHost("192.168.10.194", 35000);
}
MessangerClient::MessangerClient(QObject* parent)
    : QObject(parent), socket(new QTcpSocket(this)), mcuser(nullptr), debugger(new McDebug()), packet(new McPacket())
{

}

MessangerClient::~MessangerClient()
{
    clientDisconnect();
    delete(socket);
}

bool MessangerClient::isConnected()
{
    return (socket->state() == QAbstractSocket::ConnectedState);
}

void MessangerClient::readMessage()
{
    if(socket->bytesAvailable() >= 0)
    {
        QByteArray rawdata = socket->readAll();
        debugger->debugMessage("rawdata", rawdata);
        if(!McPacket::isValidSizePacket(rawdata))
        {
            debugger->debugMessage("Invalid Data");
            return;
        }
        if(!McPacket::isValidHeaderPacket(rawdata.mid(0, 8)))
        {
            debugger->debugMessage("Invalid Data");
            return;
        }

        McPacket::removeHeader(rawdata);
        packet->extractReadPacket(rawdata);

        if(packet->getProtocol() == "LOGIN_SUCCESS" || packet->getProtocol() == "LOGIN_FAIL")
        {
            emit resLogin(packet->getProtocol(), packet->getData());
        }
        else if(packet->getProtocol() == "SUBMIT_SUCCESS" || packet->getProtocol() == "SUBMIT_FAIL")
        {
            emit resSubmit(packet->getProtocol(), packet->getData());
        }
        else if(packet->getProtocol() == "USERDATA_SEND_START")
        {
            McPacket::writePacket(socket, "READY_TO_RECEIVE");
        }
        else if(packet->getProtocol() == "SEND_PROFILE")
        {

        }
        else if(packet->getProtocol() == "SEND_FRIENDLIST")
        {
            int received_size = packet->getData().left(packet->getData().indexOf(' ')).toInt();
            packet->setData(packet->getData().remove(0, packet->getData().indexOf(' ') + SEPARATOR));
            receivedFriendList(received_size, packet->getData());
        }
        else if(packet->getProtocol() == "SEND_CHATLIST")
        {

        }
        else if(packet->getProtocol() == "USERDATA_SEND_END")
        {
            emit loginCompleted();
        }
    }
}

QByteArray MessangerClient::intToArray(qint32 source)
{
    // Avoid use of cast, this is the Qt way to serialize objects
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}

void MessangerClient::clientDisconnect()
{
    socket->disconnectFromHost();
    emit disconnected();
    debugger->debugMessage("socket is disconnected.");
}

void MessangerClient::connectToHost(QString host, quint16 port)
{
    socket->connectToHost(host, port);
//    socket->waitForConnected((TIMEOUT));
//    if (!socket->waitForConnected(TIMEOUT))
//    {
//        emit error(socket->error(), socket->errorString());
//        qDebug() << "[Debug] Server no response.";
//        return;
//    }
}

void MessangerClient::requestSubmit(QString nickname, QString password, QString confirm)
{
    if(nickname == "" || password == "")
    {
        emit resSubmit("SUBMIT_FAIL", "Nickname or Password is empty.");
        return;
    }
    if(password != confirm)
    {
        emit resSubmit("SUBMIT_FAIL", "Both of password are NOT same.");
        return;
    }

    if(!isConnected())
    {
        emit resSubmit("SUBMIT_FAIL", "Server is no response.");
        return;
    }
    else
    {
        QString data = nickname + " " + password;
        McPacket::writePacket(socket, "REQUEST_SUBMIT", data);
    }
}

bool MessangerClient::requestLogin(QString nickname, QString password)
{
    if(nickname == "" || password == "")
    {
        emit resLogin("LOGIN_FAIL", "Nickname or Password is empty.");
        return false;
    }

    if(!isConnected())
    {
        emit resLogin("LOGIN_FAIL", "Server is no response.");
        return false;
    }

    QString data = nickname + " " + password;
    McPacket::writePacket(socket, "REQUEST_LOGIN", data);
    return true;
}

void MessangerClient::requestUserData(QString nickname)
{

    McPacket::writePacket(socket, "REQUEST_USERDATA", nickname);
}

//void MessangerClient::requestLogout(QString nickname)
//{

//}

void MessangerClient::receivedFriendList(int received_size, QString strData)
{
    QStringList list = strData.split(' ');
    int real_size = strData.split(' ').length();
    debugger->debugMessage("received_size", received_size);
    debugger->debugMessage("real_size", real_size);

    if(received_size != list.size())
    {
        McPacket::writePacket(socket, "DATA_ERROR");
    }
    else
    {
        foreach(QString s, list)
        {
            mcuser->addFriend(s, "소개말");
        }
        McPacket::writePacket(socket, "FRIENDLIST_RECEIVED");
    }
}

void MessangerClient::error(int socketError, const QString &message)
{
    switch(socketError)
    {
        case QAbstractSocket::HostNotFoundError:
        {
            qDebug() << message;
        }
        break;
        case QAbstractSocket::ConnectionRefusedError:
        {
            qDebug() << message;
        }
        break;
        default:
        break;
    }
}
