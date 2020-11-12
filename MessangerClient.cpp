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

        if(McPacket::isNewPacket(rawdata.mid(0, 8)))
        {
            packet->removeHeader(rawdata);
            packet->initPacketBuffer();
            packet->extractReadPacket(rawdata);
        }
        else
        {
            packet->extractReadPacketData(rawdata);
        }

        if(!packet->isReadCompleted())
        {
            return;
        }

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
            receivedFriendList(packet->getData());
        }
        else if(packet->getProtocol() == "SEND_CHATROOMLIST")
        {
            receivedChatroomList(packet->getData());
        }
        else if(packet->getProtocol() == "USERDATA_SEND_END")
        {
            emit loginCompleted();
        }
        else if(packet->getProtocol() == "RESPONSE_CREATE_CHATROOM")
        {

        }
        else if(packet->getProtocol() == "RESPONSE_INVITE_USER")
        {

        }
        else if(packet->getProtocol() == "RESPONSE_LEAVE_CHATROOM")
        {

        }
        else if(packet->getProtocol() == "UPDATE_CHAT")
        {
            updateChat(packet->getData());
        }
    }
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

void MessangerClient::receivedFriendList(QString strData)
{
    int friend_size = strData.left(strData.indexOf(0x1D)).toInt();
    strData.remove(0, strData.indexOf(0x1D) + SEPARATOR);
    QStringList list = strData.split(0x1D);
    int real_size = strData.split(0x1D).length();
    debugger->debugMessage("friendsize", friend_size);
    debugger->debugMessage("realsize", real_size);

    if(friend_size != list.size())
    {
        McPacket::writePacket(socket, "DATA_ERROR");
    }
    else
    {
        foreach(QString s, list)
        {
            mcuser->getFriendModel()->addFriend(mcUserFriend(s, "profile"));
        }
        McPacket::writePacket(socket, "FRIENDLIST_RECEIVED");
    }
}

void MessangerClient::receivedChatroomList(QString strData)
{
    int room_size = strData.left(strData.indexOf(0x1D)).toInt();
    strData.remove(0, strData.indexOf(0x1D) + SEPARATOR);
    QStringList room_list = strData.split(0x1D);

    if(room_size != room_list.size())
    {
        McPacket::writePacket(socket, "DATA_ERROR");
        return;
    }
    else
    {
        foreach(QString room, room_list)
        {
            QString roomid = room.left(room.indexOf(0x1E));
            room.remove(0, room.indexOf(0x1E) + SEPARATOR);
            int attendee_size = room.left(room.indexOf(0x1E)).toInt();
            room.remove(0, room.indexOf(0x1E) + SEPARATOR);
            QStringList attendee_list = room.split(0x1E);
            if(attendee_size != attendee_list.size())
            {
                McPacket::writePacket(socket, "DATA_ERROR");
                return;
            }
            else
            {
                QVector<QString> attendees;
                foreach(QString attendee, attendee_list)
                {
                    attendees.push_back(attendee);
                }
                mcuser->getChatroomModel()->addChatroom(mcChatroom(roomid, attendees));
            }
        }
        McPacket::writePacket(socket, "CHATROOMLIST_RECEIVED");
    }
}

void MessangerClient::requestEnterChatRoom(QString roomid)
{
    emit resEnterChatRoom();
//    emit resEnterChatRoom(mcuser->getChatroomModel()->getChatroom(roomid)->chatmodel());
}

void MessangerClient::updateChat(QString strData)
{
    QString roomid = strData.left(strData.indexOf(0x1D));
    strData.remove(0, strData.indexOf(0x1D) + SEPARATOR);
    int index = strData.left(strData.indexOf(0x1D)).toInt();
    strData.remove(0, strData.indexOf(0x1D) + SEPARATOR);
    QString sender = strData.left(strData.indexOf(0x1D));
    strData.remove(0, strData.indexOf(0x1D) + SEPARATOR);
    QString time = strData.left(strData.indexOf(0x1D));
    strData.remove(0, strData.indexOf(0x1D) + SEPARATOR);
    QString message = strData.left(strData.indexOf(0x1D));
    strData.remove(0, strData.indexOf(0x1D) + SEPARATOR);

    mcuser->getChatroomModel()->getChatroom(roomid)->addChat(index, sender, time, message);
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
