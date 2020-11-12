#include "mcChat.h"

mcChat::mcChat(const int &index, const QString &time, const QString &sender, const QString &chat)
    : m_index(index), m_time(time), m_sender(sender), m_chat(chat)
{

}
mcChat::~mcChat()
{

}
int mcChat::index() const
{
    return m_index;
}
QString mcChat::time() const
{
    return m_time;
}
QString mcChat::sender() const
{
    return m_sender;
}
QString mcChat::chat() const
{
    return m_chat;
}
