#include "McUser.h"

McUser::McUser() : chatroommodel(new mcChatroomModel())
{

}

McUser::~McUser()
{

}

mcFriendModel* McUser::getFriendModel()
{
    return &friendmodel;
}

mcChatroomModel* McUser::getChatroomModel()
{
    return chatroommodel;
}

//mcChatModel* McUser::getChatModel()
//{


//}
//mcChatroom McUser::findChatroom(QString roomid)
//{
//    QList<mcChatroom> roomlist = chatroommodel->getChatroomList();
//    // TODO : 찾는 방이 없을때 새로 만들어서 리턴해줘야함.

//}
