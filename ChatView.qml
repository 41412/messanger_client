import QtQuick 2.13
import QtQuick.Controls 2.13

Item {
    property alias chatmodel: listview_chatlist.model
    Component {
        id: chatlistDelegate

        Item {
            width: 500
            height: 100

            Text {
                id: text_sender
                x: 0
                y: 0
                height: 40
                anchors.left: parent.left
                anchors.leftMargin: 20
                anchors.rightMargin: 100
                text: sender
            }

            Text {
                id: text_message
                x: 0
                y: 50
                width: 400
                height: 40
                anchors.left: parent.left
                anchors.leftMargin: 20
                anchors.rightMargin: 100
                text: message
            }

            Text {
                id: text_time
                x: 400
                y: 50
                width: 100
                height: 40
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.rightMargin: 5
                text: time
            }

            MouseArea {
                id: mousearea_chat
                anchors.fill: parent

                onClicked: {
                    console.log("index" + index)
                    listview_chatlist.currentIndex = index
                }
            }
        }
    }

    ListView {
        id: listview_chatlist
        x: 0
        y: 0
        width: parent.width
        height: 540
        model: chatmodel
        delegate: chatlistDelegate
        focus: true
    }

    Item {
        x: 0
        y: 540
        width: parent.width
        height: 60

        Rectangle {
            anchors.fill: parent
            color: "black"
        }

        TextField {
            id: textfield_input_message
            width: parent.width
            height: parent.height
            anchors.leftMargin: 15
            anchors.topMargin: 10
            placeholderText: "Input Chat Message"
        }
    }
}
