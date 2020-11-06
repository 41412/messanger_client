#include "mcUserFriend.h"

mcUserFriend::mcUserFriend(const QString &nickname, const QString &profile)
    : m_nickname(nickname), m_profile(profile)
{

}
mcUserFriend::~mcUserFriend()
{

}

QString mcUserFriend::nickname() const
{
    return m_nickname;
}

QString mcUserFriend::profile() const
{
    return m_profile;
}
