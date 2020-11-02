#include "MessangerClient.h"

MessangerClient::MessangerClient()
{
    socket = new QTcpSocket(this);
    qmlRegisterType<MessangerClient>("MessangerClient", 1, 0, "MessangerClient");

    connect(socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
//    connect(socket, SIGNAL(error(int, QString)), this, SLOT(error(int, QString)));

//    connectToHost("192.168.10.200", 35000);
    connectToHost("192.168.10.194", 35000);
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
    qDebug() << "[Debug] write size : " << intToArray(msg.size());
    msg.prepend(" ");
    msg.prepend(intToArray(msg.size()));
    qDebug() << "[Debug] write data : " << msg;
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

    qDebug() << "[Debug] write size : " << intToArray(msg.size());
    msg.prepend(" ");
    msg.prepend(intToArray(msg.size()));
    qDebug() << "[Debug] write data : " << msg;
    socket->write(msg);
    return socket->waitForBytesWritten(TIMEOUT);
}

void MessangerClient::testlogin()
{
    emit resLogin(0, "[Test] resLogin");
}

void MessangerClient::testsubmit()
{
//    emit resSubmit(2, "[test] resSubmit");
}

void MessangerClient::readMessage()
{
    if(socket->bytesAvailable() >= 0)
    {
        QByteArray readData = socket->readAll();
        if(readData.size() < 1) {
            qDebug() << "[Debug] packet has no data.";
            return;
        }
        qDebug() << "[Debug] rawdata : " << readData;
        int size = extractSizeToPacket(readData);
        QString protocol = extractProtocolToPacket(readData);
        QString data = extractDataToPacket(readData);
        qDebug() << "[Debug] size : " << size;
        qDebug() << "[Debug] protocol : " << protocol;
        qDebug() << "[Debug] data : " << data;

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
            user = new User(data);
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
    qDebug() << "[Debug] socket is disconnected.";
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

void MessangerClient::receivedFriendList(int received_size, QString strData)
{
    QStringList list = strData.split(' ');
    int real_size = strData.split(' ').length();
    qDebug() << "[Debug] received size : " << received_size;
    qDebug() << "[Debug] real size : " << real_size;
    if(received_size != list.size())
    {
        writeData("DATA_ERROR");
    }
    else
    {
        foreach(QString s, list)
        {
            user->addFriend(s, "소개말");
        }
        writeData("FRIENDLIST_RECEIVED");
    }
}

int MessangerClient::extractSizeToPacket(QByteArray &packet)
{
    bool ok;
    int size = packet.mid(0, sizeof(int)).toHex().toInt(&ok, 16);
    packet.remove(0, sizeof(int) + SEPARATOR);
    return size;
}

QString MessangerClient::extractProtocolToPacket(QByteArray &packet)
{
    QString protocol;
    if(packet.indexOf(' ') == -1)
    {
        protocol = packet;
    }
    else
    {
        protocol = packet.left(packet.indexOf(' '));
        packet.remove(0, packet.indexOf(' ') + SEPARATOR);
    }
    return protocol;
}

QString MessangerClient::extractDataToPacket(QByteArray &packet)
{
    return QString::fromLocal8Bit(packet.data(), packet.size()).toUtf8();
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
