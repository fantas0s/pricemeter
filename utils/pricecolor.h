#pragma once

#include <QObject>
#include <QColor>

class PriceColor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color NOTIFY colorChanged)
    Q_PROPERTY(qreal cheapAreaLimit READ cheapAreaLimit CONSTANT)
    Q_PROPERTY(qreal expensiveAreaLimit READ expensiveAreaLimit CONSTANT)
    Q_PROPERTY(qreal price READ price WRITE setPrice NOTIFY priceChanged)
    Q_PROPERTY(qreal minPrice READ minPrice WRITE setMinPrice NOTIFY minPriceChanged)
    Q_PROPERTY(qreal maxPrice READ maxPrice WRITE setMaxPrice NOTIFY maxPriceChanged)

public:
    explicit PriceColor();
    QColor color() const;
    qreal cheapAreaLimit() const;
    qreal expensiveAreaLimit() const;
    qreal price() const;
    qreal minPrice() const;
    qreal maxPrice() const;
    void setPrice(qreal value);
    void setMinPrice(qreal value);
    void setMaxPrice(qreal value);

signals:
    void colorChanged();
    void priceChanged();
    void minPriceChanged();
    void maxPriceChanged();

private:
    qreal m_price = 0;
    qreal m_minPrice = 0;
    qreal m_maxPrice = 0;
};

