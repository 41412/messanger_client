import QtQuick 2.13
import QtQuick.Controls 2.13

Item {

    Component.onCompleted: {
        for(var i = 0; i < user.getFriendListSize(); i++) {
            listview_friendlist.model.append({ "friendlist_nickname": user.getFriendName(i),
                                                 "friendlist_profile": user.getFriendProfile(i) })
        }
    }

    Component {
        id: friendlistDelegate

        Item {
            width: 500
            height: 100

            Rectangle {
                id: friend_box
                width: parent.width
                height: parent.height
                color: "white"
            }

            Text {
                id: text_friend_nickname
                y: 0
                anchors.left: parent.left
                anchors.leftMargin: 25
                text: friendlist_nickname
            }

            Text {
                id: text_friend_profile
                y: 50
                anchors.left: parent.left
                anchors.leftMargin: 25
                text: friendlist_profile
            }
        }
    }

    ListView {
        id: listview_friendlist
        anchors.fill: parent
        model: ListModel{}
        delegate: friendlistDelegate
        highlight: Rectangle { color: "yellow"; radius: 5 }
        focus: true
    }
}
