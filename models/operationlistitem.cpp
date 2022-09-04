#include "operationlistitem.h"

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
            return QString("In 43 minutes:");
        case 2:
            return QString("15:00:");
        default:
            return QString("03:00:");
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

bool OperationListItem::isContinuous() const
{
    return (m_consumptions.isEmpty() || (m_consumptions.first().time().msecsSinceStartOfDay() == 0));
}
