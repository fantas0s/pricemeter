#pragma once
#include "operationlistitem.h"

#include <QString>
#include <QList>

class QAbstractItemModel;

class DeviceListItem
{
public:
    DeviceListItem();
    bool isValid() const;
    QString title() const;
    void setTitle(const QString& newTitle);
    QString imageFile() const;
    void setImageFile(const QString& newName);
    QAbstractItemModel* operationsModel() const;
    void addOperations(const QList<OperationListItem>& newOperations);
private:
     QList<OperationListItem> m_operations;
     QString m_title;
     QString m_imageFile;
};
