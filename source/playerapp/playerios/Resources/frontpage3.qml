import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.15

import "components"

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
		
		StackLayout {
			id: libraryMain
			currentIndex: 0
			
			AlbumView {
				model: playListModel
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
					model: playListModel
					onClicked: {
						console.log("track " + currentIndex);
					}
				}

			}
		}
		
		Rectangle {
			anchors.fill: parent.fill
			
			DigitDisplay {
				id: digitDisplay
				
				anchors.horizontalCenter: parent.horizontalCenter
				anchors.top: parent.top
				anchors.topMargin: 10
				width: (parent.width >= 300) ? 300 :  parent.width
				implicitHeight: 50
			}

            PlayButton {
	            id: playButton
	            
            	anchors.horizontalCenter: parent.horizontalCenter
				anchors.top: digitDisplay.bottom
				anchors.topMargin: 10
				implicitWidth: 100
				implicitHeight: 100
            }
            
            ColumnLayout {
	            anchors.horizontalCenter: parent.horizontalCenter
				anchors.top: playButton.bottom
				anchors.topMargin: 10
				
        	    Text {
        	    	text: "Track Name"
        	    	font.pixelSize: 20
        	    	Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
	            }
        	    Text {
        	    	text: "Album Name"
        	    	font.pixelSize: 20
        	    	Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
	            }
        	    Text {
        	    	text: "Artist"
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
					Layout.alignment: Qt.AlignCenter
					Layout.preferredHeight: 75
				}		
				Text {
					text: "FTP server is disabled"
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
		implicitHeight: 75
		spacing: 0
		
		property int currentNavIndex: 0
		
		NavButton {
			navIndex: 0
			sourceEnabled: "images/album.png"
			sourceDisabled: "images/album_disable.png"
			label: "Library"

			Layout.fillWidth: true
			Layout.preferredWidth: parent.width / 4.0
			Layout.minimumHeight: parent.height
		}

		NavButton {
			navIndex: 1
			sourceEnabled: "images/play_icon_toolbar_256x256_1.png"
			sourceDisabled: "images/play_icon_toolbar_disable_256x256_1.png"
			label: "Playback"

			Layout.fillWidth: true
			Layout.preferredWidth: parent.width / 4.0
			Layout.minimumHeight: parent.height
		}

		NavButton {
			navIndex: 2
			sourceEnabled: "images/gears.png"
			sourceDisabled: "images/gears_disable.png"
			label: "Settings"

			Layout.fillWidth: true
			Layout.preferredWidth: parent.width / 4.0
			Layout.minimumHeight: parent.height
		}
		
	}

	PlayListModel {
		id: playListModel
	}
	
}
