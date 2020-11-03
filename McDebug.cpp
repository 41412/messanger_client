#include "McDebug.h"

McDebug::McDebug()
{

}

McDebug::~McDebug()
{

}

void McDebug::debugMessage(QString varname, int var)
{
    qDebug() << "[Debug] " << varname << " : " << var;
}
void McDebug::debugMessage(QString varname, QString var)
{
    qDebug() << "[Debug] " << varname << " : " << var;
}
void McDebug::debugMessage(QString varname, QByteArray var)
{
    qDebug() << "[Debug] " << varname << " : " << var;
}
void McDebug::debugMessage(QString message)
{
    qDebug() << "[Debug] " << message;
}

void McDebug::testlogin()
{
    emit resLogin(0, "[Test] resLogin");
}

void McDebug::testsubmit()
{
//    emit resSubmit(2, "[test] resSubmit");
}
