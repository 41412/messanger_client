#ifndef MESSANGERCLIENT_H
#define MESSANGERCLIENT_H

#include <QObject>
#include <QQmlEngine>
#include <QtNetwork>
#include <QDebug>

typedef int PROTOCOL;

class MessangerClient : public QObject
{
public:
    Q_OBJECT

public:
    MessangerClient();
    ~MessangerClient();

    Q_INVOKABLE bool connectToHost(QString host, int port);
    Q_INVOKABLE void requestSubmit(QString nickname, QString password);
//    Q_INVOKABLE bool writeData(PROTOCOL protocol, QString data);
//    Q_INVOKABLE bool refresh();

public slots:


private:
    QTcpSocket *socket;
    bool isConnected = false;


private slots:
};

#endif // MESSANGERCLIENT_H
