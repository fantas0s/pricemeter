import QtQuick 2.15
import config 1.0
import listmodels 1.0
import utils 1.0

Item {
    id: _devicesViewRoot
    DeviceListModel {
        id: _allDevices
        clock: Clock
    }
    PathView {
        id: _devicesList
        anchors.fill: parent
        model: _allDevices
        delegate: DeviceListDelegate {}
        property int pathStart: (_devicesViewRoot.width / 2) - (_devicesList.count * Constants.deviceDelegateWidth / 3.5)
        path: Path {
            startX: _devicesList.pathStart; startY: _devicesViewRoot.height / -2
            PathArc {
                relativeX: _devicesList.count * Constants.deviceDelegateWidth / 1.75
                relativeY: 0
                radiusX: relativeX / 2
                radiusY: _devicesViewRoot.height
                direction: PathArc.Counterclockwise
            }
            PathArc {
                relativeX: Constants.deviceDelegateWidth / 1.35
                relativeY: 0
                radiusX: relativeX / 2
                radiusY: Constants.deviceDelegateHeight / 4
            }
        }
    }
}
