#include "mcChatModel.h"

mcChatModel::mcChatModel(QObject *parent) : QAbstractListModel(parent)
{

}
mcChatModel::~mcChatModel()
{

}

//void mcChatModel::addChat(const mcChat &c)
void mcChatModel::addChat(int index, QString sender, QString time, QString message)
{
    mcChat chat(index, sender, time, message);
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_chatlist << chat;
//    m_chatlist << c;
    endInsertRows();
}

QList<mcChat> mcChatModel::getChatList()
{
    return m_chatlist;
}

int mcChatModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_chatlist.count();
}

QVariant mcChatModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() >= m_chatlist.count())
    {
        return QVariant();
    }

    const mcChat &c = m_chatlist[index.row()];
    if(role == ChatTimeRole)
    {
        return c.time();
    }
    else if(role == ChatSenderRole)
    {
        return c.sender();
    }
    else if(role == ChatChatRole)
    {
        return c.chat();
    }
    return QVariant();
}

QHash<int, QByteArray> mcChatModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ChatTimeRole] = "time";
    roles[ChatSenderRole] = "sender";
    roles[ChatChatRole] = "chat";
    return roles;
}
