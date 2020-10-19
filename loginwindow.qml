import QtQuick 2.13
import QtQuick.Controls 2.13
import MessangerClient 1.0

Item {
    x: 0
    y: 0
    width: 500
    height: 700

    property var hostIP: "127.0.0.1"
    property real portNumber: 1024

    function resetSubmitWindow() {
        textfield_submit_nickname.text = "";
        textfield_submit_password.text = "";
        textfield_submit_confirm.text = "";
        text_error_submitwindow.text = "";
    }

    MessangerClient {
        id: messangerclient
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
            echoMode: TextInput.Password
        }

        TextField {
            id: textfield_submit_confirm
            x: (parent.x + parent.width) * 0.25
            y: (parent.y + parent.height) * 0.50
            width: (parent.x + parent.width)  * 0.60
            height: (parent.y + parent.height) * 0.08
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
                if (textfield_submit_nickname.text.length < 1 ||
                        textfield_submit_password.text.length < 1 ||
                        textfield_submit_confirm.text.length < 1) {
                    text_error_submitwindow.text = "Nickname or Password is empty."
                }
                else if(textfield_submit_password.length < 4) {
                    text_error_submitwindow.text = "Password must be more than 4 letters."
                }
                else if(textfield_submit_password.text != textfield_submit_confirm.text) {
                    text_error_submitwindow.text = "Both of password are NOT same."
                }
                else
                {
                    messangerclient.requestSubmit(textfield_nickname.text, textfield_password.text)

                    resetSubmitWindow()
                    popup_submit.close()
//                    var result = messangerclient.requestSubmit(textfield_nickname.text, textfield_password.text)
//                    if(result == error) {
//                        text_error_submitwindow.text = "Nickname is already used."
//                    }
//                    else {
//                    }
                }
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
            if (textfield_nickname.text.length < 1 ||
                    textfield_password.text.length < 1 ||
                    textfield_confirm.text.length < 1) {
                text_error_submitwindow.text = "Nickname or Password is empty."
            }
            else if(textfield_submit_password.length < 4) {
                text_error_submitwindow.text = "Password must be more than 4 letters."
            }
            else if(textfield_submit_password.text != textfield_submit_confirm.text) {
                text_error_submitwindow.text = "Both of password are NOT same."
            }
            messangerclient.connectToHost(hostIP, portNumber)
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
        }
    }
}
