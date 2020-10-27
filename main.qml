import QtQuick 2.13
import QtQuick.Window 2.13
import MessangerClient 1.0
import User 1.0

Window {
    visible: true
    width: 500
    height: 700
    title: qsTr("Messanger")

    property string currentState: ""

    Component.onCompleted: {
        currentState = "LOGIN_VIEW"

        loginwindow.visible = Qt.binding(function() { return currentState === "LOGIN_VIEW" })
        navigationtab.visible = Qt.binding(function() { return currentState !== "LOGIN_VIEW" })
        profileview.visible = Qt.binding(function() { return currentState !== "LOGIN_VIEW" && currentState !== "CHAT_VIEW" })
        chatlistview.visible = Qt.binding(function() { return currentState === "CHATLIST_VIEW" })
        friendlistview.visible = Qt.binding(function() { return currentState === "FRIENDLIST_VIEW" })
        chatview.visible = Qt.binding(function() { return currentState === "CHAT_VIEW" })
    }

    onCurrentStateChanged: {
        console.log("[Debug] currentState : " + currentState)
    }

    Connections {
        target: messangerclient
        onDisconnected: {
            currentState = "LOGIN_VIEW"
        }
    }

    MessangerClient {
        id: messangerclient
    }

    User {
        id: user
    }

    LoginWindow {
        id: loginwindow
        x: 0
        y: 0
        width: 500
        height: 700
    }

    NavigationTab {
        id: navigationtab
        x: 0
        y: 600
        width: 500
        height: 100
    }

    ProfileView {
        id: profileview
        x: 0
        y: 0
        width: 500
        height: 100
    }

    ChatListView {
        id: chatlistview
        x: 0
        y: 100
        width: 500
        height: 500
    }

    FriendListView {
        id: friendlistview
        x: 0
        y: 100
        width: 500
        height: 500
    }

    ChatView {
        id: chatview
        x: 0
        y: 0
        width: 500
        height: 600
    }
}
