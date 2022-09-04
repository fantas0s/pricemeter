import QtQuick 2.15
import config 1.0

Item {
    id: _operationListDelegateRoot
    property int rowHeight: model.title === "" ? (_operationListDelegateRoot.height / 4) - 6 : (_operationListDelegateRoot.height / 5) - 6
    property var labels: model.labels
    property var values: model.values
    property var valueColors: model.valueColors
    Column {
        anchors.fill: parent
        spacing: 6
        Text {
            color: Constants.labelTextColor
            text: model.title
            height: model.title === "" ? 0 : _operationListDelegateRoot.rowHeight
            width: parent.width
            font.pixelSize: Constants.operationTitleFontMaxSize
            fontSizeMode: Text.HorizontalFit
        }
        Repeater {
            model: 4
            OperationRow {
                height: _operationListDelegateRoot.rowHeight
                width: parent.width
                labelText: _operationListDelegateRoot.labels[index]
                valueText: _operationListDelegateRoot.values[index]
                valueTextColor: _operationListDelegateRoot.valueColors[index]
            }
        }
    }
}
