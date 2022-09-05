#include "operationlistitem.h"
#include "utils/clock.h"

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
    return "89 c";
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

void OperationListItem::recalculateValues(const Clock *clock)
{
    int minutes = 60 - clock->currentTime().minute();
    m_nextHourString = QObject::tr("In %1 minute(s):", "", minutes).arg(minutes);
    m_twoHoursString = clock->nextEvenHour().addSecs(3600).toString("hh:mm")+QString(":");
}

bool OperationListItem::isContinuous() const
{
    return (m_consumptions.isEmpty() || (m_consumptions.first().time().msecsSinceStartOfDay() == 0));
}
