import QtQuick 2.15
import utils 1.0

Text {
    PriceColor {
        id: _colorProducer
        minPrice: _colorProducer.cheapAreaLimit
        maxPrice: _colorProducer.expensiveAreaLimit
        price: _colorProducer.minPrice
        SequentialAnimation on price {
            loops: Animation.Infinite
            NumberAnimation {
                from: _colorProducer.minPrice
                to: _colorProducer.maxPrice
                duration: 500
            }
            NumberAnimation {
                from: _colorProducer.maxPrice
                to: _colorProducer.minPrice
                duration: 500
            }
        }
    }
    color: _colorProducer.color
    text: debugObj.debugString
}
