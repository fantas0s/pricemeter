#pragma once
#include "devicelistitem.h"

#include <QAbstractListModel>


class DeviceListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    DeviceListModel(QObject* parent = nullptr);
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;
private:
    QList<DeviceListItem> m_items;
};
