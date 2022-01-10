import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2

ApplicationWindow {
    id: window
    visible: true
    title: "Black Omega Playlist Manager"
	
	ListView {
		id: playListView

        anchors.fill: parent

        Layout.minimumWidth: 400
        Layout.minimumHeight: 240
        Layout.preferredWidth: 600
        Layout.preferredHeight: 400
        
        model: playListModel
        
        delegate: Text {
        	text: title + " (" + artist + ") [" + album + "]"
        }

	}
}
