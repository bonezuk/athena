import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
	property int timeInSeconds: 0

	color: "transparent"

	RowLayout {
		anchors.fill: parent
		spacing: 1
		implicitWidth: parent.implicitWidth

		Rectangle {
			color: "transparent"
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.preferredWidth: parent.width / 7

			Image {
				height: parent.height
				anchors.verticalCenter: parent.verticalCenter
				anchors.horizontalCenter: parent.horizontalCenter
				fillMode: Image.PreserveAspectFit
				source: comp.getPlayTimeDigitImageName(comp.PlayTimeHourUpperDigit, parent.timeInSeconds)
			}
		}
		
		Rectangle {
			color: "transparent"
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.preferredWidth: parent.width / 7

			Image {
				height: parent.height
				anchors.verticalCenter: parent.verticalCenter
				anchors.horizontalCenter: parent.horizontalCenter
				fillMode: Image.PreserveAspectFit
				source: comp.getPlayTimeDigitImageName(comp.PlayTimeHourLowerDigit, parent.timeInSeconds)
			}
		}
		Rectangle {
			color: "transparent"
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.preferredWidth: parent.width / 14

			Image {
				height: parent.height
				anchors.verticalCenter: parent.verticalCenter
				anchors.horizontalCenter: parent.horizontalCenter
				fillMode: Image.PreserveAspectFit
				source: comp.urlRelPath("images/digit_colon.png")
			}
		}
		Rectangle {
			color: "transparent"
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.preferredWidth: parent.width / 7

			Image {
				height: parent.height
				anchors.verticalCenter: parent.verticalCenter
				anchors.horizontalCenter: parent.horizontalCenter
				fillMode: Image.PreserveAspectFit
				source: comp.getPlayTimeDigitImageName(comp.PlayTimeMinuteUpperDigit, parent.timeInSeconds)
			}
		}
		Rectangle {
			color: "transparent"
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.preferredWidth: parent.width / 7

			Image {
				height: parent.height
				anchors.verticalCenter: parent.verticalCenter
				anchors.horizontalCenter: parent.horizontalCenter
				fillMode: Image.PreserveAspectFit
				source: comp.getPlayTimeDigitImageName(comp.PlayTimeMinuteLowerDigit, parent.timeInSeconds)
			}
		}
		Rectangle {
			color: "transparent"
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.preferredWidth: parent.width / 14

			Image {
				height: parent.height
				anchors.verticalCenter: parent.verticalCenter
				anchors.horizontalCenter: parent.horizontalCenter
				fillMode: Image.PreserveAspectFit
				source: comp.urlRelPath("images/digit_colon.png")
			}
		}
		Rectangle {
			color: "transparent"
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.preferredWidth: parent.width / 7

			Image {
				height: parent.height
				anchors.verticalCenter: parent.verticalCenter
				anchors.horizontalCenter: parent.horizontalCenter
				fillMode: Image.PreserveAspectFit
				source: comp.getPlayTimeDigitImageName(comp.PlayTimeSecondUpperDigit, parent.timeInSeconds)
			}
		}
		Rectangle {
			color: "transparent"
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.preferredWidth: parent.width / 7

			Image {
				height: parent.height
				anchors.verticalCenter: parent.verticalCenter
				anchors.horizontalCenter: parent.horizontalCenter
				fillMode: Image.PreserveAspectFit
				source: comp.getPlayTimeDigitImageName(comp.PlayTimeSecondLowerDigit, parent.timeInSeconds)
			}
		}
	}
}