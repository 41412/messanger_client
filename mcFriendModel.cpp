#include "mcFriendModel.h"

mcFriendModel::mcFriendModel(QObject *parent) : QAbstractListModel(parent)
{

}
mcFriendModel::~mcFriendModel()
{

}

void mcFriendModel::addFriend(const mcUserFriend &f)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_friends << f;
    endInsertRows();
}

int mcFriendModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_friends.count();
}

QVariant mcFriendModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() >= m_friends.count())
    {
        return QVariant();
    }

    const mcUserFriend &f = m_friends[index.row()];
    if(role == NicknameRole)
    {
        return f.nickname();
    }
    else if(role == ProfileRole)
    {
        return f.profile();
    }
    return QVariant();
}

QHash<int, QByteArray> mcFriendModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NicknameRole] = "nickname";
    roles[ProfileRole] = "profile";
    return roles;
}
