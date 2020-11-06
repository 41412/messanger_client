#ifndef MCUSER_H
#define MCUSER_H

#include <QObject>
#include <QQmlEngine>
#include <mcUserFriend.h>
#include <mcFriendModel.h>

class McUser : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString nickname READ getNickname WRITE setNickname NOTIFY nameChanged)
    Q_PROPERTY(QString profile_text READ getProfileText WRITE setProfileText NOTIFY profileChanged)

public:
    explicit McUser();
    virtual ~McUser();

    QString getNickname() { return nickname; }
    void setNickname(QString text) { nickname = text; }
    QString getProfileText() { return profile_text; }
    void setProfileText(QString text) { profile_text = text; }
    void addFriend(QString, QString);
    mcFriendModel* getFriendModel();

signals:
    void nameChanged();
    void profileChanged();

private:
    QString nickname;
    QString profile_text;
    mcFriendModel friendmodel;
//    QVector<ChatRoom> chatroom_list;


};

#endif // MCUSER_H
