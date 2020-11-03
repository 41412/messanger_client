import QtQuick 2.13
import QtQuick.Window 2.13
import MessangerClient 1.0
import McUser 1.0
import McDebug 1.0

Window {
    visible: true
    width: 500
    height: 700
    title: qsTr("Messanger")

    property string currentState: "LOGIN_VIEW"

    Component.onCompleted: {

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

//    MessangerClient {
//        id: messangerclient
//    }

//    McUser {
//        id: mcuser
//    }

    McDebug {
        id: mcdebug
    }

    LoginWindow {
        id: loginwindow
        x: 0
        y: 0
        width: 500
        height: 700
        visible: currentState === "LOGIN_VIEW"
    }

    ChatView {
        id: chatview
        x: 0
        y: 0
        width: 500
        height: 600
        visible: currentState === "CHAT_VIEW"
    }

    ChatListView {
        id: chatlistview
        x: 0
        y: 100
        width: 500
        height: 500
        visible: currentState === "CHATLIST_VIEW"
    }

    FriendListView {
        id: friendlistview
        x: 0
        y: 100
        width: 500
        height: 500
        visible: currentState === "FRIENDLIST_VIEW"
    }

    ProfileView {
        id: profileview
        x: 0
        y: 0
        width: 500
        height: 100
        visible: (currentState === "CHATLIST_VIEW" || currentState === "FRIENDLIST_VIEW")
    }

    NavigationTab {
        id: navigationtab
        x: 0
        y: 600
        width: 500
        height: 100
        visible: currentState !== "LOGIN_VIEW"
    }
}
