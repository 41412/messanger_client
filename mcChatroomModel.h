#ifndef MCCHATROOMMODEL_H
#define MCCHATROOMMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <mcChatroom.h>

class mcChatroomModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ModelRoles
    {
        RoomIDRole = Qt::UserRole + 1,
        RoomNameRole,

    };

    mcChatroomModel(QObject *parent = nullptr);
    virtual ~mcChatroomModel();

    void addChatroom(const mcChatroom &c);
    QList<mcChatroom> getChatroomList();
    mcChatroom* getChatroom(QString roomid);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QList<mcChatroom> m_chatrooms;
};

#endif // MCCHATROOMMODEL_H
