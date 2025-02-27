#ifndef MESSANGERCLIENT_H
#define MESSANGERCLIENT_H

#include <QObject>
#include <QQmlEngine>
#include <QtNetwork>
#include <QDebug>
#include <stdarg.h>
#include <McUser.h>
#include <McDebug.h>
#include <mcPacket.h>
#include <mcFriendModel.h>

#define TIMEOUT 5000
#define SEPARATOR 1



class MessangerClient : public QObject
{
    Q_OBJECT

//    Q_PROPERTY(McUser *mcuser)

public:
    MessangerClient(QObject* parent = nullptr);
    MessangerClient(McUser *mcuser);
    virtual ~MessangerClient();

    QByteArray intToArray(qint32 source);
    Q_INVOKABLE bool isConnected();
    Q_INVOKABLE void connectToHost(QString host, quint16 port);
    Q_INVOKABLE void requestSubmit(QString nickname, QString password, QString confirm);
    Q_INVOKABLE bool requestLogin(QString nickname, QString password);
    Q_INVOKABLE void requestUserData(QString nickname);

    Q_INVOKABLE void requestEnterChatRoom(QString roomid);

//    Q_INVOKABLE void requestLogout(QString nickname);

    void receivedFriendList(QString);
    void receivedChatroomList(QString);

    void updateChat(QString);

    // for debugging
    Q_INVOKABLE void clientDisconnect();

signals:
    void resLogin(QString protocol, QString data);
    void resSubmit(QString protocol, QString data);
    void resFriendList(int protocol, QString data);
    void disconnected();
    void loginCompleted();
    void resEnterChatRoom();
//    void resEnterChatRoom(mcChatModel);
//    void error(int socketError, QString &message);

public slots:
    void readMessage();

private:
    QTcpSocket *socket;
    McUser *mcuser;
    McDebug *debugger;
    McPacket *packet;

    bool isValidPacket(QByteArray header);
    bool writeData(QString sendtype);
    bool writeData(QString sendtype, QString data);

    void printDebugMessage(QString msg);

    int extractSizeToPacket(QByteArray &packet);
    QString extractProtocolToPacket(QByteArray &packet);
    QString extractDataToPacket(QByteArray &packet);

private slots:
    void error(int socketError, const QString &message);

};

#endif // MESSANGERCLIENT_H
