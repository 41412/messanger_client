#ifndef MCCHAT_H
#define MCCHAT_H

#include <QObject>

class mcChat
{
public:
    explicit mcChat(const int &index, const QString &time, const QString &sender, const QString &chat);
    virtual ~mcChat();

    int index() const;
    QString time() const;
    QString sender() const;
    QString chat() const;

private:
    int m_index;
    QString m_time;
    QString m_sender;
    QString m_chat;

};

#endif // MCCHAT_H
