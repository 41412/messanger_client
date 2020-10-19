import QtQuick 2.13
import QtQuick.Window 2.13

Window {
    visible: true
    width: 500
    height: 700
    title: qsTr("Messanger")

    Item {
        x: 20
        y: 20
        width: 460
        height: 660

        LoginWindow {
            id: loginwindow
            anchors.fill: parent
        }
    }
}
