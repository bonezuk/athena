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
			
		StackLayout {
			id: libraryMain
			currentIndex: 0
			
			Component.AlbumView {
				model: albumModel
				onClicked: {
					console.log(currentIndex);
					// Use the selected index to load the tracks belonging to the album
					albumModel.showAlbumTracks(currentIndex);
					albumTrackView.currentAlbumName = currentAlbumName;
					albumTrackView.currentArtistName = currentArtistName;
					// Set the stack view to show the track album page.
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
						height: 60
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

				Component.AlbumTrackView {
					id: albumTrackView
					
					anchors.fill: parent
					model: albumTrackModel
					
					property int noItems: {
						var numberOfItems = albumTrackModel.sizeOfModel;
						if(numberOfItems == 0)
						{
							libraryMain.currentIndex = 0;
						}
					}
					
					onClicked: {
						console.log("track " + currentIndex);
						albumTrackModel.appendTrackToPlaylist(currentIndex);
						notifyInfo.text = "Added track '" + currentTrack + "' to playlist."
						notifyInfo.visible = true;
					}
					
					Component.PLNotifyInfo {
						id: notifyInfo
						text: ""
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
				
        	    Component.PlayButton {
		            id: playButton
		            enabled: (playListModel.sizeOfModel > 0) ? true : false
		            playing: (playbackStateController.state) ? true : false

	            	anchors.left: parent.left
	            	anchors.verticalCenter: parent.verticalCenter
	            	anchors.leftMargin: 3
	            	implicitWidth: implicitHeight
	            	implicitHeight: parent.height - 7
	            	
	            	onClicked: playListModel.onPlayPausePressed();
	            }
	            
				Component.PlaybackSlider {
					id: seekSlider
					
					from: 0.0
					to: (playbackStateController.isPlayback) ? playListModel.dataAtIndex(playbackStateController.index, "length") : 1.0
					liveValue: playbackStateController.time;
					
					anchors.left: playButton.right
					anchors.leftMargin: 5
					anchors.right: parent.right
					anchors.rightMargin: 5
					anchors.top: parent.top
					anchors.topMargin: 5

					onSeek: (v) => {
						digitDisplay.timeInSeconds = parseInt(v);
						playbackStateController.onSeek(v);
					}
			
					onDisplay: (v) => {
						digitDisplay.timeInSeconds = parseInt(v);
					}
				}
	            
	            Image {
	            	id: playingAlbumImage
					source: "images/note.png"
					fillMode: Image.PreserveAspectFit
					visible: playbackStateController.isPlayback
					
					anchors.left: playButton.right
					anchors.top: seekSlider.bottom
					anchors.bottom: parent.bottom
            		anchors.leftMargin: 7
            		anchors.bottomMargin: 7
	            }

            	ColumnLayout {
            		id: playingTrackInfo
            		spacing: 1
            		
            		anchors.top: seekSlider.bottom
            		anchors.left: playingAlbumImage.right
            		anchors.right: digitDisplay.left
            		anchors.bottom: parent.bottom
            		anchors.leftMargin: 7
            		anchors.bottomMargin: 7

	        	    Text {
    	    	    	text: playListModel.dataAtIndex(playbackStateController.index, "title")
        		    	font.pixelSize: 12
        		    	Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
		            }
    	    	    Text {
        		    	text: playListModel.dataAtIndex(playbackStateController.index, "album")
        		    	font.pixelSize: 12
        	    		Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
		            }
    	    	    Text {
        		    	text: playListModel.dataAtIndex(playbackStateController.index, "artist")
        		    	font.pixelSize: 12
        	    		Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
		            }
    	        }

				Component.DigitDisplay {
					id: digitDisplay
					
					anchors.right: parent.right
					anchors.rightMargin: 5
					anchors.top: seekSlider.bottom
					anchors.topMargin: 5
					
					width: 120
					height: 25
				}
			}
			
			Component.PlayListView {
				model: playListModel
				playbackState: playbackStateController
				
				clip: true
				anchors.top: playControlsContainer.bottom
				anchors.left: parent.left
				anchors.right: parent.right
				anchors.bottom: parent.bottom
				
				onClicked: {
					console.log(currentIndex);
					playListModel.playItemAtIndex(currentIndex);
				}
			}
		}
		
		Rectangle {
			anchors.fill: parent.fill
			
			ColumnLayout {
				anchors.fill: parent.fill
				anchors.left: parent.left
				anchors.right: parent.right				

				Item {
					Switch {
						text: "Enable FTP"
						anchors.centerIn: parent
	                    checked: settings.ftpServer
    	                onClicked: settings.ftpServer = !settings.ftpServer
					}
					Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
					Layout.fillWidth: true
					Layout.preferredHeight: 50
				}
				Item {
					Text {
						text: settings.ftpStatus
						anchors.centerIn: parent
					}
					Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
					Layout.fillWidth: true
					Layout.preferredHeight: 50
				}
				Item {
					Button {
						text: "Rebuild Database"
						anchors.leftMargin: 20
						anchors.rightMargin: 20
						anchors.fill: parent
						onClicked: { 
							lockAppDialog.text = "<b>Rebuilding database from music directory.\nPlease Wait!</b>"
							lockAppDialog.visible = true;
							settings.onRebuildDatabase();
							lockAppDialog.visible = false;
						}
					}
					Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
					Layout.fillWidth: true
					Layout.preferredHeight: 50
				}
		        Item {
        		    // spacer item
		            Layout.fillWidth: true
        		    Layout.fillHeight: true
            		//Rectangle { anchors.fill: parent; color: "#ffaaaa" }
		        }
			}
		}

	}

	Component.MutexDialog {
		id: lockAppDialog
		visible: false
		text: ""
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
			label: "Album"

			Layout.fillWidth: true
			Layout.preferredWidth: parent.width / 4.0
			Layout.minimumHeight: parent.height
		}

		Component.NavButton {
			navIndex: 1
			sourceEnabled: "images/play_icon_toolbar_256x256_1.png"
			sourceDisabled: "images/play_icon_toolbar_disable_256x256_1.png"
			label: "Playback"

			Layout.fillWidth: true
			Layout.preferredWidth: parent.width / 4.0
			Layout.minimumHeight: parent.height
		}

		Component.NavButton {
			navIndex: 2
			sourceEnabled: "images/gears.png"
			sourceDisabled: "images/gears_disable.png"
			label: "Settings"

			Layout.fillWidth: true
			Layout.preferredWidth: parent.width / 4.0
			Layout.minimumHeight: parent.height
		}
		
	}
	
}
