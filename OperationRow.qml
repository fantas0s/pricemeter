import QtQuick 2.15
import config 1.0

Item {
    id: _rowRoot
    property alias labelText: _label.text
    property alias valueText: _value.text
    property alias valueTextColor: _value.color
    Text {
        id: _label
        height: parent.height
        width: parent.width / 2
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        color: Constants.labelTextColor
        font.pixelSize: parent.height - 2
        fontSizeMode: Text.HorizontalFit
    }
    Text {
        id: _value
        height: parent.height
        width: parent.width / 2
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        font.pixelSize: parent.height - 2
        fontSizeMode: Text.HorizontalFit
    }
}
