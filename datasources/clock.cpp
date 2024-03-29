#include "clock.h"
#include <QDateTime>
#include <QTimeZone>
#include <QLocale>

Clock::Clock(QObject *parent)
    : QObject{parent}
{
    startTimer(1000, Qt::VeryCoarseTimer);
    Clock::timerEvent(nullptr);
}

QDateTime Clock::utcToEvenHour(const QDateTime &time) const
{
    QDateTime evenHourDateTime(time.date(), QTime(time.time().hour(),0,0));
    evenHourDateTime.setTimeZone(QTimeZone(0));
    return evenHourDateTime;
}

void Clock::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    QDateTime date = QDateTime::currentDateTime();
    m_currentTimeString = QLocale::system().toString(date.time(), "hh:mm:ss");
    m_currentTime = QDateTime(date.date(), QTime(date.time().hour(), date.time().minute()));
    bool signalMinute = false;
    bool signalHour = false;
    if (m_currentTime.time().minute() != m_currentMinute) {
        m_currentMinute = m_currentTime.time().minute();
        signalMinute = true;
    }
    if (m_currentTime.time().hour() != m_currentHour) {
        m_currentHour = m_currentTime.time().hour();
        m_nextEvenHour = QDateTime(m_currentTime.date(), QTime(m_currentHour,0,0)).addSecs(3600);
        signalHour = true;
    }
    emit secondChanged();
    if (signalMinute)
        emit minuteChanged();
    if (signalHour)
        emit hourChanged();
}
