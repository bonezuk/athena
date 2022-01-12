import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import uk.co.blackomega 1.0

ApplicationWindow {
    id: window
    visible: true
    title: "Black Omega Playlist Manager"
	
	PlayListModel {
		id: playListModel
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
        	width: parent.width; height: 100
        	Text {
   				id: itexItem
				anchors.left: imageItem.right
				anchors.leftMargin: 20
				anchors.verticalCenter: parent.verticalCenter
				font.pixelSize: 40
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
