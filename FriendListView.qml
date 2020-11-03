import QtQuick 2.13
import QtQuick.Controls 2.13

Item {

    Component.onCompleted: {
        for(var i = 0; i < mcuser.getFriendListSize(); i++) {
            listview_friendlist.model.append({ "friendlist_nickname": mcuser.getFriendName(i),
                                                 "friendlist_profile": mcuser.getFriendProfile(i) })
        }
    }

    Component {
        id: friendlistDelegate

        Item {
            width: 500
            height: 100

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

            Rectangle {
                id: friend_box
                width: parent.width
                anchors.bottom: parent.bottom
                height: 1
                color: "black"
            }

            MouseArea {
                id: mousearea_friend
                anchors.fill: parent

                onClicked: {
                    listview_friendlist.currentIndex = index
                }
            }
        }
    }

    ListView {
        id: listview_friendlist
        anchors.fill: parent
        model: ListModel{}
//        model: mcuser.friend_list   고려해볼것
        delegate: friendlistDelegate
        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
        highlightFollowsCurrentItem: true
        highlightMoveDuration: 20
        highlightMoveVelocity: 2000
        focus: true
    }
}
