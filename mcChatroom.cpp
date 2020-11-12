#include "mcChatroom.h"

mcChatroom::mcChatroom(const QString &roomid, const QVector<QString> &attendees)
    : m_roomid(roomid), m_roomname(""), m_attendees(attendees)
{

}
mcChatroom::~mcChatroom()
{

}

mcChatroom* mcChatroom::room()
{
    return this;
}

QString mcChatroom::roomid() const
{
    return m_roomid;
}
QString mcChatroom::roomname() const
{
    return m_roomname;
}
mcChatModel* mcChatroom::chatmodel()
{
    return m_chatmodel;
}
//QList<mcChat> mcChatroom::getChatList()
//{
//    return m_chatmodel->getChatList();
//}
void mcChatroom::addChat(int index, QString sender, QString time, QString message)
{
//    const mcChat *chat = new mcChat(index, time, sender, message);
    m_chatmodel->addChat(index, sender, time, message);
//    m_chatmodel->addChat(*chat);
}
