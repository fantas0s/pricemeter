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
    enum class TextIndex {
        ThisHour = 0,
        NextHour = 1,
        TwoHours = 2,
        LowestCost = 3
    };
    bool isContinuous() const;
    qreal continuousCost(int hours) const;
    QString continuousCostString(int hours) const;
    void calculateLowestCostStringsAndValues();
    qreal consumptionCost(const QDateTime& utcTime) const;
    qreal cost(qreal kW, int seconds, QDateTime& timeOfStartUTC) const;
    QString centsToEuroString(qreal costInCents) const;
    QColor textColorFromCost(qreal cost) const;
    void useContinuousColors();
    QList<Consumption> m_consumptions;
    const Clock *m_clock = nullptr;
    const PriceFetcher* m_priceFetcher = nullptr;
    QString m_title;
    QStringList m_timeStrings;
    QStringList m_costStrings;
    QList<QColor> m_costColors;
    qreal m_lowestCostValue = 0;
    qreal m_highestCostValue = 0;
};
