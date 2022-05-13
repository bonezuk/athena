import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import uk.co.blackomega 1.0

Window {
    id: window
    visible: true

	width: 600
	height: 600

	ListView {
		anchors.fill: parent
		
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
}
