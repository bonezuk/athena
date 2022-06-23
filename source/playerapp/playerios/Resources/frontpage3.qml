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

		ListView {
			model: playListModel
		
	        delegate: Rectangle {
				width: parent.width
				height: 30
				Text {
					anchors.left: parent.left
					anchors.leftMargin: 20
					anchors.verticalCenter: parent.verticalCenter
					font.pixelSize: 20
	                text: model.track + " (" + model.artist + ")"
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
			Layout.preferredWidth: parent.width / 3.0
			Layout.minimumHeight: parent.height
		}

		NavButton {
			navIndex: 1
			sourceEnabled: "images/play_icon_toolbar_256x256_1.png"
			sourceDisabled: "images/play_icon_toolbar_disable_256x256_1.png"
			label: "Playback"

			Layout.fillWidth: true
			Layout.preferredWidth: parent.width / 3.0
			Layout.minimumHeight: parent.height
		}

		NavButton {
			navIndex: 2
			sourceEnabled: "images/gears.png"
			sourceDisabled: "images/gears_disable.png"
			label: "Settings"

			Layout.fillWidth: true
			Layout.preferredWidth: parent.width / 3.0
			Layout.minimumHeight: parent.height
		}
		
	}

ListModel {
	id: playListModel
	ListElement {	// 0
		track: "Danger Zone"
		artist: "Kenny Loggins"
		album: "Top Gun"
	}
	ListElement {	// 1
		track: "Mighty Wings"
		artist: "Cheap Trick"
		album: "Top Gun"
	}
	ListElement {	// 2
		track: "Playing with the Boys"
		artist: "Kenny Loggins"
		album: "Top Gun"
	}
	ListElement {	// 3
		track: "Lead Me On"
		artist: "Teena Marie"
		album: "Top Gun"
	}
	ListElement {	// 4
		track: "Take My Breath Away"
		artist: "Berlin"
		album: "Top Gun"
	}
	ListElement {	// 5
		track: "Hot Summer Nights"
		artist: "Miami Sound Machine"
		album: "Top Gun"
	}
	ListElement {	// 6
		track: "Heaven in Your Eyes"
		artist: "Loverboy"
		album: "Top Gun"
	}
	ListElement {	// 7
		track: "Through the Fire"
		artist: "Larry Greene"
		album: "Top Gun"
	}
	ListElement {	// 8
		track: "Destination Unknown"
		artist: "Marietta"
		album: "Top Gun"
	}
	ListElement {	// 9
		track: "Top Gun Anthem"
		artist: "Harold Faltermeyer, Steve Stevens"
		album: "Top Gun"
	}
}
	
}
