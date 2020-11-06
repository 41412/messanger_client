#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <mcUserFriend.h>

class mcFriendModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ModelRoles
    {
        NicknameRole = Qt::UserRole + 1,
        ProfileRole
    };

    mcFriendModel(QObject *parent = nullptr);
    virtual ~mcFriendModel();

    void addFriend(const mcUserFriend &f);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QList<mcUserFriend> m_friends;

};

#endif // DATAMODEL_H
