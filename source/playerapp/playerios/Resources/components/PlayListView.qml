import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.15

import "components.js" as Comp

ListView {
	id: playListView
	
	property int currentIndex: 0
	
	signal clicked()
	
	delegate: SwipeDelegate {
		id: playListItem
		
		width: parent.width
		height: 50

        swipe.right: Label {
            id: deleteLabel
            text: qsTr("Delete")
            color: "white"
            verticalAlignment: Label.AlignVCenter
            padding: 12
            height: parent.height
            anchors.right: parent.right
			
			SwipeDelegate.onClicked: playListView.model.remove(index)
			
            background: Rectangle {
                color: deleteLabel.SwipeDelegate.pressed ? Qt.darker("tomato", 1.1) : "tomato"
            }
        }

        ListView.onRemove: SequentialAnimation {
            PropertyAction {
                target: swipeDelegate
                property: "ListView.delayRemove"
                value: true
            }
            NumberAnimation {
                target: swipeDelegate
                property: "height"
                to: 0
                easing.type: Easing.InOutQuad
            }
            PropertyAction {
                target: swipeDelegate
                property: "ListView.delayRemove"
                value: false
            }
        }

		TapHandler {
			id: tapHandler
			onTapped: {
				playListView.currentIndex = index;
				playListView.clicked();
			}	
		}
		        
        background: Rectangle {
        	id: playListInformation
        	
			gradient: Gradient {
				GradientStop {
					position: 0
					color: tapHandler.pressed ? "#e0e0e0" : "#fff"
				}
				GradientStop {
					position: 1
					color: tapHandler.pressed ? "#e0e0e0" : "#f5f5f5"
				}
			}

			RowLayout {
				anchors.fill: parent
		
				Rectangle {
					color: "transparent"
					Layout.leftMargin: 1
					Layout.preferredWidth: parent.height - 2
					Layout.minimumHeight: parent.height - 2

					Image {
						source: Comp.urlRelPath("images/note.png")
						fillMode: Image.PreserveAspectFit
						anchors.fill: parent
					}
				}

				Rectangle {
					color: "transparent"
				
					Layout.leftMargin: 10
					Layout.fillWidth: true
					Layout.minimumHeight: parent.height
				
					ColumnLayout {
						Text {
							text: model.track
							font.pixelSize: 18
							horizontalAlignment: Text.AlignLeft
							verticalAlignment: Text.AlignVCenter
						}					
						Text {
							text: model.artist
							font.pixelSize: 14
							horizontalAlignment: Text.AlignLeft
							verticalAlignment: Text.AlignVCenter
						}
					}
				}

				Rectangle {
					color: "transparent"

					Layout.preferredWidth: 70
					Layout.minimumHeight: parent.height
				
					Text {
						text: "98:12"
						font.pixelSize: 18
						anchors.right: parent.right
						anchors.rightMargin: 10
						anchors.verticalCenter: parent.verticalCenter
						horizontalAlignment: Text.AlignRight
						verticalAlignment: Text.AlignVCenter
					}
					
					Rectangle {
						width: 1
						color: "#ccc"
						anchors.top: parent.top
						anchors.bottom: parent.bottom
						anchors.left: parent.left
					}
						
				}			
			}	
		}
	
		Rectangle {
			height: 1
			color: "#ccc"
			anchors.bottom: parent.bottom
			anchors.left: parent.left
			anchors.right: parent.right
		}

	}
}
