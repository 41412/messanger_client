#ifndef MESSANGERCLIENT_H
#define MESSANGERCLIENT_H

#include <QObject>
#include <QQmlEngine>
#include <QtNetwork>
#include <QDebug>
#include <stdarg.h>
#include <user.h>

#define TIMEOUT 5000
#define SEPARATOR 1

typedef int PROTOCOL_CLIENT;
typedef int PROTOCOL_SERVER;

enum { LOGIN_REQUEST = 0, SUBMIT_REQUEST = 1, REQUEST_USERDATA, FRIENDLIST_RECEIVED };     // PROTOCOL_CLIENT
enum { LOGIN_SUCCESS = 0, LOGIN_FAIL = 1, SUBMIT_SUCCESS, SUBMIT_FAIL, USERDATA_START, USERDATA_END, SEND_FRIENDLIST };         // PROTOCOL_SERVER

class MessangerClient : public QObject
{
    Q_OBJECT

public:
    MessangerClient();
    ~MessangerClient();

    QByteArray intToArray(qint32 source);
    Q_INVOKABLE void connectToHost(QString host, quint16 port);
    Q_INVOKABLE void requestSubmit(QString nickname, QString password, QString confirm);
    Q_INVOKABLE bool requestLogin(QString nickname, QString password);

    Q_INVOKABLE void requestUserData(QString nickname);

    void receivedFriendList(QString strData);

    // for debugging
    Q_INVOKABLE bool writeforDebugging(QString data);
    Q_INVOKABLE void clientDisconnect();
    Q_INVOKABLE void testlogin();
    Q_INVOKABLE void testsubmit();

signals:
    void resLogin(int protocol, QString data);
    void resSubmit(int protocol, QString data);
    void resFriendList(int protocol, QString data);
    void disconnected();
    void loginCompleted();
//    void error(int socketError, QString &message);

public slots:
    void readMessage();

private:
    QTcpSocket *socket;
    User *user;

    bool writeData(PROTOCOL_CLIENT sendtype);
    bool writeData(PROTOCOL_CLIENT sendtype, QString data);
    bool isConnected();
    void printDebugMessage(QString msg);

//    QString addString(QString target, int num, QString ...);

private slots:
    void error(int socketError, const QString &message);

};

#endif // MESSANGERCLIENT_H
