import QtQuick 2.15
import config 1.0

Item {
    id: _deviceListDelegateRoot
    property var modelForOperations: model.operationsModel
    height: Constants.deviceDelegateHeight
    width: Constants.deviceDelegateWidth
    Image {
        id: _deviceImage
        anchors.top: parent.top
        anchors.bottom: _deviceTitle.top
        anchors.left: parent.left
        width: parent.width / 2
        source: model.imageFile
        fillMode: Image.PreserveAspectFit
    }
    Text {
        id: _deviceTitle
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: parent.width / 2
        height: contentHeight
        font.pixelSize: Constants.deviceTitleFontMaxSize
        fontSizeMode: Text.HorizontalFit
        horizontalAlignment: Qt.AlignHCenter
        color: Constants.labelTextColor
        text: model.title
    }
    ListView {
        id: _operationsListView
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.width / 2
        model: _deviceListDelegateRoot.modelForOperations
        snapMode: ListView.SnapToItem
        delegate: OperationsListDelegate {
            height: _deviceListDelegateRoot.height
            width: _operationsListView.width
        }
    }
}
