import QtQuick 2.15
import utils 1.0
import config 1.0

Item {
    Text {
        anchors.centerIn: parent
        text: Clock.currentTime
        color: Constants.headerTextColor
        font.pixelSize: Constants.headerFontSize
    }
}
