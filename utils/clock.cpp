#include "clock.h"
#include <QDateTime>

Clock::Clock(QObject *parent)
    : QObject{parent}
{
    startTimer(1000, Qt::VeryCoarseTimer);
}

void Clock::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    QDateTime date = QDateTime::currentDateTime();
    m_currentTimeString = date.toString("hh:mm:ss");
    m_currentTime = date.time();
    bool signalMinute = false;
    bool signalHour = false;
    if (m_currentTime.minute() != m_currentMinute) {
        m_currentMinute = m_currentTime.minute();
        signalMinute = true;
    }
    if (m_currentTime.hour() != m_currentHour) {
        m_currentHour = m_currentTime.hour();
        if (m_currentHour == 23)
            m_nextEvenHour = QTime(0,0,0);
        else
            m_nextEvenHour = QTime(m_currentTime.hour() + 1,0,0);
        signalHour = true;
    }
    emit secondChanged();
    if (signalMinute)
        emit minuteChanged();
    if (signalHour)
        emit hourChanged();
}
