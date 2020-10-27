#ifndef USER_H
#define USER_H

#include <QObject>
#include <QQmlEngine>

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
    void addFriend(QString);
//    void deleteFriend(QString);

private:
    QString nickname;
    QString profile_text;
    QVector<QString> friend_list;
//    QVector<ChatRoom> chatroom_list;


};

#endif // USER_H
