import QtQuick 2.13
import QtQuick.Controls 2.13

Item {

    property alias nickname: text_nickname.text
    property alias introduce: text_introduce.text

    Rectangle {
        anchors.fill: parent
        color: "cornsilk"
    }

    Text {
        id: text_nickname
        x: 25
        y: 10
        font.pixelSize: 15
        text: mcuser.nickname
    }

    Text {
        id: text_introduce
        x: 25
        y: 60
        font.pixelSize: 12
        text: "Introducing Message"
    }

    Button {
        id: button_logout

        x: 400
        y: 0
        width: 100
        height: 100
        text: "로그아웃"

        onClicked: {
//            messangerclient.clientDisconnect()
        }
    }
}
