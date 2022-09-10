import QtQuick 2.15
import QtGraphicalEffects 1.15
import config 1.0

Item {
    id: _deviceListDelegateRoot
    property var modelForOperations: model.operationsModel
    height: Constants.deviceDelegateHeight
    width: Constants.deviceDelegateWidth
    Rectangle {
        id: _border
        anchors.fill: parent
        anchors.margins: 3
        border.width: 1
        border.color: Constants.deviceDelegateBorderColor
        color: "transparent"
    }
    Glow {
        anchors.fill: _border
        radius: 3
        samples: 5
        color: Constants.deviceDelegateBorderGlowColor
        source: _border
    }

    Image {
        id: _deviceImage
        anchors.top: parent.top
        anchors.bottom: _deviceTitle.top
        anchors.left: parent.left
        anchors.margins: Constants.deviceDelegateBorderMargin
        width: parent.width / 3 - Constants.deviceDelegateBorderMargin
        source: model.imageFile
        fillMode: Image.PreserveAspectFit
    }
    Text {
        id: _deviceTitle
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: Constants.deviceDelegateBorderMargin
        width: parent.width / 3 - Constants.deviceDelegateBorderMargin
        height: contentHeight
        font.pixelSize: Constants.deviceTitleFontMaxSize
        fontSizeMode: Text.HorizontalFit
        horizontalAlignment: Qt.AlignHCenter
        color: Constants.labelTextColor
        text: model.title
    }
    ListView {
        id: _operationsListView
        clip: true
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: Constants.deviceDelegateBorderMargin
        width: 2 * parent.width / 3 - Constants.deviceDelegateBorderMargin
        model: _deviceListDelegateRoot.modelForOperations
        snapMode: ListView.SnapToItem
        delegate: OperationsListDelegate {
            height: Constants.operationListHeight
            width: _operationsListView.width
        }
    }
}
