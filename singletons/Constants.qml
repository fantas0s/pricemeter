pragma Singleton
import QtQuick 2.15

QtObject {
    /* Dimensions */
    readonly property int screenWidth: 800
    readonly property int screenHeight: 480
    readonly property int headerHeight: screenHeight > screenWidth ? screenWidth / 10 : screenHeight / 10
    readonly property int deviceDelegateHeight: screenHeight - (2 * headerHeight)
    readonly property int deviceDelegateWidth: screenWidth - (2* headerHeight)

    /* Colors */
    readonly property color backgroundColor: "#000000"
    readonly property color labelTextColor: "#FFFFFF"
}
