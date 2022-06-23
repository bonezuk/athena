import QtQuick 2.14
import QtQuick.Controls 2.14

import "components.js" as Comp

Rectangle {
	property bool hovering: false
	property bool pressed: false
	
	Image {
		anchors.fill: parent;
		fillMode: Image.PreserveAspectFit
		source: Comp.playButtonImageSource(parent.pressed, parent.hovering)
	}
	
	MouseArea {
		anchors.fill:parent
		hoverEnabled: true
		
		onEntered: parent.hovering = true
		onExited: parent.hovering = false
		onPressed: parent.pressed = true
		onReleased: parent.pressed = false
	}
}
