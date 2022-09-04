#include "operationlistitem.h"

OperationListItem::OperationListItem()
{}

bool OperationListItem::isValid() const
{
    return true;
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
