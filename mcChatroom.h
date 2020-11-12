#ifndef MCCHATROOM_H
#define MCCHATROOM_H

#include <QObject>
#include <mcChatModel.h>
#include <mcChat.h>

class mcChatroom
{
public:
    mcChatroom(const QString &roomid, const QVector<QString> &attendees);
    virtual ~mcChatroom();

    mcChatroom* room();
    QString roomid() const;
    QString roomname() const;
    mcChatModel* chatmodel();
    void addChat(int index, QString time, QString sender, QString message);
    QList<mcChat> getChatList();

private:
    QString m_roomid;
    QString m_roomname;
    QVector<QString> m_attendees;
    mcChatModel* m_chatmodel;

};

#endif // MCCHATROOM_H
