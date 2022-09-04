#include "operationsmodel.h"

static const int s_valueCount = 4;

OperationsModel::OperationsModel(const QList<OperationListItem>& items, QObject *parent)
    : QAbstractListModel{parent}
    , m_items{items}
{}

int OperationsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_items.size();
}

QVariant OperationsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() >= 0 && index.row() < rowCount()) {
        const auto& item = m_items.at(index.row());
        switch (static_cast<Role>(role)) {
        case Role::Title:
            return item.title();
        case Role::Labels:
            {
                QVariantList returnValue;
                for (int idx = 0 ; idx < s_valueCount ; ++idx) {
                    returnValue.append(QVariant(item.itemTitle(idx)));
                }
                return returnValue;
            }
        case Role::Values:
            {
                QVariantList returnValue;
                for (int idx = 0 ; idx < s_valueCount ; ++idx) {
                    returnValue.append(QVariant(item.itemValue(idx)));
                }
                return returnValue;
            }
        case Role::ValueColors:
            {
                QVariantList returnValue;
                for (int idx = 0 ; idx < s_valueCount ; ++idx) {
                    returnValue.append(QVariant(item.valueColor(idx)));
                }
                return returnValue;
            }
        default:
            break;
        }
    }
    return QVariant();
}

QHash<int, QByteArray> OperationsModel::roleNames() const
{
    QHash<int, QByteArray> returnValue;
    returnValue.insert(static_cast<int>(Role::Title), "title");
    returnValue.insert(static_cast<int>(Role::Labels), "labels");
    returnValue.insert(static_cast<int>(Role::Values), "values");
    returnValue.insert(static_cast<int>(Role::ValueColors), "valueColors");
    return returnValue;
}
