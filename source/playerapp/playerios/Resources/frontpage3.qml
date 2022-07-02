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
			
			Rectangle {
				id: playControlsContainer
				
				anchors.top: parent.top
				anchors.left: parent.left
				anchors.right: parent.right
				implicitHeight: 75
				
				gradient: Gradient {
					GradientStop { position: 0.0; color: "#c6e4f7"}
					GradientStop { position: 1.0; color: "#477996"}
				}
				
        	    PlayButton {
		            id: playButton
	            	
	            	anchors.left: parent.left
	            	anchors.verticalCenter: parent.verticalCenter
	            	anchors.leftMargin: 3
	            	implicitWidth: implicitHeight
	            	implicitHeight: parent.height - 7
	            }
	            
	            Slider {
	            	id: seekSlider
	            	
	            	from: 0.0
	            	to: 1.0
	            	value: 0.0
	            	
	            	anchors.left: playButton.right
	            	anchors.right: parent.right
	            	anchors.top: parent.top
	            }
	            
	            Image {
	            	id: playingAlbumImage
					source: "images/note.png"
					fillMode: Image.PreserveAspectFit
					
					anchors.left: playButton.right
					anchors.top: seekSlider.bottom
					anchors.bottom: parent.bottom
            		anchors.leftMargin: 7
            		anchors.bottomMargin: 7
	            }

            	ColumnLayout {
            		id: playingTrackInfo
            		
            		anchors.top: seekSlider.bottom
            		anchors.left: playingAlbumImage.right
            		anchors.right: digitDisplay.left
            		anchors.bottom: parent.bottom
            		anchors.leftMargin: 7
            		anchors.bottomMargin: 7

	        	    Text {
    	    	    	text: "Track Name"
        		    	font.pixelSize: 12
        		    	Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
		            }
    	    	    Text {
        		    	text: "Album Name"
        		    	font.pixelSize: 12
        	    		Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
		            }
    	        }
	            

				DigitDisplay {
					id: digitDisplay
					
					anchors.verticalCenter: parent.verticalCenter
					anchors.right: parent.right
					anchors.top: seekSlider.bottom
					anchors.rightMargin: 5
					implicitHeight: 30
					implicitWidth: 100
				}
			}

			PlayListView {
				model: playListModel
				
				clip: true
				anchors.top: playControlsContainer.bottom
				anchors.left: parent.left
				anchors.right: parent.right
				anchors.bottom: parent.bottom
				
				onClicked: {
					console.log(currentIndex);
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
