#ifndef MCCHATMODEL_H
#define MCCHATMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <mcChat.h>

class mcChatModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ModelRoles
    {
        ChatTimeRole = Qt::UserRole + 1,
        ChatSenderRole,
        ChatChatRole,
    };
    mcChatModel(QObject *parent = nullptr);
    virtual ~mcChatModel();

//    void addChat(const mcChat &c);
    void addChat(int index, QString sender, QString time, QString message);
    QList<mcChat> getChatList();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QList<mcChat> m_chatlist;
};

#endif // MCCHATMODEL_H
