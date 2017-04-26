import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import Server 1.0
ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    Row {
        height: 100
        width: parent.width

        TextField {
            id: textField
            implicitWidth: parent.width - startServerButton.width
        }
        Button {
            id: startServerButton
            text: "START SERVER"
            onClicked:  {
                server.startServer()
            }
        }
    }

    Button {
        anchors.centerIn: parent
        onClicked: server.sendMessage("PING")
    }

    Server {
        id: server
        serverName: textField.text
    }

}
