#ifndef MCDEBUG_H
#define MCDEBUG_H

#include <QObject>
#include <QQmlEngine>
#include <QDebug>

class McDebug : public QObject
{
    Q_OBJECT
public:
    McDebug();
    ~McDebug();

    Q_INVOKABLE void debugMessage(QString varname, int var);
    Q_INVOKABLE void debugMessage(QString varname, QString var);
    Q_INVOKABLE void debugMessage(QString varname, QByteArray var);
    Q_INVOKABLE void debugMessage(QString message);

    Q_INVOKABLE void testlogin();
    Q_INVOKABLE void testsubmit();

signals:
    void resLogin(QString protocol, QString data);
    void resSubmit(QString protocol, QString data);
};

#endif // MCDEBUG_H
