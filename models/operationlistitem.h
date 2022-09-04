#pragma once
#include "consumption.h"

#include <QString>

class OperationListItem
{
public:
    OperationListItem();
    bool isValid() const;
    QString firstItemTitle() const;
    QString secondItemTitle() const;
    QString thirdItemTitle() const;
    QString firstItemValue() const;
    QString secondItemValue() const;
    QString thirdItemValue() const;
    QString title() const;
    QList<Consumption> consumptions() const;
    void setTitle(const QString& value);
    void addConsumption(const Consumption& value);
private:
    QList<Consumption> m_consumptions;
    QString m_title;
};
