import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.15
import uk.co.blackomega 1.0

import "components" as Component

Window {
    id: window
    visible: true
	
	width: 400
	height: 600

	StackLayout {
		id: mainArea
		currentIndex: navBar.currentNavIndex
		
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: parent.top
		anchors.bottom: navBar.top

		ListView {
			id: listview
			model: playListModel
		
	        delegate: Rectangle {
		        color: (index === playbackStateController.index) ? "green" : "white"
				width: parent.width
				height: 30
				Text {
					anchors.left: parent.left
					anchors.leftMargin: 20
					anchors.verticalCenter: parent.verticalCenter
					font.pixelSize: 20
                    text: model.title + " (" + model.artist + ")"
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
		
		StackLayout {
			id: libraryMain
			currentIndex: 0
			
			AlbumView {
				model: albumModel
				onClicked: {
					console.log(currentIndex);
					parent.currentIndex = 1;
				}
			}

			Page {
				header: Rectangle {
					width: parent.width
					height: 40
					
					gradient: Gradient {
						GradientStop {
							position: 0
							color: "#ffffff"
						}
						GradientStop {
							position: 1
							color: "#e0e0e0"
						}
					}
					
					Image {
						source: "images/back.png"
						fillMode: Image.PreserveAspectFit
						height: 30
						anchors.verticalCenter: parent.verticalCenter
						anchors.left: parent.left
						anchors.leftMargin: 10

						TapHandler {
							onTapped: {
								console.log("back");
								libraryMain.currentIndex = 0;
							}
						}
					}
				}

				AlbumTrackView {
					anchors.fill: parent
					model: albumTrackModel
					onClicked: {
						console.log("track " + currentIndex);
					}
				}

			}
		}

		Rectangle {
			anchors.fill: parent.fill
			
			Component.DigitDisplay {
				id: digitDisplay
				timeInSeconds: playbackStateController.timeInSeconds
				
				anchors.horizontalCenter: parent.horizontalCenter				
				anchors.top: parent.top
				anchors.topMargin: 10
				width: (parent.width >= 300) ? 300 : parent.width
				implicitHeight: 50
			}

            Component.PlayButton {
	            id: playButton
	            enabled: (playListModel.sizeOfModel > 0) ? true : false
	            playing: (playbackStateController.state) ? true : false
	            
            	anchors.horizontalCenter: parent.horizontalCenter
				anchors.top: digitDisplay.bottom
				anchors.topMargin: 10
				implicitWidth: 100
				implicitHeight: 100
				
				onClicked: playListModel.onPlayPausePressed();
            }
            
            ColumnLayout {
	            anchors.horizontalCenter: parent.horizontalCenter
				anchors.top: playButton.bottom
				anchors.topMargin: 10
				
        	    Text {
        	    	text: playListModel.dataAtIndex(playbackStateController.index, "title")
        	    	font.pixelSize: 20
        	    	Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
	            }
        	    Text {
        	    	text: playListModel.dataAtIndex(playbackStateController.index, "album")
        	    	font.pixelSize: 20
        	    	Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
	            }
        	    Text {
        	    	text: playListModel.dataAtIndex(playbackStateController.index, "artist")
        	    	font.pixelSize: 20
        	    	Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
	            }
            }
		}
		
		Rectangle {
			anchors.fill: parent.fill
			
			ColumnLayout {
				anchors.fill: parent.fill
				anchors.left: parent.left
				anchors.right: parent.right				
			
				Switch {
					text: "Enable FTP"
                    checked: settings.ftpServer
                    onClicked: settings.ftpServer = !settings.ftpServer
					Layout.alignment: Qt.AlignCenter
					Layout.preferredHeight: 75
				}		
				Text {
                    text: settings.ftpStatus
					Layout.alignment: Qt.AlignCenter
					Layout.preferredHeight: 75
				}
			}
		}

	}
	
	RowLayout {
		id: navBar
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom
        implicitHeight: 65
		spacing: 0
		
		property int currentNavIndex: 0

		Component.NavButton {
			navIndex: 0
			sourceEnabled: "images/album.png"
			sourceDisabled: "images/album_disable.png"
			label: "Library"

			Layout.fillWidth: true
			Layout.preferredWidth: parent.width / 4.0
			Layout.minimumHeight: parent.height
		}

		Component.NavButton {
			navIndex: 1
			sourceEnabled: "images/album.png"
			sourceDisabled: "images/album_disable.png"
			label: "Album"

			Layout.fillWidth: true
			Layout.preferredWidth: parent.width / 4.0
			Layout.minimumHeight: parent.height
		}

		Component.NavButton {
			navIndex: 2
			sourceEnabled: "images/play_icon_toolbar_256x256_1.png"
			sourceDisabled: "images/play_icon_toolbar_disable_256x256_1.png"
			label: "Playback"

			Layout.fillWidth: true
			Layout.preferredWidth: parent.width / 4.0
			Layout.minimumHeight: parent.height
		}

		Component.NavButton {
			navIndex: 3
			sourceEnabled: "images/gears.png"
			sourceDisabled: "images/gears_disable.png"
			label: "Settings"

			Layout.fillWidth: true
			Layout.preferredWidth: parent.width / 4.0
			Layout.minimumHeight: parent.height
		}
		
	}
	
}
