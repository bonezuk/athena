import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import uk.co.blackomega 1.0

import "playlist.js" as Playlist

Window {
    id: window
    visible: true

    ColumnLayout {
        anchors.fill: parent

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 100

            Rectangle {
                id: playControlContainer

                color: "transparent"
                implicitWidth: playButtonId.width * 2
                implicitHeight: parent.height
                anchors.left: parent.left
                anchors.leftMargin: 1
                anchors.verticalCenter: parent.verticalCenter

                Image {
                    id: playButtonId
                    height: parent.height
                    anchors.verticalCenter: playControlContainer.verticalCenter
                    anchors.horizontalCenter: playControlContainer.horizontalCenter
                    fillMode: Image.PreserveAspectFit
                    source: (!playbackStateController.state) ? "images/pc_play_normal.png" : "images/pc_pause_normal.png"

                    MouseArea {
                        anchors.fill: parent
                        onClicked: playListModel.onPlayPausePressed()
                    }
                }
/*
                Image {
                    id: backButtonId
                    anchors.verticalCenter: playButtonId.verticalCenter
                    anchors.right: playButtonId.horizontalCenter
                    anchors.rightMargin: 15
                    fillMode: Image.PreserveAspectFit
                    source: "images/pc_back_normal.png"
                }

                Image {
                    id: forwardButtonId
                    anchors.verticalCenter: playButtonId.verticalCenter
                    anchors.left: playButtonId.horizontalCenter
                    anchors.leftMargin: 15
                    fillMode: Image.PreserveAspectFit
                    source: "images/pc_forward_normal.png"
                }
*/
            }


            Rectangle {
                color: "transparent"

                implicitWidth: 300
                implicitHeight: parent.height / 2

                anchors.right: parent.right
                anchors.rightMargin: 5
                anchors.verticalCenter: parent.verticalCenter

                RowLayout {
                    anchors.fill: parent
                    spacing: 1
                    implicitWidth: parent.implicitWidth

                    Rectangle {
                        color: "transparent"
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.preferredWidth: parent.width / 7

                        Image {
                            height: parent.height
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: timeContainer.horizontalCenter
                            fillMode: Image.PreserveAspectFit
                            source: Playlist.getPlayTimeDigitImageName(Playlist.PlayTimeHourUpperDigit, playbackStateController.timeInSeconds)
                        }
                    }
                    Rectangle {
                        color: "transparent"
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.preferredWidth: parent.width / 7

                        Image {
                            height: parent.height
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: timeContainer.horizontalCenter
                            fillMode: Image.PreserveAspectFit
                            source: Playlist.getPlayTimeDigitImageName(Playlist.PlayTimeHourLowerDigit, playbackStateController.timeInSeconds)
                        }
                    }
                    Rectangle {
                        color: "transparent"
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.preferredWidth: parent.width / 14

                        Image {
                            height: parent.height
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: timeContainer.horizontalCenter
                            fillMode: Image.PreserveAspectFit
                            source: "images/digit_colon.png"
                        }
                    }
                    Rectangle {
                        color: "transparent"
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.preferredWidth: parent.width / 7

                        Image {
                            height: parent.height
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: timeContainer.horizontalCenter
                            fillMode: Image.PreserveAspectFit
                            source: Playlist.getPlayTimeDigitImageName(Playlist.PlayTimeMinuteUpperDigit, playbackStateController.timeInSeconds)
                        }
                    }
                    Rectangle {
                        color: "transparent"
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.preferredWidth: parent.width / 7

                        Image {
                            height: parent.height
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: timeContainer.horizontalCenter
                            fillMode: Image.PreserveAspectFit
                            source: Playlist.getPlayTimeDigitImageName(Playlist.PlayTimeMinuteLowerDigit, playbackStateController.timeInSeconds)
                        }
                    }
                    Rectangle {
                        color: "transparent"
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.preferredWidth: parent.width / 14

                        Image {
                            height: parent.height
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: timeContainer.horizontalCenter
                            fillMode: Image.PreserveAspectFit
                            source: "images/digit_colon.png"
                        }
                    }
                    Rectangle {
                        color: "transparent"
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.preferredWidth: parent.width / 7

                        Image {
                            height: parent.height
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: timeContainer.horizontalCenter
                            fillMode: Image.PreserveAspectFit
                            source: Playlist.getPlayTimeDigitImageName(Playlist.PlayTimeSecondUpperDigit, playbackStateController.timeInSeconds)
                        }
                    }
                    Rectangle {
                        color: "transparent"
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.preferredWidth: parent.width / 7

                        Image {
                            height: parent.height
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: timeContainer.horizontalCenter
                            fillMode: Image.PreserveAspectFit
                            source: Playlist.getPlayTimeDigitImageName(Playlist.PlayTimeSecondLowerDigit, playbackStateController.timeInSeconds)
                        }
                    }
                }
            }


        }


        ListView {
            id: playListView

            Layout.fillWidth: true
            Layout.fillHeight: true

            model: playListModel

            delegate: Rectangle {
                color: (index === playbackStateController.index) ? "green" : "white"
                width: parent.width; height: 30
                Text {
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 20
                    text: title + " (" + artist + ") [" + album + "]"
                }
                MouseArea {
                    anchors.fill: parent
                    onDoubleClicked: {
                        console.log(index + " " + title);
                        playListModel.playItemAtIndex(index);
                    }
                }
            }
        }

    }


}
