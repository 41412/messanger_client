#include "McUser.h"

McUser::McUser()
{

}

McUser::McUser(QString _nickname) :
    nickname(_nickname)
{

}

McUser::McUser(QString _nickname, QString _profile_text) :
    nickname(_nickname), profile_text(_profile_text)
{

}

McUser::~McUser()
{

}

int McUser::getFriendListSize()
{
    return friend_list.size();
}

QString McUser::getFriendName(int index)
{
    return friend_list.at(index).nickname;
}

QString McUser::getFriendProfile(int index)
{
    return friend_list.at(index).profile;
}

void McUser::addFriend(QString username, QString profile)
{
    UserFriend f = {username, profile};
    friend_list.append(f);
}
