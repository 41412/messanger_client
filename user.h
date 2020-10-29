#ifndef USER_H
#define USER_H

#include <QObject>
#include <QQmlEngine>

struct UserFriend {
    QString nickname;
    QString profile;
};

class User : public QObject
{
    Q_OBJECT
public:
    explicit User();
    explicit User(QString _nickname);
    explicit User(QString _nickname, QString _profile_text);
      ~User();

    Q_INVOKABLE QString getNickname();
    Q_INVOKABLE QString getProfileText();
    Q_INVOKABLE void setProfileText(QString);
    Q_INVOKABLE int getFriendListSize();
    Q_INVOKABLE QString getFriendName(int index);
    Q_INVOKABLE QString getFriendProfile(int index);

    void addFriend(QString, QString);

private:
    QString nickname;
    QString profile_text;
    QVector<UserFriend> friend_list;
//    QVector<ChatRoom> chatroom_list;


};

#endif // USER_H
