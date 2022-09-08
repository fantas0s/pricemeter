#pragma once

#include <QAbstractListModel>
#include "operationlistitem.h"

class Clock;
class PriceFetcher;

class OperationsModel : public QAbstractListModel
{
public:
    explicit OperationsModel(const QList<OperationListItem>& items, QObject *parent = nullptr);
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;
    void recalculateValues(const Clock* clock, const PriceFetcher* priceFetcher);
private:
    enum class Role {
        Title = Qt::ItemDataRole::UserRole,
        Labels,
        Values,
        ValueColors
    };
    QList<OperationListItem> m_items;
};
