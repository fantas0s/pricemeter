#include "devicelistitem.h"
#include "operationsmodel.h"

DeviceListItem::DeviceListItem()
{}

DeviceListItem::~DeviceListItem()
{
    delete m_model;
}

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

QAbstractItemModel* DeviceListItem::operationsModel()
{
    if (!m_model) {
        m_model = new OperationsModel(m_operations);
    }
    return m_model;
}

void DeviceListItem::addOperations(const QList<OperationListItem> &newOperations)
{
    m_operations.append(newOperations);
}
