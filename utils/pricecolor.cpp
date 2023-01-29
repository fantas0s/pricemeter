#include "pricecolor.h"

static const qreal s_cheapPriceInCents = 0.33; /* times 30 days = 10 cents */
static const qreal s_expensivePriceInCents = 33.0; /* times 30 days = 10 EUR */

PriceColor::PriceColor()
    : QObject{nullptr}
{}

QColor PriceColor::color() const
{
    if (m_price < cheapAreaLimit())
        return QColor(0, 255, 0); // Green, as in "cheap" area
    if (m_price > expensiveAreaLimit())
        return QColor(255, 0, 0); // Red, as in "expensive" area

    const qreal absoluteMaxPrice = qMin(m_maxPrice, expensiveAreaLimit());
    const qreal absoluteMinPrice = qMax(m_minPrice, cheapAreaLimit());
    const qreal maxCost = absoluteMaxPrice - absoluteMinPrice;
    const qreal normalizedCost = m_price - absoluteMinPrice;

    /* Merely two safety calls */
    if (maxCost < 0)
        return QColor(255,255,255);
    if (normalizedCost > maxCost)
        return QColor(255, 0, 0);
    if ((normalizedCost < 0) ||
        (maxCost == 0)) {
        return QColor(0, 255, 0); // Green, as cheapest price always
    }
    if ((normalizedCost / maxCost) < 0.5) {
        /* Cheaper half */
        return QColor::fromRgbF((normalizedCost / maxCost) * 2.0, 1.0, 0.0);
    } else {
        /* more expensive half */
        return QColor::fromRgbF(1.0, 2 - ((normalizedCost / maxCost) * 2.0), 0.0);
    }
}

qreal PriceColor::cheapAreaLimit() const
{
    return s_cheapPriceInCents;
}

qreal PriceColor::expensiveAreaLimit() const
{
    return s_expensivePriceInCents;
}

qreal PriceColor::price() const
{
    return m_price;
}

qreal PriceColor::minPrice() const
{
    return m_minPrice;
}

qreal PriceColor::maxPrice() const
{
    return m_maxPrice;
}

void PriceColor::setPrice(qreal value)
{
    if (m_price != value)
    {
        m_price = value;
        emit priceChanged();
        emit colorChanged();
    }
}

void PriceColor::setMinPrice(qreal value)
{
    if (m_minPrice != value)
    {
        m_minPrice = value;
        emit minPriceChanged();
        emit colorChanged();
    }
}

void PriceColor::setMaxPrice(qreal value)
{
    if (m_maxPrice != value)
    {
        m_maxPrice = value;
        emit maxPriceChanged();
        emit colorChanged();
    }
}
