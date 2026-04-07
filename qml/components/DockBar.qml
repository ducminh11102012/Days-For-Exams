import QtQuick 2.15
import QtQuick.Controls 2.15
import "../styles"

Rectangle {
    id: root
    color: Theme.surface
    radius: 14
    height: 84

    property var appList: []

    signal launchRequested(string appName)

    Row {
        anchors.centerIn: parent
        spacing: 16

        Repeater {
            model: root.appList

            delegate: Rectangle {
                width: 132
                height: 52
                radius: 10
                color: mouse.containsMouse ? Theme.accent : "#262626"
                scale: mouse.containsMouse ? 1.04 : 1.0

                Behavior on scale {
                    NumberAnimation { duration: 150 }
                }

                Text {
                    anchors.centerIn: parent
                    text: modelData
                    color: Theme.text
                    font.family: "Segoe UI"
                    font.pixelSize: 14
                    elide: Text.ElideRight
                }

                MouseArea {
                    id: mouse
                    anchors.fill: parent
                    hoverEnabled: true
                    acceptedButtons: Qt.LeftButton
                    onClicked: root.launchRequested(modelData)
                }
            }
        }
    }
}
