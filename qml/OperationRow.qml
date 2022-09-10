import QtQuick 2.15
import config 1.0

Item {
    id: _rowRoot
    property alias labelText: _label.text
    property alias valueText: _value.text
    property alias valueTextColor: _value.color
    Item {
        height: parent.height
        width: parent.width / 2 - 2 * Constants.operationDelegateTextBoxMargin
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: Constants.operationDelegateTextBoxMargin
        Text {
            id: _label
            anchors.fill: parent
            anchors.margins: Constants.operationDelegateTextBoxMargin
            color: Constants.labelTextColor
            font.pixelSize: parent.height - 2 * Constants.operationDelegateTextBoxMargin
            fontSizeMode: Text.HorizontalFit
            horizontalAlignment: Qt.AlignRight
            verticalAlignment: Qt.AlignBottom
        }
    }
    Rectangle {
        color: Constants.backgroundColor
        border.color: Constants.consumptionBoxBorderColor
        border.width: 1
        radius: 5
        height: parent.height
        width: parent.width / 2 - 2 * Constants.operationDelegateTextBoxMargin
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: Constants.operationDelegateTextBoxMargin
        Text {
            id: _value
            anchors.fill: parent
            anchors.margins: Constants.operationDelegateTextBoxMargin
            font.pixelSize: parent.height - 2 * Constants.operationDelegateTextBoxMargin
            fontSizeMode: Text.HorizontalFit
            horizontalAlignment: Qt.AlignLeft
            verticalAlignment: Qt.AlignBottom
        }
    }
}
