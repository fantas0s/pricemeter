#pragma once
#include "models/devicelistitem.h"

class QXmlStreamReader;

class XmlFileReader
{
public:
    XmlFileReader();
    QList<DeviceListItem> createItems();
private:
    bool parseDeviceList(QXmlStreamReader& reader);
    void parseDevice(QXmlStreamReader& reader);
    OperationListItem parseOperation(QXmlStreamReader& reader);
    Consumption parseConsumption(QXmlStreamReader& reader);
    int toInt(const QString& text) const;
    qreal toReal(const QString& text) const;
    QList<DeviceListItem> m_items;
};
