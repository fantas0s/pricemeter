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
    if (index.row() >= 0 && index.row() < rowCount()) {
        const auto& item = m_items.at(index.row());
        switch (static_cast<Role>(role)) {
        case Role::Title:
            return item.title();
        case Role::ImageFile:
            return item.imageFile();
        case Role::OperationsModel:
            return QVariant::fromValue<QAbstractItemModel*>(item.operationsModel());
        default:
            break;
        }
    }
    return QVariant();
}

QHash<int, QByteArray> DeviceListModel::roleNames() const
{
    QHash<int, QByteArray> returnValue;
    returnValue.insert(static_cast<int>(Role::Title), "title");
    returnValue.insert(static_cast<int>(Role::ImageFile), "imageFile");
    returnValue.insert(static_cast<int>(Role::OperationsModel), "operationsModel");
    return returnValue;
}
