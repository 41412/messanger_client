#ifndef MCUSER_H
#define MCUSER_H

#include <QObject>
#include <QQmlEngine>
#include <mcUserFriend.h>
#include <mcFriendModel.h>
#include <mcChatroom.h>
#include <mcChatroomModel.h>

class McUser : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString nickname READ getNickname WRITE setNickname NOTIFY nameChanged)
    Q_PROPERTY(QString profile_text READ getProfileText WRITE setProfileText NOTIFY profileChanged)
    // Idea : Q_PROPERTY로 chatmodel을 읽고, 변경을 감지하는 것을 만들어보면?

public:
    explicit McUser();
    virtual ~McUser();

    QString getNickname() { return nickname; }
    void setNickname(QString text) { nickname = text; }
    QString getProfileText() { return profile_text; }
    void setProfileText(QString text) { profile_text = text; }
    mcFriendModel* getFriendModel();
    mcChatroomModel* getChatroomModel();
    mcChatModel* getChatModel();                // Test
//    mcChatroom findChatroom(QString roomid);

signals:
    void nameChanged();
    void profileChanged();

private:
    QString nickname;
    QString profile_text;
    mcFriendModel friendmodel;
    mcChatroomModel* chatroommodel;

};

#endif // MCUSER_H
