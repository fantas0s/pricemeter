pragma Singleton
import QtQuick 2.15

QtObject {
    /* Dimensions */
    readonly property int screenWidth: 800
    readonly property int screenHeight: 480
    readonly property int headerHeight: screenHeight > screenWidth ? screenWidth / 10 : screenHeight / 10
    readonly property int deviceDelegateHeight: screenHeight - (2 * headerHeight)
    readonly property int deviceDelegateWidth: screenWidth - (3* headerHeight)
    readonly property int deviceDelegateBorderMargin: 8
    readonly property int operationListHeight: deviceDelegateHeight - (2 * deviceDelegateBorderMargin)
    readonly property int operationDelegateTextBoxMargin: 4

    /* Colors */
    readonly property color backgroundColor: "#000000"
    readonly property color headerTextColor: "#FFFFFF"
    readonly property color labelTextColor: "#FFFFFF"
    readonly property color deviceDelegateBorderColor: "#EEEEEE"
    readonly property color deviceDelegateBorderGlowColor: "#88FFBB"
    readonly property color consumptionBoxBorderColor: "#DDDDDD"

    /* Fonts */
    readonly property int headerFontSize: headerHeight - 8
    readonly property int deviceTitleFontMaxSize: 32
    readonly property int operationTitleFontMaxSize: 40
}
