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

bool MessangerClient::writeData(PROTOCOL_CLIENT sendtype, QString data)
{
    data.prepend(" ");
    data.prepend(QString::number(sendtype));
    QByteArray msg = data.toStdString().c_str();

    if(isConnected())
    {
        qDebug() << "[Debug] write size : " << intToArray(msg.size());
        msg.prepend(intToArray(msg.size()));
        qDebug() << "[Debug] write data : " << msg;
        socket->write(msg);
        return socket->waitForBytesWritten();
    }
    else
    {
        return false;
    }
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
    emit resSubmit(2, "[test] resSubmit");
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
        while(readData.at(index) == '\0')
        {
            readData[index] = '0';
            index++;
        }
        int size = (readData.left(sizeof(int))).toInt();
        readData.remove(0, sizeof(int) + SEPARATOR);
//        PROTOCOL_CLIENT protocol = (readData.left(sizeof(int))).toInt();
        QString strData = (QString::fromLocal8Bit(readData.data(), readData.size())).toUtf8();
        PROTOCOL_CLIENT protocol = (strData.left(strData.indexOf(' '))).toInt();
        strData.remove(0, strData.indexOf(' ') + SEPARATOR);
        qDebug() << "[Debug] size : " << size;
        qDebug() << "[Debug] protocol : " << protocol;
        qDebug() << "[Debug] data : " << strData;

        switch(protocol)
        {
            case LOGIN_SUCCESS:
            case LOGIN_FAIL:
            {
                emit resLogin(protocol, strData);
            }
            break;

            case SUBMIT_SUCCESS:
            case SUBMIT_FAIL:
            {
                emit resSubmit(protocol, strData);
            }

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
        emit resSubmit(SUBMIT_FAIL, "Nickname or Password is empty.");
        return;
    }
    if(password != confirm)
    {
        emit resSubmit(SUBMIT_FAIL, "Both of password are NOT same.");
        return;
    }

    if(!isConnected())
    {
        emit resSubmit(SUBMIT_FAIL, "Server is no response.");
        return;
    }
    else
    {
        QString data = nickname;
        data.append(" ");
        data.append(password);
        writeData(SUBMIT_REQUEST, data);
    }
}

bool MessangerClient::requestLogin(QString nickname, QString password)
{
    if(nickname == "" || password == "")
    {
        emit resLogin(LOGIN_FAIL, "Nickname or Password is empty.");
        return false;
    }

    if(!isConnected())
    {
        emit resLogin(LOGIN_FAIL, "Server is no response.");
        return false;
    }

    QString data = nickname;
    data.append(" ");
    data.append(password);
    writeData(LOGIN_REQUEST, data);
    return true;
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

//QString MessangerClient::resLogin(PROTOCOL_SERVER protocol, QString data)
//{
//    if(protocol == LOGIN_SUCCESS)
//    {
//        // TODO : To change view from [Login] to [Chatlist]
//        return "Login Success";
//    }
//    else
//    {
//        return data;
//    }
//}

