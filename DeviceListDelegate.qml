import QtQuick 2.15
import config 1.0

Rectangle {
    color: "transparent"
    border.width: 1
    border.color: "red"
    height: Constants.deviceDelegateHeight
    width: Constants.deviceDelegateWidth
    Text {
        color: Constants.labelTextColor
        text: model.title
        anchors.centerIn: parent
    }
}
