#ifndef MESSANGERCLIENT_H
#define MESSANGERCLIENT_H

#include <QObject>
#include <QQmlEngine>
#include <QtNetwork>
#include <QDebug>
#include <stdarg.h>

#define TIMEOUT 5000
#define SEPARATOR 1

typedef int PROTOCOL_CLIENT;
typedef int PROTOCOL_SERVER;

enum { LOGIN_REQUEST = 0, SUBMIT_REQUEST = 1 };     // PROTOCOL_CLIENT
enum { LOGIN_SUCCESS = 0, LOGIN_FAIL = 1, SUBMIT_SUCCESS, SUBMIT_FAIL };         // PROTOCOL_SERVER

class MessangerClient : public QObject
{
public:
    Q_OBJECT

public:
    MessangerClient();
    ~MessangerClient();

    QByteArray intToArray(qint32 source);
    Q_INVOKABLE void connectToHost(QString host, quint16 port);
//    Q_INVOKABLE bool connectToHost(QString host, quint16 port);
    Q_INVOKABLE void requestSubmit(QString nickname, QString password, QString confirm);
    Q_INVOKABLE bool requestLogin(QString nickname, QString password);
//    Q_INVOKABLE bool writeData(PROTOCOL protocol, QString data);
//    Q_INVOKABLE bool refresh();

    // for debugging
    Q_INVOKABLE bool writeforDebugging(QString data);
    Q_INVOKABLE void clientDisconnect();
    Q_INVOKABLE void testlogin();
    Q_INVOKABLE void testsubmit();

signals:
    void resLogin(int protocol, QString data);
    void resSubmit(int protocol, QString data);
//    void error(int socketError, QString &message);

public slots:
    void readMessage();

private:
    QTcpSocket *socket;

    bool writeData(PROTOCOL_CLIENT sendtype, QString data);
    bool isConnected();
    void printDebugMessage(QString msg);

//    QString addString(QString target, int num, QString ...);

private slots:
    void error(int socketError, const QString &message);

};

#endif // MESSANGERCLIENT_H
