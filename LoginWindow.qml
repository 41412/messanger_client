import QtQuick 2.13
import QtQuick.Controls 2.13
import MessangerClient 1.0

Item {
    id: item_loginwindow
    x: 0
    y: 0
    width: 500
    height: 700

    property var hostIP: "192.168.10.200"
    property real portNumber: 35000

    function eraseLoginInfo() {
        textfield_nickname.text = ""
        textfield_password.text = ""
    }

    function resetSubmitWindow() {
        textfield_submit_nickname.text = ""
        textfield_submit_password.text = ""
        textfield_submit_confirm.text = ""
        text_error_submitwindow.text = ""
    }

//    MessangerClient {
//        id: messangerclient
//    }

    Connections {
        target: messangerclient
        onResLogin: {
            if(protocol === "LOGIN_SUCCESS") {
                // TODO : change view from Loginview to Chatlistview
//                messangerclient.loadingUserData(textfield_nickname.text)
                messangerclient.requestUserData(textfield_nickname.text)
            }
            else if(protocol === "LOGIN_FAIL") {
                text_error_loginwindow.text = data
                messangerclient.clientDisconnect()
            }
            eraseLoginInfo()
        }

        onResSubmit: {
            if(protocol === "SUBMIT_SUCCESS") {
                resetSubmitWindow()
                popup_submit_success.open()
            }
            else if(protocol === "SUBMIT_FAIL") {
                text_error_submitwindow.text = data
                messangerclient.clientDisconnect()
            }
        }

        onLoginCompleted: {
            currentState = "CHATLIST_VIEW"
        }
    }

// Popup (Submit Window) start
    Popup {
        id: popup_submit
        width: 400
        height: 600
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        Text {
            x: (parent.x + parent.width) * 0.05
            y: (parent.y + parent.height) * 0.35
            text: "Nickname"
        }

        Text {
            x: (parent.x + parent.width) * 0.05
            y: (parent.y + parent.height) * 0.45
            text: "Password"
        }

        Text {
            x: (parent.x + parent.width) * 0.05
            y: (parent.y + parent.height) * 0.525
            text: "Confirm"
        }

        Text {
            id: text_error_submitwindow
            x: (parent.x + parent.width) * 0.05
            y: (parent.y + parent.height) * 0.60
            color: "red"
            text: ""
        }

        TextField {
            id: textfield_submit_nickname
            x: (parent.x + parent.width) * 0.25
            y: (parent.y + parent.height) * 0.325
            width: (parent.x + parent.width)  * 0.60
            height: (parent.y + parent.height) * 0.08
        }

        TextField {
            id: textfield_submit_password
            x: (parent.x + parent.width) * 0.25
            y: (parent.y + parent.height) * 0.425
            width: (parent.x + parent.width)  * 0.60
            height: (parent.y + parent.height) * 0.08
            placeholderText: "more than 4 letters required."
            echoMode: TextInput.Password
        }

        TextField {
            id: textfield_submit_confirm
            x: (parent.x + parent.width) * 0.25
            y: (parent.y + parent.height) * 0.50
            width: (parent.x + parent.width)  * 0.60
            height: (parent.y + parent.height) * 0.08
            placeholderText: "please repeat password."
            echoMode: TextInput.Password
        }

        Button {
            id: button_submit

            x: (parent.x + parent.width) * 0.15
            y: (parent.y + parent.height) * 0.75
            width: (parent.x + parent.width) * 0.25
            height: (parent.y + parent.height) * 0.10
            text: "등록"

            onClicked: {
                messangerclient.connectToHost(hostIP, portNumber)
                messangerclient.requestSubmit(textfield_submit_nickname.text, textfield_submit_password.text, textfield_submit_confirm.text)
            }
        }

        Button {
            id: button_cancel

            x: (parent.x + parent.width) * 0.60
            y: (parent.y + parent.height) * 0.75
            width: (parent.x + parent.width) * 0.25
            height: (parent.y + parent.height) * 0.10
            text: "취소"

            onClicked: {
                resetSubmitWindow()
                popup_submit.close()
            }
        }

    }
// Popup (Submit Window) end

// Popup (Submit Success Window)
    Popup {
        id: popup_submit_success
        width: 250
        height: 150
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        Text {
            x: 25
            y: 50
            text: "Submit Success !"
        }

        Button {
            id: button_confirm_submitsuccess
            x: 80
            y: 80
            width: 90
            height: 50
            text: "확인"

            onClicked: {
                popup_submit.close()
                popup_submit_success.close()
                messangerclient.clientDisconnect()
            }
        }
    }

// Popup (Submit Success Window) end

    Text {
        x: 25
        y: 300
        text: "Nickname"
    }

    Text {
        x: 25
        y: 360
        text: "Password"
    }

    Text {
        id: text_error_loginwindow
        x: 125
        y: 410
        color: "red"
        text: ""
    }


    TextField {
        id: textfield_nickname
        x: 125
        y: 280
        width: 300
        height: 60
    }

    TextField {
        id: textfield_password
        x: 125
        y: 340
        width: 300
        height: 60
        echoMode: TextInput.Password
    }

    Button {
        id: button_login

        x: 50
        y: 500
        width: 130
        height: 70
        text: "로그인"

        onClicked: {
            messangerclient.connectToHost(hostIP, portNumber)
            messangerclient.requestLogin(textfield_nickname.text, textfield_password.text)
        }
    }

    Button {
        id: button_opensubmit

        x: 280
        y: 500
        width: 130
        height: 70
        text: "사용자 등록"

        onClicked: {
            popup_submit.open()
            eraseLoginInfo()
            text_error_loginwindow.text = "";
        }
    }

    Button {
        id: button_toggle_debugmenu

        x: 25
        y: 600
        width: 120
        height: 50
        text: "Debug Menu"

        onClicked: {
            item_debugmenu.visible = !(item_debugmenu.visible)
        }
    }

    Item {
        id: item_debugmenu

        Component.onCompleted: {
            this.visible = false
        }

        Button {
            id: test_connectbutton

            x: 150
            y: 600
            width: 100
            height: 50
            text:"Connect"

            onClicked: {
                if(messangerclient.connectToHost(hostIP, portNumber)) {
                    messangerclient.writeforDebugging(testfield.text)
                }
            }
        }

        Button {
            id: test_closebutton

            x: 150
            y: 650
            width: 100
            height: 50
            text:"Disconnect"

            onClicked: {
                messangerclient.clientDisconnect()
            }
        }

        Button {
            id: test_resloginsignal

            x: 260
            y: 600
            width: 100
            height: 50
            text: "Login Signal"

            onClicked: {
                messangerclient.testlogin()
            }
        }

        Button {
            id: test_ressubmitsignal

            x: 260
            y: 650
            width: 100
            height: 50
            text: "Submit Signal"

            onClicked: {
                messangerclient.testsubmit()
            }
        }

        CheckBox {
            id: checkbox_debug_localhost
            x: 350
            y: 0
            text: "Link Localhost"

            onCheckStateChanged: {
                if(checkbox_debug_localhost.checkState === Qt.Checked) {
                    hostIP = "192.168.10.194"
                }
                else if(checkbox_debug_localhost.checkState === Qt.Unchecked) {
                    hostIP = "192.168.10.200"
                }
            }
        }

//        CheckBox {
//            id: checkbox_debug_debugger
//            x: 350
//            y: 60
//            text: "Link Debugger"

//            onCheckStateChanged: {
//                if(checkbox_debug_localhost.checkState === Qt.Checked) {
//                    hostIP = "192.168.10.102"
//                }
//                else if(checkbox_debug_localhost.checkState === Qt.Unchecked) {
//                    hostIP = "192.168.10.200"
//                }
//            }
//        }
    }
}
