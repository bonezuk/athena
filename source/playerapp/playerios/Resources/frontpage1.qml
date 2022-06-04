/*
#import QtQuick 2.14
#import QtQuick.Controls 2.14
#import QtQuick.Window 2.14
#import QtQuick.Layouts 1.15
#import uk.co.blackomega 1.0
*/

import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.15
import uk.co.blackomega 1.0


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
                    text: model.title + " (" + model.artist + ")"
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
		
		Button {
			background: Rectangle {
				color: 'black'
			}
			Layout.fillWidth: true
    	    Layout.preferredWidth: parent.width / 2.0
	        Layout.minimumHeight: parent.height
	        
	        readonly property int navIndex: 0
	        
	        Image {
	        	id: albumButton
		        source: (navBar.currentNavIndex == parent.navIndex) ? "images/album.png" : "images/album_disable.png"
		        width: ((parent.width < parent.height) ? parent.width : parent.height) / 2.0
		        height: ((parent.width < parent.height) ? parent.width : parent.height) / 2.0
	        	anchors.horizontalCenter: parent.horizontalCenter
	        	anchors.verticalCenter: parent.verticalCenter
	        	anchors.verticalCenterOffset: -10.0
	        }
	        Text {
				text: "Library"
				color: (navBar.currentNavIndex == parent.navIndex) ? "white" : "#323232"
                font.pixelSize: 14
	        	anchors.horizontalCenter: parent.horizontalCenter
	        	anchors.top: albumButton.bottom
	        }
	        
	        onClicked: parent.currentNavIndex = navIndex
		}
		
		Button {
			background: Rectangle {
				color: 'black'
			}		
			Layout.fillWidth: true
    	    Layout.preferredWidth: parent.width / 2.0
	        Layout.minimumHeight: parent.height
	        
	        readonly property int navIndex: 1

	        Image {
	        	id: settingsButton
		        source: (navBar.currentNavIndex == parent.navIndex) ? "images/gears.png" : "images/gears_disable.png"
		        width: ((parent.width < parent.height) ? parent.width : parent.height) / 2.0
		        height: ((parent.width < parent.height) ? parent.width : parent.height) / 2.0
	        	anchors.horizontalCenter: parent.horizontalCenter
	        	anchors.verticalCenter: parent.verticalCenter
	        	anchors.verticalCenterOffset: -10.0
	        }
	        Text {
	        	text: "Settings"
	        	color: (navBar.currentNavIndex == parent.navIndex) ? "white" : "#323232"
                font.pixelSize: 14
	        	anchors.horizontalCenter: parent.horizontalCenter
	        	anchors.top: settingsButton.bottom
	        }
	        
	        onClicked: parent.currentNavIndex = navIndex
		}
		
	}
	
}
