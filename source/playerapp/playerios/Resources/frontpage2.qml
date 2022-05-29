import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.15

Window {
    id: window
    visible: true

	width: 200
	height: 600
	
	StackLayout {
		width: parent.width
		height: parent.height * 0.9
		anchors.top: parent.top
		
		currentIndex: toolBar.currentIndex
		
		// Library Tab
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
	                text: title + " (" + artist + ")"
				}
			}
		}
		
		// Settings Tab
		Rectangle {
			Switch {
				text: "Enable FTP"
			}
			Text {
				text: "FTP server is disabled"
			}
		}		
	}
	
	TabBar {
		id: toolBar
		
		width: parent.width
		height: parent.height * 0.1
		anchors.bottom: parent.bottom
		
		TabButton {
			Item {
				Image {
					anchors.centerIn: parent
        		    horizontalAlignment: Image.AlignHCenter
		            verticalAlignment: Image.AlignVCenter
					source: "images/20-gear2.png"
				}
			}
		}
		TabButton {
			Item {
				Image {
					anchors.centerIn: parent
					source: "images/album3.png"
				}
			}
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
