#include "htmlfetcher.h"
#include "htmltableextractor.h"
#include "datasources/clock.h"
#ifdef DEBUG_NETWORK_ACCESS
#include "utils/debuginfo.h"
#endif

#include <QNetworkAccessManager>
#include <QNetworkReply>

/* Here should be an URL which contains a table with proper values.
 * Naturally such an abuse of public website is not recommended or encouraged, so the URL is omitted. */
static const QLatin1String s_url("");
static const qreal s_alv = 0.10; /* tax in percent */
static const qreal s_priceMargin = 0.22; /* cent / kWh */

HtmlFetcher::HtmlFetcher(Clock* clock)
    : PriceFetcher{clock}
{
    connect(clock, &Clock::hourChanged, this, &HtmlFetcher::fetchTomorrowData);
    connect(clock, &Clock::hourChanged, this, &PriceFetcher::currentPriceChanged);
    m_accessManager = new QNetworkAccessManager(this);
    m_accessManager->setTransferTimeout();
    connect(m_accessManager, &QNetworkAccessManager::finished,
            this, &HtmlFetcher::networkReplyReceived);
    QList<QPair<QString, QString>> defaultData;
    defaultData << QPair<QString, QString>("00:00", "00.01");
    defaultData << QPair<QString, QString>("01:00", "00.01");
    defaultData << QPair<QString, QString>("02:00", "00.01");
    defaultData << QPair<QString, QString>("03:00", "00.01");
    defaultData << QPair<QString, QString>("04:00", "00.01");
    defaultData << QPair<QString, QString>("05:00", "00.01");
    defaultData << QPair<QString, QString>("06:00", "00.01");
    defaultData << QPair<QString, QString>("07:00", "00.01");
    defaultData << QPair<QString, QString>("08:00", "00.01");
    defaultData << QPair<QString, QString>("09:00", "00.01");
    defaultData << QPair<QString, QString>("10:00", "00.01");
    defaultData << QPair<QString, QString>("11:00", "00.01");
    defaultData << QPair<QString, QString>("12:00", "00.01");
    defaultData << QPair<QString, QString>("13:00", "00.01");
    defaultData << QPair<QString, QString>("14:00", "00.01");
    defaultData << QPair<QString, QString>("15:00", "00.01");
    defaultData << QPair<QString, QString>("16:00", "00.01");
    defaultData << QPair<QString, QString>("17:00", "00.01");
    defaultData << QPair<QString, QString>("18:00", "00.01");
    defaultData << QPair<QString, QString>("19:00", "00.01");
    defaultData << QPair<QString, QString>("20:00", "00.01");
    defaultData << QPair<QString, QString>("21:00", "00.01");
    defaultData << QPair<QString, QString>("22:00", "00.01");
    defaultData << QPair<QString, QString>("23:00", "00.01");
    updatePrices(defaultData, QDateTime::currentDateTimeUtc().date());
    fetchTodayData();
}

qreal HtmlFetcher::getPrice(const QDateTime &evenHourInUtc) const
{
    return m_prices.value(evenHourInUtc);
}

QString HtmlFetcher::currentPrice() const
{
    return tr("%1 c/kWh").arg(m_prices.value(m_clock->toEvenHour(m_clock->currentTime().toUTC())), 0, 'f', 2);
}

void HtmlFetcher::fetchTodayData()
{
    disconnect(m_clock, &Clock::minuteChanged, this, &HtmlFetcher::fetchTodayData);
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
                /* Row value is euros / MWh, but we need cents / kWh.
                 * So we have to multiply by 100 and divide by 1000,
                 * that is divide by 10.
                 * Price is without tax, so add "ALV".
                 * Also apply seller margin.
                 */
                QDateTime keyTime(date, rowTime);
                m_prices.insert(keyTime, ((1.0 + s_alv) * rowValue / 10.0) + s_priceMargin);
            }
        }
    }
    emit pricesUpdated();
    emit currentPriceChanged();
}
