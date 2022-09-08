import QtQuick 2.15
import datasources 1.0
import config 1.0

Item {
    Text {
        anchors.right: parent.horizontalCenter
        anchors.rightMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        width: contentWidth
        text: Clock.currentTime
        color: Constants.headerTextColor
        font.pixelSize: Constants.headerFontSize
    }
    Text {
        anchors.left: parent.horizontalCenter
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        width: contentWidth
        text: PriceFetcher.currentPrice
        color: Constants.headerTextColor
        font.pixelSize: Constants.headerFontSize
    }
}
