#include "user.h"

User::User()
{
    qmlRegisterType<User>("User", 1, 0, "User");
}

User::User(QString _nickname) :
    nickname(_nickname)
{

}

User::User(QString _nickname, QString _profile_text) :
    nickname(_nickname), profile_text(_profile_text)
{

}

User::~User()
{

}

QString User::getNickname()
{
    return nickname;
}

QString User::getProfileText()
{
    return profile_text;
}

void User::setProfileText(QString text)
{
    this->profile_text = text;
}

void User::addFriend(QString username)
{
    this->friend_list.push_back(username);
}

//void User::deleteFriend(QString username)
//{
//    int index =
//}
