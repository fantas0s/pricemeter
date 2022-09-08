#pragma once
#include "consumption.h"

#include <QString>
#include <QColor>

class Clock;
class PriceFetcher;

class OperationListItem
{
public:
    OperationListItem();
    bool isValid() const;
    QString itemTitle(int index) const;
    QString itemValue(int index) const;
    QColor valueColor(int index) const;
    QString title() const;
    QList<Consumption> consumptions() const;
    void setTitle(const QString& value);
    void addConsumption(const Consumption& value);
    void recalculateValues(const Clock* clock, const PriceFetcher* priceFetcher);
private:
    bool isContinuous() const;
    QString costString(const QDateTime& time, const Clock *clock, const PriceFetcher* priceFetcher) const;
    QString continuousCostString(int hours, const Clock *clock, const PriceFetcher* priceFetcher) const;
    QString lowestCostString(QString& timeTitle, const Clock *clock, const PriceFetcher* priceFetcher) const;
    qreal consumptionCost(const QDateTime& time, const Clock *clock, const PriceFetcher* priceFetcher) const;
    qreal cost(qreal kW, int seconds, QDateTime& timeOfStart, const Clock *clock, const PriceFetcher* priceFetcher) const;
    QList<Consumption> m_consumptions;
    QString m_title;
    QString m_nextHourString;
    QString m_twoHoursString;
    QString m_lowestCostTimeString;
    QString m_currentCost;
    QString m_nextHourCost;
    QString m_twoHoursCost;
    QString m_lowestCost;
};
