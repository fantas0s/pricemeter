#pragma once
#include "operationlistitem.h"

#include <QString>
#include <QList>

class DeviceListItem
{
public:
    DeviceListItem();
    bool isValid() const;
    QString title() const;
    void setTitle(const QString& newTitle);
    QString imageFile() const;
    void setImageFile(const QString& newName);
    QList<OperationListItem> operations() const;
    void addOperations(const QList<OperationListItem>& newOperations);
private:
     QList<OperationListItem> m_operations;
     QString m_title;
     QString m_imageFile;
};
