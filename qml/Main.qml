import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "components"
import "styles"

ApplicationWindow {
    id: root
    visible: true
    visibility: Window.FullScreen
    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint
    color: Theme.background
    title: "Day For Exams"

    Component.onCompleted: {
        // Reduce accidental context menu or drag behavior.
        root.requestActivate()
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        TopBar {
            Layout.fillWidth: true
            studentName: backend.studentName
            examDate: backend.examDate
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: Theme.background

            RowLayout {
                anchors.fill: parent
                anchors.margins: 14
                spacing: 14

                Rectangle {
                    Layout.preferredWidth: 330
                    Layout.fillHeight: true
                    radius: 12
                    color: Theme.surface

                    Column {
                        anchors.fill: parent
                        anchors.margins: 18
                        spacing: 12

                        Text {
                            text: "Session"
                            color: Theme.text
                            font.family: "Segoe UI"
                            font.pixelSize: 30
                        }
                        Text {
                            text: "Daily end: " + backend.dailyEndTime
                            color: Theme.subtext
                            font.family: "Segoe UI"
                            font.pixelSize: 16
                        }
                        Text {
                            text: "Allowed Apps"
                            color: Theme.text
                            font.family: "Segoe UI"
                            font.pixelSize: 18
                        }

                        ListView {
                            model: backend.allowedApps()
                            clip: true
                            spacing: 8
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.bottom: parent.bottom
                            anchors.top: previous.bottom

                            delegate: Rectangle {
                                width: parent.width
                                height: 42
                                radius: 8
                                color: "#232323"

                                Text {
                                    anchors.centerIn: parent
                                    text: modelData
                                    color: Theme.text
                                    font.family: "Segoe UI"
                                    font.pixelSize: 14
                                }
                            }
                        }
                    }
                }

                SchedulePanel {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    scheduleModel: backend.scheduleModel()
                }
            }
        }

        DockBar {
            Layout.fillWidth: true
            Layout.leftMargin: 18
            Layout.rightMargin: 18
            Layout.bottomMargin: 8
            appList: backend.allowedApps()
            onLaunchRequested: backend.launchApp(appName)
        }
    }

    Shortcut {
        sequence: "Esc"
        onActivated: Qt.quit()
    }
}
