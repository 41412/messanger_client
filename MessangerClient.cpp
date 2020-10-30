#include "MessangerClient.h"

MessangerClient::MessangerClient()
{
    socket = new QTcpSocket(this);
    qmlRegisterType<MessangerClient>("MessangerClient", 1, 0, "MessangerClient");

    connect(socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
//    connect(socket, SIGNAL(error(int, QString)), this, SLOT(error(int, QString)));
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

bool MessangerClient::writeforDebugging(QString data)
{
    QByteArray msg = data.toStdString().c_str();

    if(isConnected())
    {
        socket->write(intToArray(msg.size()));
        socket->write(msg);
        return socket->waitForBytesWritten();
    }
    else
    {
        return false;
    }
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
        int index = 0;
//        while(readData.at(index) == '\0')
//        {
//            readData[index] = '0';
//            index++;
//        }
        int size = static_cast<int>(readData.at(0));
        readData.remove(0, sizeof(int) + SEPARATOR);
        QString strData = (QString::fromLocal8Bit(readData.data(), readData.size())).toUtf8();
        QString protocol = strData.left(strData.indexOf(' '));
        strData.remove(0, strData.indexOf(' ') + SEPARATOR);
        qDebug() << "[Debug] size : " << size;
        qDebug() << "[Debug] protocol : " << protocol;
        qDebug() << "[Debug] data : " << strData;

        if(protocol == "LOGIN_SUCCESS" || protocol == "LOGIN_FAIL")
        {
            emit resLogin(protocol, strData);
        }
        else if(protocol == "SUBMIT_SUCCESS" || protocol == "SUBMIT_FAIL")
        {
            emit resSubmit(protocol, strData);
        }
        else if(protocol == "USERDATA_SEND_START")
        {
            user = new User(strData);
            writeData("READY_TO_RECEIVE");
        }
        else if(protocol == "SEND_PROFILE")
        {

        }
        else if(protocol == "SEND_FRIENDLIST")
        {
            int received_size = (strData.left(sizeof(int))).toInt();
            strData.remove(0, sizeof(int) + SEPARATOR);
            receivedFriendList(received_size, strData);
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

//QString MessangerClient::addString(QString target, int num, QString, ...)
//{
//    va_list ap;
//    va_start(ap, num);
//    QString arg = "";
//    QString result = target;

//    for(int i = 0; i < num; i++)
//    {
//        arg = va_arg(ap, const char*);
//        result.append(arg);
//    }
//    va_end(ap);
//    return result;
//}

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

//bool MessangerClient::connectToHost(QString host, quint16 port)
//{
//    socket->connectToHost(host, port);
//    return socket->waitForConnected();
//}

void MessangerClient::connectToHost(QString host, quint16 port)
{
    socket->connectToHost(host, port);
    socket->waitForConnected((TIMEOUT));
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
