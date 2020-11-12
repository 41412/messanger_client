#include "mcChatroomModel.h"

mcChatroomModel::mcChatroomModel(QObject *parent) : QAbstractListModel(parent)
{

}
mcChatroomModel::~mcChatroomModel()
{

}

void mcChatroomModel::addChatroom(const mcChatroom &c)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_chatrooms << c;
    endInsertRows();
}

QList<mcChatroom> mcChatroomModel::getChatroomList()
{
    return m_chatrooms;
}

mcChatroom* mcChatroomModel::getChatroom(QString roomid)
{
    QList<mcChatroom*> roomlist;
    for(auto it = m_chatrooms.begin(); it != m_chatrooms.end(); it++)
    {
        roomlist.push_back(it->room());
    }
    foreach(mcChatroom* room, roomlist)
    {
        if(room->roomid() == roomid)
        {
            return room;
        }
    }

    // TODO : 없으면 방을 만들어서 추가한다.

}

int mcChatroomModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_chatrooms.count();
}

QVariant mcChatroomModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() >= m_chatrooms.count())
    {
        return QVariant();
    }

    const mcChatroom &c = m_chatrooms[index.row()];
    if(role == RoomIDRole)
    {
        return c.roomid();
    }
    else if(role == RoomNameRole)
    {
        return c.roomname();
    }
    return QVariant();
}

QHash<int, QByteArray> mcChatroomModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[RoomIDRole] = "roomid";
    roles[RoomNameRole] = "roomname";
    return roles;
}
