#include "operationlistitem.h"
#include "datasources/clock.h"
#include "datasources/pricefetcher.h"

#include <QObject>

OperationListItem::OperationListItem()
{}

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
    m_lowestCostTimeString = "00:00";
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
        m_lowestCost = lowestCostString(m_lowestCostTimeString, clock, priceFetcher);
    }
}

bool OperationListItem::isContinuous() const
{
    return (m_consumptions.isEmpty() || (m_consumptions.first().time().msecsSinceStartOfDay() == 0));
}

QString OperationListItem::costString(const QDateTime &time, const Clock *clock, const PriceFetcher *priceFetcher) const
{
    qreal combinedCost = consumptionCost(time, clock, priceFetcher);
    /* cost is in cents. */
    if (combinedCost >= 100.0) {
        combinedCost = combinedCost / 100.0;
        return QObject::tr("%1 €").arg(combinedCost, 0, 'f', 2);
    } else {
        return QObject::tr("%1 c").arg(combinedCost, 0, 'f', 2);
    }
}

QString OperationListItem::continuousCostString(int hours, const Clock *clock, const PriceFetcher *priceFetcher) const
{
    /* Always only one consumption rate for continuous */
    const qreal kW = m_consumptions.first().kW();
    QDateTime momentToEvaluate = clock->currentTime();
    const int seconds = 60 * 60 * hours;
    qreal localCost = cost(kW, seconds, momentToEvaluate, clock, priceFetcher);
    /* cost is in cents. */
    if (localCost >= 100.0) {
        localCost = localCost / 100.0;
        return QObject::tr("%1 €").arg(localCost, 0, 'f', 2);
    } else {
        return QObject::tr("%1 c").arg(localCost, 0, 'f', 2);
    }
}

QString OperationListItem::lowestCostString(QString &timeTitle, const Clock *clock, const PriceFetcher *priceFetcher) const
{
    /* Get next even hour */
    const QDateTime baseTime = clock->toEvenHour(clock->currentTime().addSecs(3600));
    int lowestCostAdd = 0;
    qreal lowestCost = 0;
    /* Try next 23 hours. */
    for (int additionalHours = 0 ; additionalHours < 24 ; ++additionalHours)
    {
        const qreal costOfHour = consumptionCost(baseTime.addSecs(3600 * additionalHours), clock, priceFetcher);
        if ((lowestCost == 0) ||
            (lowestCost > costOfHour)) {
            lowestCost = costOfHour;
            lowestCostAdd = additionalHours;
        }
    }
    timeTitle = baseTime.addSecs(3600 * lowestCostAdd).toString("hh:mm");
    /* lowestCost is in cents. */
    if (lowestCost >= 100.0) {
        lowestCost = lowestCost / 100.0;
        return QObject::tr("%1 €").arg(lowestCost, 0, 'f', 2);
    } else {
        return QObject::tr("%1 c").arg(lowestCost, 0, 'f', 2);
    }
}

qreal OperationListItem::consumptionCost(const QDateTime &time, const Clock *clock, const PriceFetcher *priceFetcher) const
{
    qreal combinedCost = 0;
    QDateTime momentToEvaluate = time;
    for (const auto& consumption : m_consumptions) {
        qreal kW = consumption.kW();
        int secondsToConsume = consumption.time().msecsSinceStartOfDay() / 1000;
        combinedCost += cost(kW, secondsToConsume, momentToEvaluate, clock, priceFetcher);
    }
    return combinedCost;
}

qreal OperationListItem::cost(qreal kW, int seconds, QDateTime &timeOfStart, const Clock *clock, const PriceFetcher *priceFetcher) const
{
    qreal cost = 0;
    qreal price = priceFetcher->getPrice(clock->toEvenHour(timeOfStart));
    while (seconds > 0) {
        /* Consume minutes up to next full hour */
        const QDateTime searchTime = clock->toEvenHour(timeOfStart);
        /* In case we haven't got price data yet, use the last we got. */
        const qreal tempPrice = priceFetcher->getPrice(searchTime);
        if (tempPrice > 0)
            price = tempPrice;
        const int remainingSeconds = 3600 - (60 * timeOfStart.time().minute()) - timeOfStart.time().second();
        qreal kWh = 0;
        if (seconds > remainingSeconds) {
            /* We have more time than just the remaining part of this hour.
             * Calculate cost for the remainder of this hour and continue.
             */
            kWh = (kW * ((qreal)remainingSeconds)/3600.0);
            seconds -= remainingSeconds;
            timeOfStart = timeOfStart.addSecs(remainingSeconds);
        } else {
            /* All of the remaining seconds happen within this hour. */
            kWh = (kW * ((qreal)seconds)/3600.0);
            seconds = 0;
            timeOfStart = timeOfStart.addSecs(seconds);
        }
        cost += price * kWh;
    }
    return cost;
}
