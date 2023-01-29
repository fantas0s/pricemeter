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
        property int pathStart: (_devicesViewRoot.width / 2) - ((_devicesList.count -1) * Constants.deviceDelegateWidth / 2)
        path: Path {
            startX: _devicesList.pathStart; startY: _devicesViewRoot.height / -2
            PathArc {
                relativeX: _devicesList.count * Constants.deviceDelegateWidth * 0.73
                relativeY: 0
                radiusX: relativeX / 2
                radiusY: _devicesViewRoot.height
                direction: PathArc.Counterclockwise
            }
            PathArc {
                relativeX: Constants.deviceDelegateWidth / (_devicesList.count * 0.1)
                relativeY: 0
                radiusX: relativeX / 2
                radiusY: Constants.deviceDelegateHeight / 4
            }
        }
    }
}
