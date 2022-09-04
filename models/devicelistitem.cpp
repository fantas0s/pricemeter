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

QList<OperationListItem> DeviceListItem::operations() const
{
    return m_operations;
}

void DeviceListItem::addOperations(const QList<OperationListItem> &newOperations)
{
    m_operations.append(newOperations);
}
