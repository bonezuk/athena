import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import uk.co.blackomega 1.0

import "playlist.js" as Playlist

ApplicationWindow {
    id: window
    visible: true
    title: "Black Omega Playlist Manager"

    toolBar: ToolBar {
        id: toolBarId

        Rectangle {
            id: playControlContainer
            
            color: "transparent"
            width: playButtonId.width * 2
            height: playButtonId.height
            anchors.left: parent.left
            anchors.leftMargin: 1
            anchors.verticalCenter: parent.verticalCenter

            Image {
                id: playButtonId
                height: 45
                anchors.verticalCenter: playControlContainer.verticalCenter
                anchors.horizontalCenter: playControlContainer.horizontalCenter
                fillMode: Image.PreserveAspectFit
                source: (playbackState.playbackState === PlaybackState.Play) ? "images/pc_play_normal.png" : "images/pc_pause_normal.png";
            }
            Image {
                id: backButtonId
                height: 30
                anchors.verticalCenter: playButtonId.verticalCenter
                anchors.right: playButtonId.horizontalCenter
                anchors.rightMargin: 15
                fillMode: Image.PreserveAspectFit
                source: "images/pc_back_normal.png"
            }
            Image {
                id: forwardButtonId
                height: 30
                anchors.verticalCenter: playButtonId.verticalCenter
                anchors.left: playButtonId.horizontalCenter
                anchors.leftMargin: 15
                fillMode: Image.PreserveAspectFit
                source: "images/pc_forward_normal.png"
            }
        }

        Rectangle {
        	id: playbackTimeContainer
            objectName: "playbackTimeContainer"
        
            color: "transparent"
            implicitWidth: digitLayoutId.width
            implicitHeight: digitLayoutId.height
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter

            Row {
                id: digitLayoutId
                spacing: 1
                Image {
                    id: digitHourUpper
                    source: Playlist.getPlayTimeDigitImageName(Playlist.PlayTimeHourUpperDigit, playbackState.playbackTimeInSeconds)
                }
                Image {
                    id: digitHourLower
                    source: Playlist.getPlayTimeDigitImageName(Playlist.PlayTimeHourLowerDigit, playbackState.playbackTimeInSeconds)
                }
                Image {
                    source: "images/colon@2x.png"
                }
                Image {
                    id: digitMinuteUpper
                    source: Playlist.getPlayTimeDigitImageName(Playlist.PlayTimeMinuteUpperDigit, playbackState.playbackTimeInSeconds)
                }
                Image {
                    id: digitMinuteLower
                    source: Playlist.getPlayTimeDigitImageName(Playlist.PlayTimeMinuteLowerDigit, playbackState.playbackTimeInSeconds)
                }
                Image {
                    source: "images/colon@2x.png"
                }
                Image {
                    id: digitSecondUpper
                    source: Playlist.getPlayTimeDigitImageName(Playlist.PlayTimeSecondUpperDigit, playbackState.playbackTimeInSeconds)
                }
                Image {
                    id: digitSecondLower
                    source: Playlist.getPlayTimeDigitImageName(Playlist.PlayTimeSecondLowerDigit, playbackState.playbackTimeInSeconds)
                }
            }
        }
    }

	ListView {
		id: playListView

        anchors.fill: parent

        Layout.minimumWidth: 400
        Layout.minimumHeight: 240
        Layout.preferredWidth: 600
        Layout.preferredHeight: 400
        
        model: playListModel

        delegate: Rectangle {
            color: (index === playbackState.playbackIndex) ? "green" : "white"
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
