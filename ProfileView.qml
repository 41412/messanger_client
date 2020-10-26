import QtQuick 2.13
import QtQuick.Controls 2.13

Item {
    Text {
        text: "ProfileView"
    }

    Button {
        id: button_logout

        x: 400
        y: 0
        width: 100
        height: 100
        text: "로그아웃"

        onClicked: {
            messangerclient.clientDisconnect()
        }
    }
}
