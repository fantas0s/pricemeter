#pragma once
#include "devicelistitem.h"

#include <QAbstractListModel>

class QXmlStreamReader;

class DeviceListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    DeviceListModel(QObject* parent = nullptr);
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;
private:
    bool parseDeviceList(QXmlStreamReader& reader);
    void parseDevice(QXmlStreamReader& reader);
    OperationListItem parseOperation(QXmlStreamReader& reader);
    Consumption parseConsumption(QXmlStreamReader& reader);
    int toInt(const QString& text) const;
    qreal toReal(const QString& text) const;
    QList<DeviceListItem> m_items;
};
