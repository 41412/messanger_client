#ifndef USERFRIEND_H
#define USERFRIEND_H

#include <QObject>

class mcUserFriend
{
public:
    explicit mcUserFriend(const QString &nickname, const QString &profile);
    virtual ~mcUserFriend();
//![0]

    QString nickname() const;
    QString profile() const;

private:
    QString m_nickname;
    QString m_profile;
};

#endif // USERFRIEND_H
