import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import uk.co.blackomega 1.0

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
            anchors.left: toolBarId.left
            anchors.leftMargin: 1
            anchors.verticalCenter: parent.verticalCenter

            Image {
                id: playButtonId
                height: 45
                anchors.verticalCenter: playControlContainer.verticalCenter
                anchors.horizontalCenter: playControlContainer.horizontalCenter
                fillMode: Image.PreserveAspectFit
                source: "images/pc_play_normal.png"
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
            color: "transparent"
            width: digitLayoutId.width
            height: digitLayoutId.height
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter

            Row {
                id: digitLayoutId
                spacing: 1
                Image {
                    source: "images/no0@2x.png"
                }
                Image {
                    source: "images/no0@2x.png"
                }
                Image {
                    source: "images/colon@2x.png"
                }
                Image {
                    source: "images/no0@2x.png"
                }
                Image {
                    source: "images/no0@2x.png"
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
            width: parent.width; height: 30
        	Text {
   				id: itexItem
				anchors.left: imageItem.right
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
