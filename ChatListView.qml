import QtQuick 2.13
import QtQuick.Controls 2.13

Item {

    Connections {
        target: messangerclient

        onResEnterChatRoom: {
            currentState = "CHAT_VIEW"
            // TODO: model 변경
        }
    }

    Component {
        id: chatroomlistDelegate

        Item {
            width: 500
            height: 100

            property var chatroomid: roomid

            Text {
                id: text_chatroom_name
                y: 0
                anchors.left: parent.left
                anchors.leftMargin: 25
                text: roomid                // Test
//                text: roomname
            }

//            Text {
//                id: text_chatroom_??
//                y: 50
//                anchors.left: parent.left
//                anchors.leftMargin: 25
//                text: ??
//            }

            Rectangle {
                id: chatroom_box
                width: parent.width
                anchors.bottom: parent.bottom
                height: 1
                color: "black"
            }

            MouseArea {
                id: mousearea_chatroom
                anchors.fill: parent

                onClicked: {
                    console.log("index" + index)
                    listview_chatroomlist.currentIndex = index
                }

                onDoubleClicked: {
                    messangerclient.requestEnterChatRoom(chatroomid)
                }
            }
        }
    }

    ListView {
        id: listview_chatroomlist
        anchors.fill: parent
        model: chatroommodel
        delegate: chatroomlistDelegate
        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
        highlightFollowsCurrentItem: true
        highlightMoveDuration: 20
        highlightMoveVelocity: 2000
        focus: true
    }
}
