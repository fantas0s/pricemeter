#include "operationlistitem.h"
#include "datasources/clock.h"
#include "datasources/pricefetcher.h"

#include <QObject>

OperationListItem::OperationListItem()
{
    /* Should be updated when prices are updated */
    m_lowestCostTimeString = "03:00:";
}

bool OperationListItem::isValid() const
{
    return m_consumptions.size() > 0;
}

QString OperationListItem::itemTitle(int index) const
{
    if (isContinuous()) {
        switch (index) {
        case 0:
            return QObject::tr("Next hour:");
        case 1:
            return QObject::tr("2 hours:");
        case 2:
            return QObject::tr("8 hours:");
        default:
            return QObject::tr("24 hours:");
        }
    } else {
        switch (index) {
        case 0:
            return QObject::tr("Now:");
        case 1:
            return m_nextHourString;
        case 2:
            return m_twoHoursString;
        default:
            return m_lowestCostTimeString;
        }
    }
}

QString OperationListItem::itemValue(int index) const
{
    switch (index) {
    case 0:
        return m_currentCost;
    case 1:
        return m_nextHourCost;
    case 2:
        return m_twoHoursCost;
    default:
        return m_lowestCost;
    }
}

QColor OperationListItem::valueColor(int index) const
{
    return QColor(255,255,255);
}

QString OperationListItem::title() const
{
    return m_title;
}

QList<Consumption> OperationListItem::consumptions() const
{
    return m_consumptions;
}

void OperationListItem::setTitle(const QString &value)
{
    m_title = value;
}

void OperationListItem::addConsumption(const Consumption &value)
{
    m_consumptions.append(value);
}

void OperationListItem::recalculateValues(const Clock *clock, const PriceFetcher* priceFetcher)
{
    int minutes = 60 - clock->currentTime().time().minute();
    /* Titles */
    m_nextHourString = QObject::tr("In %1 minute(s):", "", minutes).arg(minutes);
    m_twoHoursString = clock->nextEvenHour().addSecs(3600).toString("hh:mm")+QString(":");
    m_lowestCostTimeString = "00:00"; // TODO: find lowest cost time
    /* Values */
    if (isContinuous()) {
        m_currentCost = continuousCostString(1, clock, priceFetcher);
        m_nextHourCost = continuousCostString(2, clock, priceFetcher);
        m_twoHoursCost = continuousCostString(8, clock, priceFetcher);
        m_lowestCost = continuousCostString(24, clock, priceFetcher);
    } else {
        m_currentCost = costString(clock->currentTime(), clock, priceFetcher);
        m_nextHourCost = costString(clock->nextEvenHour(), clock, priceFetcher);
        m_twoHoursCost = costString(clock->nextEvenHour().addSecs(3600), clock, priceFetcher);
        // TODO: lowest cost
    }
}

bool OperationListItem::isContinuous() const
{
    return (m_consumptions.isEmpty() || (m_consumptions.first().time().msecsSinceStartOfDay() == 0));
}

QString OperationListItem::costString(const QDateTime &time, const Clock *clock, const PriceFetcher *priceFetcher)
{
    // TODO: minute-level accuracy and processing the list of consumptions.
    const QDateTime searchTime = clock->toEvenHour(time);
    qreal hours = m_consumptions.first().time().msecsSinceStartOfDay();
    /* msecs to hours */
    hours = (hours / 1000.0) / 3600.0;
    qreal kWh = m_consumptions.first().kW() * hours;
    qreal cost = priceFetcher->getPrice(searchTime) * kWh;
    /* in cents. */
    if (cost >= 100.0) {
        cost = cost / 100.0;
        return QObject::tr("%1 €").arg(cost, 0, 'f', 2);
    } else {
        return QObject::tr("%1 c").arg(cost, 0, 'f', 2);
    }
}

QString OperationListItem::continuousCostString(int hours, const Clock *clock, const PriceFetcher *priceFetcher)
{
    // TODO: minute-level accuracy.
    const QDateTime searchTime = clock->toEvenHour(clock->currentTime());
    /* msecs to hours */
    qreal realHours = hours;
    /* Always only one consumption rate for continuous */
    qreal kWh = m_consumptions.first().kW() * realHours;
    qreal cost = priceFetcher->getPrice(searchTime) * kWh;
    /* in cents. */
    if (cost >= 100.0) {
        cost = cost / 100.0;
        return QObject::tr("%1 €").arg(cost, 0, 'f', 2);
    } else {
        return QObject::tr("%1 c").arg(cost, 0, 'f', 2);
    }
}
