#include "htmlfetcher.h"
#include "htmltableextractor.h"
#include "datasources/clock.h"
#include <QTimeZone>
#ifdef DEBUG_NETWORK_ACCESS
#include "utils/debuginfo.h"
#endif

#include <QNetworkAccessManager>
#include <QNetworkReply>

/* Here should be an URL which contains a table with proper values.
 * Naturally such an abuse of public website is not recommended or encouraged, so the URL is omitted. */
const QLatin1String HtmlFetcher::s_url("");

HtmlFetcher::HtmlFetcher(Clock* clock)
    : PriceFetcher{clock}
{
    /* As a safety precaution (e.g. no time on device boot) also fetch today when refreshing. */
    connect(clock, &Clock::hourChanged, this, &HtmlFetcher::fetchTodayData);
    connect(clock, &Clock::hourChanged, this, &PriceFetcher::currentPriceChanged);
    m_accessManager = new QNetworkAccessManager(this);
    m_accessManager->setTransferTimeout();
    connect(m_accessManager, &QNetworkAccessManager::finished,
            this, &HtmlFetcher::networkReplyReceived);
    fetchTodayData();
}

qreal HtmlFetcher::getPrice(const QDateTime &evenHourInUtc) const
{
    return m_prices.value(evenHourInUtc, 0.0);
}

QString HtmlFetcher::currentPrice() const
{
    return tr("%1 c/kWh").arg(m_prices.value(m_clock->utcToEvenHour(m_clock->currentTimeUTC()), 0.0), 0, 'f', 2);
}

void HtmlFetcher::fetchTodayData()
{
    disconnect(m_clock, &Clock::minuteChanged, this, &HtmlFetcher::fetchTodayData);
    m_fetchingTomorrow = false;
    const QString today = QDateTime::currentDateTimeUtc().toString("dd.MM.yyyy");
    fetchData(today);
}

void HtmlFetcher::fetchTomorrowData()
{
    disconnect(m_clock, &Clock::minuteChanged, this, &HtmlFetcher::fetchTomorrowData);
    if (m_fetchingTomorrow) {
        const QString tomorrow = QDateTime::currentDateTimeUtc().addDays(1).toString("dd.MM.yyyy");
        fetchData(tomorrow);
    }
}

void HtmlFetcher::networkReplyReceived(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NetworkError::NoError) {
        qWarning() << "Got Error:" << reply->error() << "retrying...";
        if (m_fetchingTomorrow) {
            connect(m_clock, &Clock::minuteChanged, this, &HtmlFetcher::fetchTomorrowData, Qt::UniqueConnection);
        } else {
            connect(m_clock, &Clock::minuteChanged, this, &HtmlFetcher::fetchTodayData, Qt::UniqueConnection);
        }
    } else {
        HtmlTableExtractor extractor;
        auto result = extractor.extractTable(reply->readAll());
        if (m_fetchingTomorrow) {
            updatePrices(result, QDateTime::currentDateTimeUtc().addDays(1).date());
        } else {
            updatePrices(result, QDateTime::currentDateTimeUtc().date());
        }
        if (!m_fetchingTomorrow) {
            m_fetchingTomorrow = true;
            /* Try to fetch also tomorrow data. If no data available, we will anyway try every hour starting from next hour. */
            fetchTomorrowData();
        }
#ifdef DEBUG_NETWORK_ACCESS
        else {
            for (auto iter = m_prices.constBegin() ; iter != m_prices.constEnd() ; iter++) {
                qDebug() << iter.key().toString("dd.MM.yyyy hh:mm") << ":" << QString::number(iter.value());
            }
            qDebug() << "Time now (UTC):" << QDateTime::currentDateTimeUtc().toString("dd.MM.yyyy hh:mm");
        }
#endif
    }
    reply->deleteLater();
}

void HtmlFetcher::fetchData(QString fromDate)
{
#ifdef DEBUG_NETWORK_ACCESS
    static int fetches = 1;
    DebugInfo* debugInfo = DebugInfo::instance();
    debugInfo->print(QString("Fetch number %1 at %2").arg(fetches).arg(QDateTime::currentDateTime().toString()));
    fetches++;
#endif
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
#ifdef DEBUG_NETWORK_ACCESS
                qDebug() << pair.first << pair.second << "read OK:" << rowValue;
#endif
                /* Row value is euros / MWh, but we need cents / kWh.
                 * So we have to multiply by 100 and divide by 1000,
                 * that is divide by 10.
                 * Price is without tax, so add "ALV".
                 * Also apply seller margin.
                 */
                QDateTime keyTime(date, rowTime);
                keyTime.setTimeZone(QTimeZone(0));
                m_prices.insert(keyTime, ((1.0 + s_alv) * rowValue / 10.0) + s_priceMargin);
            }
        }
    }
    emit pricesUpdated();
    emit currentPriceChanged();
}
