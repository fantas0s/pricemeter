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
            return QVariant::fromValue<QAbstractItemModel*>(const_cast<DeviceListItem&>(item).operationsModel());
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

void DeviceListModel::setClock(QObject *clockPtr)
{
    if (m_clock) {
        disconnect(m_clock, &Clock::minuteChanged, this, &DeviceListModel::refreshItems);
    }
    m_clock = qobject_cast<Clock*>(clockPtr);
    connect(m_clock, &Clock::minuteChanged, this, &DeviceListModel::refreshItems);
    if (m_clock && m_priceFetcher) {
        refreshItems();
    }
    emit clockChanged();
}

void DeviceListModel::setPriceFetcher(QObject *priceFetcherPtr)
{
    if (m_priceFetcher) {
        disconnect(m_priceFetcher, &PriceFetcher::pricesUpdated, this, &DeviceListModel::refreshItems);
    }
    m_priceFetcher = qobject_cast<PriceFetcher*>(priceFetcherPtr);
    connect(m_priceFetcher, &PriceFetcher::pricesUpdated, this, &DeviceListModel::refreshItems);
    if (m_clock && m_priceFetcher) {
        refreshItems();
    }
    emit priceFetcherChanged();
}

void DeviceListModel::refreshItems()
{
    for (DeviceListItem& item : m_items) {
        item.recalculateValues(m_clock, m_priceFetcher);
    }
}
