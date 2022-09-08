#include "htmlfetcher.h"
#include "htmltableextractor.h"
#include "datasources/clock.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>

HtmlFetcher::HtmlFetcher(Clock* clock)
    : PriceFetcher{clock}
{
    connect(clock, &Clock::hourChanged, this, &HtmlFetcher::fetchTomorrowData);
    connect(clock, &Clock::hourChanged, this, &PriceFetcher::currentPriceChanged);
    m_accessManager = new QNetworkAccessManager(this);
    connect(m_accessManager, &QNetworkAccessManager::finished,
            this, &HtmlFetcher::networkReplyReceived);
    const QString today = QDateTime::currentDateTime().toString("dd.MM.yyyy");
    fetchData(today);
    QList<QPair<QString, QString>> defaultData;
    defaultData << QPair<QString, QString>("00:00", "9.78");
    defaultData << QPair<QString, QString>("01:00", "11.56");
    defaultData << QPair<QString, QString>("02:00", "12.89");
    defaultData << QPair<QString, QString>("03:00", "12.28");
    defaultData << QPair<QString, QString>("04:00", "23.39");
    defaultData << QPair<QString, QString>("05:00", "34.46");
    defaultData << QPair<QString, QString>("06:00", "45.54");
    defaultData << QPair<QString, QString>("07:00", "56.63");
    defaultData << QPair<QString, QString>("08:00", "78.72");
    defaultData << QPair<QString, QString>("09:00", "103.34");
    defaultData << QPair<QString, QString>("10:00", "89.48");
    defaultData << QPair<QString, QString>("11:00", "79.68");
    defaultData << QPair<QString, QString>("12:00", "67.99");
    defaultData << QPair<QString, QString>("13:00", "89.08");
    defaultData << QPair<QString, QString>("14:00", "67.17");
    defaultData << QPair<QString, QString>("15:00", "79.66");
    defaultData << QPair<QString, QString>("16:00", "77.35");
    defaultData << QPair<QString, QString>("17:00", "98.54");
    defaultData << QPair<QString, QString>("18:00", "129.73");
    defaultData << QPair<QString, QString>("19:00", "99.32");
    defaultData << QPair<QString, QString>("20:00", "66.35");
    defaultData << QPair<QString, QString>("21:00", "25.44");
    defaultData << QPair<QString, QString>("22:00", "12.73");
    defaultData << QPair<QString, QString>("23:00", "9.82");
    updatePrices(defaultData, QDateTime::currentDateTime().date());
}

qreal HtmlFetcher::getPrice(const QDateTime &evenHour) const
{
    return m_prices.value(evenHour);
}

QString HtmlFetcher::currentPrice() const
{
    return tr("%1 c/kWh").arg(m_prices.value(m_clock->toEvenHour(m_clock->currentTime())), 0, 'f', 2);
}

void HtmlFetcher::fetchTomorrowData()
{
    if (m_fetchingTomorrow) {
        const QString tomorrow = QDateTime::currentDateTime().addDays(1).toString("dd.MM.yyyy");
        fetchData(tomorrow);
    }
}

void HtmlFetcher::networkReplyReceived(QNetworkReply *reply)
{
    HtmlTableExtractor extractor;
    auto result = extractor.extractTable(reply->readAll());
    if (m_fetchingTomorrow) {
        updatePrices(result, QDateTime::currentDateTime().addDays(1).date());
    } else {
        updatePrices(result, QDateTime::currentDateTime().date());
    }
    reply->deleteLater();
    if (!m_fetchingTomorrow) {
        m_fetchingTomorrow = true;
        /* Try to fetch also tomorrow data. If no data available, we will anyway try every hour starting from next hour. */
        fetchTomorrowData();
    }
}

void HtmlFetcher::fetchData(QString fromDate)
{
    m_accessManager->get(QNetworkRequest(QUrl(s_url.arg(fromDate))));
}

void HtmlFetcher::updatePrices(const QList<QPair<QString, QString> > &parsedData, const QDate& date)
{
    for (const auto& pair : parsedData) {
        QTime rowTime = QTime::fromString(pair.first, "hh:mm");
        if (rowTime.isValid()) {
            bool readOk = false;
            qreal rowValue = pair.second.toFloat(&readOk);
            if (readOk) {
                /* Row value is euros / MWh, but we need cents / kWh.
                 * So we have to multiply by 100 and divide by 1000,
                 * that is divide by 10.
                 */
                QDateTime keyTime(date, rowTime);
                m_prices.insert(keyTime, rowValue / 10.0);
            }
        }
    }
    emit pricesUpdated();
    emit currentPriceChanged();
}
