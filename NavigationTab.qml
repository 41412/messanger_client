import QtQuick 2.13
import QtQuick.Controls 2.13

Item {

    Button {
        id: button_friendlist
        x: 0
        y: 0
        width: 250
        height: 100
        text: "FriendListView"

        onClicked: {
            currentState = "FRIENDLIST_VIEW"
        }
    }

    Button {
        id: button_chatlist
        x: 250
        y: 0
        width: 250
        height: 100
        text: "ChatListView"

        onClicked: {
            currentState = "CHATLIST_VIEW"
        }
    }
}
