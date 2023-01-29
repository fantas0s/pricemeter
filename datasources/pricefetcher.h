#pragma once
#include <QObject>
#include <QDateTime>
#include <QMap>

class QNetworkAccessManager;
class QNetworkReply;
class Clock;

class PriceFetcher : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentPrice READ currentPrice NOTIFY currentPriceChanged)
public:
    explicit PriceFetcher(Clock* clock);
    virtual ~PriceFetcher() {};
    virtual qreal getPrice(const QDateTime& evenHourInUtc) const = 0;
    virtual QString currentPrice() const = 0;
signals:
    void pricesUpdated();
    void currentPriceChanged();
protected:
    Clock* m_clock;
};
