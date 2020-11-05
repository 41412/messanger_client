#ifndef MCUSER_H
#define MCUSER_H

#include <QObject>
#include <QQmlEngine>

struct UserFriend {
    UserFriend(QString nickname, QString profile) : nickname(nickname), profile(profile) {}
    QString nickname;
    QString profile;
};

class McUser : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString nickname READ getNickname WRITE setNickname NOTIFY nameChanged)
    Q_PROPERTY(QString profile_text READ getProfileText WRITE setProfileText NOTIFY profileChanged)

public:
    explicit McUser();
    explicit McUser(QString _nickname);
    explicit McUser(QString _nickname, QString _profile_text);
    virtual ~McUser();

    Q_INVOKABLE int getFriendListSize();
    Q_INVOKABLE QString getFriendName(int index);
    Q_INVOKABLE QString getFriendProfile(int index);

    QString getNickname() { return nickname; }
    void setNickname(QString text) { nickname = text; }
    QString getProfileText() { return profile_text; }
    void setProfileText(QString text) { profile_text = text; }
    void addFriend(QString, QString);

signals:
    void nameChanged();
    void profileChanged();

private:
    QString nickname;
    QString profile_text;
    QVector<UserFriend> friend_list;
//    QVector<ChatRoom> chatroom_list;


};

#endif // MCUSER_H
