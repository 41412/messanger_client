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

//void User::setProfileText(QString text)
//{
//    profile_text = text;
//}

int User::getFriendListSize()
{
    return friend_list.size();
}

QString User::getFriendName(int index)
{
    return friend_list.at(index).nickname;
}

QString User::getFriendProfile(int index)
{
    return friend_list.at(index).profile;
}

void User::addFriend(QString username, QString profile)
{
    struct UserFriend f = {username, profile};
    friend_list.push_back(f);
}
