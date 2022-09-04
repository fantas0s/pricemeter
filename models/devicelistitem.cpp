#include "devicelistitem.h"

DeviceListItem::DeviceListItem()
{}

bool DeviceListItem::isValid() const
{
    return !m_operations.isEmpty() && !m_title.isEmpty();
}

QString DeviceListItem::title() const
{
    return m_title;
}

void DeviceListItem::setTitle(const QString &newTitle)
{
    m_title = newTitle;
}

QString DeviceListItem::imageFile() const
{
    return m_imageFile;
}

void DeviceListItem::setImageFile(const QString &newName)
{
    m_imageFile = newName;
}

QAbstractItemModel* DeviceListItem::operationsModel() const
{
    return nullptr;
}

void DeviceListItem::addOperations(const QList<OperationListItem> &newOperations)
{
    m_operations.append(newOperations);
}
