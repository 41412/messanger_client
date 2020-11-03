#include "MessangerClient.h"

MessangerClient::MessangerClient(McUser *mcuser)
    : QObject(), socket(new QTcpSocket(this)), mcuser(mcuser), debugger(new McDebug())
{
    connect(socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
//    connect(socket, SIGNAL(error(int, QString)), this, SLOT(error(int, QString)));

//    connectToHost("192.168.10.200", 35000);
    connectToHost("192.168.10.194", 35000);
}
MessangerClient::MessangerClient(QObject* parent)
    : QObject(parent), socket(new QTcpSocket(this)), mcuser(nullptr), debugger(new McDebug())
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

bool MessangerClient::writeData(QString sendtype)
{
    QByteArray msg = sendtype.toStdString().c_str();
    if(!isConnected())
    {
        return false;
    }

    debugger->debugMessage("write size", intToArray(msg.size()));
    msg.prepend(" ");
    msg.prepend(intToArray(msg.size()));
    debugger->debugMessage("write data", msg);
    socket->write(msg);
    return socket->waitForBytesWritten(TIMEOUT);
}
bool MessangerClient::writeData(QString sendtype, QString data)
{
    data.prepend(" ");
    data.prepend(sendtype);
    QByteArray msg = data.toStdString().c_str();

    if(!isConnected())
    {
        return false;
    }

    debugger->debugMessage("write size", intToArray(msg.size()));
    msg.prepend(" ");
    msg.prepend(intToArray(msg.size()));
    debugger->debugMessage("write data", msg);
    socket->write(msg);
    return socket->waitForBytesWritten(TIMEOUT);
}

void MessangerClient::readMessage()
{
    if(socket->bytesAvailable() >= 0)
    {
        QByteArray rawdata = socket->readAll();
        if(rawdata.size() < 1) {
            debugger->debugMessage("packet has no data.");
            return;
        }

        debugger->debugMessage("rawdata", rawdata);
        int size = extractSizeToPacket(rawdata);
        QString protocol = extractProtocolToPacket(rawdata);
        QString data = extractDataToPacket(rawdata);

        if(protocol == "LOGIN_SUCCESS" || protocol == "LOGIN_FAIL")
        {
            emit resLogin(protocol, data);
        }
        else if(protocol == "SUBMIT_SUCCESS" || protocol == "SUBMIT_FAIL")
        {
            emit resSubmit(protocol, data);
        }
        else if(protocol == "USERDATA_SEND_START")
        {
            writeData("READY_TO_RECEIVE");
        }
        else if(protocol == "SEND_PROFILE")
        {

        }
        else if(protocol == "SEND_FRIENDLIST")
        {
            int received_size = data.left(data.indexOf(' ')).toInt();
            data.remove(0, data.indexOf((' ')) + SEPARATOR);
            receivedFriendList(received_size, data);
        }
        else if(protocol == "SEND_CHATLIST")
        {

        }
        else if(protocol == "USERDATA_SEND_END")
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
        QString data = nickname;
        data.append(" ");
        data.append(password);
        writeData("REQUEST_SUBMIT", data);
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

    QString data = nickname;
    data.append(" ");
    data.append(password);
    writeData("REQUEST_LOGIN", data);
    return true;
}

void MessangerClient::requestUserData(QString nickname)
{

    writeData("REQUEST_USERDATA", nickname);
}

void MessangerClient::requestLogout(QString nickname)
{
//    user
}

void MessangerClient::receivedFriendList(int received_size, QString strData)
{
    QStringList list = strData.split(' ');
    int real_size = strData.split(' ').length();
    debugger->debugMessage("received_size", received_size);
    debugger->debugMessage("real_size", real_size);

    if(received_size != list.size())
    {
        writeData("DATA_ERROR");
    }
    else
    {
        foreach(QString s, list)
        {
            mcuser->addFriend(s, "소개말");
        }
        writeData("FRIENDLIST_RECEIVED");
    }
}

int MessangerClient::extractSizeToPacket(QByteArray &packet)
{
    bool ok;
    int size = packet.mid(0, sizeof(int)).toHex().toInt(&ok, 16);
    packet.remove(0, sizeof(int) + SEPARATOR);
    debugger->debugMessage("size", size);
    return size;
}

QString MessangerClient::extractProtocolToPacket(QByteArray &packet)
{
    QString protocol;
    if(packet.indexOf(' ') == -1)
    {
        protocol = packet;
        packet = "";
    }
    else
    {
        protocol = packet.left(packet.indexOf(' '));
        packet.remove(0, packet.indexOf(' ') + SEPARATOR);
    }
    debugger->debugMessage("protocol", protocol);
    return protocol;
}

QString MessangerClient::extractDataToPacket(QByteArray &packet)
{
    QString data = QString::fromLocal8Bit(packet.data(), packet.size()).toUtf8();
    debugger->debugMessage("data", data);
    return data;
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
