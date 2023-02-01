import QtQuick 2.15
import config 1.0
import listmodels 1.0
import datasources 1.0

Item {
    id: _devicesViewRoot
    DeviceListModel {
        id: _allDevices
        clock: Clock
        priceFetcher: PriceFetcher
    }
    PathView {
        id: _devicesList
        anchors.fill: parent
        model: _allDevices
        delegate: DeviceListDelegate {}
        property real pathWidth: (_devicesList.count * Constants.deviceDelegateWidth * 0.6667)
        property real pathStart: (_devicesViewRoot.width / 2) - (_devicesList.pathWidth / 2)
        path: Path {
            startX: _devicesList.pathStart; startY: _devicesViewRoot.height / -2
            PathArc {
                relativeX: _devicesList.pathWidth
                relativeY: 0
                radiusX: relativeX / 2
                radiusY: _devicesViewRoot.height
                direction: PathArc.Counterclockwise
            }
        }
    }
}
