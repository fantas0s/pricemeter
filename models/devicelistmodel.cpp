#include "devicelistmodel.h"
#include "utils/xmlfilereader.h"

DeviceListModel::DeviceListModel(QObject* parent)
    : QAbstractListModel(parent)
{
    XmlFileReader reader;
    m_items = reader.createItems();
}

int DeviceListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_items.size();
}

QVariant DeviceListModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

QHash<int, QByteArray> DeviceListModel::roleNames() const
{
    QHash<int, QByteArray> returnValue;
    return returnValue;
}
