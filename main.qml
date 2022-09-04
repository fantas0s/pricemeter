import QtQuick 2.15
import QtQuick.Window 2.15
import config 1.0
import listmodels 1.0

Window {
    width: Constants.screenWidth
    height: Constants.screenHeight
    visible: true
    title: qsTr("Price Meter")
    color: Constants.backgroundColor
    DeviceListModel {
        id: _allDevices
    }
}
