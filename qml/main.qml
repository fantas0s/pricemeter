import QtQuick 2.15
import QtQuick.Window 2.15
import config 1.0

Window {
    width: Constants.screenWidth
    height: Constants.screenHeight
    visible: true
    title: qsTr("Price Meter")
    color: Constants.backgroundColor
    Header {
        id: _header
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: Constants.headerHeight
    }
    DevicesView {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: _header.bottom
        anchors.bottom: parent.bottom
    }
    Loader {
        id: _debugPrint
        anchors.top: parent.top
        anchors.topMargin: 2
        anchors.left: parent.left
        anchors.leftMargin: 2
        source: debugObj.debugString !== "" ? "DebugPrint.qml" : ""
    }
}
