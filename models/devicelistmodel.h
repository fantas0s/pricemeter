#pragma once
#include "devicelistitem.h"
#include "datasources/clock.h"
#include "datasources/pricefetcher.h"

#include <QAbstractListModel>

class DeviceListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QObject* clock READ clock WRITE setClock NOTIFY clockChanged);
    Q_PROPERTY(QObject* priceFetcher READ priceFetcher WRITE setPriceFetcher NOTIFY priceFetcherChanged);
public:
    DeviceListModel(QObject* parent = nullptr);
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;
    QObject* clock() {return m_clock;}
    QObject* priceFetcher() {return m_priceFetcher;}
    void setClock(QObject* clockPtr);
    void setPriceFetcher(QObject* priceFetcherPtr);
signals:
    void clockChanged();
    void priceFetcherChanged();
private slots:
    void refreshItems();
private:
    enum class Role {
        Title = Qt::ItemDataRole::UserRole,
        ImageFile,
        OperationsModel
    };
    QList<DeviceListItem> m_items;
    Clock* m_clock = nullptr;
    PriceFetcher* m_priceFetcher = nullptr;
};
