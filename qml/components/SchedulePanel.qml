import QtQuick 2.15
import QtQuick.Controls 2.15
import "../styles"

Rectangle {
    id: root
    color: Theme.background
    radius: 12

    property var scheduleModel: []

    Column {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 14

        Text {
            text: "Today's Study Roadmap"
            color: Theme.text
            font.family: "Segoe UI"
            font.pixelSize: 30
        }

        ListView {
            id: scheduleList
            model: root.scheduleModel
            clip: true
            spacing: 10
            boundsBehavior: Flickable.StopAtBounds
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: 10
            anchors.bottom: parent.bottom
            anchors.top: previous.bottom

            delegate: Rectangle {
                width: scheduleList.width
                height: 76
                radius: 10
                color: index === 0 ? Theme.accent : Theme.surface

                Row {
                    anchors.fill: parent
                    anchors.margins: 14
                    spacing: 12

                    Text {
                        text: modelData.startsAt + " → " + modelData.endsAt
                        color: Theme.text
                        font.family: "Segoe UI"
                        font.pixelSize: 15
                        width: 240
                        elide: Text.ElideRight
                    }

                    Text {
                        text: modelData.subject
                        color: Theme.text
                        font.family: "Segoe UI"
                        font.bold: true
                        font.pixelSize: 16
                        width: 160
                        elide: Text.ElideRight
                    }

                    Text {
                        text: modelData.task
                        color: Theme.subtext
                        font.family: "Segoe UI"
                        font.pixelSize: 15
                        elide: Text.ElideRight
                    }
                }
            }
        }
    }
}
