#pragma once
#include "consumption.h"

#include <QString>
#include <QColor>

class Clock;

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
    void recalculateValues(const Clock* clock);
private:
    bool isContinuous() const;
    QList<Consumption> m_consumptions;
    QString m_title;
    QString m_nextHourString;
    QString m_twoHoursString;
    QString m_lowestCostTimeString;
};
