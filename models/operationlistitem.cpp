#include "operationlistitem.h"
#include "datasources/clock.h"
#include "datasources/pricefetcher.h"
#include "utils/pricecolor.h"
#include <QLocale>
#include <QObject>

static const int s_textCount = 4;

OperationListItem::OperationListItem()
{
    for (int idx = 0 ; idx < s_textCount ; ++idx)
    {
        m_timeStrings.append("");
        m_costStrings.append("");
        m_costColors.append(QColor(255, 255, 255));
    }
    m_timeStrings[static_cast<int>(TextIndex::ThisHour)] = QObject::tr("Now:");
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
        return m_timeStrings.at(index);
    }
}

QString OperationListItem::itemValue(int index) const
{
    return m_costStrings.at(index);
}

QColor OperationListItem::valueColor(int index) const
{
    return m_costColors.at(index);
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
    m_clock = clock;
    m_priceFetcher = priceFetcher;
    int minutes = 60 - m_clock->currentTimeUTC().time().minute();
    const QTime twoHoursTime = m_clock->nextEvenHourUTC().addSecs(3600).time();
    /* Titles */
    m_timeStrings[static_cast<int>(TextIndex::NextHour)] = QObject::tr("In %n minute(s):", "", minutes);
    m_timeStrings[static_cast<int>(TextIndex::TwoHours)] = QLocale::system().toString(twoHoursTime, "hh:mm")+QObject::tr(":");
    m_timeStrings[static_cast<int>(TextIndex::LowestCost)] = QObject::tr("00:00");
    /* Values */
    if (isContinuous()) {
        m_costStrings[static_cast<int>(TextIndex::ThisHour)] = continuousCostString(1);
        m_costStrings[static_cast<int>(TextIndex::NextHour)] = continuousCostString(2);
        m_costStrings[static_cast<int>(TextIndex::TwoHours)] = continuousCostString(8);
        m_costStrings[static_cast<int>(TextIndex::LowestCost)] = continuousCostString(24);
        useContinuousColors();
    } else {
        qreal thisHourCost = consumptionCost(m_clock->currentTimeUTC());
        qreal nextHourCost = consumptionCost(m_clock->nextEvenHourUTC());
        qreal twoHourCost = consumptionCost(m_clock->nextEvenHourUTC().addSecs(3600));
        m_costStrings[static_cast<int>(TextIndex::ThisHour)] = centsToEuroString(thisHourCost);
        m_costStrings[static_cast<int>(TextIndex::NextHour)] = centsToEuroString(nextHourCost);
        m_costStrings[static_cast<int>(TextIndex::TwoHours)] = centsToEuroString(twoHourCost);
        calculateLowestCostStringsAndValues(); /* Needed to do at this point for proper color selection. */
        m_costColors[static_cast<int>(TextIndex::ThisHour)] = textColorFromCost(thisHourCost);
        m_costColors[static_cast<int>(TextIndex::NextHour)] = textColorFromCost(nextHourCost);
        m_costColors[static_cast<int>(TextIndex::TwoHours)] = textColorFromCost(twoHourCost);
    }
}

bool OperationListItem::isContinuous() const
{
    return (m_consumptions.isEmpty() || (m_consumptions.first().time().msecsSinceStartOfDay() == 0));
}

qreal OperationListItem::continuousCost(int hours) const
{
    /* Always only one consumption rate for continuous */
    const qreal kW = m_consumptions.first().kW();
    QDateTime momentToEvaluate = m_clock->currentTimeUTC();
    const int seconds = 60 * 60 * hours;
    /* Cost in cents */
    return cost(kW, seconds, momentToEvaluate);
}

QString OperationListItem::continuousCostString(int hours) const
{
    /* cost is in cents. */
    return centsToEuroString(continuousCost(hours));
}

void OperationListItem::calculateLowestCostStringsAndValues()
{
    /* We try to find the "next cost that is lower than current costs". */
    QList<qreal> compareCosts;
    const QDateTime currentTimeUTC = m_clock->currentTimeUTC();
    compareCosts.append(consumptionCost(currentTimeUTC));
    compareCosts.append(consumptionCost(m_clock->nextEvenHourUTC()));
    compareCosts.append(consumptionCost(m_clock->nextEvenHourUTC().addSecs(3600)));
    /* Get next even hour */
    const QDateTime baseTime = m_clock->utcToEvenHour(m_clock->currentTimeUTC().addSecs(3600));
    m_lowestCostValue = 0;
    m_highestCostValue = 0;
    qreal nextLowestCost = compareCosts[0];
    int nextLowestCostAdd = -1;
    const int nextLowestCostCompareStart = compareCosts.size() - 1;
    for (int hourAddition = 0 ; hourAddition < nextLowestCostCompareStart ; ++hourAddition)
    {
         const qreal currentCost = compareCosts.at(hourAddition+1);
         if (currentCost < nextLowestCost)
         {
             nextLowestCost = currentCost;
             nextLowestCostAdd = hourAddition;
         }
    }
    /* Try next 23 hours. */
    for (int additionalHours = 0 ; additionalHours < 24 ; ++additionalHours)
    {
        const qreal costOfHour = consumptionCost(baseTime.addSecs(3600 * additionalHours));
        if (costOfHour > 0) {
            if ((m_lowestCostValue == 0) ||
                (m_lowestCostValue > costOfHour)) {
                m_lowestCostValue = costOfHour;
            }
            if ((m_highestCostValue == 0) ||
                (m_highestCostValue < costOfHour)) {
                m_highestCostValue = costOfHour;
            }
            if (additionalHours >= nextLowestCostCompareStart &&
                nextLowestCostAdd < nextLowestCostCompareStart)
            {
                /* Next lowest cost not found yet. */
                if (costOfHour < nextLowestCost)
                {
                    nextLowestCost = costOfHour;
                    nextLowestCostAdd = additionalHours;
                }
             }
        }
    }
    /* Add next lowest cost. */
    if (nextLowestCostAdd < nextLowestCostCompareStart) {
        /* No cheaper cost was found. Take "next cost in chain" */
        nextLowestCostAdd = nextLowestCostCompareStart;
        nextLowestCost = consumptionCost(baseTime.addSecs(3600 * nextLowestCostCompareStart));
    }
    m_timeStrings[static_cast<int>(TextIndex::LowestCost)] = baseTime.addSecs(3600 * nextLowestCostAdd).toLocalTime().toString("hh:mm");
    m_costStrings[static_cast<int>(TextIndex::LowestCost)] = centsToEuroString(nextLowestCost);
    m_costColors[static_cast<int>(TextIndex::LowestCost)] = textColorFromCost(nextLowestCost);
}

qreal OperationListItem::consumptionCost(const QDateTime &utcTime) const
{
    qreal combinedCost = 0;
    QDateTime momentToEvaluate = utcTime;
    for (const auto& consumption : m_consumptions) {
        qreal kW = consumption.kW();
        int secondsToConsume = consumption.time().msecsSinceStartOfDay() / 1000;
        combinedCost += cost(kW, secondsToConsume, momentToEvaluate);
    }
    return combinedCost;
}

qreal OperationListItem::cost(qreal kW, int seconds, QDateTime &timeOfStartUTC) const
{
    qreal cost = 0;
    qreal price = m_priceFetcher->getPrice(m_clock->utcToEvenHour(timeOfStartUTC));
    while (seconds > 0) {
        /* Consume minutes up to next full hour */
        const QDateTime searchTime = m_clock->utcToEvenHour(timeOfStartUTC);
        /* In case we haven't got price data yet, use the last we got. */
        const qreal tempPrice = m_priceFetcher->getPrice(searchTime.toUTC());
        if (tempPrice > 0)
            price = tempPrice;
        const int remainingSeconds = 3600 - (60 * timeOfStartUTC.time().minute()) - timeOfStartUTC.time().second();
        qreal kWh = 0;
        if (seconds > remainingSeconds) {
            /* We have more time than just the remaining part of this hour.
             * Calculate cost for the remainder of this hour and continue.
             */
            kWh = (kW * ((qreal)remainingSeconds)/3600.0);
            seconds -= remainingSeconds;
            timeOfStartUTC = timeOfStartUTC.addSecs(remainingSeconds);
        } else {
            /* All of the remaining seconds happen within this hour. */
            kWh = (kW * ((qreal)seconds)/3600.0);
            seconds = 0;
            timeOfStartUTC = timeOfStartUTC.addSecs(seconds);
        }
        cost += price * kWh;
    }
    return cost;
}

QString OperationListItem::centsToEuroString(qreal costInCents) const
{
    /* starting from 10 cents we use euros (e.g. "0,10 €"). Until then it's cents (i.e. "9,99 c"). */
    if (costInCents >= 10.0) {
        costInCents = costInCents / 100.0;
        return QObject::tr("%1 €").arg(costInCents, 0, 'f', 2);
    } else {
        return QObject::tr("%1 c").arg(costInCents, 0, 'f', 2);
    }
}

QColor OperationListItem::textColorFromCost(qreal cost) const
{
    PriceColor color;
    color.setPrice(cost);
    color.setMaxPrice(m_highestCostValue);
    color.setMinPrice(m_lowestCostValue);
    return color.color();
}

void OperationListItem::useContinuousColors()
{
    PriceColor color;
    color.setMaxPrice(color.expensiveAreaLimit());
    color.setMinPrice(color.cheapAreaLimit());
    m_costColors.clear();
    color.setPrice(continuousCost(1)); /* 1 hour */
    m_costColors << color.color();
    color.setPrice(continuousCost(2)); /* 2 hours */
    m_costColors << color.color();
    color.setPrice(continuousCost(8)); /* 8 hours */
    m_costColors << color.color();
    color.setPrice(continuousCost(24)); /* 24 hours */
    m_costColors << color.color();
}
