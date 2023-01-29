#pragma once

#include "datasources/pricefetcher.h"

class HtmlFetcher : public PriceFetcher
{
    Q_OBJECT
public:
    explicit HtmlFetcher(Clock* clock);
    qreal getPrice(const QDateTime& evenHourInUtc) const override;
    QString currentPrice() const override;
private slots:
    void fetchTomorrowData();
    void networkReplyReceived(QNetworkReply* reply);
private:
    void fetchData(QString fromDate);
    void updatePrices(const QList<QPair<QString, QString>>& parsedData, const QDate& date);
    QNetworkAccessManager* m_accessManager;
    QMap<QDateTime, qreal> m_prices;
    bool m_fetchingTomorrow = false;
};
