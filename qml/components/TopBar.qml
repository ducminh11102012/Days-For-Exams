import QtQuick 2.15
import QtQuick.Controls 2.15
import "../styles"

Rectangle {
    id: root
    height: 56
    color: Theme.surface

    property string studentName: ""
    property string examDate: ""

    Text {
        anchors.left: parent.left
        anchors.leftMargin: 24
        anchors.verticalCenter: parent.verticalCenter
        color: Theme.text
        font.family: "Segoe UI"
        font.pixelSize: 24
        text: "Day For Exams"
    }

    Text {
        anchors.centerIn: parent
        color: Theme.subtext
        font.family: "Segoe UI"
        font.pixelSize: 16
        text: "Student: " + root.studentName + "  •  Exam: " + root.examDate
    }

    Text {
        id: clockText
        anchors.right: parent.right
        anchors.rightMargin: 24
        anchors.verticalCenter: parent.verticalCenter
        color: Theme.text
        font.family: "Segoe UI"
        font.pixelSize: 20
        text: Qt.formatTime(new Date(), "hh:mm:ss")
    }

    Timer {
        interval: 1000
        running: true
        repeat: true
        onTriggered: clockText.text = Qt.formatTime(new Date(), "hh:mm:ss")
    }
}
